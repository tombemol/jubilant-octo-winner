const STORAGE_KEY = 'livraria-c-demo-v1';
const $ = (q, root = document) => root.querySelector(q);
const $$ = (q, root = document) => [...root.querySelectorAll(q)];
const money = value => new Intl.NumberFormat('pt-BR', { style: 'currency', currency: 'BRL' }).format(value || 0);
const today = () => new Intl.DateTimeFormat('pt-BR').format(new Date());

const seed = {
  clientes: [
    { codigo: 1, nome: 'Marina Alves', cpf: '123.456.789-01', telefone: '(75) 99911-2233', email: 'marina@example.com', cidade: 'Feira de Santana' },
    { codigo: 2, nome: 'Caio Nascimento', cpf: '987.654.321-00', telefone: '(75) 98844-1122', email: 'caio@example.com', cidade: 'Santo Amaro' },
    { codigo: 3, nome: 'Lívia Rocha', cpf: '741.852.963-10', telefone: '(71) 97710-0303', email: 'livia@example.com', cidade: 'Salvador' }
  ],
  produtos: [
    { codigo: 1, titulo: 'Torto Arado', autor: 'Itamar Vieira Junior', categoria: 'Romance', preco: 54.9, estoque: 12, estoqueMinimo: 5 },
    { codigo: 2, titulo: 'O Avesso da Pele', autor: 'Jeferson Tenório', categoria: 'Literatura brasileira', preco: 49.9, estoque: 3, estoqueMinimo: 5 },
    { codigo: 3, titulo: 'A Hora da Estrela', autor: 'Clarice Lispector', categoria: 'Clássico', preco: 39.9, estoque: 9, estoqueMinimo: 4 },
    { codigo: 4, titulo: 'Quarto de Despejo', autor: 'Carolina Maria de Jesus', categoria: 'Memórias', preco: 44.9, estoque: 2, estoqueMinimo: 4 }
  ],
  vendas: [
    { codigo: 1, codigoCliente: 1, data: today(), itens: [{ codigoProduto: 1, quantidade: 1, precoUnitario: 54.9, subtotal: 54.9 }], total: 54.9 },
    { codigo: 2, codigoCliente: 2, data: today(), itens: [{ codigoProduto: 3, quantidade: 2, precoUnitario: 39.9, subtotal: 79.8 }], total: 79.8 }
  ]
};

let state = load();
let currentView = 'dashboard';
let toastTimer;

function load() {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    return raw ? JSON.parse(raw) : structuredClone(seed);
  } catch {
    return structuredClone(seed);
  }
}
function save() { localStorage.setItem(STORAGE_KEY, JSON.stringify(state)); }
function nextCode(items) { return items.reduce((m, item) => Math.max(m, item.codigo), 0) + 1; }
function clientByCode(code) { return state.clientes.find(x => x.codigo === Number(code)); }
function productByCode(code) { return state.produtos.find(x => x.codigo === Number(code)); }
function totalRevenue() { return state.vendas.reduce((sum, v) => sum + v.total, 0); }
function stockValue() { return state.produtos.reduce((sum, p) => sum + p.preco * p.estoque, 0); }
function lowStock() { return state.produtos.filter(p => p.estoque <= p.estoqueMinimo).sort((a,b) => a.estoque - b.estoque); }

function showToast(message) {
  const el = $('#toast');
  el.textContent = message;
  el.classList.add('show');
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => el.classList.remove('show'), 2600);
}

function setView(view) {
  currentView = view;
  $$('.nav-item').forEach(btn => btn.classList.toggle('active', btn.dataset.view === view));
  $$('.view').forEach(section => section.classList.toggle('active', section.id === `view-${view}`));
  const labels = {
    dashboard: ['OPERAÇÃO', 'Visão geral', 'Nova venda'],
    clientes: ['CADASTRO', 'Clientes', 'Cadastrar cliente'],
    produtos: ['CATÁLOGO', 'Livros', 'Cadastrar livro'],
    vendas: ['CAIXA', 'Vendas', 'Registrar venda'],
    relatorios: ['ANÁLISE', 'Relatórios', 'Nova venda']
  };
  const [eyebrow, title, action] = labels[view];
  $('#pageEyebrow').textContent = eyebrow;
  $('#pageTitle').textContent = title;
  $('#primaryAction').textContent = action;
  renderAll();
  window.scrollTo({ top: 0, behavior: 'smooth' });
}

function renderDashboard() {
  const metrics = [
    ['Clientes', state.clientes.length, 'cadastros ativos'],
    ['Livros', state.produtos.length, `${state.produtos.reduce((s,p)=>s+p.estoque,0)} un. em estoque`],
    ['Vendas', state.vendas.length, 'registros no histórico'],
    ['Faturamento', money(totalRevenue()), `estoque avaliado em ${money(stockValue())}`]
  ];
  $('#metricGrid').innerHTML = metrics.map(([label, value, meta]) => `<article class="metric"><span>${label}</span><strong>${value}</strong><small>${meta}</small></article>`).join('');

  const lows = lowStock();
  $('#lowStockList').innerHTML = lows.length ? lows.map(p => `<div class="data-row"><div class="row-main"><strong>${escapeHtml(p.titulo)}</strong><span>${escapeHtml(p.autor)} · mínimo ${p.estoqueMinimo}</span></div><span class="stock-indicator">${p.estoque} un.</span></div>`).join('') : '<div class="data-row"><div class="row-main"><strong>Estoque saudável</strong><span>Nenhum livro abaixo do mínimo.</span></div></div>';

  const recent = [...state.vendas].reverse().slice(0, 5);
  $('#recentSales').innerHTML = recent.length ? recent.map(v => {
    const c = clientByCode(v.codigoCliente);
    return `<div class="activity-row"><div><strong>Venda #${v.codigo}</strong><span>${escapeHtml(c?.nome || 'Cliente')} · ${v.data}</span></div><strong>${money(v.total)}</strong></div>`;
  }).join('') : '<div class="activity-row"><div><strong>Sem movimento</strong><span>As vendas recentes aparecem aqui.</span></div></div>';
}

function renderClients() {
  const term = ($('#clientSearch').value || '').toLowerCase().trim();
  const rows = state.clientes.filter(c => [c.nome,c.cpf,c.cidade].some(v => v.toLowerCase().includes(term)));
  $('#clientsTable').innerHTML = rows.map(c => `<tr><td>#${c.codigo}</td><td><span class="cell-title">${escapeHtml(c.nome)}</span><span class="cell-meta">${escapeHtml(c.email)}</span></td><td>${escapeHtml(c.cpf)}</td><td>${escapeHtml(c.telefone)}</td><td>${escapeHtml(c.cidade)}</td><td><div class="row-actions"><button data-edit-client="${c.codigo}">Editar</button><button class="delete" data-delete-client="${c.codigo}">Excluir</button></div></td></tr>`).join('');
  $('#clientsEmpty').hidden = rows.length > 0;
}

function renderProducts() {
  const term = ($('#productSearch').value || '').toLowerCase().trim();
  const rows = state.produtos.filter(p => [p.titulo,p.autor,p.categoria].some(v => v.toLowerCase().includes(term)));
  $('#productsTable').innerHTML = rows.map(p => `<tr><td>#${p.codigo}</td><td><span class="cell-title">${escapeHtml(p.titulo)}</span><span class="cell-meta">${escapeHtml(p.autor)}</span></td><td>${escapeHtml(p.categoria)}</td><td>${money(p.preco)}</td><td class="${p.estoque <= p.estoqueMinimo ? 'status-low' : ''}">${p.estoque} un.<span class="cell-meta">mín. ${p.estoqueMinimo}</span></td><td><div class="row-actions"><button data-edit-product="${p.codigo}">Editar</button><button class="delete" data-delete-product="${p.codigo}">Excluir</button></div></td></tr>`).join('');
  $('#productsEmpty').hidden = rows.length > 0;
}

function renderSales() {
  $('#salesTable').innerHTML = [...state.vendas].reverse().map(v => {
    const c = clientByCode(v.codigoCliente);
    const qtd = v.itens.reduce((s,i)=>s+i.quantidade,0);
    return `<tr><td><span class="cell-title">#${v.codigo}</span></td><td>${v.data}</td><td>${escapeHtml(c?.nome || 'Cliente')}</td><td>${qtd} item(ns)</td><td><strong>${money(v.total)}</strong></td></tr>`;
  }).join('');
  $('#salesEmpty').hidden = state.vendas.length > 0;
}

function bestProduct() {
  const map = new Map();
  state.vendas.forEach(v => v.itens.forEach(i => map.set(i.codigoProduto, (map.get(i.codigoProduto) || 0) + i.quantidade)));
  const best = [...map.entries()].sort((a,b)=>b[1]-a[1])[0];
  if (!best) return null;
  return { produto: productByCode(best[0]), quantidade: best[1] };
}
function bestClient() {
  const map = new Map();
  state.vendas.forEach(v => map.set(v.codigoCliente, (map.get(v.codigoCliente) || 0) + v.total));
  const best = [...map.entries()].sort((a,b)=>b[1]-a[1])[0];
  if (!best) return null;
  return { cliente: clientByCode(best[0]), valor: best[1] };
}
function renderReports() {
  const bp = bestProduct();
  const bc = bestClient();
  const cards = [
    ['Valor estimado do estoque', money(stockValue()), `${state.produtos.reduce((s,p)=>s+p.estoque,0)} unidade(s)`],
    ['Valor total vendido', money(totalRevenue()), `${state.vendas.length} venda(s)`],
    ['Produto mais vendido', bp?.produto?.titulo || 'Sem dados', bp ? `${bp.quantidade} unidade(s)` : 'Ainda sem vendas'],
    ['Maior volume de compras', bc?.cliente?.nome || 'Sem dados', bc ? money(bc.valor) : 'Ainda sem vendas'],
    ['Clientes cadastrados', state.clientes.length, 'cadastros disponíveis'],
    ['Estoque baixo', lowStock().length, 'produto(s) pedindo atenção']
  ];
  $('#reportGrid').innerHTML = cards.map(([label,value,meta]) => `<article class="report-card"><span>${label}</span><strong>${escapeHtml(String(value))}</strong><small>${escapeHtml(String(meta))}</small></article>`).join('');
  const lows = lowStock();
  $('#reportLowStock').innerHTML = lows.length ? lows.map(p => `<div class="data-row"><div class="row-main"><strong>${escapeHtml(p.titulo)}</strong><span>${escapeHtml(p.autor)} · ${money(p.preco)}</span></div><span class="stock-indicator">${p.estoque}/${p.estoqueMinimo}</span></div>`).join('') : '<div class="data-row"><div class="row-main"><strong>Nenhum alerta</strong><span>Todos os produtos estão acima do estoque mínimo.</span></div></div>';
}

function renderSaleOptions() {
  $('#saleClient').innerHTML = '<option value="">Selecione um cliente</option>' + state.clientes.map(c => `<option value="${c.codigo}">${escapeHtml(c.nome)}</option>`).join('');
  $('#saleProduct').innerHTML = '<option value="">Selecione um livro</option>' + state.produtos.filter(p=>p.estoque>0).map(p => `<option value="${p.codigo}">${escapeHtml(p.titulo)} · ${p.estoque} un.</option>`).join('');
  updateSaleSummary();
}

function renderAll() {
  renderDashboard();
  renderClients();
  renderProducts();
  renderSales();
  renderReports();
  renderSaleOptions();
}

function openClientDialog(client = null) {
  const form = $('#clientForm');
  form.reset(); $('#clientError').textContent = '';
  $('#clientDialogTitle').textContent = client ? 'Editar cliente' : 'Cadastrar cliente';
  form.codigo.value = client?.codigo || '';
  ['nome','cpf','telefone','email','cidade'].forEach(k => form[k].value = client?.[k] || '');
  $('#clientDialog').showModal();
}
function openProductDialog(product = null) {
  const form = $('#productForm');
  form.reset(); $('#productError').textContent = '';
  $('#productDialogTitle').textContent = product ? 'Editar livro' : 'Cadastrar livro';
  form.codigo.value = product?.codigo || '';
  ['titulo','autor','categoria','preco','estoque','estoqueMinimo'].forEach(k => form[k].value = product?.[k] ?? '');
  $('#productDialog').showModal();
}
function openSaleDialog() {
  if (!state.clientes.length || !state.produtos.some(p=>p.estoque>0)) {
    showToast('Cadastre um cliente e um livro com estoque antes da venda.'); return;
  }
  $('#saleForm').reset(); $('#saleError').textContent = ''; renderSaleOptions(); $('#saleDialog').showModal();
}

function validateCpf(cpf, editingCode = null) {
  const digits = cpf.replace(/\D/g, '');
  if (digits.length !== 11) return 'Informe um CPF com 11 dígitos.';
  const duplicate = state.clientes.some(c => c.codigo !== editingCode && c.cpf.replace(/\D/g,'') === digits);
  return duplicate ? 'Esse CPF já está cadastrado.' : '';
}

$('#clientForm').addEventListener('submit', e => {
  e.preventDefault();
  const f = e.currentTarget; const codigo = Number(f.codigo.value) || null;
  const cpfError = validateCpf(f.cpf.value, codigo);
  if (cpfError) { $('#clientError').textContent = cpfError; return; }
  const item = { codigo: codigo || nextCode(state.clientes), nome: f.nome.value.trim(), cpf: f.cpf.value.trim(), telefone: f.telefone.value.trim(), email: f.email.value.trim(), cidade: f.cidade.value.trim() };
  if (codigo) state.clientes[state.clientes.findIndex(c=>c.codigo===codigo)] = item; else state.clientes.push(item);
  save(); $('#clientDialog').close(); renderAll(); showToast(codigo ? 'Cliente atualizado.' : 'Cliente cadastrado.');
});

$('#productForm').addEventListener('submit', e => {
  e.preventDefault();
  const f = e.currentTarget; const codigo = Number(f.codigo.value) || null;
  const item = { codigo: codigo || nextCode(state.produtos), titulo: f.titulo.value.trim(), autor: f.autor.value.trim(), categoria: f.categoria.value.trim(), preco: Number(f.preco.value), estoque: Number(f.estoque.value), estoqueMinimo: Number(f.estoqueMinimo.value) };
  if ([item.preco,item.estoque,item.estoqueMinimo].some(v => Number.isNaN(v) || v < 0)) { $('#productError').textContent = 'Preço e estoque não podem ser negativos.'; return; }
  if (codigo) state.produtos[state.produtos.findIndex(p=>p.codigo===codigo)] = item; else state.produtos.push(item);
  save(); $('#productDialog').close(); renderAll(); showToast(codigo ? 'Livro atualizado.' : 'Livro cadastrado.');
});

function updateSaleSummary() {
  const f = $('#saleForm');
  const p = productByCode(f.produto?.value);
  const qtd = Number(f.quantidade?.value || 0);
  $('#saleSummary').innerHTML = p ? `<span>${escapeHtml(p.titulo)} · ${p.estoque} disponível(is)</span><br><strong>Total previsto: ${money(p.preco * qtd)}</strong>` : '<span>Selecione um livro para ver estoque e total.</span>';
}
$('#saleProduct').addEventListener('change', updateSaleSummary);
$('#saleForm').quantidade.addEventListener('input', updateSaleSummary);
$('#saleForm').addEventListener('submit', e => {
  e.preventDefault();
  const f = e.currentTarget;
  const cliente = clientByCode(f.cliente.value), produto = productByCode(f.produto.value), quantidade = Number(f.quantidade.value);
  if (!cliente || !produto) { $('#saleError').textContent = 'Selecione cliente e livro.'; return; }
  if (!Number.isInteger(quantidade) || quantidade <= 0) { $('#saleError').textContent = 'A quantidade precisa ser maior que zero.'; return; }
  if (quantidade > produto.estoque) { $('#saleError').textContent = `Estoque insuficiente. Disponível: ${produto.estoque}.`; return; }
  const subtotal = produto.preco * quantidade;
  state.vendas.push({ codigo: nextCode(state.vendas), codigoCliente: cliente.codigo, data: today(), itens: [{ codigoProduto: produto.codigo, quantidade, precoUnitario: produto.preco, subtotal }], total: subtotal });
  produto.estoque -= quantidade;
  save(); $('#saleDialog').close(); renderAll(); showToast(`Venda finalizada: ${money(subtotal)}.`);
});

function handleClick(e) {
  const nav = e.target.closest('[data-view]'); if (nav) return setView(nav.dataset.view);
  const go = e.target.closest('[data-go]'); if (go) return setView(go.dataset.go);
  const opener = e.target.closest('[data-open]'); if (opener) {
    if (opener.dataset.open === 'clientDialog') openClientDialog();
    if (opener.dataset.open === 'productDialog') openProductDialog();
    if (opener.dataset.open === 'saleDialog') openSaleDialog();
    return;
  }
  const editClient = e.target.closest('[data-edit-client]'); if (editClient) return openClientDialog(clientByCode(editClient.dataset.editClient));
  const editProduct = e.target.closest('[data-edit-product]'); if (editProduct) return openProductDialog(productByCode(editProduct.dataset.editProduct));
  const delClient = e.target.closest('[data-delete-client]'); if (delClient) {
    const codigo = Number(delClient.dataset.deleteClient);
    if (state.vendas.some(v => v.codigoCliente === codigo)) return showToast('Cliente possui vendas e não pode ser excluído.');
    if (confirm('Excluir este cliente?')) { state.clientes = state.clientes.filter(c=>c.codigo!==codigo); save(); renderAll(); showToast('Cliente excluído.'); }
    return;
  }
  const delProduct = e.target.closest('[data-delete-product]'); if (delProduct) {
    const codigo = Number(delProduct.dataset.deleteProduct);
    if (state.vendas.some(v => v.itens.some(i=>i.codigoProduto===codigo))) return showToast('Livro aparece em vendas e não pode ser excluído.');
    if (confirm('Excluir este livro?')) { state.produtos = state.produtos.filter(p=>p.codigo!==codigo); save(); renderAll(); showToast('Livro excluído.'); }
  }
}
document.addEventListener('click', handleClick);
$('#clientSearch').addEventListener('input', renderClients);
$('#productSearch').addEventListener('input', renderProducts);

$('#primaryAction').addEventListener('click', () => {
  if (currentView === 'clientes') openClientDialog();
  else if (currentView === 'produtos') openProductDialog();
  else openSaleDialog();
});

$('#themeToggle').addEventListener('click', () => {
  const html = document.documentElement;
  const next = html.dataset.theme === 'dark' ? 'light' : 'dark';
  html.dataset.theme = next;
  localStorage.setItem('livraria-c-theme', next);
  $('#themeLabel').textContent = next === 'dark' ? 'claro' : 'escuro';
});

function escapeHtml(value) {
  return String(value ?? '').replace(/[&<>'"]/g, ch => ({'&':'&amp;','<':'&lt;','>':'&gt;',"'":'&#39;','"':'&quot;'}[ch]));
}

const preferredTheme = localStorage.getItem('livraria-c-theme') || (matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light');
document.documentElement.dataset.theme = preferredTheme;
$('#themeLabel').textContent = preferredTheme === 'dark' ? 'claro' : 'escuro';
renderAll();
