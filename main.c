#include "conflito.h"
int main(int argc, char const *argv[]){
    char line[1024];
    scheduleList S;
    S.total=0;
    checkInput(&S);
    updateSchedule(&S);
    edges(&S);
    output(&S);

    return 0;
}
