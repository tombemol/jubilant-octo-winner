#ifndef UTILS_H
#define UTILS_H

void limparTela(void);
void pausar(void);
void lerLinha(const char *rotulo, char *destino, int tamanho);
int lerInteiro(const char *rotulo, int minimo, int maximo);
double lerDouble(const char *rotulo, double minimo);
int textoContemIgnorandoCaixa(const char *texto, const char *termo);
int stringVazia(const char *texto);
int validarCPFBasico(const char *cpf);
int validarEmailBasico(const char *email);
void dataAtual(char *destino, int tamanho);

#endif
