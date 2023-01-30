#include "conflito.h"
#include "visao.h"
int main(int argc, char const *argv[]){
    // char line[1024];
    // scheduleList S;
    // S.total=0;
    // checkInput(&S);
    // updateSchedule(&S);
    // edges(&S);
    // output(&S);
    char line[1024];
    scheduleListV S;
    S.total=0;
    checkInputVision(&S);
    updateScheduleVisao(&S);
    buildPermuts(&S);
    // imprime(S.first);
    return 0;
}
