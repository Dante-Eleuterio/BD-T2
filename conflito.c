#include "conflito.h"

// aux=S->first;
    // for (int i = 0; i < aux->totalOps; i++){
    //     printf("time = %d, T = %d type = %c, var = %c \n",aux->ops[i].time,aux->ops[i].T->name, aux->ops[i].type, aux->ops[i].var);
    // }
    // printf("SCHEDULE 2\n");
    // aux=aux->next;
    // for (int i = 0; i < aux->totalOps; i++){
    //     printf("time = %d, T = %d type = %c, var = %c \n",aux->ops[i].time,aux->ops[i].T->name, aux->ops[i].type, aux->ops[i].var);
    // }
    // printf("var i = %c, var j = %c, name i = %d, name j = %d\n",S->ops[i].var,S->ops[j].var,S->ops[i].T->name,S->ops[j].T->name);
void buildEdges(schedule *S){
    for (int i = 0; i < S->totalOps; i++){
        switch (S->ops[i].type){
            case 'W':
                for (int j = i+1; j < S->totalOps; j++){
                    if(S->ops[j].type=='R'){
                        if((S->ops[i].var== S->ops[j].var) && (S->ops[i].T->name != S->ops[j].T->name)) {
                            S->ops[i].T->n++;
                            realloc(S->ops[i].T->adj,S->ops[i].T->n * sizeof(transaction));
                        }

                    }else{
                        if((S->ops[i].var== S->ops[j].var) && (S->ops[i].T->name != S->ops[j].T->name)) 
                            S->graph[S->ops[i].T->name - S->firstT] = *(S->ops[j].T);
                    }
                }
                break;
            case 'R':
                for (int j = i+1; j < S->totalOps; j++){
                    if(S->ops[j].type=='W')
                        if((S->ops[i].var== S->ops[j].var) && (S->ops[i].T->name != S->ops[j].T->name)) 
                            S->graph[S->ops[i].T->name - S->firstT] = *(S->ops[j].T);
                }                
                break;        
            default:
                break;
        }
    }
    for (int i = 0; i < S->totalT; i++){
        while ()
        {
            /* code */
        }
        
        printf("Aresta de T%d para T%d",)
    }
    
    
}

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
            aux->ops[totalOps].time=time;
            aux->ops[totalOps].type=type;
            aux->ops[totalOps].var=var;
            aux->ops[totalOps].T=&(aux->graph[T-aux->firstT]);
            totalOps++;
        }
        if(commits==bigT){
            aux=aux->next;
            bigT=0;
            totalOps=0;
        }
    }
    
}

void checkInput(scheduleList *S){
    schedule *aux;
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
            aux = malloc(sizeof(schedule));
            aux->next=NULL;
            aux->name=S->total;
            aux->firstT=firstT;
            aux->totalT=commits-firstT+1;
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
            scheduleCommits=0;
        }

    }
    rewind(stdin);
}

int main(int argc, char const *argv[]){
    char line[1024];
    scheduleList S;
    S.total=0;
    int time;
    int transaction;
    char type;
    char var;
    checkInput(&S);
    updateSchedule(&S);
    buildEdges(S.first);
    
    return 0;
}
