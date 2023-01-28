#include<stdio.h>
#include<stdlib.h>
#define INITIAL 0
#define VISITED 1
#define FINISHED 2

typedef struct scheduleList{
    struct schedule *first; 
    struct schedule *last;
    int total; 
}scheduleList;

typedef struct schedule{
    int name; //ID do Schedule
    int firstT; //ID da primeira Transaction
    int totalT; //Total de Transactions
    int totalOps; //Total de operações
    struct transaction *graph; //Lista de vértices do grafo
    struct operations *ops; //Lista de operações
    struct schedule *next; //Ponteiro pro proximo schedule
}schedule;

typedef struct operations{
    struct transaction *T; //Ponteiro pra Transaction referenciada
    int time; //Tempo lógico da Op
    char type; //Tipo da op (R,W)
    char var; //Variavel referenciada
}operations;

typedef struct transaction{
    int name; //Id da transaction
    int n; //Vizinhança
    int status; //Para o algoritmo de ciclo em grafo
    struct transaction *adj; //Arestas do nodo
}transaction;
