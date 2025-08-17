#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VITORIA 1
#define DERROTA 2

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
    int n_movimentos;
    int altura_maxima_alcancada;
    int altura_maxima_atropelada;
    int altura_minima_atropelada;
    int n_movimentos_opostos;
} tEstatisticas;

typedef struct
{
    int id_pista;
    int id_carro;
    int iteracao;
} tRanking;

typedef struct
{
    int animacao;
    int largura_mapa;
    int qtd_pistas;

    tPista pistas[MAX_PISTAS];
    tGalinha galinha;

    tSprites sprites;

    tEstatisticas stats;

    int fim;
    int iteracao;
} tJogo;

tSprites CarregaSprites(char diretorio[]);

tPista DeslocaCarrosPista(tPista pista, int largura_mapa);

tCarro DeslocaCarro(int x, int velocidade, char direcao, int largura_mapa);
int PegaNumeroCarroPista(int x, tPista pista);

tJogo InicializaJogo(char diretorio[]);
tJogo JogaJogo(tJogo jogo, char diretorio[]);
void ImprimeFimJogo(tJogo jogo);

void DesenhaMapa(tJogo jogo, FILE *saida);

char LeJogada();
tJogo FazJogada(char jogada, tJogo jogo);
int VerificaColisoes(tJogo jogo);
int VerificaFim(tJogo jogo);

FILE *AbreArquivoResumo(char diretorio[]);
void EscreveColisaoResumo(int iteracao, int n_pista, tPista t_pista, int x, int galinha_x, int galinha_y, char diretorio[]);
void EscreveFimJogoResumo(int iteracao, char diretorio[]);

void EscreveRanking(tRanking rankings[], char diretorio[], int n_atropelamentos);
FILE *AbreArquivoRanking(char diretorio[]);
void OrdenaRankings(tRanking rankings[], int n_atropelamentos);

FILE *AbreArquivoHeatpmap(char diretorio[]);
void EscreveHeatmap(int heatmap[36][100], int linhas, int colunas, char diretorio[]);
void AtualizaHeatmap(int heatmap[36][100], int linhas, int colunas, int xGalinha, int yGalinha, int atropelamento);

tEstatisticas InicializaEstatisticas(tEstatisticas stats, int qtd_pistas);
tEstatisticas AtualizaMovimentos(tEstatisticas stats, char mov);
tEstatisticas AtualizaAlturasAtropelamenteEstatisticas(tEstatisticas stats, tGalinha galinha, int qtd_pistas);
tEstatisticas AtualizaAlturaMaxima(tEstatisticas stats, tGalinha galinha, int qtd_pistas);
void EscreveEstatisticas(tEstatisticas stats, char diretorio[]);

void Debug(tJogo jogo);

tJogo jogo;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERRO: Informe o diretorio com os arquivos de configuracao!\n");
        exit(1);
    }

    jogo = InicializaJogo(argv[1]);
    jogo = JogaJogo(jogo, argv[1]);

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
}

FILE *AbreArquivoRanking(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/ranking.txt", diretorio);

    FILE *arquivo_ranking = fopen(arquivo_nome, "w+");

    if (arquivo_ranking == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de ranking.\n");
        exit(1);
    }

    return arquivo_ranking;
}

void EscreveRanking(tRanking rankings[], char diretorio[], int n_atropelamentos)
{
    FILE *arquivo_ranking = AbreArquivoRanking(diretorio);

    OrdenaRankings(rankings, n_atropelamentos);

    fprintf(arquivo_ranking, "id_pista,id_carro,iteracao\n");

    int i;
    for (i = 0; i < n_atropelamentos; i++)
    {
        fprintf(arquivo_ranking, "%d,%d,%d\n", rankings[i].id_pista, rankings[i].id_carro, rankings[i].iteracao);
    }

    fclose(arquivo_ranking);
}

void OrdenaRankings(tRanking rankings[], int n_atropelamentos)
{
    int i, j, troca = 0;

    for (i = 0; i < n_atropelamentos - 1; i++)
    {
        for (j = 0; j < n_atropelamentos - i - 1; j++)
        {
            troca = 0;

            if (rankings[j].id_pista > rankings[j + 1].id_pista)
            {
                troca = 1;
            }
            else if (rankings[j].id_pista == rankings[j + 1].id_pista)
            {
                if (rankings[j].id_carro > rankings[j + 1].id_carro)
                {
                    troca = 1;
                }
                else if (rankings[j].id_carro == rankings[j + 1].id_carro)
                {
                    if (rankings[j].iteracao < rankings[j + 1].iteracao)
                    {
                        troca = 1;
                    }
                }
            }

            if (troca)
            {
                tRanking tmp = rankings[j];
                rankings[j] = rankings[j + 1];
                rankings[j + 1] = tmp;
            }
        }
    }
}

FILE *AbreArquivoResumo(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/resumo.txt", diretorio);

    FILE *arquivo_resumo = fopen(arquivo_nome, "a+");

    if (arquivo_resumo == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de resumo.\n");
        exit(1);
    }

    return arquivo_resumo;
}

void EscreveColisaoResumo(int iteracao, int n_pista, tPista t_pista, int x, int galinha_x, int galinha_y, char diretorio[])
{
    FILE *arquivo_resumo = AbreArquivoResumo(diretorio);

    int carro = PegaNumeroCarroPista(x, t_pista);

    fprintf(arquivo_resumo, "[%d] Na pista %d o carro %d atropelou a galinha na posicao (%d,%d).\n", iteracao, n_pista + 1, carro, galinha_x, galinha_y * 3 + 1);

    fclose(arquivo_resumo);
}

int PegaNumeroCarroPista(int x, tPista pista)
{
    int i;
    for (i = 0; i < pista.num_carros; i++)
    {
        if (pista.carros[i].x == x)
            return (i + 1);
    }

    return -1;
}

void EscreveFimJogoResumo(int iteracao, char diretorio[])
{
    FILE *arquivo_resumo = AbreArquivoResumo(diretorio);

    fprintf(arquivo_resumo, "[%d] Fim de jogo\n", iteracao);

    fclose(arquivo_resumo);
}

void Debug(tJogo jogo)
{
    printf("JOGO:\nanimacao:%d\tlargura:%d\tqtd_pistas:%d\n", jogo.animacao, jogo.largura_mapa, jogo.qtd_pistas);
    printf("GALINHA:\tx:%d\ty:%d\tvidas:%d\tpontos:%d\n", jogo.galinha.x, jogo.galinha.y, jogo.galinha.vidas, jogo.galinha.pontos);

    int i;
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        printf("PISTA:%d\tdir:%c\tvel:%d\tnum_carros:%d\t", i, jogo.pistas[i].direcao, jogo.pistas[i].velocidade, jogo.pistas[i].num_carros);

        int j;
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
            printf("CARRO:%d\t", jogo.pistas[i].carros[j].x);
        }
        printf("\n");
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

FILE *AbreArquivoHeatpmap(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/heatmap.txt", diretorio);

    FILE *arquivo_heatmap = fopen(arquivo_nome, "w");

    if (arquivo_heatmap == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de heatmap.\n");
        exit(1);
    }

    return arquivo_heatmap;
}

void AtualizaHeatmap(int heatmap[36][100], int linhas, int colunas, int xGalinha, int yGalinha, int atropelamento)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int y = yGalinha * 3 + i;
            int x = xGalinha + j;

            if (y >= 0 && y < linhas && x >= 0 && x < colunas && heatmap[i][j] != -1)
            {
                if (heatmap[y][x] < 99)
                {
                    heatmap[y][x - 1]++;
                }
            }
        }
    }

    if (atropelamento)
    {
        for (int i = 0; i < 2; i++)
        {
            int y = yGalinha * 3 + i;
            if (y >= 0 && y < linhas)
            {
                for (int x = 0; x < colunas; x++)
                {
                    heatmap[y][x] = -1;
                }
            }
        }
    }
}

void EscreveHeatmap(int heatmap[36][100], int linhas, int colunas, char diretorio[])
{
    FILE *arquivo_heatmap = AbreArquivoHeatpmap(diretorio);

    for (int i = 0; i < linhas - 1; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if (heatmap[i][0] == -1)
            {
                fprintf(arquivo_heatmap, " * ");
            }
            else
            {
                fprintf(arquivo_heatmap, "%2d ", heatmap[i][j]);
            }
        }
        fprintf(arquivo_heatmap, "\n");
    }

    fclose(arquivo_heatmap);
}

void InicializaHeatmap(int heatmap[36][100], int linhas, int colunas)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            heatmap[i][j] = 0;
        }
    }
}

tJogo JogaJogo(tJogo jogo, char diretorio[])
{
    tRanking rankings[jogo.galinha.vidas];
    int vidas_iniciais = jogo.galinha.vidas;
    int n_atropelamentos = 0;

    int heatpmap[36][100];

    InicializaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa);
    AtualizaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, jogo.galinha.x, jogo.galinha.y, 0);

    while (!jogo.fim)
    {
        char jogada = LeJogada();

        DesenhaMapa(jogo, stdout);

        jogo.iteracao++;

        jogo = FazJogada(jogada, jogo);

        jogo.stats = AtualizaMovimentos(jogo.stats, jogada);

        jogo = DeslocaTodasPistas(jogo);

        int carro_x = VerificaColisoes(jogo);

        AtualizaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, jogo.galinha.x, jogo.galinha.y, (carro_x != -1));

        if (carro_x != -1)
        {
            jogo.galinha.vidas--;
            jogo.galinha.pontos = 0;

            EscreveColisaoResumo(jogo.iteracao, jogo.galinha.y, jogo.pistas[jogo.galinha.y], carro_x, jogo.galinha.x, jogo.galinha.y, diretorio);

            jogo.stats = AtualizaAlturasAtropelamenteEstatisticas(jogo.stats, jogo.galinha, jogo.qtd_pistas);

            tRanking atropelamento = {jogo.galinha.y + 1, PegaNumeroCarroPista(carro_x, jogo.pistas[jogo.galinha.y]), jogo.iteracao};
            rankings[n_atropelamentos] = atropelamento;
            n_atropelamentos++;

            jogo.galinha.y = jogo.qtd_pistas - 1;

            AtualizaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, jogo.galinha.x, jogo.galinha.y, 0);
        }
        else if (carro_x == -1 && jogada == 'w')
        {
            jogo.galinha.pontos++;
            jogo.stats = AtualizaAlturaMaxima(jogo.stats, jogo.galinha, jogo.qtd_pistas);
        }

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
    }

    if (jogo.stats.altura_minima_atropelada == jogo.qtd_pistas * 3 + 1)
        jogo.stats.altura_minima_atropelada = 0;

    if (jogo.stats.altura_maxima_alcancada == 0)
        jogo.stats.altura_maxima_alcancada = 2;

    DesenhaMapa(jogo, stdout);

    EscreveFimJogoResumo(jogo.iteracao, diretorio);
    EscreveEstatisticas(jogo.stats, diretorio);
    EscreveRanking(rankings, diretorio, n_atropelamentos);
    EscreveHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, diretorio);

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

int VerificaColisoes(tJogo jogo)
{
    int i, j;
    int carro = -1;

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
            carro = x_carro;
            break;
        }
    }

    return carro;
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

    char mapa[jogo.qtd_pistas * 2 + 2][jogo.largura_mapa];

    for (i = 0; i < jogo.qtd_pistas * 2; i++)
        for (j = 0; j < jogo.largura_mapa; j++)
            mapa[i][j] = '\0';

    mapa[jogo.galinha.y * 2][jogo.galinha.x - 1 - 1] = jogo.sprites.galinha[0];
    mapa[jogo.galinha.y * 2][jogo.galinha.x - 1] = jogo.sprites.galinha[1];
    mapa[jogo.galinha.y * 2][jogo.galinha.x + 1 - 1] = jogo.sprites.galinha[2];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x - 1 - 1] = jogo.sprites.galinha[3];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x - 1] = jogo.sprites.galinha[4];
    mapa[(jogo.galinha.y * 2) + 1][jogo.galinha.x + 1 - 1] = jogo.sprites.galinha[5];

    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        for (j = 0; j < jogo.pistas[i].num_carros; j++)
        {
            int meio = (jogo.pistas[i].carros[j].x - 1 + jogo.largura_mapa) % jogo.largura_mapa;
            int anterior = (meio - 1 + jogo.largura_mapa) % jogo.largura_mapa;
            int posterior = (meio + 1) % jogo.largura_mapa;

            int linha = i * 2;

            mapa[linha][anterior] = jogo.sprites.carro[0];
            mapa[linha][meio] = jogo.sprites.carro[1];
            mapa[linha][posterior] = jogo.sprites.carro[2];
            mapa[linha + 1][anterior] = jogo.sprites.carro[3];
            mapa[linha + 1][meio] = jogo.sprites.carro[4];
            mapa[linha + 1][posterior] = jogo.sprites.carro[5];
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

tEstatisticas InicializaEstatisticas(tEstatisticas stats, int qtd_pistas)
{
    stats.n_movimentos = 0;
    stats.n_movimentos_opostos = 0;
    stats.altura_maxima_alcancada = 0;
    stats.altura_maxima_atropelada = 0;
    stats.altura_minima_atropelada = qtd_pistas * 3 + 1;

    return stats;
}

tEstatisticas AtualizaMovimentos(tEstatisticas stats, char mov)
{
    if (mov == 'w')
    {
        stats.n_movimentos++;
    }
    else if (mov == 's')
    {
        stats.n_movimentos++;
        stats.n_movimentos_opostos++;
    }

    return stats;
}

tEstatisticas AtualizaAlturasAtropelamenteEstatisticas(tEstatisticas stats, tGalinha galinha, int qtd_pistas)
{
    int altura = (qtd_pistas - galinha.y) * 3 - 1;

    if (altura > stats.altura_maxima_atropelada)
        stats.altura_maxima_atropelada = altura;

    if (altura < stats.altura_minima_atropelada)
        stats.altura_minima_atropelada = altura;

    return stats;
}

tEstatisticas AtualizaAlturaMaxima(tEstatisticas stats, tGalinha galinha, int qtd_pistas)
{
    int altura = (qtd_pistas - galinha.y) * 3 - 1;
    stats.altura_maxima_alcancada = ((altura) > stats.altura_maxima_alcancada) ? (altura) : stats.altura_maxima_alcancada;

    return stats;
}

void EscreveEstatisticas(tEstatisticas stats, char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/estatistica.txt", diretorio);

    FILE *arquivo_stats = fopen(arquivo_nome, "w");

    if (arquivo_stats == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de estatisticas.\n");
        exit(1);
    }

    fprintf(arquivo_stats, "Numero total de movimentos: %d\n", stats.n_movimentos);
    fprintf(arquivo_stats, "Altura maxima que a galinha chegou: %d\n", stats.altura_maxima_alcancada);
    fprintf(arquivo_stats, "Altura maxima que a galinha foi atropelada: %d\n", stats.altura_maxima_atropelada);
    fprintf(arquivo_stats, "Altura minima que a galinha foi atropelada: %d\n", stats.altura_minima_atropelada);
    fprintf(arquivo_stats, "Numero de movimentos na direcao oposta: %d\n", stats.n_movimentos_opostos);

    fclose(arquivo_stats);
}

tJogo InicializaJogo(char diretorio[])
{
    tJogo jogo;
    char arquivo_config_nome[1100];
    sprintf(arquivo_config_nome, "%s/config_inicial.txt", diretorio);

    FILE *arquivo_config = fopen(arquivo_config_nome, "r");

    if (arquivo_config == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de configuracao inicial.\n");
        exit(1);
    }

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
            jogo.galinha.pontos = 0;

            jogo.pistas[i].direcao = '\0';
            jogo.pistas[i].velocidade = 0;
            jogo.pistas[i].num_carros = 0;
        }
        else
        {
            jogo.pistas[i].direcao = '\0';
            jogo.pistas[i].velocidade = 0;
            jogo.pistas[i].num_carros = 0;
            continue;
        }

        fscanf(arquivo_config, "%*[^\n]");
        fscanf(arquivo_config, "%*c");
    }

    jogo.sprites = CarregaSprites(diretorio);

    fclose(arquivo_config);

    jogo.iteracao = 0;
    jogo.fim = 0;

    // DesenhaMapa(jogo, stdout);
    GeraArquivoInicializacao(jogo, diretorio);

    jogo.stats = InicializaEstatisticas(jogo.stats, jogo.qtd_pistas);

    return jogo;
}