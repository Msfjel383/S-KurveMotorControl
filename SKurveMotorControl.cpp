#include "SKurveMotorControl.h"


SKurveMotorControl::SKurveMotorControl(float maxAcceleration, float maxDeceleration, float maxJerk) {
    _currentSpeed = 0;
    _targetSpeed = 0;
    _currentSpeed = 0;
    t_jerk = 0;
    t_max = 0;
    _maxAcceleration = maxAcceleration;
    _maxDeceleration = maxDeceleration;
    _maxJerk = maxJerk;
    _currentAcceleration = 0;
    _startTime = millis();
    _lastUpdateTime = _startTime;
}

void SKurveMotorControl::setSpeed(float targetSpeed) {
  float acc;
  float speedDifference = targetSpeed - _currentSpeed;
  if (speedDifference > 0) {
    acc = _maxAcceleration;
  } else if(speedDifference < 0) {
    acc = _maxDeceleration;
  } else {  //(=0)
    return;
  }
  _targetSpeed = targetSpeed;
  _startSpeed = _currentSpeed;
  _startTime = millis();
  _lastUpdateTime = _startTime;
  
  t_jerk = (acc/_maxJerk)*1000;
  float t_const = ((abs(_targetSpeed - _startSpeed))/acc)*1000;
  t_max = (t_jerk) + t_const;
  //Serial.println(_targetSpeed);
  //Serial.println(_startSpeed);
  //Serial.println(t_jerk);
  //Serial.println(t_const);
  //Serial.println(t_max);
  //Serial.println("Start");
}

void SKurveMotorControl::update() {
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - _lastUpdateTime;
    unsigned long deltaStart = millis() - _startTime;

    _lastUpdateTime = currentTime;

    float speedDifference = _targetSpeed - _currentSpeed;

    if (speedDifference > 0) {
        handleAcceleration(deltaStart, deltaTime, speedDifference);
    } else if(speedDifference < 0) {
        handleDeceleration(deltaStart, deltaTime, speedDifference);
    } else {  //(=0)
      return;
    }
}

void SKurveMotorControl::handleAcceleration(unsigned long deltaStart, unsigned long deltaTime, float speedDifference) {
//arbeitspunkt bestimmen und acc berechnen
  if(deltaStart>=(t_max-t_jerk)){  //abbremsen
    _currentAcceleration -= (deltaTime*_maxJerk)/1000;  
  }else if(deltaStart>=t_jerk){   //konstant
    _currentAcceleration = _maxAcceleration;
  }else{    //anfahren
    _currentAcceleration += (deltaTime*_maxJerk)/1000;
  }

//fehler abfangen

  if(_currentAcceleration > _maxAcceleration){
    _currentAcceleration = _maxAcceleration;
  }
  if(_currentAcceleration < 0){
    _currentAcceleration = 0;
  }
  
  
//geschwindigkeit berechnen
  _currentSpeed += (_currentAcceleration * deltaTime)/1000;
  if(_currentSpeed > _targetSpeed){
    _currentSpeed = _targetSpeed;
  }
  if(t_max<deltaStart){
    _currentSpeed = _targetSpeed;
  }
}


void SKurveMotorControl::handleDeceleration(unsigned long deltaStart, unsigned long deltaTime, float speedDifference) {
    //arbeitspunkt bestimmen und acc berechnen
  if(deltaStart>=(t_max-t_jerk)){  //abbremsen
    _currentAcceleration -= -(deltaTime*_maxJerk)/1000;  
  }else if(deltaStart>=t_jerk){   //konstant
    _currentAcceleration = -_maxDeceleration;
  }else{    //anfahren
    _currentAcceleration -= (deltaTime*_maxJerk)/1000;
  }

//fehler abfangen
  if(_currentAcceleration < -_maxDeceleration){
    _currentAcceleration = -_maxDeceleration;
  }
  if(_currentAcceleration > 0){
    _currentAcceleration = 0;
  }
//geschwindigkeit berechnen
  _currentSpeed += (_currentAcceleration * deltaTime)/1000;
  if(_currentSpeed < _targetSpeed){
    _currentSpeed = _targetSpeed;
  }
  if(t_max<deltaStart){
    _currentSpeed = _targetSpeed;
  }
}

void SKurveMotorControl::reset() {
    _currentSpeed = 0;
    _targetSpeed = 0;
    _currentAcceleration = 0;
    _lastUpdateTime = millis();
}

float SKurveMotorControl::getCurrentSpeed() {
    return _currentSpeed;
}
float SKurveMotorControl::getCurrentAcceleration() {
    return _currentAcceleration;
}