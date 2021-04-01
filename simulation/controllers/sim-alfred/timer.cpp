#include "controllers/sim-alfred/timer.h"


CTimer::CTimer() {
    landingTimer = 0;
    avoidTimer = 0;
}

void CTimer::SetTimer(TimerType timerType, int value) {
    switch (timerType) {
    case TimerType::kLandingTimer:
        landingTimer = value;
        break;
    case TimerType::kAvoidTimer:
        avoidTimer = value;
        break;
    default:
        break;
    }
}

int CTimer::GetTimer(TimerType timerType) {
    switch (timerType) {
    case TimerType::kLandingTimer:
        return landingTimer;
        break;
    case TimerType::kAvoidTimer:
        return avoidTimer;
        break;
    default:
        break;
    }
}

void CTimer::CountOneCycle() {
    landingTimer--;
    avoidTimer--;
}