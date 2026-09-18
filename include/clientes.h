#ifndef CLIENTES_H
#define CLIENTES_H

#include "modelos.h"

Cliente *buscarClientePorCodigo(int codigo);
int clienteTemVendas(int codigoCliente);
void menuClientes(void);

#endif
