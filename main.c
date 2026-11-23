#include <stdio.h>
#include "banco.h"

int main(void) {
    Banco banco;
    int opcao;

    inicializarBanco(&banco);

    do {
        printf("\n========== CARTEIRA DIGITAL ==========\n");
        printf("1 - Abrir banco\n");
        printf("2 - Cadastrar cliente/contas\n");
        printf("3 - Consultar historico\n");
        printf("4 - Solicitar emprestimo\n");
        printf("5 - Efetuar sorteio\n");
        printf("6 - Listar clientes e contas\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }

        switch (opcao) {
            case 1:
                abrirBanco(&banco);
                break;
            case 2:
                cadastrarCliente(&banco);
                break;
            case 3:
                consultarHistorico(&banco);
                break;
            case 4:
                solicitarEmprestimo(&banco);
                break;
            case 5:
                realizarSorteio(&banco);
                break;
            case 6:
                if (!banco.bancoAberto)
                    printf("Abra o banco primeiro.\n");
                else
                    listarClientes(&banco);
                break;
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    liberarBanco(&banco);
    return 0;
}