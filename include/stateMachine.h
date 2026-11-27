#ifndef STATEMACHINE_H
#define STATEMACHINE_H

struct StateCondition {
    bool systemReady = false;
    bool startRequested = false;
    bool parkingDetected = false;
    bool turnPositionReached = false;
    bool rotationComplete = false;
    bool parkingComplete = false;
    bool systemError = false;
    bool timedOut = false;
};

class stateMachine {
    public:
        enum class states {
            IDLE,
            ROTATING,
            TRACK_LANE,
            TRACK_PARKING,
            PARKING_GUIDE,
            COMPLETE,
            ERROR
        };
        void updateState(StateCondition drivingState);
        state getState();
    private:
        state currentState = IDLE;
}
#endif