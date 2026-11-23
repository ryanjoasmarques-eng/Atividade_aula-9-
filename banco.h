#ifndef BANCO_H
#define BANCO_H

#define MAX_CONTAS 3
#define TAM_HISTORICO 2048
#define TAM_NOME 100

typedef struct {
    char numero[8];
    int tipo;
    double saldo;
    char historico[TAM_HISTORICO];
} Conta;

typedef struct {
    char nome[TAM_NOME];
    Conta contas[MAX_CONTAS];
    int qtdContas;
    int emprestimoSolicitado;
    double valorEmprestimo;
} Cliente;

typedef struct {
    Cliente *clientes;
    int qtdClientes;
    int capacidade;
    int bancoAberto;
    int proximoNumero;
} Banco;

void inicializarBanco(Banco *banco);
void liberarBanco(Banco *banco);
void abrirBanco(Banco *banco);
void cadastrarCliente(Banco *banco);
void consultarHistorico(const Banco *banco);
void solicitarEmprestimo(Banco *banco);
void realizarSorteio(const Banco *banco);
void listarClientes(const Banco *banco);

#endif