#include <stdio.h>
#include "modelos.h"
#include "arquivos.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#define MKDIR(p) mkdir(p, 0777)
#endif

#define ARQ_CLIENTES "dados/clientes.dat"
#define ARQ_PRODUTOS "dados/produtos.dat"
#define ARQ_VENDAS "dados/vendas.dat"

void criarPastaDados(void) { MKDIR("dados"); }

static void carregarClientes(void) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) return;
    if (fread(&totalClientes, sizeof totalClientes, 1, f) != 1) { totalClientes = 0; fclose(f); return; }
    if (totalClientes < 0 || totalClientes > MAX_CLIENTES) totalClientes = 0;
    fread(clientes, sizeof(Cliente), (size_t)totalClientes, f);
    fclose(f);
}

static void carregarProdutos(void) {
    FILE *f = fopen(ARQ_PRODUTOS, "rb");
    if (!f) return;
    if (fread(&totalProdutos, sizeof totalProdutos, 1, f) != 1) { totalProdutos = 0; fclose(f); return; }
    if (totalProdutos < 0 || totalProdutos > MAX_PRODUTOS) totalProdutos = 0;
    fread(produtos, sizeof(Produto), (size_t)totalProdutos, f);
    fclose(f);
}

static void carregarVendas(void) {
    FILE *f = fopen(ARQ_VENDAS, "rb");
    if (!f) return;
    if (fread(&totalVendas, sizeof totalVendas, 1, f) != 1) { totalVendas = 0; fclose(f); return; }
    if (totalVendas < 0 || totalVendas > MAX_VENDAS) totalVendas = 0;
    fread(vendas, sizeof(Venda), (size_t)totalVendas, f);
    fclose(f);
}

void salvarClientes(void) {
    FILE *f = fopen(ARQ_CLIENTES, "wb");
    if (!f) { printf("Erro ao salvar clientes.\n"); return; }
    fwrite(&totalClientes, sizeof totalClientes, 1, f);
    fwrite(clientes, sizeof(Cliente), (size_t)totalClientes, f);
    fclose(f);
}

void salvarProdutos(void) {
    FILE *f = fopen(ARQ_PRODUTOS, "wb");
    if (!f) { printf("Erro ao salvar produtos.\n"); return; }
    fwrite(&totalProdutos, sizeof totalProdutos, 1, f);
    fwrite(produtos, sizeof(Produto), (size_t)totalProdutos, f);
    fclose(f);
}

void salvarVendas(void) {
    FILE *f = fopen(ARQ_VENDAS, "wb");
    if (!f) { printf("Erro ao salvar vendas.\n"); return; }
    fwrite(&totalVendas, sizeof totalVendas, 1, f);
    fwrite(vendas, sizeof(Venda), (size_t)totalVendas, f);
    fclose(f);
}

void carregarDados(void) { carregarClientes(); carregarProdutos(); carregarVendas(); }
void salvarDados(void) { salvarClientes(); salvarProdutos(); salvarVendas(); }
