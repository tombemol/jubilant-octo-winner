# DESIGN.md

## Direção
Interface operacional, sóbria e editorial. Deve parecer um sistema real de pequena empresa, não um template genérico de dashboard.

## Princípios
- Uma ação primária clara por contexto.
- Hierarquia antes de decoração.
- Evitar cards aninhados e excesso de chips.
- Tabelas para comparação; cards apenas para métricas ou agrupamentos relevantes.
- Densidade moderada: confortável em tablet sem desperdiçar espaço.
- Estados vazios explicam o próximo passo.
- Motion discreto e funcional.

## Cores
- Fundo claro: #f4f3ef
- Superfície: #ffffff
- Texto principal: #1d211f
- Texto secundário: #656b67
- Borda: #d9dcd8
- Ação: #174a3c
- Ação hover: #10382e
- Alerta: #a8492d
- Sucesso: #2f6a4f
- Dark background: #151816
- Dark surface: #1e221f

## Tipografia
- Família: system-ui / Segoe UI / sans-serif
- Título de página: 30-36px / 700
- Seção: 18-20px / 650
- Corpo: 14-16px / 400
- Meta: 12-13px / 500
- Números-chave usam tabular-nums.

## Forma
- Radius padrão: 10px
- Radius de controles: 8px
- Sem gradientes decorativos.
- Sombras leves apenas em menus/dialogs elevados.
- Focus ring sempre visível.

## Layout
- Sidebar persistente no desktop.
- Navegação inferior compacta em telas pequenas.
- Conteúdo máximo 1440px.
- Tabelas viram cartões de linha em telas estreitas.

## Componentes
- Button: primary, secondary, ghost, danger.
- Field: label sempre visível; ajuda e erro separados do placeholder.
- Metric: valor, rótulo e contexto; sem ícones inúteis.
- Table: cabeçalho firme, hover discreto, ações alinhadas à direita.
- Dialog: título, contexto curto, formulário, rodapé com cancelar + ação.
- Toast: feedback curto; nunca substitui mensagem de erro persistente.
