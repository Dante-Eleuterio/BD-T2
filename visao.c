#include "visao.h"

//Funcao para calcular fatorial
int fatorial(int x){
    int aux =1;
    for (int i = 1; i <=x; i++){
        aux*=i;
    }
    return aux;
}

//Funcao para trocar posicoes de vetor
void swap(transactionV *a,transactionV *b){
    transactionV temp;
    temp = *a;
    *a = *b;
    *b = temp;
}



//Funcao para conferir a regra 2 do algoritmo de visao
void checkPermutRuleTwo(scheduleV *S,operationsV op1,operationsV op2){
    int index1,index2;
    for (int i = 0; i < S->validPermuts; i++){ //Percorre as permutacoes
        for (int j = 0; j < S->totalT; j++){    //Percorre as transacoes da permutacao
            if(S->Permuts[i].valid){
                if(S->Permuts[i].transactions[j].name==op1.T->name){ //Achou a primeira operacao
                    index1=S->Permuts[i].transactions[j].index;
                }
                if(S->Permuts[i].transactions[j].name==op2.T->name){ //Achou a segunda operacao
                    index2=S->Permuts[i].transactions[j].index;
                }
            }
        }
        if(index1>index2){ //Se a escrita passa a ocorrer antes da leitura, invalida a permutacao
            S->Permuts[i].valid=0;
            index1=0;
            index2=0;
        }
    }
}


//Funcao para conferir regra 3 do algoritmo de visao
void checkPermutRuleThree(scheduleV *S,int name,char var){
    int index=0;
    for (int i = 0; i < S->validPermuts; i++){
        if(S->Permuts[i].valid){
            for (int m = 0; m < S->totalT; m++){
                if(S->Permuts[i].transactions[m].name==name){ //Procura o index da transacao
                    index=m;
                }
            }
            for (int j = index+1; j < S->totalT; j++){
                for (int k = 0; k < S->Permuts[i].transactions[j].totalOps; k++){
                    if(S->Permuts[i].transactions[j].ops[k].type=='W' && S->Permuts[i].transactions[j].ops[k].var==var){ //Se ha outra escrita no mesmo atributo
                        if(j>index){ 
                            S->Permuts[i].valid=0; //Se a escrita ocorrer após a escrita original, invalida a permutacao
                        }
                    }
                }
            }
        }
    }
}

//Confere a visao
int checkVision(scheduleListV *S){
    scheduleV *aux=S->first;
    
    int transaction=0;
    char var;
    while (aux){
        for (int i = 0; i <aux->totalOps; i++){
            if(aux->ops[i].type=='W'){ //Achou escrita
                transaction=aux->ops[i].T->name;
                var=aux->ops[i].var;
            }
            for (int j = i+1; j <aux->totalOps; j++){//Procura nas proximas operacoes se ha escrita na mesma variavel
                if((aux->ops[i].type=='W' && aux->ops[j].type =='W') && (aux->ops[i].var == aux->ops[j].var)){
                    transaction=aux->ops[j].T->name;
                    var=aux->ops[j].var;
                }
            }
            if(transaction!=0)
                checkPermutRuleThree(aux,transaction,var);//Confere se a ultima escrita permanece
            transaction=0;
        }
        
        for (int i = 0; i <=aux->totalOps; i++){
            for (int j = i+1; j <=aux->totalOps; j++){
                if(aux->ops[i].type == 'W' && aux->ops[j].type=='R'){//Confere se há leitura após escrita na mesma variavel
                    if((aux->ops[i].var == aux->ops[j].var) && (aux->ops[i].T->name != aux->ops[j].T->name)) {
                        checkPermutRuleTwo(aux, aux->ops[i],aux->ops[j]);//Confere se a ordem permanece
                    }
                }
            }
        }
        aux=aux->next;
    }

}

//Permuta o vetor de transacoes do agendamento
void permutation(scheduleV *S,transactionV *arr,int *counter, int start, int end){
    if(start==end){
        S->Permuts[*counter].name=*counter;
        S->Permuts[*counter].totalOps=S->totalOps;
        S->Permuts[*counter].totalT = S->totalT;
        S->Permuts[*counter].firstT = S->firstT;
        S->Permuts[*counter].ops = S->ops;
        S->Permuts[*counter].valid=1;
        S->Permuts[*counter].transactions = malloc(S->totalT * sizeof(transactionV));
        for (int i = 0; i < S->totalT; i++){
            arr[i].index=i;
        }
        for (int i = 0; i < S->totalT; i++){
            S->Permuts[*counter].transactions[i] = arr[i];
        }
       
        *counter +=1;
        return;
    }
    int i;
    for(i=start;i<=end;i++){
        swap((arr+i), (arr+start));
        permutation(S,arr,counter, start+1, end);
        swap((arr+i), (arr+start));
    }
}


//Percorre todos os escalonamentos construindo suas permutacoes
void buildPermuts(scheduleListV *S){
    scheduleV *aux= S->first;
    while (aux){
        for (int i = 0; i < aux->totalT; i++){
            aux->validPermuts=fatorial(aux->totalT);
            aux->Permuts = malloc(fatorial(aux->totalT) * sizeof(scheduleV));
        }
        aux=aux->next;        
    }
    aux=S->first;
    while (aux){
        int a=0;
        transactionV T[aux->totalT];
        for (int i = 0; i < aux->totalT; i++){
            T[i] = aux->transactions[i];
        }
        permutation(aux,T,&a,0,aux->totalT-1);
        aux=aux->next;        
    }
    checkVision(S);
}

//Atualiza as variaveis do schedule de visao
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
            aux->transactions[aux->ops[i].T->name - aux->firstT].ops[aux->transactions[aux->ops[i].T->name - aux->firstT].counter]= aux->ops[i]; //Gambiarra de ponteiros para guardar a operação em uma transacao
            aux->transactions[aux->ops[i].T->name - aux->firstT].counter++; //Aumenta o contador de transacoes
        }
        aux=aux->next;
    }

    rewind(stdin);
}

//Le o input e faz os mallocs
void checkInputVision(scheduleListV *S){
    scheduleV *aux;
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
        if(commits==bigT){
            S->total++;
            aux = malloc(sizeof(scheduleV));
            aux->next=NULL;
            aux->name=S->total;
            aux->valid=1;
            aux->firstT=smallT;
            aux->totalT=scheduleCommits;
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
            smallT=10000;
            scheduleCommits=0;
        }

    }
    rewind(stdin);
}
