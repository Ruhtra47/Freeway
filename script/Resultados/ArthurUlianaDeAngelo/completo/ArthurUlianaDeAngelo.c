#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUNAS 100
#define MAX_LINHAS 35
#define MAX_PISTAS 12
#define MAX_CARROS 10
#define MAX_STR 1000

// Structs principais
typedef struct
{
    int x;
} Carro;

typedef struct
{
    char direcao;
    int velocidade;
    int num_carros;
    Carro carros[MAX_CARROS];
} Pista;

typedef struct
{
    int x;
    int y;
    int vidas;
    int pontos;
} Galinha;

typedef struct
{
    int animacao;
    int largura_mapa;
    int qtd_pistas;
    Pista pistas[MAX_PISTAS];
    Galinha galinha;
    int iteracao;
} Jogo;

// Funcoes auxiliares
int calcula_y_central(int qtd_pistas, int indice_pista)
{
    return 1 + (qtd_pistas - 1 - indice_pista) * 3 + 1;
}

void inicializar_jogo(Jogo *jogo, char caminho[])
{
    char path_config[MAX_STR];
    FILE *f;

    sprintf(path_config, "%s/config_inicial.txt", caminho);
    f = fopen(path_config, "r");
    if (f == NULL)
    {
        printf("ERRO: Nao foi possivel ler o arquivo: %s\n", path_config);
        exit(1);
    }

    int i, j;
    fscanf(f, "%d", &jogo->animacao);
    fscanf(f, "%d %d", &jogo->largura_mapa, &jogo->qtd_pistas);
    fgetc(f); // Pula \n
    for (i = 0; i < jogo->qtd_pistas; i++)
    {
        char linha[MAX_STR];
        fgets(linha, MAX_STR, f);

        if (linha[0] == 'G')
        {
            sscanf(linha, "G %d %d", &jogo->galinha.x, &jogo->galinha.vidas);
            jogo->galinha.pontos = 0;
            jogo->galinha.y = calcula_y_central(jogo->qtd_pistas, i);
        }
        else if (linha[0] == 'D' || linha[0] == 'E')
        {
            char direcao;
            int vel, numc, pos, offset = 0;
            char *p = linha;

            sscanf(p, "%c %d %d %n", &direcao, &vel, &numc, &offset);
            jogo->pistas[i].direcao = direcao;
            jogo->pistas[i].velocidade = vel;
            jogo->pistas[i].num_carros = numc;
            p += offset;

            for (j = 0; j < numc; j++)
            {
                sscanf(p, "%d%n", &pos, &offset);
                jogo->pistas[i].carros[j].x = pos;
                p += offset;
            }
        }
        else
        {
            jogo->pistas[i].num_carros = 0;
        }
    }
    fclose(f);
}

void desenhar_jogo(Jogo *jogo)
{
    int linhas = jogo->qtd_pistas * 3;
    int colunas = jogo->largura_mapa;
    int mapa[MAX_LINHAS][MAX_COLUNAS] = {0};

    int i, j;
    for (i = 0; i < jogo->qtd_pistas; i++)
    {
        int y_central = calcula_y_central(jogo->qtd_pistas, i);
        for (j = 0; j < jogo->pistas[i].num_carros; j++)
        {
            int x = jogo->pistas[i].carros[j].x;
            mapa[y_central][x] = 1;
        }
    }

    mapa[jogo->galinha.y][jogo->galinha.x] = 2;

    printf("Pontos: %d | Vidas: %d | Iteracoes: %d\n", jogo->galinha.pontos, jogo->galinha.vidas, jogo->iteracao);
    for (i = 0; i < linhas; i++)
    {
        printf("|");
        for (j = 0; j < colunas; j++)
        {
            if (mapa[i][j] == 1)
                printf("[-]");
            else if (mapa[i][j] == 2)
                printf("^_^");
            else
                printf("   ");
        }
        printf("|\n");
    }
}

void jogar(Jogo *jogo)
{
    char comando;
    while (jogo->galinha.vidas > 0 && jogo->galinha.y > 1)
    {
        scanf(" %c", &comando);

        if (comando == 'w')
        {
            jogo->galinha.y -= 3;
            jogo->galinha.pontos += 1;
        }
        else if (comando == 's' && jogo->galinha.y + 3 < jogo->qtd_pistas * 3)
        {
            jogo->galinha.y += 3;
        }

        // Desloca carros
        int i, j;
        for (i = 0; i < jogo->qtd_pistas; i++)
        {
            if (jogo->pistas[i].num_carros == 0)
                continue;
            for (j = 0; j < jogo->pistas[i].num_carros; j++)
            {
                if (jogo->pistas[i].direcao == 'D')
                {
                    jogo->pistas[i].carros[j].x = (jogo->pistas[i].carros[j].x + jogo->pistas[i].velocidade) % jogo->largura_mapa;
                }
                else
                {
                    jogo->pistas[i].carros[j].x -= jogo->pistas[i].velocidade;
                    if (jogo->pistas[i].carros[j].x < 0)
                        jogo->pistas[i].carros[j].x += jogo->largura_mapa;
                }
            }
        }

        // Verifica colisão
        int y = jogo->galinha.y;
        int x = jogo->galinha.x;
        int colisao = 0;
        for (i = 0; i < jogo->qtd_pistas; i++)
        {
            if (y == calcula_y_central(jogo->qtd_pistas, i))
            {
                for (j = 0; j < jogo->pistas[i].num_carros; j++)
                {
                    if (x == jogo->pistas[i].carros[j].x)
                    {
                        colisao = 1;
                    }
                }
            }
        }

        if (colisao)
        {
            jogo->galinha.vidas--;
            jogo->galinha.pontos = 0;
            jogo->galinha.y = calcula_y_central(jogo->qtd_pistas, jogo->qtd_pistas - 1);
        }

        jogo->iteracao++;
        desenhar_jogo(jogo);
    }

    if (jogo->galinha.vidas == 0)
    {
        printf("Voce perdeu todas as vidas! Fim de jogo.\n");
    }
    else
    {
        jogo->galinha.pontos += 10;
        printf("Parabens! Voce atravessou todas as pistas e venceu!\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao.\n");
        return 1;
    }

    Jogo jogo;
    jogo.iteracao = 0;
    inicializar_jogo(&jogo, argv[1]);
    desenhar_jogo(&jogo);
    jogar(&jogo);
    return 0;
}