#ifndef SKURVEMOTORCONTROL_H
#define SKURVEMOTORCONTROL_H

#include <Arduino.h>
#include <math.h>

class SKurveMotorControl {
public:
    SKurveMotorControl(float maxAcceleration, float maxDeceleration, float maxJerk);
    void setSpeed(float targetSpeed);
    void update();
    void reset();
    float getCurrentSpeed();
    float getCurrentAcceleration();

private:
    void handleAcceleration(unsigned long deltaStart, unsigned long deltaTime, float speedDifference);
    void handleDeceleration(unsigned long deltaStart, unsigned long deltaTime, float speedDifference);

    float _currentSpeed;
    float _targetSpeed;
    float _startSpeed;
    float _maxAcceleration;
    float _maxDeceleration;
    float _maxJerk;
    float _currentAcceleration;
    unsigned long _lastUpdateTime;
    unsigned long _startTime;

    float t_jerk;
    float t_max;
};

#endif