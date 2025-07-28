#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int largura_mapa;
    int qtd_pistas;
} tMapa;

typedef struct
{
    int animacao;
    int estado;
    char *diretorio;
    tMapa mapa;
} tJogo;

tJogo jogo;
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao.");
        exit(1);
    }

    char dir[] = argv[2];

    printf("%s\n", dir);

    return 0;
}

tJogo LeConfigsJogo(int argc, char **argv)
{
}
