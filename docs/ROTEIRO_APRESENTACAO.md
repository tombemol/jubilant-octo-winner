# Roteiro curto para apresentação

1. **Modelagem**: mostrar `Cliente`, `Produto`, `ItemVenda` e `Venda` em `include/modelos.h`.
2. **Modularização**: explicar por que `main.c` só coordena menus e cada módulo cuida de uma responsabilidade.
3. **Arquivos**: mostrar `src/arquivos.c` e explicar persistência binária em `dados/*.dat`.
4. **CRUD**: cadastrar e buscar um cliente; cadastrar e consultar um livro.
5. **Venda**: registrar uma venda, tentar uma quantidade maior que o estoque e mostrar o bloqueio; depois realizar uma venda válida.
6. **Estoque**: voltar aos produtos e mostrar que a quantidade diminuiu.
7. **Relatórios**: exibir total vendido, estoque, produto mais vendido, cliente com maior volume e estoque baixo.
8. **Recursão**: abrir `src/relatorios.c` e explicar os casos-base de `estoqueTotalRecursivo()` e `valorEstoqueRecursivo()`.
9. **Ponteiros**: usar `buscarClientePorCodigo()` ou `buscarProdutoPorCodigo()` como exemplo de função que retorna ponteiro para a struct encontrada.
10. **Persistência**: fechar o programa, abrir novamente e mostrar que os cadastros e vendas continuam disponíveis.
