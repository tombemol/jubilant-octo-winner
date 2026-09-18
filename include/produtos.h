#ifndef PRODUTOS_H
#define PRODUTOS_H

#include "modelos.h"

Produto *buscarProdutoPorCodigo(int codigo);
int produtoTemVendas(int codigoProduto);
void listarProdutosResumido(void);
void menuProdutos(void);

#endif
