# Frontend demonstrativo

Interface web estática e opcional para apresentar visualmente o domínio do projeto.

Ela **não substitui o programa em C** e não lê os arquivos binários `dados/*.dat`. Os dados da demonstração ficam no `localStorage` do navegador.

## Executar

Abra `index.html` diretamente ou sirva esta pasta com um servidor local:

```bash
python -m http.server 8080 -d frontend
```

Depois acesse `http://localhost:8080`.

## Recursos

- dashboard com indicadores;
- CRUD demonstrativo de clientes e livros;
- registro simples de venda com validação de estoque;
- atualização automática dos indicadores;
- relatório de faturamento, estoque, produto mais vendido e melhor cliente;
- alerta de estoque baixo;
- light/dark mode;
- layout responsivo para desktop, tablet e celular.
