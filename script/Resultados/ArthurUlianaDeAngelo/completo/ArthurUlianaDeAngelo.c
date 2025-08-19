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
    char carro[4][6];
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

/* FUNCOES PARA LER E GERAR A CONFIGURACAO INICIAL */
tJogo InicializaJogo(char diretorio[]);
void EscreveInicializacao(tJogo jogo, char diretorio[]);
tSprites CarregaSprites(char diretorio[], int animacao);

/* FUNCOES DA GERACAO DO ARQUIVO DE RESUMO */
FILE *AbreArquivoResumo(char diretorio[]);
void EscreveColisaoResumo(int iteracao, int n_pista, tPista t_pista, int x, int galinha_x, int galinha_y, char diretorio[]);
void EscreveFimJogoResumo(int iteracao, char diretorio[]);

/* FUNCOES DA GERACAO DO ARQUIVO DE RANKING */
FILE *AbreArquivoRanking(char diretorio[]);
void EscreveRanking(tRanking rankings[], char diretorio[], int n_atropelamentos);
void OrdenaRankings(tRanking rankings[], int n_atropelamentos);

/* FUNCOES DA GERACAO DO ARQUIVO DE ESTATISTICA */
FILE *AbreArquivoEstatistics(char diretorio[]);
tEstatisticas InicializaEstatisticas(tEstatisticas stats, int qtd_pistas);
tEstatisticas AtualizaMovimentos(tEstatisticas stats, char mov);
tEstatisticas AtualizaAlturasAtropelamenteEstatisticas(tEstatisticas stats, tGalinha galinha, int qtd_pistas);
tEstatisticas AtualizaAlturaMaxima(tEstatisticas stats, tGalinha galinha, int qtd_pistas);
void EscreveEstatisticas(tEstatisticas stats, char diretorio[]);

/* FUNCOES DA GERACAO DO ARQUIVO DE HEATMAP */
FILE *AbreArquivoHeatpmap(char diretorio[]);
void InicializaHeatmap(int heatmap[36][100], int linhas, int colunas);
void AtualizaHeatmap(int heatmap[36][100], int linhas, int colunas, int xGalinha, int yGalinha, int atropelamento);
void EscreveHeatmap(int heatmap[36][100], int linhas, int colunas, char diretorio[]);

/* FUNCOES PARA GERENCIAR JOGADAS */
char LeJogada();

/* FUNCOES DA GALINHA */
tGalinha FazJogada(tGalinha galinha, char jogada, int qtd_pistas);
tGalinha PerdeVidaGalinha(tGalinha galinha);
tGalinha ResetaPontosGalinha(tGalinha galinha);
tGalinha ResetaYGalinha(tGalinha galinha, int novo_y);

/* FUNCOES DAS PISTAS */
tPista DiminuiVelocidadePista(tPista pista);
tPista DeslocaCarrosPista(tPista pista, int largura_mapa);
int PegaNumeroCarroPista(int x, tPista pista);
tJogo DeslocaTodasPistas(tJogo jogo);

/* FUNCOES DOS CARROS */
tCarro DeslocaCarro(int x, int velocidade, char direcao, int largura_mapa);

/* FUNCOES PARA GERENCIAR O JOGO */
tJogo JogaJogo(tJogo jogo, char diretorio[]);
int VerificaFim(tJogo jogo);
int VerificaColisoes(tJogo jogo);
void DesenhaMapa(tJogo jogo, FILE *saida);
void ImprimeFimJogo(tJogo jogo);

/* FUNCOES PARA LER E GERAR A CONFIGURACAO INICIAL */
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

    jogo.sprites = CarregaSprites(diretorio, jogo.animacao);

    fclose(arquivo_config);

    jogo.iteracao = 0;
    jogo.fim = 0;

    // DesenhaMapa(jogo, stdout);
    EscreveInicializacao(jogo, diretorio);

    jogo.stats = InicializaEstatisticas(jogo.stats, jogo.qtd_pistas);

    return jogo;
}

FILE *AbreArquivoInicializacao(char diretorio[])
{
    char caminho_arquivo[1100];
    sprintf(caminho_arquivo, "%s/saida/inicializacao.txt", diretorio);

    FILE *arquivo = fopen(caminho_arquivo, "w");

    if (arquivo == NULL)
    {
        printf("ERRO: Erro ao criar arquivo de saida da inicializacao em %s\n", caminho_arquivo);
        exit(1);
    }

    return arquivo;
}

void EscreveInicializacao(tJogo jogo, char diretorio[]) // Gera o arquivo
{
    FILE *arquivo = AbreArquivoInicializacao(diretorio);

    DesenhaMapa(jogo, arquivo);
    fprintf(arquivo, "A posicao central da galinha iniciara em (%d %d).", jogo.galinha.x, jogo.galinha.y * 3 + 1);

    fclose(arquivo);
}

tSprites CarregaSprites(char diretorio[], int animacao) // Le os sprites
{
    char arquivo_personagens_nome[1100];
    sprintf(arquivo_personagens_nome, "%s/personagens.txt", diretorio);

    tSprites sprites;

    FILE *arquivo = fopen(arquivo_personagens_nome, "r");

    fscanf(arquivo, "%c%c%c\n%c%c%c\n", &sprites.galinha[0], &sprites.galinha[1], &sprites.galinha[2], &sprites.galinha[3], &sprites.galinha[4], &sprites.galinha[5]);

    if (animacao)
    {
        int i;
        for (i = 0; i < 4; i++)
        {
            fscanf(arquivo, "%c%c%c\n%c%c%c\n", &sprites.carro[i][0], &sprites.carro[i][1], &sprites.carro[i][2], &sprites.carro[i][3], &sprites.carro[i][4], &sprites.carro[i][5]);
        }
    }
    else
    {
        fscanf(arquivo, "%c%c%c\n%c%c%c\n", &sprites.carro[0][0], &sprites.carro[0][1], &sprites.carro[0][2], &sprites.carro[0][3], &sprites.carro[0][4], &sprites.carro[0][5]);
    }

    fclose(arquivo);

    return sprites;
}

/* FUNCOES DA GERACAO DO ARQUIVO DE RESUMO */
FILE *AbreArquivoResumo(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/resumo.txt", diretorio);

    FILE *arquivo_resumo = fopen(arquivo_nome, "a+");

    if (arquivo_resumo == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de resumo %s\n", arquivo_nome);
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

void EscreveFimJogoResumo(int iteracao, char diretorio[])

{
    FILE *arquivo_resumo = AbreArquivoResumo(diretorio);

    fprintf(arquivo_resumo, "[%d] Fim de jogo\n", iteracao);

    fclose(arquivo_resumo);
}

/* FUNCOES DA GERACAO DO ARQUIVO DE RANKING */
FILE *AbreArquivoRanking(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/ranking.txt", diretorio);

    FILE *arquivo_ranking = fopen(arquivo_nome, "w+");

    if (arquivo_ranking == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de ranking %s\n", arquivo_nome);
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

/* FUNCOES DA GERACAO DO ARQUIVO DE ESTATISTICA */
FILE *AbreArquivoEstatistics(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/estatistica.txt", diretorio);

    FILE *arquivo_stats = fopen(arquivo_nome, "w");

    if (arquivo_stats == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de estatisticas %s\n", arquivo_nome);
        exit(1);
    }

    return arquivo_stats;
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
    FILE *arquivo_stats = AbreArquivoEstatistics(diretorio);

    fprintf(arquivo_stats, "Numero total de movimentos: %d\n", stats.n_movimentos);
    fprintf(arquivo_stats, "Altura maxima que a galinha chegou: %d\n", stats.altura_maxima_alcancada);
    fprintf(arquivo_stats, "Altura maxima que a galinha foi atropelada: %d\n", stats.altura_maxima_atropelada);
    fprintf(arquivo_stats, "Altura minima que a galinha foi atropelada: %d\n", stats.altura_minima_atropelada);
    fprintf(arquivo_stats, "Numero de movimentos na direcao oposta: %d\n", stats.n_movimentos_opostos);

    fclose(arquivo_stats);
}

/* FUNCOES DA GERACAO DO ARQUIVO DE HEATMAP */
FILE *AbreArquivoHeatpmap(char diretorio[])
{
    char arquivo_nome[1100];
    sprintf(arquivo_nome, "%s/saida/heatmap.txt", diretorio);

    FILE *arquivo_heatmap = fopen(arquivo_nome, "w");

    if (arquivo_heatmap == NULL)
    {
        printf("ERRO: Erro ao abrir o arquivo de heatmap %s\n", arquivo_nome);
        exit(1);
    }

    return arquivo_heatmap;
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

/* FUNCOES PARA GERENCIAR JOGADAS */
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

tGalinha FazJogada(tGalinha galinha, char jogada, int qtd_pistas)
{
    if (jogada == 'w')
        galinha.y--;
    else if (jogada == 's' && galinha.y < qtd_pistas - 1)
        galinha.y++;

    return galinha;
}

tGalinha PerdeVidaGalinha(tGalinha galinha)
{
    galinha.vidas--;

    return galinha;
}

tGalinha ResetaPontosGalinha(tGalinha galinha)
{
    galinha.pontos = 0;

    return galinha;
}

tGalinha ResetaYGalinha(tGalinha galinha, int novo_y)
{
    galinha.y = novo_y;

    return galinha;
}

/* FUNCOES DAS PISTAS */
tPista DiminuiVelocidadePista(tPista pista)
{
    if (pista.velocidade > 1)
        pista.velocidade--;
    return pista;
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

tJogo DeslocaTodasPistas(tJogo jogo)
{
    int i;
    for (i = 0; i < jogo.qtd_pistas; i++)
    {
        jogo.pistas[i] = DeslocaCarrosPista(jogo.pistas[i], jogo.largura_mapa);
    }

    return jogo;
}

/* FUNCOES DOS CARROS */
tCarro DeslocaCarro(int x, int velocidade, char direcao, int largura_mapa)
{
    tCarro carro;
    velocidade = (direcao == 'E') ? -velocidade : velocidade;

    carro.x = (x + velocidade) % largura_mapa;

    if (carro.x < 0)
        carro.x += largura_mapa;

    return carro;
}

/* FUNCOES PARA GERENCIAR O JOGO */
tJogo JogaJogo(tJogo jogo, char diretorio[])
{
    tRanking rankings[jogo.galinha.vidas];
    int vidas_iniciais = jogo.galinha.vidas;
    int n_atropelamentos = 0;

    int heatpmap[36][100];

    InicializaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa);
    AtualizaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, jogo.galinha.x, jogo.galinha.y, 0);

    DesenhaMapa(jogo, stdout);

    while (!jogo.fim)
    {
        char jogada = LeJogada();

        jogo.galinha = FazJogada(jogo.galinha, jogada, jogo.qtd_pistas);

        jogo.iteracao++;

        jogo.stats = AtualizaMovimentos(jogo.stats, jogada);

        jogo = DeslocaTodasPistas(jogo);

        int carro_x = VerificaColisoes(jogo);

        AtualizaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, jogo.galinha.x, jogo.galinha.y, (carro_x != -1));

        if (carro_x != -1)
        {
            jogo.galinha = PerdeVidaGalinha(jogo.galinha);
            jogo.galinha = ResetaPontosGalinha(jogo.galinha);

            EscreveColisaoResumo(jogo.iteracao, jogo.galinha.y, jogo.pistas[jogo.galinha.y], carro_x, jogo.galinha.x, jogo.galinha.y, diretorio);

            jogo.stats = AtualizaAlturasAtropelamenteEstatisticas(jogo.stats, jogo.galinha, jogo.qtd_pistas);

            tRanking atropelamento = {jogo.galinha.y + 1, PegaNumeroCarroPista(carro_x, jogo.pistas[jogo.galinha.y]), jogo.iteracao};
            rankings[n_atropelamentos] = atropelamento;
            n_atropelamentos++;

            if (jogo.animacao == 1)
            {
                jogo.pistas[jogo.galinha.y] = DiminuiVelocidadePista(jogo.pistas[jogo.galinha.y]);
            }

            jogo.galinha = ResetaYGalinha(jogo.galinha, jogo.qtd_pistas - 1);

            AtualizaHeatmap(heatpmap, jogo.qtd_pistas * 3, jogo.largura_mapa, jogo.galinha.x, jogo.galinha.y, 0);
        }
        else if (carro_x == -1 && jogada == 'w')
        {
            jogo.galinha.pontos++;
            jogo.stats = AtualizaAlturaMaxima(jogo.stats, jogo.galinha, jogo.qtd_pistas);
        }

        jogo.fim = VerificaFim(jogo);

        if (jogo.fim == VITORIA)
            jogo.galinha.pontos += 10;

        DesenhaMapa(jogo, stdout);

        if (jogo.fim)
            break;
    }

    if (jogo.stats.altura_minima_atropelada == jogo.qtd_pistas * 3 + 1)
        jogo.stats.altura_minima_atropelada = 0;

    if (jogo.stats.altura_maxima_alcancada == 0)
        jogo.stats.altura_maxima_alcancada = 2;

    // DesenhaMapa(jogo, stdout);

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

            if (jogo.animacao)
            {
                mapa[linha][anterior] = jogo.sprites.carro[jogo.iteracao % 4][0];
                mapa[linha][meio] = jogo.sprites.carro[jogo.iteracao % 4][1];
                mapa[linha][posterior] = jogo.sprites.carro[jogo.iteracao % 4][2];
                mapa[linha + 1][anterior] = jogo.sprites.carro[jogo.iteracao % 4][3];
                mapa[linha + 1][meio] = jogo.sprites.carro[jogo.iteracao % 4][4];
                mapa[linha + 1][posterior] = jogo.sprites.carro[jogo.iteracao % 4][5];
            }
            else
            {
                mapa[linha][anterior] = jogo.sprites.carro[0][0];
                mapa[linha][meio] = jogo.sprites.carro[0][1];
                mapa[linha][posterior] = jogo.sprites.carro[0][2];
                mapa[linha + 1][anterior] = jogo.sprites.carro[0][3];
                mapa[linha + 1][meio] = jogo.sprites.carro[0][4];
                mapa[linha + 1][posterior] = jogo.sprites.carro[0][5];
            }
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
