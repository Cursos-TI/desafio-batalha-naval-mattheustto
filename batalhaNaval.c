#include <stdio.h>

/*
 Desafio Batalha Naval - MateCheck
 Integração dos níveis:
  - Novato: posicionamento de navios (horizontal + vertical)
  - Aventureiro: mais dois navios posicionados em diagonal
  - Mestre: habilidades especiais (cone, cruz e octaedro) com sobreposição ao tabuleiro

 Convenções:
  - TAB[TAM][TAM] contém 0 (água) ou 3 (navio) inicialmente.
  - EFEITO[TAM][TAM] contém 0 (não afetado) ou 1 (afetado por alguma habilidade).
  - Na exibição final, priorizamos mostrar áreas afetadas como 5; 
    caso contrário, mostramos 3 para navio e 0 para água.
*/

#define TAM 10          // tamanho do tabuleiro (10x10)
#define NAVIO 3         // tamanho fixo dos navios
#define AGUA 0          // representa água
#define NAVIO_VALOR 3   // representa parte do navio
#define EFEITO_VAL 5    // valor visual para área afetada

int main() {
    int tabuleiro[TAM][TAM];
    int efeito[TAM][TAM]; // matriz auxiliar para marcar áreas afetadas
    int i, j;

    /* -----------------------------
       Inicialização do tabuleiro
       ----------------------------- */
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            tabuleiro[i][j] = AGUA;
            efeito[i][j] = 0;
        }
    }

    /* =====================
       NÍVEL NOVATO (linear)
       ===================== */
    // Navio horizontal → começa na linha 2, coluna 4
    int linhaH = 2, colunaH = 4;
    if (colunaH + NAVIO <= TAM) {
        for (j = colunaH; j < colunaH + NAVIO; j++) {
            tabuleiro[linhaH][j] = NAVIO_VALOR;
            printf("Navio horizontal posicionado em: (%d, %d)\n", linhaH, j);
        }
    } else {
        printf("Erro: navio horizontal ultrapassa o limite do tabuleiro.\n");
        return 1;
    }

    // Navio vertical → começa na linha 5, coluna 7
    int linhaV = 5, colunaV = 7;
    if (linhaV + NAVIO <= TAM) {
        for (i = linhaV; i < linhaV + NAVIO; i++) {
            if (tabuleiro[i][colunaV] == NAVIO_VALOR) {
                printf("Erro: sobreposição detectada (linear)!\n");
                return 1;
            }
            tabuleiro[i][colunaV] = NAVIO_VALOR;
            printf("Navio vertical posicionado em: (%d, %d)\n", i, colunaV);
        }
    } else {
        printf("Erro: navio vertical ultrapassa o limite do tabuleiro.\n");
        return 1;
    }

    /* ============================
       NÍVEL AVENTUREIRO (diagonal)
       ============================ */
    // Navio diagonal principal (↘) → linhaD1,colunaD1
    int linhaD1 = 0, colunaD1 = 0;
    if (linhaD1 + NAVIO <= TAM && colunaD1 + NAVIO <= TAM) {
        for (i = 0; i < NAVIO; i++) {
            if (tabuleiro[linhaD1 + i][colunaD1 + i] == NAVIO_VALOR) {
                printf("Erro: sobreposição detectada na diagonal principal!\n");
                return 1;
            }
            tabuleiro[linhaD1 + i][colunaD1 + i] = NAVIO_VALOR;
            printf("Navio diagonal ↘ posicionado em: (%d, %d)\n", linhaD1 + i, colunaD1 + i);
        }
    } else {
        printf("Erro: navio diagonal principal ultrapassa o limite do tabuleiro.\n");
        return 1;
    }

    // Navio diagonal secundária (↙) → linhaD2,colunaD2
    int linhaD2 = 6, colunaD2 = 9;
    if (linhaD2 + NAVIO <= TAM && colunaD2 - (NAVIO - 1) >= 0) {
        for (i = 0; i < NAVIO; i++) {
            int r = linhaD2 + i;
            int c = colunaD2 - i;
            if (tabuleiro[r][c] == NAVIO_VALOR) {
                printf("Erro: sobreposição detectada na diagonal secundária!\n");
                return 1;
            }
            tabuleiro[r][c] = NAVIO_VALOR;
            printf("Navio diagonal ↙ posicionado em: (%d, %d)\n", r, c);
        }
    } else {
        printf("Erro: navio diagonal secundária ultrapassa o limite do tabuleiro.\n");
        return 1;
    }

    /* ============================
       NÍVEL MESTRE (habilidades)
       ============================ */

    // Dimensão escolhida para as matrizes de habilidade (usar 5x5 para clareza)
    const int H = 5;
    int centro = H / 2; // 5->centro=2

    // Matrizes de habilidade (5x5). Vamos inicializar dinamicamente usando loops
    int cone[H][H];
    int cruz[H][H];
    int octaedro[H][H];

    // Inicializa todas com 0
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            cone[i][j] = 0;
            cruz[i][j] = 0;
            octaedro[i][j] = 0;
        }
    }

    /* ---------------------
       Construção do CONE
       forma: topo (um ponto) que se expande para baixo
       Exemplo 5x5 (↧):
       0 0 1 0 0
       0 1 1 1 0
       1 1 1 1 1
       0 0 0 0 0
       0 0 0 0 0
       Implementação: para cada linha r (0..H-1), preenche colunas do centro-r até centro+r,
       mas limitamos o "alcance" do cone para as primeiras 3 linhas (r = 0..2) nesse exemplo.
    */
    for (i = 0; i < H; i++) {
        // limitamos a altura efetiva do cone a 3 linhas (índices 0,1,2)
        if (i <= centro) {
            int minc = centro - i;
            int maxc = centro + i;
            for (j = minc; j <= maxc; j++) {
                cone[i][j] = 1;
            }
        } else {
            // linhas abaixo do alcance permanecem 0 (poderia expandir se quisesse)
            for (j = 0; j < H; j++) cone[i][j] = 0;
        }
    }

    /* ---------------------
       Construção da CRUZ
       forma: linha central e coluna central preenchidas
       Exemplo 5x5:
       0 0 1 0 0
       0 0 1 0 0
       1 1 1 1 1
       0 0 1 0 0
       0 0 1 0 0
    */
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            if (i == centro || j == centro) cruz[i][j] = 1;
            else cruz[i][j] = 0;
        }
    }

    /* ---------------------
       Construção do OCTAEDRO (vista frontal = losango/diamante)
       Critério: positions where |r-centro| + |c-centro| <= centro -> 1
       Exemplo 5x5 (centro=2):
       0 0 1 0 0
       0 1 1 1 0
       1 1 1 1 1
       0 1 1 1 0
       0 0 1 0 0
    */
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            int dist = (i > centro ? i - centro : centro - i) + (j > centro ? j - centro : centro - j);
            if (dist <= centro) octaedro[i][j] = 1;
            else octaedro[i][j] = 0;
        }
    }

    // Pontos de origem (centro) das habilidades no tabuleiro (definidos diretamente)
    // Escolhi exemplos que não colidam com os navios principais, mas o código valida limites.
    int origem_cone_r = 1, origem_cone_c = 2;   // próximo ao topo
    int origem_cruz_r = 4, origem_cruz_c = 4;   // centro do tabuleiro
    int origem_oct_r  = 7, origem_oct_c  = 6;   // região inferior direita

    // Função inline (implementada aqui com loops) para sobrepor uma matriz HxH centrada
    // nas coordenadas (or_r, or_c) no tabuleiro, marcando efeito[][] = 1 quando a habilidade tem 1.
    // Observação: fazemos checagem de limites para não sair do tabuleiro.
    // ---------------- Sobreposição: CONE ----------------
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            if (cone[i][j] == 1) {
                int bi = origem_cone_r + (i - centro);
                int bj = origem_cone_c + (j - centro);
                if (bi >= 0 && bi < TAM && bj >= 0 && bj < TAM) {
                    efeito[bi][bj] = 1;
                }
            }
        }
    }

    // ---------------- Sobreposição: CRUZ ----------------
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            if (cruz[i][j] == 1) {
                int bi = origem_cruz_r + (i - centro);
                int bj = origem_cruz_c + (j - centro);
                if (bi >= 0 && bi < TAM && bj >= 0 && bj < TAM) {
                    efeito[bi][bj] = 1;
                }
            }
        }
    }

    // ---------------- Sobreposição: OCTAEDRO ----------------
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            if (octaedro[i][j] == 1) {
                int bi = origem_oct_r + (i - centro);
                int bj = origem_oct_c + (j - centro);
                if (bi >= 0 && bi < TAM && bj >= 0 && bj < TAM) {
                    efeito[bi][bj] = 1;
                }
            }
        }
    }

    /* ============================
       Criação da matriz de exibição
       Prioridade visual: mostrar áreas de efeito (5) quando presentes,
       caso contrário mostrar navio (3) ou água (0).
       ============================ */
    int exibicao[TAM][TAM];
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            if (efeito[i][j] == 1) {
                // área afetada ganha prioridade visual
                exibicao[i][j] = EFEITO_VAL;
            } else {
                exibicao[i][j] = tabuleiro[i][j]; // 0 ou 3
            }
        }
    }

    /* ---------------------
       Exibição detalhada:
       - Mostra as mensagens de posicionamento já impressas antes.
       - Em seguida exibe o tabuleiro final com 0, 3 e 5.
    */
    printf("\n===== TABULEIRO FINAL (0=água, 3=navio, 5=efeito) =====\n\n");
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            printf("%d ", exibicao[i][j]);
        }
        printf("\n");
    }

    /* ============================
       (Opcional) Exibição das matrizes de habilidade (para debugging/visual)
       Comentário: úteis para visualizar como cada habilidade foi construída.
       ============================ */
    printf("\nMATRIZ CONE (1 = afetado):\n");
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            printf("%d ", cone[i][j]);
        }
        printf("\n");
    }

    printf("\nMATRIZ CRUZ (1 = afetado):\n");
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            printf("%d ", cruz[i][j]);
        }
        printf("\n");
    }

    printf("\nMATRIZ OCTAEDRO (1 = afetado):\n");
    for (i = 0; i < H; i++) {
        for (j = 0; j < H; j++) {
            printf("%d ", octaedro[i][j]);
        }
        printf("\n");
    }

    return 0;
}
