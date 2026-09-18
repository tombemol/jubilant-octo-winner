#include <stdio.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "arquivos.h"
#include "clientes.h"
#include "produtos.h"
#include "vendas.h"
#include "relatorios.h"
#include "utils.h"

static void cabecalho(void) {
    printf("========================================\n");
    printf(" SISTEMA DE GERENCIAMENTO EMPRESARIAL \n");
    printf("               LIVRARIA                \n");
    printf("========================================\n");
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "");
    criarPastaDados();
    carregarDados();

    int opcao;
    do {
        limparTela();
        cabecalho();
        printf("1 - Gerenciar Clientes\n");
        printf("2 - Gerenciar Produtos / Livros\n");
        printf("3 - Gerenciar Vendas\n");
        printf("4 - Consultas e Relatorios\n");
        printf("5 - Estatisticas\n");
        printf("0 - Sair\n");
        printf("========================================\n");
        opcao = lerInteiro("Escolha: ", 0, 5);
        switch (opcao) {
            case 1: menuClientes(); break;
            case 2: menuProdutos(); break;
            case 3: menuVendas(); break;
            case 4: menuRelatorios(); break;
            case 5: limparTela(); mostrarEstatisticas(); pausar(); break;
            case 0: salvarDados(); printf("\nDados salvos. Encerrando o sistema.\n"); break;
        }
    } while (opcao != 0);
    return 0;
}
