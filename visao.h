#include<stdio.h>
#include<stdlib.h>
#define INITIAL 0
#define VISITED 1
#define FINISHED 2

typedef struct scheduleListV{
    struct scheduleV *first; 
    struct scheduleV *last;
    int total; 
}scheduleListV;

typedef struct scheduleV{
    int name; //ID do Schedule
    int firstT; //ID da primeira Transaction
    int totalT; //Total de Transactions
    int totalOps; //Total de operações
    int valid;  //Se já morreu
    int validPermuts;
    struct transactionV *transactions; //Lista de transactions
    struct operationsV *ops; //Lista de operações
    struct scheduleV *next; //Ponteiro pro proximo schedule
    struct scheduleListV *Permuts; //Ponteiro pruma lista de permutações
}scheduleV;

typedef struct operationsV{
    struct transactionV *T; //Ponteiro pra Transaction referenciada
    int time; //Tempo lógico da Op
    char type; //Tipo da op (R,W)
    char var; //Variavel referenciada
}operationsV;

typedef struct transactionV{
    int name; //Id da transaction
    int totalOps; //Total de operações
    int counter;
    struct operationsV *ops; //Lista de operações
}transactionV;

void updateScheduleVisao(scheduleListV *S);
void checkInputVision(scheduleListV *S);
void imprime(scheduleV *S);
int fatorial(int x);