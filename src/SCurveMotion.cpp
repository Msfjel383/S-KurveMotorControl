#include "SCurveMotion.h"

void SCurveMotion::init(float AccTime, float DecTime, float JerkTime, float maxSpeed) {

  //if((AccTime - 2 * JerkTime) < 0 &&)
  AccTime_ = AccTime;
  DecTime_ = DecTime;
  JerkTime_ = JerkTime;
  maxSpeed_ = maxSpeed;
  
  // Berechnung von Maximalbeschleunigung/-verzögerung
  A_max_ = maxSpeed_ / (AccTime_ - JerkTime_);
  A_dec_ = -maxSpeed_ / (DecTime_ - JerkTime_);
}

void SCurveMotion::start(float targetPos) {
  startPos_ = currentPos_;
  targetPos_ = targetPos;
  direction_ = (targetPos_ > startPos_) ? 1 : -1;
  startTime_ = millis();
  currentPhase_ = ACCELERATION;
}

void SCurveMotion::update() {
  if (currentPhase_ == INACTIVE) return;
  
  float elapsed = (millis() - startTime_) / 1000.0f;
  
  switch (currentPhase_) {
    case ACCELERATION:
      if (elapsed < AccTime_) {
        computeAcceleration(elapsed);
      } else {
        currentPhase_ = CRUISE;
        startTime_ = millis();
      }
      break;
      
    case CRUISE: {
      float cruiseTime = (abs(targetPos_ - startPos_) - 0.5 * maxSpeed_ * (AccTime_ + DecTime_)) / maxSpeed_;
      if (elapsed < cruiseTime) {
        computeCruise(elapsed);
      } else {
        currentPhase_ = DECELERATION;
        startTime_ = millis();
      }
      break;
    }
      
    case DECELERATION:
      if (elapsed < DecTime_) {
        computeDeceleration(elapsed);
      } else {
        currentPhase_ = INACTIVE;
        currentPos_ = targetPos_;
        currentVel_ = 0;
        currentAcc_ = 0;
        currentJerk_ = 0;
      }
      break;
  }
}

void SCurveMotion::computeAcceleration(float t) {
  if (t < JerkTime_) {    //tj
    currentJerk_ = A_max_ / JerkTime_;
    currentAcc_ = currentJerk_ * t;
  } else if (t < (AccTime_ - JerkTime_)) {
    currentJerk_ = 0;
    currentAcc_ = A_max_;
  } else {
    currentJerk_ = -A_max_ / JerkTime_;
    currentAcc_ = A_max_ + currentJerk_ * (t - (AccTime_ - JerkTime_));
  }
  
  // Geschwindigkeitsberechnung
  if (t < JerkTime_) {    //tj
    currentVel_ = 0.5 * currentJerk_ * t * t;
  } else if (t < (AccTime_ - JerkTime_)) {  //tj2
    currentVel_ = 0.5 * A_max_ * JerkTime_ + A_max_ * (t - JerkTime_);
  } else {
    float tDec = JerkTime_ - (t - (AccTime_ - JerkTime_));
    currentVel_ = maxSpeed_ - (0.5 * (-currentJerk_) * tDec * tDec);
  }
  
  // Positionsberechnung
  if (t < JerkTime_) {
    currentPos_ = (currentJerk_ * t * t * t) / 6.0f;
  } else if (t < (AccTime_ - JerkTime_)) {
    float phase1 = (A_max_ * JerkTime_ * JerkTime_) / 6.0f;
    currentPos_ = phase1 + 0.5 * A_max_ * JerkTime_ * (t - JerkTime_) + 
                 0.5 * A_max_ * (t - JerkTime_) * (t - JerkTime_);
  } else {
    float phase1 = (A_max_ * JerkTime_ * JerkTime_) / 6.0f;
    float phase2 = 0.5 * A_max_ * JerkTime_ * (AccTime_ - 2 * JerkTime_) + 
                  0.5 * A_max_ * (AccTime_ - 2 * JerkTime_) * (AccTime_ - 2 * JerkTime_);
    float tDec = t - (AccTime_ - JerkTime_);
    //currentPos_ = phase1 + phase2 + maxSpeed_ * tDec - 
    //              (A_max_ * tDec * tDec * tDec) / (6 * JerkTime_);
    currentPos_ = phase1 + phase2 + 
             maxSpeed_ * tDec - 
             (A_max_ / (6 * JerkTime_)) * tDec * tDec * tDec;
  }
  
  currentVel_ *= direction_;
  currentPos_ = startPos_ + direction_ * currentPos_;
}

void SCurveMotion::computeCruise(float t) {
  currentJerk_ = 0;
  currentAcc_ = 0;
  currentVel_ = maxSpeed_ * direction_;
  currentPos_ = startPos_ + direction_ * (0.5 * maxSpeed_ * AccTime_ + maxSpeed_ * t);
}

void SCurveMotion::computeDeceleration(float t) {
  if (t < JerkTime_) {
    currentJerk_ = A_dec_ / JerkTime_;
    currentAcc_ = currentJerk_ * t;
  } else if (t < (DecTime_ - JerkTime_)) {
    currentJerk_ = 0;
    currentAcc_ = A_dec_;
  } else {
    currentJerk_ = -A_dec_ / JerkTime_;
    currentAcc_ = A_dec_ + currentJerk_ * (t - (DecTime_ - JerkTime_));
  }
  
  // Geschwindigkeitsberechnung
  float absSpeed;
  if (t < JerkTime_) {
    absSpeed = maxSpeed_ + 0.5 * currentJerk_ * t * t;
  } else if (t < (DecTime_ - JerkTime_)) {
    absSpeed = maxSpeed_ + A_dec_ * (t - JerkTime_) + 0.5 * A_dec_ * JerkTime_;
  } else {
    float tAcc = JerkTime_ - (t - (DecTime_ - JerkTime_));
    absSpeed = 0.5 * (currentJerk_) * tAcc * tAcc;
  }
  
  // Positionsberechnung
  float cruiseDist = 0.5 * maxSpeed_ * AccTime_ + 
                    maxSpeed_ * ((abs(targetPos_ - startPos_) - 
                    0.5 * maxSpeed_ * (AccTime_ + DecTime_)) / maxSpeed_);
  float decPos;
  
  if (t < JerkTime_) {
    decPos = maxSpeed_ * t + 0.5 * currentJerk_ * t * t * t / 3.0f;
  } else if (t < (DecTime_ - JerkTime_)) {
    float phase1 = maxSpeed_ * JerkTime_ + (A_dec_ * JerkTime_ * JerkTime_) / 6.0f;
    decPos = phase1 + maxSpeed_ * (t - JerkTime_) + 
            0.5 * A_dec_ * (t - JerkTime_) * (t - JerkTime_);
  } else {
    float phase1 = maxSpeed_ * JerkTime_ + (A_dec_ * JerkTime_ * JerkTime_) / 6.0f;
    float phase2 = phase1 + maxSpeed_ * (DecTime_ - 2 * JerkTime_) + 
                  0.5 * A_dec_ * (DecTime_ - 2 * JerkTime_) * (DecTime_ - 2 * JerkTime_);
    float tAcc = t - (DecTime_ - JerkTime_);
    decPos = phase2 + 0.5 * (-currentJerk_) * tAcc * tAcc * tAcc / 3.0f;
  }
  
  currentVel_ = absSpeed * direction_;
  currentPos_ = startPos_ + direction_ * (cruiseDist + decPos);
}

float SCurveMotion::getAcc() { return currentAcc_ * direction_; }
float SCurveMotion::getJerk() { return currentJerk_ * direction_; }
float SCurveMotion::getVel() { return currentVel_; }
float SCurveMotion::getPos() { return currentPos_; }