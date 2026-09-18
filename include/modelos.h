#ifndef MODELOS_H
#define MODELOS_H

#define MAX_CLIENTES 200
#define MAX_PRODUTOS 400
#define MAX_VENDAS 500
#define MAX_ITENS_VENDA 20

#define TAM_NOME 100
#define TAM_CPF 20
#define TAM_TELEFONE 24
#define TAM_EMAIL 120
#define TAM_CIDADE 80
#define TAM_TITULO 120
#define TAM_AUTOR 100
#define TAM_CATEGORIA 60
#define TAM_DATA 11

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char telefone[TAM_TELEFONE];
    char email[TAM_EMAIL];
    char cidade[TAM_CIDADE];
} Cliente;

typedef struct {
    int codigo;
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    char categoria[TAM_CATEGORIA];
    double preco;
    int estoque;
    int estoqueMinimo;
} Produto;

typedef struct {
    int codigoProduto;
    int quantidade;
    double precoUnitario;
    double subtotal;
} ItemVenda;

typedef struct {
    int codigo;
    int codigoCliente;
    char data[TAM_DATA];
    int quantidadeItens;
    ItemVenda itens[MAX_ITENS_VENDA];
    double total;
} Venda;

extern Cliente clientes[MAX_CLIENTES];
extern Produto produtos[MAX_PRODUTOS];
extern Venda vendas[MAX_VENDAS];
extern int totalClientes;
extern int totalProdutos;
extern int totalVendas;

#endif
