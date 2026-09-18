#include <stdio.h>
#include <string.h>
#include "modelos.h"
#include "clientes.h"
#include "arquivos.h"
#include "utils.h"

static int proximoCodigoCliente(void) {
    int maior = 0;
    for (int i = 0; i < totalClientes; i++) if (clientes[i].codigo > maior) maior = clientes[i].codigo;
    return maior + 1;
}

Cliente *buscarClientePorCodigo(int codigo) {
    for (int i = 0; i < totalClientes; i++) if (clientes[i].codigo == codigo) return &clientes[i];
    return NULL;
}

static int cpfJaExiste(const char *cpf, int ignorarCodigo) {
    for (int i = 0; i < totalClientes; i++) {
        if (clientes[i].codigo != ignorarCodigo && strcmp(clientes[i].cpf, cpf) == 0) return 1;
    }
    return 0;
}

int clienteTemVendas(int codigoCliente) {
    for (int i = 0; i < totalVendas; i++) if (vendas[i].codigoCliente == codigoCliente) return 1;
    return 0;
}

static void imprimirCliente(const Cliente *c) {
    printf("\n[%d] %s\n", c->codigo, c->nome);
    printf("CPF: %s | Telefone: %s\n", c->cpf, c->telefone);
    printf("E-mail: %s | Cidade: %s\n", c->email, c->cidade);
}

static void cadastrarCliente(void) {
    if (totalClientes >= MAX_CLIENTES) { printf("Limite de clientes atingido.\n"); return; }
    Cliente c = {0};
    c.codigo = proximoCodigoCliente();
    printf("\n=== CADASTRO DE CLIENTE ===\nCodigo gerado: %d\n", c.codigo);
    lerLinha("Nome: ", c.nome, TAM_NOME);
    do {
        lerLinha("CPF (11 digitos, pontuacao opcional): ", c.cpf, TAM_CPF);
        if (!validarCPFBasico(c.cpf)) printf("CPF invalido.\n");
        else if (cpfJaExiste(c.cpf, -1)) printf("CPF ja cadastrado.\n");
        else break;
    } while (1);
    lerLinha("Telefone: ", c.telefone, TAM_TELEFONE);
    do {
        lerLinha("E-mail: ", c.email, TAM_EMAIL);
        if (!validarEmailBasico(c.email)) printf("E-mail invalido.\n");
        else break;
    } while (1);
    lerLinha("Cidade: ", c.cidade, TAM_CIDADE);
    clientes[totalClientes++] = c;
    salvarClientes();
    printf("Cliente cadastrado com sucesso.\n");
}

static void listarClientes(void) {
    printf("\n=== CLIENTES ===\n");
    if (totalClientes == 0) { printf("Nenhum cliente cadastrado.\n"); return; }
    for (int i = 0; i < totalClientes; i++) imprimirCliente(&clientes[i]);
}

static void buscarCliente(void) {
    if (totalClientes == 0) { printf("Nenhum cliente cadastrado.\n"); return; }
    int opcao = lerInteiro("Buscar por 1-Codigo  2-Nome  3-CPF: ", 1, 3);
    if (opcao == 1) {
        int codigo = lerInteiro("Codigo: ", 1, 999999);
        Cliente *c = buscarClientePorCodigo(codigo);
        if (c) imprimirCliente(c); else printf("Cliente nao encontrado.\n");
        return;
    }
    char termo[120];
    lerLinha(opcao == 2 ? "Nome ou parte do nome: " : "CPF: ", termo, sizeof termo);
    int achou = 0;
    for (int i = 0; i < totalClientes; i++) {
        int ok = opcao == 2 ? textoContemIgnorandoCaixa(clientes[i].nome, termo) : strcmp(clientes[i].cpf, termo) == 0;
        if (ok) { imprimirCliente(&clientes[i]); achou = 1; }
    }
    if (!achou) printf("Nenhum cliente encontrado.\n");
}

static void alterarCliente(void) {
    int codigo = lerInteiro("Codigo do cliente: ", 1, 999999);
    Cliente *c = buscarClientePorCodigo(codigo);
    if (!c) { printf("Cliente nao encontrado.\n"); return; }
    printf("Alterando: %s\n", c->nome);
    lerLinha("Novo nome: ", c->nome, TAM_NOME);
    do {
        lerLinha("Novo CPF: ", c->cpf, TAM_CPF);
        if (!validarCPFBasico(c->cpf)) printf("CPF invalido.\n");
        else if (cpfJaExiste(c->cpf, c->codigo)) printf("CPF ja pertence a outro cliente.\n");
        else break;
    } while (1);
    lerLinha("Novo telefone: ", c->telefone, TAM_TELEFONE);
    do {
        lerLinha("Novo e-mail: ", c->email, TAM_EMAIL);
        if (!validarEmailBasico(c->email)) printf("E-mail invalido.\n"); else break;
    } while (1);
    lerLinha("Nova cidade: ", c->cidade, TAM_CIDADE);
    salvarClientes();
    printf("Cliente alterado com sucesso.\n");
}

static void excluirCliente(void) {
    int codigo = lerInteiro("Codigo do cliente: ", 1, 999999);
    int pos = -1;
    for (int i = 0; i < totalClientes; i++) if (clientes[i].codigo == codigo) { pos = i; break; }
    if (pos < 0) { printf("Cliente nao encontrado.\n"); return; }
    if (clienteTemVendas(codigo)) {
        printf("Nao e possivel excluir: este cliente possui vendas registradas.\n");
        return;
    }
    int confirma = lerInteiro("Confirmar exclusao? 1-Sim 0-Nao: ", 0, 1);
    if (!confirma) { printf("Exclusao cancelada.\n"); return; }
    for (int i = pos; i < totalClientes - 1; i++) clientes[i] = clientes[i + 1];
    totalClientes--;
    salvarClientes();
    printf("Cliente excluido com sucesso.\n");
}

void menuClientes(void) {
    int opcao;
    do {
        limparTela();
        printf("========================================\n");
        printf("              CLIENTES                  \n");
        printf("========================================\n");
        printf("1 - Cadastrar\n2 - Listar\n3 - Buscar\n4 - Alterar\n5 - Excluir\n0 - Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 5);
        limparTela();
        switch (opcao) {
            case 1: cadastrarCliente(); break;
            case 2: listarClientes(); break;
            case 3: buscarCliente(); break;
            case 4: alterarCliente(); break;
            case 5: excluirCliente(); break;
        }
        if (opcao) pausar();
    } while (opcao != 0);
}
