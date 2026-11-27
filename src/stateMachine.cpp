#include "stateMachine.h"

void stateMachine::updateState(StateCondition drivingState) {
    if(drivingState.systemError || drivingState.timedOut) {
        currentState = states::ERROR;
        return;
    }

    switch(currentState) {
        case states::IDLE:
            if(drivingState.systemReady && drivingState.startRequested) {
                currentState = states::TRACK_LANE;
            }
            break;
        case states::TRACK_LANE:
            if(drivingState.parkingDetected) {
                currentState = states::TRACK_PARKING;
            }
            break;
        case states::TRACK_PARKING:
            if(drivingState.turnPositionReached) {
                currentState = states::ROTATING;
            }
            break;
        case states::PARKING_GUIDE:
            if(drivingState.parkingComplete) {
                currentState = states::COMPLETE;
            }
            break;
        case states::ROTATING:
            if(drivingState.rotationComplete) {
                currentState = states::PARKING_GUIDE;
            }
            break;
        case states::COMPLETE:
            
            break;
        default:
            break;
    }
}

stateMachine::states stateMachine::getState() {
    return currentState;
}