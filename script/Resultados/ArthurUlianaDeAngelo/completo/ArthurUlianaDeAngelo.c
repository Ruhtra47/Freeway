#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

<<<<<<< HEAD
#define VITORIA 1
#define DERROTA 2

=======
>>>>>>> main
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

<<<<<<< HEAD
tPista DeslocaCarrosPista(tPista pista, int largura_mapa);

tCarro DeslocaCarro(int x, int velocidade, char direcao, int largura_mapa);

tJogo InicializaJogo(char diretorio[]);
tJogo JogaJogo(tJogo jogo);

void DesenhaMapa(tJogo jogo, FILE *saida);

char LeJogada();
tJogo FazJogada(char jogada, tJogo jogo);
int VerificaColisoes(tJogo jogo);

=======
tJogo InicializaJogo(tJogo jogo, char diretorio[]);
void JogaJogo(tJogo jogo);

void DesenhaMapa(tJogo jogo, FILE *saida);

int VerificaVitoria(tJogo jogo);
char LeJogada();
tJogo FazJogada(char jogada, tJogo jogo);
tJogo DeslocarCarrosJogo(tJogo jogo);
tJogo VerificaColisoes(tJogo jogo);

>>>>>>> main
void Debug(tJogo jogo);

tJogo jogo;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao!\n");
        exit(1);
    }

<<<<<<< HEAD
    jogo = InicializaJogo(argv[1]);
    jogo = JogaJogo(jogo);

    ImprimeFimJogo(jogo);

    return 0;
}

void ImprimeFimJogo(tJogo jogo)
{
    if (jogo.fim == VITORIA)
    {
        printf("Parabens! Voce atravessou todas as pistas e venceu!\n");
    }
    else if (jogo.fim == DERROTA)
    {
        printf("Voce perdeu todas as vidas! Fim de jogo.\n");
    }
=======
    jogo = InicializaJogo(jogo, argv[1]);
    // JogaJogo(jogo);
>>>>>>> main
}

void Debug(tJogo jogo)
{
<<<<<<< HEAD
    printf("JOGO:\nanimacao:%d\tlargura:%d\tqtd_pistas:%d\n", jogo.animacao, jogo.largura_mapa, jogo.qtd_pistas);
    printf("GALINHA:\tx:%d\ty:%d\tvidas:%d\tpontos:%d\n", jogo.galinha.x, jogo.galinha.y, jogo.galinha.vidas, jogo.galinha.pontos);
=======
    printf("JOGO:\nanimacao:%d\nlargura:%d\nqtd_pistas:%d\n", jogo.animacao, jogo.largura_mapa, jogo.qtd_pistas);
    printf("GALINHA:\nx:%d\ny:%d\nvidas:%d\npontos:%d\n", jogo.galinha.x, jogo.galinha.y, jogo.galinha.vidas, jogo.galinha.pontos);
>>>>>>> main

    int i;
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
<<<<<<< HEAD
        printf("PISTA:%d\tdir:%c\tvel:%d\tnum_carros:%d\t", i, jogo.pistas[i].direcao, jogo.pistas[i].velocidade, jogo.pistas[i].num_carros);
=======
        printf("PISTA:%d\ndir:%c\nvel:%d\nnum_carros:%d\n", i, jogo.pistas[i].direcao, jogo.pistas[i].velocidade, jogo.pistas[i].num_carros);
>>>>>>> main

        int j;
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
<<<<<<< HEAD
            printf("CARRO:%d\t", jogo.pistas[i].carros[j].x);
        }
        printf("\n");
=======
            printf("CARRO:%d\n", jogo.pistas[i].carros[j].x);
        }
>>>>>>> main
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
<<<<<<< HEAD
    scanf("%c%*c", &jogada);

    if (jogada != 'w' && jogada != 's' && jogada != ' ')
    {
        while (scanf("%c%*c", &jogada) == 1 && (jogada != 'w' && jogada != 's' && jogada != ' '))
            ;
    }

    return jogada;
}

tJogo DeslocaTodasPistas(tJogo jogo)
{
    int i;
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        jogo.pistas[i] = DeslocaCarrosPista(jogo.pistas[i], jogo.largura_mapa);
    }

    return jogo;
}

tJogo JogaJogo(tJogo jogo)
{
    while (!jogo.fim)
    {
        char jogada = LeJogada();

        // printf("Jogada: %d %c | ", jogada, jogada);
        DesenhaMapa(jogo, stdout);

        jogo = FazJogada(jogada, jogo);

        jogo = DeslocaTodasPistas(jogo);

        if (VerificaColisoes(jogo))
        {
            jogo.galinha.vidas--;
            jogo.galinha.pontos = 0;
            jogo.galinha.y = jogo.qtd_pistas - 1;
        }
        else if (!VerificaColisoes(jogo) && jogada == 'w')
        {
            jogo.galinha.pontos++;
        }

        jogo.iteracao++;

        jogo.fim = VerificaFim(jogo);

        if (jogo.fim == VITORIA)
        {
            jogo.galinha.pontos += 10;
            break;
        }

        if (jogo.fim == DERROTA)
        {
            break;
        }

        // Debug(jogo);
    }

    DesenhaMapa(jogo, stdout);

    return jogo;
}

int VerificaFim(tJogo jogo)
{
    if (jogo.galinha.y == 0)
    {
        return VITORIA;
    }

    if (jogo.galinha.vidas <= 0)
    {
        return DERROTA;
    }

    return 0;
}

tPista DeslocaCarrosPista(tPista pista, int largura_mapa)
{
    int i;
    for (i = 0; i < pista.num_carros; i++)
    {
        pista.carros[i] = DeslocaCarro(pista.carros[i].x, pista.velocidade, pista.direcao, largura_mapa);
    }

    return pista;
}

tCarro DeslocaCarro(int x, int velocidade, char direcao, int largura_mapa)
{
    tCarro carro;
    velocidade = (direcao == 'E') ? -velocidade : velocidade;

    carro.x = (x + velocidade) % largura_mapa;

    if (carro.x < 0)
        carro.x += largura_mapa;

    return carro;
=======
    if (scanf("%c\n", &jogada) == 1)
        return jogada;

    return '\0';
}

void JogaJogo(tJogo jogo)
{
    while (!jogo.fim)
    {
    }
>>>>>>> main
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

    return jogo;
}

<<<<<<< HEAD
int VerificaColisoes(tJogo jogo)
{
    int i, j;
    int colidiu = 0;

    int esquerda_galinha = jogo.galinha.x - 1;
    int direita_galinha = jogo.galinha.x + 1;

    for (j = 0; j < jogo.pistas[jogo.galinha.y].num_carros; j++)
    {
        int x_carro = jogo.pistas[jogo.galinha.y].carros[j].x;

        int esquerda_carro = x_carro - 1;
        int direita_carro = x_carro + 1;

        if (esquerda_carro < 0)
            esquerda_carro = jogo.largura_mapa - 1;
        if (direita_carro > jogo.largura_mapa - 1)
            direita_carro = 0;

        if (!(direita_galinha < esquerda_carro || esquerda_galinha > direita_carro))
        {
            colidiu = 1;
            break;
        }
    }

    return colidiu;
=======
tJogo VerificaColisoes(tJogo jogo)
{
>>>>>>> main
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

<<<<<<< HEAD
    char mapa[jogo.qtd_pistas * 2 + 2][jogo.largura_mapa];
=======
    char mapa[jogo.qtd_pistas * 2][jogo.largura_mapa];
>>>>>>> main

    for (i = 0; i < jogo.qtd_pistas * 2; i++)
        for (j = 0; j < jogo.largura_mapa; j++)
            mapa[i][j] = '\0';

    mapa[jogo.galinha.y * 2][jogo.galinha.x - 1 - 1] = jogo.sprites.galinha[0];
    mapa[jogo.galinha.y * 2][jogo.galinha.x - 1] = jogo.sprites.galinha[1];
    mapa[jogo.galinha.y * 2][jogo.galinha.x + 1 - 1] = jogo.sprites.galinha[2];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x - 1 - 1] = jogo.sprites.galinha[3];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x - 1] = jogo.sprites.galinha[4];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x + 1 - 1] = jogo.sprites.galinha[5];

<<<<<<< HEAD
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
            int meio = jogo.pistas[i].carros[j].x % jogo.largura_mapa;
            if (meio < 0)
                meio += jogo.largura_mapa; // segurança

            int anterior = (meio - 1 + jogo.largura_mapa) % jogo.largura_mapa;
            int posterior = (meio + 1) % jogo.largura_mapa;

            mapa[i * 2][anterior] = jogo.sprites.carro[0];
            mapa[i * 2][meio] = jogo.sprites.carro[1];
            mapa[i * 2][posterior] = jogo.sprites.carro[2];
            mapa[(i * 2) + 1][anterior] = jogo.sprites.carro[3];
            mapa[(i * 2) + 1][meio] = jogo.sprites.carro[4];
            mapa[(i * 2) + 1][posterior] = jogo.sprites.carro[5];
=======
    for (i = 0; i < jogo.qtd_pistas * 2; i++)
    {
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
            mapa[i * 2][jogo.pistas[i].carros[j].x - 1 - 1 < 0 ? jogo.largura_mapa - 1 : jogo.pistas[i].carros[j].x - 1 - 1] = jogo.sprites.carro[0];
            mapa[i * 2][jogo.pistas[i].carros[j].x - 1] = jogo.sprites.carro[1];
            mapa[i * 2][jogo.pistas[i].carros[j].x + 1 - 1 >= jogo.largura_mapa ? 0 : jogo.pistas[i].carros[j].x + 1 - 1] = jogo.sprites.carro[2];
            mapa[(i * 2) + 1][jogo.pistas[i].carros[j].x - 1 - 1 < 0 ? jogo.largura_mapa - 1 : jogo.pistas[i].carros[j].x - 1 - 1] = jogo.sprites.carro[3];
            mapa[(i * 2) + 1][jogo.pistas[i].carros[j].x - 1] = jogo.sprites.carro[4];
            mapa[(i * 2) + 1][jogo.pistas[i].carros[j].x + 1 - 1 >= jogo.largura_mapa ? 0 : jogo.pistas[i].carros[j].x + 1 - 1] = jogo.sprites.carro[5];
>>>>>>> main
        }
    }

    int eh_pista = 2;
    int eh_traco = 2;

    if (saida == stdout)
    {
<<<<<<< HEAD
        printf("Pontos: %d | Vidas: %d | Iteracoes: %d\n", jogo.galinha.pontos, jogo.galinha.vidas, jogo.iteracao, jogo.galinha.y);
=======
        printf("Pontos: %d | Vidas: %d | Iteracoes: %d\n", jogo.galinha.pontos, jogo.galinha.vidas, jogo.iteracao);
>>>>>>> main
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

<<<<<<< HEAD
tJogo InicializaJogo(char diretorio[])
{
    tJogo jogo;
=======
tJogo InicializaJogo(tJogo jogo, char diretorio[])
{
>>>>>>> main
    char arquivo_config_nome[1100];
    sprintf(arquivo_config_nome, "%s/config_inicial.txt", diretorio);

    FILE *arquivo_config = fopen(arquivo_config_nome, "r");

    if (arquivo_config == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de configuracao inicial.\n");
        exit(1);
    }

<<<<<<< HEAD
    fscanf(arquivo_config, "%d %d %d", &jogo.animacao, &jogo.largura_mapa, &jogo.qtd_pistas);
    fscanf(arquivo_config, "%*c");

    int i;
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        char direcao;
        fscanf(arquivo_config, "%c", &direcao);

        if (direcao == 'E' || direcao == 'D')
        {
            jogo.pistas[i].direcao = direcao;

            fscanf(arquivo_config, "%d", &jogo.pistas[i].velocidade);
            fscanf(arquivo_config, "%d", &jogo.pistas[i].num_carros);

            int j;
            for (j = 0; j < jogo.pistas[i].num_carros; j++)
            {
                fscanf(arquivo_config, "%d", &jogo.pistas[i].carros[j].x);
            }
        }
        else if (direcao == 'G')
        {
            fscanf(arquivo_config, "%d", &jogo.galinha.x);
            fscanf(arquivo_config, "%d", &jogo.galinha.vidas);

            jogo.galinha.y = i;

            jogo.pistas[i].direcao = '\0';
            jogo.pistas[i].velocidade = 0;
            jogo.pistas[i].num_carros = 0;
        }
        else
=======
    fscanf(arquivo_config, "%d\n%d %d\n", &jogo.animacao, &jogo.largura_mapa, &jogo.qtd_pistas);

    int i;
    char linha[256];
    for (i = 1; i < jogo.qtd_pistas; i++)
    {
        if (!fgets(linha, sizeof(linha), arquivo_config))
            break;

        linha[strcspn(linha, "\r\n")] = '\0';

        if (strlen(linha) == 0)
>>>>>>> main
        {
            jogo.pistas[i].direcao = '\0';
            jogo.pistas[i].velocidade = 0;
            jogo.pistas[i].num_carros = 0;
            continue;
        }
<<<<<<< HEAD

        fscanf(arquivo_config, "%*[^\n]");
        fscanf(arquivo_config, "%*c");
    }

    jogo.sprites = CarregaSprites(diretorio);

    fclose(arquivo_config);

    jogo.iteracao = 0;
    jogo.fim = 0;

    // DesenhaMapa(jogo, stdout);
    GeraArquivoInicializacao(jogo, diretorio);

    return jogo;
}
=======
    }

    return jogo;
}

// tJogo Antigo(tJogo jogo, char diretorio[])
// {
//     char arquivo_config_nome[1100];
//     sprintf(arquivo_config_nome, "%s/config_inicial.txt", diretorio);

//     FILE *arquivo_config = fopen(arquivo_config_nome, "r");

//     if (arquivo_config == NULL)
//     {
//         printf("ERRO: Erro ao abrir o arquivo de configuracao inicial.\n");
//         exit(1);
//     }

//     // TESTADO
//     fscanf(arquivo_config, "%d\n%d %d\n", &jogo.animacao, &jogo.largura_mapa, &jogo.qtd_pistas);

//     // FUNCIONOU
//     int i;
//     char linha[256];
//     for (i = 1; i < jogo.qtd_pistas; i++)
//     {
//         if (!fgets(linha, sizeof(linha), arquivo_config))
//             break;

//         linha[strcspn(linha, "\r\n")] = '\0';

//         if (strlen(linha) == 0)
//         {
//             jogo.pistas[i].direcao = '\0';
//             jogo.pistas[i].velocidade = 0;
//             jogo.pistas[i].num_carros = 0;
//             continue;
//         }

//         if (linha[0] == 'G')
//         {
//             char letra;
//             sscanf(linha, "%c %d %d", &letra, &jogo.galinha.x, &jogo.galinha.vidas);
//             jogo.galinha.y = i;
//             continue;
//         }

//         sscanf(linha, "%c %d %d", &jogo.pistas[i].direcao, &jogo.pistas[i].velocidade, &jogo.pistas[i].num_carros);

//         int offset = 0;
//         int skip = 0, count = 0;
//         for (skip = 0, count = 0; count < 3; count++)
//         {
//             while (linha[offset] && !isspace((unsigned char)linha[offset]))
//                 offset++;
//             while (linha[offset] && isspace((unsigned char)linha[offset]))
//                 offset++;
//         }

//         char *ptr = linha + offset;
//         for (int c = 0; c < jogo.pistas[i].num_carros; c++)
//         {
//             sscanf(ptr, "%d", &jogo.pistas[i].carros[c].x);
//             while (*ptr && !isspace((unsigned char)*ptr))
//                 ptr++;
//             while (*ptr && isspace((unsigned char)*ptr))
//                 ptr++;
//         }

//         char direcao;

//         printf("%c: %d\n", direcao, direcao);

//         if (direcao == 'G')
//         {
//             // TESTADO
//             fscanf(arquivo_config, " %d %d", &jogo.galinha.x, &jogo.galinha.vidas);
//             jogo.galinha.y = i;
//         }
//         else if (direcao == 'E' || direcao == 'D')
//         {
//             jogo.pistas[i].direcao = direcao;
//             int velocidade, num_carros;
//             fscanf(arquivo_config, " %d %d", &jogo.pistas[i].velocidade, &jogo.pistas[i].num_carros);

//             int j;
//             for (j = 0; j < jogo.pistas[i].num_carros; j++)
//             {
//                 // nao funciona ainda
//                 fscanf(arquivo_config, " %d", &jogo.pistas[i].carros[j].x);
//                 // printf("teste:%d\n", jogo.pistas[i].carros[j].x);
//             }
//         }

//         fscanf(arquivo_config, "%*[^\n]\n");
//     }

//     jogo.sprites = CarregaSprites(diretorio);

//     Debug(jogo);

//     fclose(arquivo_config);

//     jogo.iteracao = 0;
//     jogo.fim = 0;

//     DesenhaMapa(jogo, stdout);
//     GeraArquivoInicializacao(jogo, diretorio);

//     return jogo;
// }
>>>>>>> main
