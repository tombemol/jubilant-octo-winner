#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    if (getenv("TERM") != NULL) system("clear");
#endif
}

void pausar(void) {
    char buffer[8];
    printf("\nPressione ENTER para continuar...");
    fgets(buffer, sizeof(buffer), stdin);
}

void lerLinha(const char *rotulo, char *destino, int tamanho) {
    while (1) {
        printf("%s", rotulo);
        if (fgets(destino, tamanho, stdin) == NULL) {
            destino[0] = '\0';
            continue;
        }
        destino[strcspn(destino, "\r\n")] = '\0';
        if (!stringVazia(destino)) return;
        printf("Entrada obrigatoria. Tente novamente.\n");
    }
}

int lerInteiro(const char *rotulo, int minimo, int maximo) {
    char linha[128];
    char *fim;
    long valor;
    while (1) {
        printf("%s", rotulo);
        if (!fgets(linha, sizeof(linha), stdin)) continue;
        valor = strtol(linha, &fim, 10);
        while (isspace((unsigned char)*fim)) fim++;
        if (fim != linha && *fim == '\0' && valor >= minimo && valor <= maximo) return (int)valor;
        printf("Valor invalido. Informe um numero entre %d e %d.\n", minimo, maximo);
    }
}

double lerDouble(const char *rotulo, double minimo) {
    char linha[128];
    char *fim;
    double valor;
    while (1) {
        printf("%s", rotulo);
        if (!fgets(linha, sizeof(linha), stdin)) continue;
        for (int i = 0; linha[i]; i++) if (linha[i] == ',') linha[i] = '.';
        valor = strtod(linha, &fim);
        while (isspace((unsigned char)*fim)) fim++;
        if (fim != linha && *fim == '\0' && valor >= minimo) return valor;
        printf("Valor invalido. Informe um numero maior ou igual a %.2f.\n", minimo);
    }
}

int textoContemIgnorandoCaixa(const char *texto, const char *termo) {
    if (!termo || !*termo) return 1;
    char a[256], b[256];
    size_t i;
    for (i = 0; texto[i] && i < sizeof(a)-1; i++) a[i] = (char)tolower((unsigned char)texto[i]);
    a[i] = '\0';
    for (i = 0; termo[i] && i < sizeof(b)-1; i++) b[i] = (char)tolower((unsigned char)termo[i]);
    b[i] = '\0';
    return strstr(a, b) != NULL;
}

int stringVazia(const char *texto) {
    if (!texto) return 1;
    while (*texto) {
        if (!isspace((unsigned char)*texto)) return 0;
        texto++;
    }
    return 1;
}

int validarCPFBasico(const char *cpf) {
    int digitos = 0;
    for (int i = 0; cpf[i]; i++) {
        if (isdigit((unsigned char)cpf[i])) digitos++;
        else if (cpf[i] != '.' && cpf[i] != '-') return 0;
    }
    return digitos == 11;
}

int validarEmailBasico(const char *email) {
    const char *arroba = strchr(email, '@');
    if (!arroba || arroba == email) return 0;
    const char *ponto = strchr(arroba + 1, '.');
    return ponto && ponto[1] != '\0';
}

void dataAtual(char *destino, int tamanho) {
    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    if (!info) {
        snprintf(destino, tamanho, "00/00/0000");
        return;
    }
    strftime(destino, (size_t)tamanho, "%d/%m/%Y", info);
}
