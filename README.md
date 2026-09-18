# Livraria em C

Sistema de Gerenciamento Empresarial para uma livraria, desenvolvido em **C11** para o trabalho prático de Algoritmos e Programação II.

O repositório contém dois artefatos independentes:

- **aplicação principal em C**, responsável pelo projeto acadêmico e por todos os requisitos funcionais;
- **frontend web demonstrativo**, usado apenas para apresentar o domínio com uma interface visual responsiva.

## Funcionalidades do programa em C

- cadastro, listagem, busca, alteração e exclusão de clientes;
- busca de clientes por código, nome ou CPF;
- cadastro, listagem, busca, alteração e exclusão de livros/produtos;
- controle de estoque e alerta de estoque baixo;
- vendas com múltiplos itens, cliente, data, quantidades e total;
- validação de estoque antes da venda;
- baixa automática do estoque ao confirmar a venda;
- histórico e busca de vendas;
- persistência em arquivos binários;
- relatórios de clientes, produtos, estoque, vendas e faturamento;
- produto mais vendido;
- cliente com maior volume de compras;
- recursão para cálculo de quantidade e valor total do estoque;
- tratamento centralizado de entradas inválidas.

## Estrutura

```text
livraria-em-c/
├── include/            # contratos, structs e cabeçalhos
├── src/                # implementação C
├── dados/              # arquivos .dat gerados em execução
├── docs/               # matriz de requisitos e roteiro de apresentação
├── frontend/           # demonstração visual opcional
├── DESIGN.md           # sistema visual do frontend
├── PRODUCT.md          # contexto de produto do frontend
├── Makefile
├── build.sh
└── build.ps1
```

## Compilar

### Linux / macOS

```bash
./build.sh
./build/livraria
```

Ou:

```bash
make
./build/livraria
```

### Windows com GCC/MinGW

```powershell
.\build.ps1
.\build\livraria.exe
```

Também é possível compilar manualmente:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude src/*.c -o livraria
```

## Persistência

Na primeira execução, o programa cria/usa a pasta `dados/`. Os arquivos são:

- `clientes.dat`
- `produtos.dat`
- `vendas.dat`

Eles são binários e não são versionados no Git.

## Onde cada conteúdo da disciplina aparece

| Conteúdo | Exemplo |
|---|---|
| Funções | todos os módulos |
| `struct` | `include/modelos.h` |
| Ponteiros | buscas retornando `Cliente*` e `Produto*`; manipulação de itens |
| Arquivos | `src/arquivos.c` |
| Strings/caracteres | cadastros, buscas e validações |
| Recursão | `estoqueTotalRecursivo()` e `valorEstoqueRecursivo()` |
| Vetores | coleções de clientes, produtos, vendas e itens |
| Modularização | separação por `.c` + `.h` |

## Frontend demonstrativo

```bash
python -m http.server 8080 -d frontend
```

Acesse `http://localhost:8080`.

O frontend usa `localStorage` para demonstrar cadastros, estoque e vendas. Ele é deliberadamente separado dos arquivos `.dat` para não introduzir uma camada web que o trabalho em C não exige.

## Decisões de segurança de dados

Clientes e produtos que já aparecem em vendas não podem ser excluídos. Isso preserva o histórico e evita referências quebradas nos relatórios.

## Documentação de apoio

- `docs/REQUISITOS.md`: rastreia cada requisito do enunciado até o código.
- `docs/ROTEIRO_APRESENTACAO.md`: sequência curta para demonstrar o projeto e explicar os conceitos cobrados.
- `PRODUCT.md` e `DESIGN.md`: contexto e regras visuais do frontend demonstrativo.
