#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARROS 10
#define MAX_PISTAS 12

typedef struct
{
    int x;
    int y;

    int vidas;
    int pontos;
} tGalinha;

typedef struct
{
    int x;
} tCarro;

typedef struct
{
    char direcao;
    int velocidade;
    int num_carros;
    tCarro carros[MAX_CARROS];
} tPista;

typedef struct
{
    int animacao;
    int largura_mapa;
    int qtd_pistas;

    tPista pistas[MAX_PISTAS];
    tGalinha galinha;

    int iteracao;
} tJogo;

tJogo InicializaJogo(tJogo jogo, char diretorio[]);

void Debug(tJogo jogo);

tJogo jogo;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao!\n");
        exit(1);
    }

    jogo = InicializaJogo(jogo, argv[1]);
}

void Debug(tJogo jogo)
{
    printf("JOGO:\nanimacao:%d\nlargura:%d\nqtd_pistas:%d\n", jogo.animacao, jogo.largura_mapa, jogo.qtd_pistas);
    printf("GALINHA:\nx:%d\ny:%d\nvidas:%d\npontos:%d\n", jogo.galinha.x, jogo.galinha.y, jogo.galinha.vidas, jogo.galinha.pontos);

    int i;
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        printf("PISTA:%d\ndir:%c\nvel:%d\nnum_carros:%d\n", i, jogo.pistas[i].direcao, jogo.pistas[i].velocidade, jogo.pistas[i].num_carros);

        int j;
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
            printf("CARRO:%d\n", jogo.pistas[i].carros[j].x);
        }
    }
}

tJogo InicializaJogo(tJogo jogo, char diretorio[])
{
    char arquivo_config_nome[1100];
    strcpy(arquivo_config_nome, diretorio);
    strcat(arquivo_config_nome, "config_inicial.txt");

    FILE *arquivo_config = fopen(arquivo_config_nome, "r");

    if (arquivo_config == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de configuracao inicial.\n");
        exit(1);
    }

    // TESTADO
    fscanf(arquivo_config, "%d\n%d %d", &jogo.animacao, &jogo.largura_mapa, &jogo.qtd_pistas);

    // FUNCIONOU
    int i;
    for (i = 1; i < jogo.qtd_pistas; i++)
    {
        char direcao;
        fscanf(arquivo_config, "\n%c ", &direcao);

        if (direcao == 'G')
        {
            // TESTADO
            fscanf(arquivo_config, " %d %d", &jogo.galinha.x, &jogo.galinha.vidas);
        }
        else if (direcao == 'E' || direcao == 'D')
        {
            jogo.pistas[i].direcao = direcao;
            int velocidade, num_carros;
            fscanf(arquivo_config, " %d %d", &jogo.pistas[i].velocidade, &jogo.pistas[i].num_carros);

            int j;
            for (j = 0; j < num_carros; j++)
            {
                // nao funciona ainda
                fscanf(arquivo_config, "%d", &jogo.pistas[i].carros[j].x);
            }
        }

        fscanf(arquivo_config, "%*[^\n]");
    }

    Debug(jogo);

    return jogo;
}