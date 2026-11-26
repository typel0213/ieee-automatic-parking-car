#include "stateMachine.h"

void stateMachine::updateState(StateCondition drivingState) {
    if(drivingState.systemError || drivingState.timedOut) {
        currentState = state::ERROR;
        return;
    }

    switch(currentState) {
        case state::IDLE:
            if(drivingState.systemReady && drivingState.startRequested) {
                currentState = state::TRACK_LANE;
            }
            break;
        case state::TRACK_LANE:
            if(drivingState.parkingDetected) {
                currentState = state::TRACK_PARKING;
            }
            break;
        case state::TRACK_PARKING:
            if(drivingState.turnPositionReached) {
                currentState = state::ROTATING;
            }
            break;
        case state::PARKING_GUIDE:
            if(drivingState.parkingComplete) {
                currentState = state::COMPLETE;
            }
            break;
        case state::ROTATING:
            if(drivingState.rotationComplete) {
                currentState = state::PARKING_GUIDE;
            }
            break;
        case state::COMPLETE:
            
            break;
        default:
            break;
    }
}

stateMachine::state stateMachine::getState() {
    return currentState;
}