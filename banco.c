#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banco.h"

static const char *tipoNome(int tipo) {
    if (tipo == 1) return "Corrente";
    if (tipo == 2) return "Salario";
    if (tipo == 3) return "Poupanca";
    return "Desconhecida";
}

static Conta *buscarConta(Cliente *c, int tipo) {
    for (int i = 0; i < c->qtdContas; i++)
        if (c->contas[i].tipo == tipo)
            return &c->contas[i];
    return NULL;
}

static void historico(Conta *c, const char *msg) {
    size_t n = strlen(c->historico);
    if (n < sizeof(c->historico) - 1)
        snprintf(c->historico + n, sizeof(c->historico) - n,
                 "%s\n", msg);
}

void inicializarBanco(Banco *b) {
    *b = (Banco){0};
    b->proximoNumero = 1000;
}

void liberarBanco(Banco *b) {
    free(b->clientes);
    b->clientes = NULL;
    b->qtdClientes = b->capacidade = 0;
}

void abrirBanco(Banco *b) {
    if (b->bancoAberto)
        printf("Banco ja esta aberto.\n");
    else {
        b->bancoAberto = 1;
        printf("Banco aberto com sucesso!\n");
    }
}

void cadastrarCliente(Banco *b) {
    if (!b->bancoAberto) {
        printf("Abra o banco primeiro.\n");
        return;
    }

    if (b->qtdClientes == b->capacidade) {
        int nova = b->capacidade ? b->capacidade * 2 : 4;
        Cliente *p = realloc(b->clientes, nova * sizeof(Cliente));
        if (!p) {
            printf("Erro de memoria.\n");
            return;
        }
        b->clientes = p;
        b->capacidade = nova;
    }

    Cliente c = {0};
    int ch, qtd;

    while ((ch = getchar()) != '\n' && ch != EOF) {}
    printf("Nome do cliente: ");
    if (!fgets(c.nome, sizeof(c.nome), stdin)) return;
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("Quantidade de contas (1 a 3): ");
    if (scanf("%d", &qtd) != 1 || qtd < 1 || qtd > 3) {
        printf("Quantidade invalida.\n");
        return;
    }

    for (int i = 0; i < qtd; i++) {
        int tipo;
        double saldo;

        printf("Tipo (1-Corrente, 2-Salario, 3-Poupanca): ");
        if (scanf("%d", &tipo) != 1 || tipo < 1 || tipo > 3) {
            printf("Tipo invalido.\n");
            return;
        }

        if (buscarConta(&c, tipo) ||
            (tipo == 1 && buscarConta(&c, 2)) ||
            (tipo == 2 && buscarConta(&c, 1))) {
            printf("Combinacao de contas invalida.\n");
            return;
        }

        printf("Saldo inicial: R$ ");
        if (scanf("%lf", &saldo) != 1 || saldo < 0) {
            printf("Saldo invalido.\n");
            return;
        }

        Conta *conta = &c.contas[c.qtdContas++];
        conta->tipo = tipo;
        conta->saldo = saldo;

        snprintf(conta->numero, sizeof(conta->numero),
                 "%04d-%02d", b->proximoNumero++, tipo);

        char msg[150];
        snprintf(msg, sizeof(msg),
                 "Conta criada. Saldo inicial: R$ %.2f", saldo);
        historico(conta, msg);
    }

    b->clientes[b->qtdClientes++] = c;
    printf("Cliente %s cadastrado!\n", c.nome);
}

void listarClientes(const Banco *b) {
    for (int i = 0; i < b->qtdClientes; i++) {
        const Cliente *c = &b->clientes[i];
        printf("%d - %s\n", i + 1, c->nome);

        for (int j = 0; j < c->qtdContas; j++)
            printf("%s | %s | R$ %.2f\n",
                   c->contas[j].numero,
                   tipoNome(c->contas[j].tipo),
                   c->contas[j].saldo);
    }

    if (!b->qtdClientes)
        printf("Nenhum cliente cadastrado.\n");
}

void consultarHistorico(const Banco *b) {
    if (!b->bancoAberto || !b->qtdClientes) {
        printf("Abra o banco e cadastre clientes primeiro.\n");
        return;
    }

    listarClientes(b);
    int n;
    printf("Numero do cliente: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > b->qtdClientes) {
        printf("Cliente invalido.\n");
        return;
    }

    Cliente *c = &b->clientes[n - 1];
    for (int i = 0; i < c->qtdContas; i++) {
        printf("\nConta %s:\n%s",
               c->contas[i].numero,
               c->contas[i].historico);
    }
}

void solicitarEmprestimo(Banco *b) {
    if (!b->bancoAberto || !b->qtdClientes) {
        printf("Abra o banco e cadastre clientes primeiro.\n");
        return;
    }

    listarClientes(b);
    int n;
    printf("Numero do cliente: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > b->qtdClientes) {
        printf("Cliente invalido.\n");
        return;
    }

    Cliente *c = &b->clientes[n - 1];
    Conta *corrente = buscarConta(c, 1);
    Conta *poupanca = buscarConta(c, 3);

    if (c->emprestimoSolicitado || !corrente ||
        !poupanca || poupanca->saldo <= 0) {
        printf("Cliente nao atende aos requisitos.\n");
        return;
    }

    double total = 0, valor;
    for (int i = 0; i < c->qtdContas; i++)
        total += c->contas[i].saldo;

    double limite = total * 2.5;
    printf("Limite: R$ %.2f\nValor do emprestimo: R$ ", limite);

    if (scanf("%lf", &valor) != 1 || valor <= 0 || valor > limite) {
        printf("Valor invalido.\n");
        return;
    }

    corrente->saldo += valor;
    c->emprestimoSolicitado = 1;
    c->valorEmprestimo = valor;

    char msg[150];
    snprintf(msg, sizeof(msg), "Emprestimo aprovado: R$ %.2f", valor);
    historico(corrente, msg);

    printf("Emprestimo aprovado!\n");
}

void realizarSorteio(const Banco *b) {
    if (!b->bancoAberto || !b->qtdClientes) {
        printf("Abra o banco e cadastre clientes primeiro.\n");
        return;
    }

    srand((unsigned)time(NULL));
    int n = rand() % b->qtdClientes;
    printf("Cliente sorteado: %s\n", b->clientes[n].nome);
}