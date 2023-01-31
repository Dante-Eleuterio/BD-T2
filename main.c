#include "conflito.h"
#include "visao.h"

void output(scheduleList *S,scheduleListV *SV){
    schedule *aux;
    scheduleV *auxV;
    int counter=0;
    aux=S->first;
    auxV=SV->first;
    while (aux){
        printf("%d ",aux->name);
        for (int i = 0; i < aux->totalT; i++){
            if(i==aux->totalT-1){
                printf("%d ",aux->graph[i].name);
            }else{
                printf("%d,",aux->graph[i].name);
            }
        }
        if(!detectCycle(aux)){
            printf("SS ");
        }else{
            printf("NS ");
        }
        counter=0;
        for (int i = 0; i < auxV->validPermuts; i++){
            if(auxV->Permuts[i].valid){
                counter++;
            }
        }
        if(counter>0){
            printf("SV\n");
        }else{
            printf("NV\n");
        }
        aux=aux->next;
        auxV=auxV->next;
    }
}
int main(int argc, char const *argv[]){
    scheduleList S;
    S.total=0;
    checkInput(&S);
    updateSchedule(&S);
    edges(&S);
    scheduleListV SV;
    SV.total=0;
    checkInputVision(&SV);
    updateScheduleVisao(&SV);
    buildPermuts(&SV);
    output(&S,&SV);
    return 0;
}
