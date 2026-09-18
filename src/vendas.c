#include <stdio.h>
#include "modelos.h"
#include "clientes.h"
#include "produtos.h"
#include "arquivos.h"
#include "utils.h"

static int proximoCodigoVenda(void) {
    int maior = 0;
    for (int i = 0; i < totalVendas; i++) if (vendas[i].codigo > maior) maior = vendas[i].codigo;
    return maior + 1;
}

static void listarClientesResumido(void) {
    printf("\n%-6s %-35s %-18s\n", "COD", "CLIENTE", "CPF");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < totalClientes; i++)
        printf("%-6d %-35.35s %-18s\n", clientes[i].codigo, clientes[i].nome, clientes[i].cpf);
}

static void imprimirVenda(const Venda *v) {
    Cliente *c = buscarClientePorCodigo(v->codigoCliente);
    printf("\nVenda #%d | Data: %s\n", v->codigo, v->data);
    printf("Cliente: %s (codigo %d)\n", c ? c->nome : "Cliente removido", v->codigoCliente);
    printf("Itens:\n");
    for (int i = 0; i < v->quantidadeItens; i++) {
        Produto *p = buscarProdutoPorCodigo(v->itens[i].codigoProduto);
        printf("  - %s | %d x R$ %.2f = R$ %.2f\n",
               p ? p->titulo : "Produto removido",
               v->itens[i].quantidade,
               v->itens[i].precoUnitario,
               v->itens[i].subtotal);
    }
    printf("Total: R$ %.2f\n", v->total);
}

static void realizarVenda(void) {
    if (totalVendas >= MAX_VENDAS) { printf("Limite de vendas atingido.\n"); return; }
    if (totalClientes == 0 || totalProdutos == 0) {
        printf("Cadastre pelo menos um cliente e um produto antes de vender.\n");
        return;
    }

    listarClientesResumido();
    int codigoCliente = lerInteiro("\nCodigo do cliente: ", 1, 999999);
    Cliente *cliente = buscarClientePorCodigo(codigoCliente);
    if (!cliente) { printf("Cliente nao encontrado.\n"); return; }

    Venda nova = {0};
    nova.codigo = proximoCodigoVenda();
    nova.codigoCliente = codigoCliente;
    dataAtual(nova.data, TAM_DATA);

    int continuar = 1;
    while (continuar && nova.quantidadeItens < MAX_ITENS_VENDA) {
        listarProdutosResumido();
        int codigoProduto = lerInteiro("\nCodigo do produto: ", 1, 999999);
        Produto *produto = buscarProdutoPorCodigo(codigoProduto);
        if (!produto) { printf("Produto nao encontrado.\n"); continuar = lerInteiro("Tentar outro? 1-Sim 0-Nao: ", 0, 1); continue; }
        if (produto->estoque <= 0) { printf("Produto sem estoque.\n"); continuar = lerInteiro("Tentar outro? 1-Sim 0-Nao: ", 0, 1); continue; }

        int repetido = 0;
        for (int i = 0; i < nova.quantidadeItens; i++) if (nova.itens[i].codigoProduto == codigoProduto) repetido = 1;
        if (repetido) { printf("Esse produto ja foi adicionado. Escolha outro produto.\n"); continuar = 1; continue; }

        int quantidade = lerInteiro("Quantidade: ", 1, 1000000);
        if (quantidade > produto->estoque) {
            printf("Estoque insuficiente. Disponivel: %d.\n", produto->estoque);
            continuar = lerInteiro("Tentar outro item? 1-Sim 0-Nao: ", 0, 1);
            continue;
        }

        ItemVenda *item = &nova.itens[nova.quantidadeItens];
        item->codigoProduto = codigoProduto;
        item->quantidade = quantidade;
        item->precoUnitario = produto->preco;
        item->subtotal = quantidade * produto->preco;
        nova.total += item->subtotal;
        nova.quantidadeItens++;

        printf("Item adicionado: %s | Subtotal R$ %.2f\n", produto->titulo, item->subtotal);
        if (nova.quantidadeItens < MAX_ITENS_VENDA)
            continuar = lerInteiro("Adicionar outro produto? 1-Sim 0-Nao: ", 0, 1);
    }

    if (nova.quantidadeItens == 0) { printf("Venda cancelada: nenhum item adicionado.\n"); return; }

    printf("\nResumo da venda #%d para %s\n", nova.codigo, cliente->nome);
    printf("Total: R$ %.2f\n", nova.total);
    int confirma = lerInteiro("Finalizar venda? 1-Sim 0-Nao: ", 0, 1);
    if (!confirma) { printf("Venda cancelada. O estoque nao foi alterado.\n"); return; }

    for (int i = 0; i < nova.quantidadeItens; i++) {
        Produto *p = buscarProdutoPorCodigo(nova.itens[i].codigoProduto);
        if (p) p->estoque -= nova.itens[i].quantidade;
    }
    vendas[totalVendas++] = nova;
    salvarProdutos();
    salvarVendas();
    printf("Venda registrada com sucesso. Estoque atualizado.\n");
}

static void listarVendas(void) {
    printf("\n=== HISTORICO DE VENDAS ===\n");
    if (totalVendas == 0) { printf("Nenhuma venda registrada.\n"); return; }
    for (int i = 0; i < totalVendas; i++) imprimirVenda(&vendas[i]);
}

static void buscarVenda(void) {
    int codigo = lerInteiro("Codigo da venda: ", 1, 999999);
    for (int i = 0; i < totalVendas; i++) {
        if (vendas[i].codigo == codigo) { imprimirVenda(&vendas[i]); return; }
    }
    printf("Venda nao encontrada.\n");
}

void menuVendas(void) {
    int opcao;
    do {
        limparTela();
        printf("========================================\n");
        printf("                VENDAS                  \n");
        printf("========================================\n");
        printf("1 - Registrar venda\n2 - Historico de vendas\n3 - Buscar venda por codigo\n0 - Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 3);
        limparTela();
        switch (opcao) {
            case 1: realizarVenda(); break;
            case 2: listarVendas(); break;
            case 3: buscarVenda(); break;
        }
        if (opcao) pausar();
    } while (opcao != 0);
}
