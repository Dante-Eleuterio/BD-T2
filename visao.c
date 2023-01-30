#include "visao.h"

void imprime(scheduleV *S){
    for (int i = 0; i <S->totalT; i++){
        printf("T%d: ",S->transactions[i].name);
        for (int j = 0; j < S->transactions[i].totalOps; j++){
            printf("%d %c %c\n",S->transactions[i].ops[j].time,S->transactions[i].ops[j].type,S->transactions[i].ops[j].var);
        }
    }
        
}

int fatorial(int x){
    int aux =1;
    for (int i = 1; i <=x; i++){
        aux*=i;
    }
    return aux;
}

void buildPermuts(scheduleListV *S){
    scheduleV *aux= S->first;
    while (aux){
        for (int i = 0; i < aux->totalT; i++){
            aux->validPermuts=fatorial(aux->totalT);
            aux->Permuts = malloc(fatorial(aux->totalT));
        }
        aux=aux->next;        
    }
    aux=S->first;
    //PERMUTAR AQUI
}


void updateScheduleVisao(scheduleListV *S){
    scheduleV *aux = S->first;
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
            aux->transactions[T-aux->firstT].name=T;
            aux->transactions[T-aux->firstT].totalOps=0;
            aux->transactions[T-aux->firstT].counter=0;
            aux->ops[totalOps].time=time;
            aux->ops[totalOps].type=type;
            aux->ops[totalOps].var=var;
            aux->ops[totalOps].T=&(aux->transactions[T-aux->firstT]);
            totalOps++;
        }
        if(commits==bigT){
            aux=aux->next;
            bigT=0;
            totalOps=0;
        }
    }
    aux = S->first;
    while (aux){
        for (int i = 0; i <aux->totalOps; i++){
            aux->transactions[aux->ops[i].T->name - aux->firstT].totalOps++;
        }
        for (int i = 0; i <aux->totalT; i++){
            aux->transactions[i].ops= malloc(aux->transactions[i].totalOps * sizeof(operationsV));
        }
        for (int i = 0; i <aux->totalOps; i++){
            aux->transactions[aux->ops[i].T->name - aux->firstT].ops[aux->transactions[aux->ops[i].T->name - aux->firstT].counter]= aux->ops[i];
            aux->transactions[aux->ops[i].T->name - aux->firstT].counter++;
        }
        aux=aux->next;
    }

    rewind(stdin);
}


void checkInputVision(scheduleListV *S){
    scheduleV *aux;
    aux->next=NULL;
    char line[1024];
    int totalOps=0;
    int time=0;
    int scheduleCommits=0; //Total de commits do schedule atual
    int commits=0;         //Total de commits de tudo
    int bigT=0; //Maior transaction
    int T=0;    //transaction
    int firstT=0;
    int firstTime=0;
    char type;
    char var;
    while(!feof(stdin)){
        fgets(line,1000,stdin);
        sscanf(line,"%d %d %c %c",&time,&T,&type,&var);
        if(bigT==0){
            firstT=T;
            firstTime=time;
        }
        if(T>bigT){
            bigT=T;
        }
        if(type=='C'){
            scheduleCommits++;
            commits++;
        }
        if(commits==bigT){
            S->total++;
            aux = malloc(sizeof(scheduleV));
            aux->next=NULL;
            aux->name=S->total;
            aux->valid=1;
            aux->firstT=firstT;
            aux->totalT=commits-firstT+1;
            aux->totalOps = time-scheduleCommits-firstTime+1; //FUNCIONA KKK
            aux->transactions = malloc(aux->totalT * sizeof(transactionV));
            aux->ops = malloc(aux->totalOps * sizeof(operationsV));
            if(S->total==1){
                S->first=aux;
                S->last=aux;
            }else{
                S->last->next=aux;
                S->last=aux;
            }
            bigT=0;
            scheduleCommits=0;
        }

    }
    rewind(stdin);
}
