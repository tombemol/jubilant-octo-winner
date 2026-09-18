#include <stdio.h>
#include <string.h>
#include "modelos.h"
#include "produtos.h"
#include "arquivos.h"
#include "utils.h"

static int proximoCodigoProduto(void) {
    int maior = 0;
    for (int i = 0; i < totalProdutos; i++) if (produtos[i].codigo > maior) maior = produtos[i].codigo;
    return maior + 1;
}

Produto *buscarProdutoPorCodigo(int codigo) {
    for (int i = 0; i < totalProdutos; i++) if (produtos[i].codigo == codigo) return &produtos[i];
    return NULL;
}

int produtoTemVendas(int codigoProduto) {
    for (int v = 0; v < totalVendas; v++)
        for (int i = 0; i < vendas[v].quantidadeItens; i++)
            if (vendas[v].itens[i].codigoProduto == codigoProduto) return 1;
    return 0;
}

static void imprimirProduto(const Produto *p) {
    printf("\n[%d] %s\n", p->codigo, p->titulo);
    printf("Autor: %s | Categoria: %s\n", p->autor, p->categoria);
    printf("Preco: R$ %.2f | Estoque: %d | Minimo: %d\n", p->preco, p->estoque, p->estoqueMinimo);
}

void listarProdutosResumido(void) {
    if (totalProdutos == 0) { printf("Nenhum produto cadastrado.\n"); return; }
    printf("\n%-6s %-38s %-10s %-8s\n", "COD", "TITULO", "PRECO", "ESTOQUE");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < totalProdutos; i++)
        printf("%-6d %-38.38s R$%-7.2f %-8d\n", produtos[i].codigo, produtos[i].titulo, produtos[i].preco, produtos[i].estoque);
}

static void cadastrarProduto(void) {
    if (totalProdutos >= MAX_PRODUTOS) { printf("Limite de produtos atingido.\n"); return; }
    Produto p = {0};
    p.codigo = proximoCodigoProduto();
    printf("\n=== CADASTRO DE LIVRO / PRODUTO ===\nCodigo gerado: %d\n", p.codigo);
    lerLinha("Titulo: ", p.titulo, TAM_TITULO);
    lerLinha("Autor: ", p.autor, TAM_AUTOR);
    lerLinha("Categoria: ", p.categoria, TAM_CATEGORIA);
    p.preco = lerDouble("Preco: R$ ", 0.0);
    p.estoque = lerInteiro("Quantidade em estoque: ", 0, 1000000);
    p.estoqueMinimo = lerInteiro("Estoque minimo para alerta: ", 0, 1000000);
    produtos[totalProdutos++] = p;
    salvarProdutos();
    printf("Produto cadastrado com sucesso.\n");
}

static void listarProdutos(void) {
    printf("\n=== PRODUTOS ===\n");
    if (totalProdutos == 0) { printf("Nenhum produto cadastrado.\n"); return; }
    for (int i = 0; i < totalProdutos; i++) imprimirProduto(&produtos[i]);
}

static void buscarProduto(void) {
    if (totalProdutos == 0) { printf("Nenhum produto cadastrado.\n"); return; }
    int opcao = lerInteiro("Buscar por 1-Codigo  2-Titulo  3-Categoria: ", 1, 3);
    if (opcao == 1) {
        int codigo = lerInteiro("Codigo: ", 1, 999999);
        Produto *p = buscarProdutoPorCodigo(codigo);
        if (p) imprimirProduto(p); else printf("Produto nao encontrado.\n");
        return;
    }
    char termo[120];
    lerLinha(opcao == 2 ? "Titulo ou parte do titulo: " : "Categoria: ", termo, sizeof termo);
    int achou = 0;
    for (int i = 0; i < totalProdutos; i++) {
        const char *campo = opcao == 2 ? produtos[i].titulo : produtos[i].categoria;
        if (textoContemIgnorandoCaixa(campo, termo)) { imprimirProduto(&produtos[i]); achou = 1; }
    }
    if (!achou) printf("Nenhum produto encontrado.\n");
}

static void alterarProduto(void) {
    int codigo = lerInteiro("Codigo do produto: ", 1, 999999);
    Produto *p = buscarProdutoPorCodigo(codigo);
    if (!p) { printf("Produto nao encontrado.\n"); return; }
    printf("Alterando: %s\n", p->titulo);
    lerLinha("Novo titulo: ", p->titulo, TAM_TITULO);
    lerLinha("Novo autor: ", p->autor, TAM_AUTOR);
    lerLinha("Nova categoria: ", p->categoria, TAM_CATEGORIA);
    p->preco = lerDouble("Novo preco: R$ ", 0.0);
    p->estoque = lerInteiro("Novo estoque: ", 0, 1000000);
    p->estoqueMinimo = lerInteiro("Novo estoque minimo: ", 0, 1000000);
    salvarProdutos();
    printf("Produto alterado com sucesso.\n");
}

static void excluirProduto(void) {
    int codigo = lerInteiro("Codigo do produto: ", 1, 999999);
    int pos = -1;
    for (int i = 0; i < totalProdutos; i++) if (produtos[i].codigo == codigo) { pos = i; break; }
    if (pos < 0) { printf("Produto nao encontrado.\n"); return; }
    if (produtoTemVendas(codigo)) {
        printf("Nao e possivel excluir: este produto aparece no historico de vendas.\n");
        return;
    }
    int confirma = lerInteiro("Confirmar exclusao? 1-Sim 0-Nao: ", 0, 1);
    if (!confirma) { printf("Exclusao cancelada.\n"); return; }
    for (int i = pos; i < totalProdutos - 1; i++) produtos[i] = produtos[i + 1];
    totalProdutos--;
    salvarProdutos();
    printf("Produto excluido com sucesso.\n");
}

static void consultarEstoque(void) {
    listarProdutosResumido();
    printf("\nProdutos com estoque baixo:\n");
    int achou = 0;
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].estoque <= produtos[i].estoqueMinimo) {
            printf("- [%d] %s: %d unidade(s), minimo %d\n", produtos[i].codigo, produtos[i].titulo, produtos[i].estoque, produtos[i].estoqueMinimo);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum produto em estoque baixo.\n");
}

void menuProdutos(void) {
    int opcao;
    do {
        limparTela();
        printf("========================================\n");
        printf("           LIVROS / PRODUTOS            \n");
        printf("========================================\n");
        printf("1 - Cadastrar\n2 - Listar\n3 - Buscar\n4 - Alterar\n5 - Excluir\n6 - Consultar estoque\n0 - Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 6);
        limparTela();
        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: buscarProduto(); break;
            case 4: alterarProduto(); break;
            case 5: excluirProduto(); break;
            case 6: consultarEstoque(); break;
        }
        if (opcao) pausar();
    } while (opcao != 0);
}
