#ifndef SCURVEMOTION_H
#define SCURVEMOTION_H

#include <Arduino.h>

class SCurveMotion {
  public:
    void init(float AccTime, float DecTime, float JerkTime, float maxSpeed);
    void start(float targetPos);
    float getAcc();
    float getJerk();
    float getVel();
    float getPos();
    void update();

  private:
    enum Phase { INACTIVE, ACCELERATION, CRUISE, DECELERATION };
    
    // Konfigurationsparameter
    float AccTime_, DecTime_, JerkTime_, maxSpeed_;
    float A_max_, A_dec_;
    
    // Zustandsvariablen
    Phase currentPhase_ = INACTIVE;
    unsigned long startTime_;
    float startPos_, targetPos_;
    float currentPos_, currentVel_, currentAcc_, currentJerk_;
    int direction_;
    
    void computeAcceleration(float t);
    void computeCruise(float t);
    void computeDeceleration(float t);
};

#endif