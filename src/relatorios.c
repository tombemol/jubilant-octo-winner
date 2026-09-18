#include <stdio.h>
#include <float.h>
#include "modelos.h"
#include "clientes.h"
#include "produtos.h"
#include "utils.h"
#include "relatorios.h"

int estoqueTotalRecursivo(int indice) {
    if (indice >= totalProdutos) return 0;
    return produtos[indice].estoque + estoqueTotalRecursivo(indice + 1);
}

double valorEstoqueRecursivo(int indice) {
    if (indice >= totalProdutos) return 0.0;
    return produtos[indice].preco * produtos[indice].estoque + valorEstoqueRecursivo(indice + 1);
}

static double faturamentoTotal(void) {
    double total = 0.0;
    for (int i = 0; i < totalVendas; i++) total += vendas[i].total;
    return total;
}

static void produtoMaisVendido(void) {
    if (totalProdutos == 0 || totalVendas == 0) { printf("Produto mais vendido: sem dados.\n"); return; }
    int codigoMelhor = -1, maiorQtd = 0;
    for (int p = 0; p < totalProdutos; p++) {
        int qtd = 0;
        for (int v = 0; v < totalVendas; v++)
            for (int i = 0; i < vendas[v].quantidadeItens; i++)
                if (vendas[v].itens[i].codigoProduto == produtos[p].codigo) qtd += vendas[v].itens[i].quantidade;
        if (qtd > maiorQtd) { maiorQtd = qtd; codigoMelhor = produtos[p].codigo; }
    }
    Produto *p = buscarProdutoPorCodigo(codigoMelhor);
    if (p && maiorQtd > 0) printf("Produto mais vendido: %s (%d unidade(s))\n", p->titulo, maiorQtd);
    else printf("Produto mais vendido: sem dados.\n");
}

static void clienteMaiorVolume(void) {
    if (totalClientes == 0 || totalVendas == 0) { printf("Cliente com maior volume de compras: sem dados.\n"); return; }
    int codigoMelhor = -1;
    double maior = -1.0;
    for (int c = 0; c < totalClientes; c++) {
        double total = 0.0;
        for (int v = 0; v < totalVendas; v++) if (vendas[v].codigoCliente == clientes[c].codigo) total += vendas[v].total;
        if (total > maior) { maior = total; codigoMelhor = clientes[c].codigo; }
    }
    Cliente *c = buscarClientePorCodigo(codigoMelhor);
    if (c && maior > 0) printf("Cliente com maior volume de compras: %s (R$ %.2f)\n", c->nome, maior);
    else printf("Cliente com maior volume de compras: sem dados.\n");
}

static void produtosEstoqueBaixo(void) {
    int achou = 0;
    printf("\nProdutos com estoque baixo:\n");
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].estoque <= produtos[i].estoqueMinimo) {
            printf("- [%d] %s: %d unidade(s), minimo %d\n", produtos[i].codigo, produtos[i].titulo, produtos[i].estoque, produtos[i].estoqueMinimo);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum produto com estoque baixo.\n");
}

static void relatorioGeral(void) {
    printf("========================================\n");
    printf("          CONSULTAS E RELATORIOS        \n");
    printf("========================================\n");
    printf("Clientes cadastrados: %d\n", totalClientes);
    printf("Produtos cadastrados: %d\n", totalProdutos);
    printf("Quantidade total em estoque: %d\n", estoqueTotalRecursivo(0));
    printf("Valor total estimado do estoque: R$ %.2f\n", valorEstoqueRecursivo(0));
    printf("Vendas realizadas: %d\n", totalVendas);
    printf("Valor total vendido: R$ %.2f\n", faturamentoTotal());
    produtoMaisVendido();
    clienteMaiorVolume();
    produtosEstoqueBaixo();
}

void mostrarEstatisticas(void) {
    printf("========================================\n");
    printf("             ESTATISTICAS               \n");
    printf("========================================\n");
    printf("Clientes: %d\nProdutos: %d\nVendas: %d\n", totalClientes, totalProdutos, totalVendas);
    printf("Estoque total (calculado com recursao): %d unidade(s)\n", estoqueTotalRecursivo(0));
    printf("Valor do estoque (calculado com recursao): R$ %.2f\n", valorEstoqueRecursivo(0));
    if (totalVendas > 0) printf("Ticket medio: R$ %.2f\n", faturamentoTotal() / totalVendas);
    else printf("Ticket medio: R$ 0.00\n");

    if (totalProdutos > 0) {
        int maior = 0, menor = 0;
        for (int i = 1; i < totalProdutos; i++) {
            if (produtos[i].estoque > produtos[maior].estoque) maior = i;
            if (produtos[i].estoque < produtos[menor].estoque) menor = i;
        }
        printf("Maior estoque: %s (%d)\n", produtos[maior].titulo, produtos[maior].estoque);
        printf("Menor estoque: %s (%d)\n", produtos[menor].titulo, produtos[menor].estoque);
    }
}

void menuRelatorios(void) {
    int opcao;
    do {
        limparTela();
        printf("========================================\n");
        printf("          CONSULTAS E RELATORIOS        \n");
        printf("========================================\n");
        printf("1 - Relatorio geral\n2 - Produtos com estoque baixo\n0 - Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 2);
        limparTela();
        if (opcao == 1) relatorioGeral();
        else if (opcao == 2) produtosEstoqueBaixo();
        if (opcao) pausar();
    } while (opcao != 0);
}
