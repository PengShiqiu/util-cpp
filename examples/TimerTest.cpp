#include "util/timer/Timer.h"
#include <iostream>

using namespace ananas;

int main(void){

    TimerManager timer;

    timer.ScheduleAfter(DurationMs(500), []{
        std::cout << "timer 500ms." << std::endl;
    });
    timer.ScheduleAfterWithRepeat<kForever>(DurationMs(1000), []{
        std::cout << "timer 1000ms." << std::endl;
    });
    while(1){
        timer.Update();
    }
    return 0;
}