#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Tamanho máximo da fila circular
#define MAX_PILHA 3  // Capacidade máxima da pilha de reserva

// -------------------- Estruturas de Dados --------------------

// Estrutura que representa uma peça do jogo
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;


// -------------------- Funções da Fila --------------------

void inicializarFila(Fila *f) {
    if (!f) return;
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaCheia(const Fila *f) {
    return f && f->tamanho == MAX_FILA;
}

int filaVazia(const Fila *f) {
    return !f || f->tamanho == 0;
}

// Insere uma peça no final da fila (enqueue)
int enqueue(Fila *f, Peca nova) {
    if (!f || filaCheia(f)) return 0;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = nova;
    f->tamanho++;
    return 1;
}

// Remove a peça da frente da fila (dequeue)
int dequeue(Fila *f, Peca *removida) {
    if (!f || !removida || filaVazia(f)) return 0;
    *removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return 1;
}

// Exibe a fila completa
void exibirFila(const Fila *f) {
    if (!f || filaVazia(f)) {
        printf("Fila de peças\t(vazia)\n");
        return;
    }

    printf("Fila de peças\t");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}


// -------------------- Funções da Pilha --------------------

void inicializarPilha(Pilha *p) {
    if (!p) return;
    p->topo = -1;
}

int pilhaCheia(const Pilha *p) {
    return p && p->topo == MAX_PILHA - 1;
}

int pilhaVazia(const Pilha *p) {
    return !p || p->topo == -1;
}

int push(Pilha *p, Peca nova) {
    if (!p || pilhaCheia(p)) return 0;
    p->itens[++p->topo] = nova;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (!p || !removida || pilhaVazia(p)) return 0;
    *removida = p->itens[p->topo--];
    return 1;
}

void exibirPilha(const Pilha *p) {
    printf("Pilha de reserva \t(Topo -> Base): ");
    if (!p || pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}


// -------------------- Funções Auxiliares --------------------

// Gera uma nova peça automaticamente
Peca gerarPeca(int idAtual) {
    Peca nova;
    const char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = idAtual;
    return nova;
}

// Exibe o estado completo
void exibirEstado(const Fila *fila, const Pilha *pilha) {
    printf("\n====================================\n");
    printf("Estado atual:\n\n");
    exibirFila(fila);
    exibirPilha(pilha);
    printf("====================================\n");
}

// Lê uma opção inteira com validação
int lerOpcao() {
    int opcao;
    char lixo;
    if (scanf("%d", &opcao) != 1) {
        while ((lixo = getchar()) != '\n' && lixo != EOF);
        printf("Entrada inválida. Digite um número entre 0 e 5.\n");
        return -1;
    }
    while ((lixo = getchar()) != '\n' && lixo != EOF);
    return opcao;
}


// -------------------- Novas Funções de Troca --------------------

// Troca a peça da frente da fila com o topo da pilha
int trocarPeeca(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) return 0;

    int idxFrente = fila->frente;
    Peca temp = fila->itens[idxFrente];
    fila->itens[idxFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    return 1;
}

// Troca múltipla: as 3 primeiras da fila com as 3 da pilha
int trocaMultipla(Fila *fila, Pilha *pilha) {
    if (!fila || !pilha) return 0;
    if (fila->tamanho < 3 || pilha->topo < 2) return 0;

    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->frente + i) % MAX_FILA;
        int idxPilha = pilha->topo - i;

        Peca temp = fila->itens[idxFila];
        fila->itens[idxFila] = pilha->itens[idxPilha];
        pilha->itens[idxPilha] = temp;
    }

    return 1;
}


// -------------------- Função Principal --------------------
int main() {
    srand((unsigned)time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idGlobal = 0;
    int opcao = -1;

    // Inicializa a fila com 5 peças iniciais
    printf("Inicializando fila de peças...\n");
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(idGlobal++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça da frente com o topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        opcao = lerOpcao();
        if (opcao == -1) continue;

        Peca p; // variável temporária

        switch (opcao) {
            case 1: // Jogar peça
                if (dequeue(&fila, &p)) {
                    printf("Peça jogada: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca(idGlobal++)); // repõe peça
                } else {
                    printf("Não há peças para jogar.\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("A pilha está cheia. Use uma peça antes de reservar outra.\n");
                } else if (dequeue(&fila, &p)) {
                    push(&pilha, p);
                    printf("Peça [%c %d] reservada com sucesso!\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca(idGlobal++)); // repõe na fila
                } else {
                    printf("Fila vazia. Não há peça para reservar.\n");
                }
                break;

            case 3: // Usar peça reservada
                if (pop(&pilha, &p)) {
                    printf("Peça usada da reserva: [%c %d]\n", p.nome, p.id);
                } else {
                    printf("A pilha está vazia. Nenhuma peça para usar.\n");
                }
                break;

            case 4: // Troca simples
                if (trocarPeeca(&fila, &pilha)) {
                    printf("Troca realizada entre frente da fila e topo da pilha.\n");
                } else {
                    printf("Não foi possível realizar a troca (verifique se há peças em ambas as estruturas).\n");
                }
                break;

            case 5: // Troca múltipla
                if (trocaMultipla(&fila, &pilha)) {
                    printf("Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
                } else {
                    printf("Não há peças suficientes para a troca múltipla.\n");
                }
                break;

            case 0:
                printf("\nEncerrando simulação...\n");
                break;

            default:
                printf("Opção inválida. Digite 0, 1, 2, 3, 4 ou 5.\n");
        }

    } while (opcao != 0);

    return 0;
}
