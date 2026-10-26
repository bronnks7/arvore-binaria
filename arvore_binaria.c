/*
 * Empresa Renalf S/A
 * Manipulação de Árvore Binária de Busca (ABB)
 * - Inclusão e remoção de nós
 * - Percursos em profundidade: pré-ordem, em ordem e pós-ordem
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

/* Estrutura do nó da árvore */
typedef struct No {
    int valor;
    struct No *esq;
    struct No *dir;
} No;

/* Cria um novo nó com o valor informado */
No *criarNo(int valor) {
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

/* Inclui um valor na árvore (não permite valores repetidos).
   O parâmetro 'ok' recebe 1 se incluiu e 0 se o valor já existia. */
No *incluir(No *raiz, int valor, int *ok) {
    if (raiz == NULL) {
        *ok = 1;
        return criarNo(valor);
    }
    if (valor < raiz->valor) {
        raiz->esq = incluir(raiz->esq, valor, ok);
    } else if (valor > raiz->valor) {
        raiz->dir = incluir(raiz->dir, valor, ok);
    } else {
        *ok = 0; /* valor repetido */
    }
    return raiz;
}

/* Retorna o nó de menor valor da subárvore */
No *menorNo(No *raiz) {
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}

/* Remove um valor da árvore.
   O parâmetro 'ok' recebe 1 se removeu e 0 se o valor não foi encontrado. */
No *remover(No *raiz, int valor, int *ok) {
    if (raiz == NULL) {
        *ok = 0; /* valor não encontrado */
        return NULL;
    }

    if (valor < raiz->valor) {
        raiz->esq = remover(raiz->esq, valor, ok);
    } else if (valor > raiz->valor) {
        raiz->dir = remover(raiz->dir, valor, ok);
    } else {
        *ok = 1;

        /* Caso 1: nó folha (sem filhos) */
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            return NULL;
        }
        /* Caso 2: nó com apenas um filho */
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        }
        if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }
        /* Caso 3: nó com dois filhos.
           Substitui pelo sucessor (menor valor da subárvore direita). */
        No *sucessor = menorNo(raiz->dir);
        raiz->valor = sucessor->valor;
        int aux;
        raiz->dir = remover(raiz->dir, sucessor->valor, &aux);
    }
    return raiz;
}

/* Percurso em pré-ordem: raiz, esquerda, direita */
void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

/* Percurso em ordem: esquerda, raiz, direita */
void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        emOrdem(raiz->dir);
    }
}

/* Percurso em pós-ordem: esquerda, direita, raiz */
void posOrdem(No *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->valor);
    }
}

/* Libera toda a memória da árvore */
void liberar(No *raiz) {
    if (raiz != NULL) {
        liberar(raiz->esq);
        liberar(raiz->dir);
        free(raiz);
    }
}

/* Lê um inteiro do teclado com validação */
int lerInteiro(const char *mensagem) {
    char linha[128];

    for (;;) {
        char *fim;
        long valor;
        int linha_completa;

        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo...\n");
            exit(0);
        }

        linha_completa = strchr(linha, '\n') != NULL || feof(stdin);
        if (!linha_completa) {
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
                /* Descarta o restante de uma linha maior que o buffer. */
            }
            printf("Entrada invalida. Digite um numero inteiro: ");
            continue;
        }

        errno = 0;
        valor = strtol(linha, &fim, 10);
        while (isspace((unsigned char)*fim)) {
            fim++;
        }

        if (fim == linha || *fim != '\0' || errno == ERANGE ||
            valor < INT_MIN || valor > INT_MAX) {
            printf("Entrada invalida. Digite um numero inteiro: ");
            continue;
        }
        return (int)valor;
    }
}

int main() {
    No *raiz = NULL;
    int opcao, valor, ok;

    do {
        printf("\n* * * MENU DE OPCOES * * *\n\n");
        printf("1. Incluir no.\n");
        printf("2. Remover no.\n");
        printf("3. Exibir percurso em pre-ordem.\n");
        printf("4. Exibir percurso em ordem.\n");
        printf("5. Exibir percurso em pos-ordem.\n\n");
        opcao = lerInteiro("Opcao [0 para encerrar]: ");

        switch (opcao) {
            case 1:
                valor = lerInteiro("Digite o valor a incluir: ");
                raiz = incluir(raiz, valor, &ok);
                if (ok)
                    printf("No %d incluido com sucesso.\n", valor);
                else
                    printf("O valor %d ja existe na arvore.\n", valor);
                break;

            case 2:
                if (raiz == NULL) {
                    printf("A arvore esta vazia.\n");
                    break;
                }
                valor = lerInteiro("Digite o valor a remover: ");
                raiz = remover(raiz, valor, &ok);
                if (ok)
                    printf("No %d removido com sucesso.\n", valor);
                else
                    printf("O valor %d nao foi encontrado.\n", valor);
                break;

            case 3:
                if (raiz == NULL) {
                    printf("A arvore esta vazia.\n");
                } else {
                    printf("Pre-ordem: ");
                    preOrdem(raiz);
                    printf("\n");
                }
                break;

            case 4:
                if (raiz == NULL) {
                    printf("A arvore esta vazia.\n");
                } else {
                    printf("Em ordem: ");
                    emOrdem(raiz);
                    printf("\n");
                }
                break;

            case 5:
                if (raiz == NULL) {
                    printf("A arvore esta vazia.\n");
                } else {
                    printf("Pos-ordem: ");
                    posOrdem(raiz);
                    printf("\n");
                }
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    liberar(raiz);
    return 0;
}
