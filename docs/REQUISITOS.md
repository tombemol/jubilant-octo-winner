# Matriz de requisitos do trabalho

| Requisito | Implementação |
|---|---|
| Menu principal | `src/main.c` |
| CRUD de clientes | `src/clientes.c` |
| Busca de cliente por código, nome ou CPF | `src/clientes.c` |
| Cliente com cidade | `Cliente` em `include/modelos.h` |
| CRUD de produtos | `src/produtos.c` |
| Consulta e alerta de estoque baixo | `src/produtos.c` |
| Venda com vários produtos | `src/vendas.c` |
| Código, cliente, data, itens, quantidades e total da venda | `Venda` e `ItemVenda` em `include/modelos.h` |
| Baixa de estoque após venda | `src/vendas.c` |
| Bloqueio de venda acima do estoque | `src/vendas.c` |
| Dados persistentes | `src/arquivos.c` (`dados/*.dat`) |
| Structs | `include/modelos.h` |
| Ponteiros | CRUD, buscas e manipulação de itens |
| Funções e modularização | módulos em `src/` e `include/` |
| Strings/caracteres | `src/utils.c` e módulos de cadastro/busca |
| Recursão | `estoqueTotalRecursivo()` e `valorEstoqueRecursivo()` |
| Total de clientes | `src/relatorios.c` |
| Total de produtos | `src/relatorios.c` |
| Valor estimado do estoque | `src/relatorios.c` |
| Quantidade de vendas | `src/relatorios.c` |
| Valor total vendido | `src/relatorios.c` |
| Produto mais vendido | `src/relatorios.c` |
| Cliente com maior volume de compras | `src/relatorios.c` |
| Produtos com estoque baixo | `src/relatorios.c` |
| Validação de códigos duplicados | códigos são gerados automaticamente a partir do maior ID |
| Preço não negativo | `lerDouble(..., 0.0)` |
| Quantidade válida | `lerInteiro()` + regras de venda |
| Cliente/produto existente antes da venda | `src/vendas.c` |
| Tratamento de entrada inválida | `src/utils.c` |
