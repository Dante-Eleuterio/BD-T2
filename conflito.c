#include "conflito.h"


//Funcao de busca em profundidade em um grafo
int DepthSearch(transaction *T){
    T->status=VISITED;
    for (int i = 0; i < T->n; i++){
        if(T->adj[i].status==INITIAL){
            if(!DepthSearch(&(T->adj[i]))){
                return 0;
            }
        }
        if(T->adj[i].status==VISITED){
            return 0;
        }
    }
    T->status=FINISHED;
    return 1;
}

//Parte 1 da funcao de busca em profundidade em um grafo
int detectCycle(schedule *S){
    for (int i = 0; i < S->totalT; i++){
        if(S->graph[i].status==INITIAL){
            if(!DepthSearch(&(S->graph[i])))
                return 1;
        }
    }
    return 0;
}


//Constroi as arestas do grafo seguindo as regras do algoritmo de conflito
void buildEdges(schedule *S){
    //Percorre contando a quantidade de arestas
    for (int i = 0; i <=S->totalOps; i++){
        for (int j = i+1; j <=S->totalOps; j++){
            if( (S->ops[i].type == 'W' && S->ops[j].type=='R') ||
                (S->ops[i].type == 'R' && S->ops[j].type=='W') ||
                (S->ops[i].type == 'W' && S->ops[j].type=='W')){
                    if((S->ops[i].var == S->ops[j].var) && (S->ops[i].T->name != S->ops[j].T->name)) {
                            S->ops[i].T->n++;
                    }    
                }
        }
    }
    //Reserva memoria
    for (int i = 0; i < S->totalT; i++){
        S->graph[i].adj = malloc(S->graph[i].n * sizeof(transaction));
    }
    //Constroi as arestas de fato
    for (int i = 0; i <=S->totalOps; i++){
        for (int j = i+1; j <=S->totalOps; j++){
            if( (S->ops[i].type == 'W' && S->ops[j].type=='R') ||
                (S->ops[i].type == 'R' && S->ops[j].type=='W') ||
                (S->ops[i].type == 'W' && S->ops[j].type=='W')){
                    if((S->ops[i].var == S->ops[j].var) && (S->ops[i].T->name != S->ops[j].T->name)) {
                            S->ops[i].T->adj[S->ops[i].T->count]=*(S->ops[j].T);
                            S->ops[i].T->count++;
                    }    
                }
        }
    }
}

//Funcao meio gambiarra por problemas com ponteiro
void edges(scheduleList *S){
    schedule *aux = S->first;
    while(aux){
        buildEdges(aux);
        aux=aux->next;
    }
    
}

//Guardar os dados das transacoes no escalonamento
void updateSchedule(scheduleList *S){
    schedule *aux = S->first;
    char line[1024];
    int time=0;
    int T=0;    //transaction
    char type;
    char var;
    int commits=0;         
    int totalOps=0;
    int bigT=0; //Maior transaction
    while(!feof(stdin)){
        fgets(line,1000,stdin);
        sscanf(line,"%d %d %c %c",&time,&T,&type,&var);
        if(T>bigT){
            bigT=T;
        }
        if(type=='C'){
            commits++;
        }else{
            aux->graph[T-aux->firstT].name=T;
            aux->graph[T-aux->firstT].n=0;
            aux->graph[T-aux->firstT].count=0;
            aux->graph[T-aux->firstT].status=INITIAL;
            aux->ops[totalOps].time=time;
            aux->ops[totalOps].type=type;
            aux->ops[totalOps].var=var;
            aux->ops[totalOps].T=&(aux->graph[T-aux->firstT]);
            totalOps++;
        }
        if(commits==bigT){ //Acabou os commits
            aux=aux->next;
            bigT=0;
            totalOps=0;
        }
    }
    rewind(stdin);
}

//Confere o input
void checkInput(scheduleList *S){
    schedule *aux;
    char line[1024];
    int totalOps=0;
    int time=0;
    int scheduleCommits=0; //Total de commits do schedule atual
    int commits=0;         //Total de commits de tudo
    int bigT=0; //Maior transaction
    int smallT=10000; //Menor transaction
    int T=0;    //transaction
    int firstTime=0;
    char type;
    char var;
    while(!feof(stdin)){
        fgets(line,1000,stdin);
        sscanf(line,"%d %d %c %c",&time,&T,&type,&var);
        if(bigT==0){
            firstTime=time;
        }
        if(T>bigT){
            bigT=T;
        }
        if(T<smallT){
            smallT=T;
        }
        if(type=='C'){
            scheduleCommits++;
            commits++;
        }
        if(commits==bigT){ //Acabou os commits
            S->total++;
            aux = malloc(sizeof(schedule));
            aux->next=NULL;
            aux->name=S->total;
            aux->firstT=smallT;
            aux->totalT=scheduleCommits;
            aux->totalOps = time-scheduleCommits-firstTime+1; //FUNCIONA KKK
            aux->graph = malloc(aux->totalT * sizeof(transaction));
            aux->ops = malloc(aux->totalOps * sizeof(operations));
            if(S->total==1){
                S->first=aux;
                S->last=aux;
            }else{
                S->last->next=aux;
                S->last=aux;
            }
            bigT=0;
            smallT = 10000;
            scheduleCommits=0;
        }

    }
    rewind(stdin);
}


