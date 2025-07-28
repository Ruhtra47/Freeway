#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_CONFIG_NOME "config_inicial.txt"

typedef struct
{
    int largura_mapa;
    int qtd_pistas;
} tMapa;

typedef struct
{
    int animacao;
    int estado;
    char diretorio[1000];
    tMapa mapa;
} tJogo;

tJogo LeDiretorioJogo(int argc, char *argv[]);
tJogo LeConfigInicialJogo(int argc, char *argv[]);
tJogo LeMapaJogo(tJogo jogo);

tJogo jogo;
int main(int argc, char *argv[])
{
    jogo = LeConfigInicialJogo(argc, argv);

    return 0;
}

tJogo LeConfigInicialJogo(int argc, char *argv[])
{
    tJogo jogo;
    jogo = LeDiretorioJogo(argc, argv);
    jogo = LeMapaJogo(jogo);
}

tJogo LeMapaJogo(tJogo jogo)
{
    char config_inicial[1000] = {'\0'};
    strcat(config_inicial, jogo.diretorio);
    strcat(config_inicial, ARQUIVO_CONFIG_NOME);

    FILE *arquivo_config = fopen(config_inicial, "r");

    return jogo;
}

tJogo LeDiretorioJogo(int argc, char *argv[])
{
    tJogo jogo;
    if (argc < 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao.");
        exit(1);
    }

    strcpy(jogo.diretorio, argv[1]);

    return jogo;
}
