#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUNAS 100
#define MAX_LINHAS 35
#define MAX_PISTAS 12
#define MAX_CARROS 10
#define MAX_CAMINHO_DIR 1000

#define NOME_ARQUIVO_CONFIGS "config_inicial.txt"

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
    int x;
    int y;
    int vidas;
    int pontos;
} tGalinha;

typedef struct
{
    int animacao;
    int largura_mapa;
    int qtd_pistas;
    tPista pistas[MAX_PISTAS];
    tGalinha galinha;
    int iteracao;
} tJogo;

tJogo InicializarJogo(tJogo jogo, char argv[]);
tJogo CarregaConfigsJogo(tJogo jogo, char argv[]);

tJogo jogo;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao.\n");
        exit(1);
    }

    InicializarJogo(jogo, argv[1]);
}

tJogo InicializarJogo(tJogo jogo, char argv[])
{
    jogo.iteracao = 0;

    jogo = CarregaConfigsJogo(jogo, argv[1]);
}

tJogo CarregaConfigsJogo(tJogo jogo, char diretorio[])
{
    char nome_arquivo[1100];
    strcpy(nome_arquivo, diretorio[1]);
    strcat(nome_arquivo, NOME_ARQUIVO_CONFIGS);

    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "r");

    if (arquivo == NULL)
    {
        printf("ERRO: Nao foi possivel ler o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    int i = 0, j = 0;

    fscanf(arquivo, "%d", &jogo.animacao);
    fscanf(arquivo, "%d %d", &jogo.largura_mapa, &jogo.qtd_pistas);

    fgetc(arquivo); // Le o \n

    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        char linha[200];
        fgets(linha, 200, arquivo);

        if (linha[0] == 'G')
        {
            jogo.galinha = InicializarGalinha(jogo.galinha);
            jogo.galinha = LeGalinha(jogo.galinha, linha);
        }
        else if ((linha[0] == 'E') || (linha[0] == 'D'))
        {
            char direcao;
            int velocidade, num_carros;
            tPista pista;
            sscanf(linha, "%c %d %d", &pista.direcao, &pista.velocidade, &pista.num_carros);

            int j = 0;
            for (j = 0; j < num_carros; j++)
            {
                tCarro carro;

                sscanf(linha, " %d", &carro.x);

                pista.carros[j] = carro;
            }
        }
    }
}

tGalinha InicializarGalinha(tGalinha galinha, int qtd_pistas, int indice_pista)
{
    galinha.x = 0;
    galinha.y = CalculaYGalinha(qtd_pistas, indice_pista);
    galinha.vidas = 0;
    galinha.pontos = 0;
}

int CalculaYGalinha(int qtd_pistas, int indice_pista)
{
    return 1 + (qtd_pistas - 1 - indice_pista) * 3 + 1;
}

tGalinha LeGalinha(tGalinha galinha, char linha[])
{
    sscanf(linha, "G %d %d", &galinha.x, &galinha.vidas);
    return galinha;
}