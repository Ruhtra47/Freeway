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
    char galinha[6];
    char carro[6];
} tSprites;

typedef struct
{
    int animacao;
    int largura_mapa;
    int qtd_pistas;

    tPista pistas[MAX_PISTAS];
    tGalinha galinha;

    tSprites sprites;

    int fim;
    int iteracao;
} tJogo;

tSprites CarregaSprites(char diretorio[]);

tJogo InicializaJogo(tJogo jogo, char diretorio[]);
void JogaJogo(tJogo jogo);

void DesenhaMapa(tJogo jogo, FILE *saida);

char LeJogada();
tJogo FazJogada(char jogada, tJogo jogo);
tJogo VerificaColisoes(tJogo jogo);

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
    // JogaJogo(jogo);
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

    for (i = 0; i < 6; i++)
    {
        printf("%c", jogo.sprites.galinha[i]);
    }

    for (i = 0; i < 6; i++)
    {
        printf("%c", jogo.sprites.carro[i]);
    }

    printf("\n");
}

char LeJogada()
{
    char jogada;
    scanf("%c\n", &jogada);

    return jogada;
}

void JogaJogo(tJogo jogo)
{
    while (!jogo.fim)
    {
        char jogada = LeJogada();

        FazJogada(jogada, jogo);

        DesenhaMapa(jogo, stdout);
    }
}

tJogo FazJogada(char jogada, tJogo jogo)
{
    if (jogada == 'w')
    {
        jogo.galinha.y--;
    }
    else if (jogada == 's')
    {
        jogo.galinha.y++;
    }
    else if (jogada == ' ')
    {
    }
}

tJogo VerificaColisoes(tJogo jogo)
{
}

void GeraArquivoInicializacao(tJogo jogo, char diretorio[])
{
    char caminho_arquivo[1100];
    sprintf(caminho_arquivo, "%s/saida/inicializacao.txt", diretorio);

    FILE *arquivo = fopen(caminho_arquivo, "w");

    if (arquivo == NULL)
    {
        printf("ERRO: Erro ao criar arquivo de saida da inicializacao.\n");
        exit(1);
    }

    DesenhaMapa(jogo, arquivo);
    fprintf(arquivo, "A posicao central da galinha iniciara em (%d %d).", jogo.galinha.x, jogo.galinha.y * 3 + 1);

    fclose(arquivo);
}

void DesenhaMapa(tJogo jogo, FILE *saida)
{
    int i, j;

    char mapa[jogo.qtd_pistas * 2][jogo.largura_mapa];

    for (i = 0; i < jogo.qtd_pistas * 2; i++)
        for (j = 0; j < jogo.largura_mapa; j++)
            mapa[i][j] = '\0';

    mapa[jogo.galinha.y * 2][jogo.galinha.x - 1 - 1] = jogo.sprites.galinha[0];
    mapa[jogo.galinha.y * 2][jogo.galinha.x - 1] = jogo.sprites.galinha[1];
    mapa[jogo.galinha.y * 2][jogo.galinha.x + 1 - 1] = jogo.sprites.galinha[2];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x - 1 - 1] = jogo.sprites.galinha[3];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x - 1] = jogo.sprites.galinha[4];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x + 1 - 1] = jogo.sprites.galinha[5];

    for (i = 0; i < jogo.qtd_pistas * 2; i++)
    {
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
            mapa[i * 2][jogo.pistas[i].carros[j].x - 1 - 1] = jogo.sprites.carro[0];
            mapa[i * 2][jogo.pistas[i].carros[j].x - 1] = jogo.sprites.carro[1];
            mapa[i * 2][jogo.pistas[i].carros[j].x + 1 - 1] = jogo.sprites.carro[2];
            mapa[(i * 2) + 1][jogo.pistas[i].carros[j].x - 1 - 1] = jogo.sprites.carro[3];
            mapa[(i * 2) + 1][jogo.pistas[i].carros[j].x - 1] = jogo.sprites.carro[4];
            mapa[(i * 2) + 1][jogo.pistas[i].carros[j].x + 1 - 1] = jogo.sprites.carro[5];
        }
    }

    int eh_pista = 2;
    int eh_traco = 2;

    if (saida == stdout)
    {
        printf("Pontos: %d | Vidas: %d | Iteracoes: %d\n", jogo.galinha.pontos, jogo.galinha.vidas, jogo.iteracao);
    }

    for (i = 0; i <= (jogo.qtd_pistas * 2); i++)
    {
        if (i == jogo.qtd_pistas * 2 && !eh_pista)
            break;

        eh_traco = 2;

        fprintf(saida, "|");

        for (j = 0; j < jogo.largura_mapa; j++)
        {
            if (eh_pista == 2)
            {
                if (i == 0 || i == (jogo.qtd_pistas * 2))
                {
                    fprintf(saida, "-");
                    continue;
                }

                if (eh_traco > 0)
                {
                    fprintf(saida, "-");
                    eh_traco--;
                }
                else
                {
                    eh_traco = 2;
                    fprintf(saida, " ");
                }
                continue;
            }

            if (mapa[i][j])
                fprintf(saida, "%c", mapa[i][j]);
            else
                fprintf(saida, " ");
        }

        if (eh_pista == 2)
        {
            eh_pista = 0;
            i--;
        }
        else
        {
            eh_pista++;
        }

        fprintf(saida, "|\n");
    }
}

tSprites CarregaSprites(char diretorio[])
{
    char arquivo_personagens_nome[1100];
    sprintf(arquivo_personagens_nome, "%s/personagens.txt", diretorio);

    tSprites sprites;

    FILE *arquivo = fopen(arquivo_personagens_nome, "r");

    fscanf(arquivo, "%c%c%c\n%c%c%c\n", &sprites.galinha[0], &sprites.galinha[1], &sprites.galinha[2], &sprites.galinha[3], &sprites.galinha[4], &sprites.galinha[5]);

    fscanf(arquivo, "%c%c%c\n%c%c%c", &sprites.carro[0], &sprites.carro[1], &sprites.carro[2], &sprites.carro[3], &sprites.carro[4], &sprites.carro[5]);

    fclose(arquivo);

    return sprites;
}

tJogo InicializaJogo(tJogo jogo, char diretorio[])
{
    char arquivo_config_nome[1100];
    sprintf(arquivo_config_nome, "%s/config_inicial.txt", diretorio);

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
            jogo.galinha.y = i;
        }
        else if (direcao == 'E' || direcao == 'D')
        {
            jogo.pistas[i].direcao = direcao;
            int velocidade, num_carros;
            fscanf(arquivo_config, " %d %d", &jogo.pistas[i].velocidade, &jogo.pistas[i].num_carros);

            int j;
            for (j = 0; j < jogo.pistas[i].num_carros; j++)
            {
                // nao funciona ainda
                fscanf(arquivo_config, " %d", &jogo.pistas[i].carros[j].x);
                // printf("teste:%d\n", jogo.pistas[i].carros[j].x);
            }
        }

        fscanf(arquivo_config, "%*[^\n]");
    }

    jogo.sprites = CarregaSprites(diretorio);

    Debug(jogo);

    fclose(arquivo_config);

    jogo.iteracao = 0;
    jogo.fim = 0;

    DesenhaMapa(jogo, stdout);
    GeraArquivoInicializacao(jogo, diretorio);

    return jogo;
}