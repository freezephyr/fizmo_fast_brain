#pragma once
#include<commands.h>
#include<Arduino.h>
#include<config.h>
#include<Wire.h>
#include <walk.h>
#include <Adafruit_PWMServoDriver.h>

Walk walk;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
void Walk::init(){};

int  Walk::tick_from_lvl1_normalized(float norm, uint8_t jointSweepMin, uint8_t jointSweepMax){
   int tick=(jointSweepMax-jointSweepMin)*norm+jointSweepMin;
  return (int)tick;

}

void  Walk::apply_pose(const float pose[NUM_JOINTS]) {
    for (uint8_t j = 0; j < NUM_JOINTS; j++) {
    float norm = (pose[j] + 1.0f) * 0.5f;                 // signed [-1,1] -> [0,1]
    int tick = tick_from_lvl1_normalized(norm, JOINT_MIN[j], JOINT_MAX[j]);
    pwm.setPWM(j, 0, tick);
      
   }
  }
CommandState Walk::start(ParsedCommand currentParsedCommand, CommandState previousState){
    uint8_t walkSteps=(uint8_t)currentParsedCommand.param1.toInt();
    uint8_t walkStep=previousState.stateParam1;
    uint8_t gaitFrame=previousState.stateParam2;
    long gaitFrameAppliedMillis=previousState.commandImplMillis;
    CommandStatus currentCommandStatus = CommandStatus::INPROGRESS;
    long currentMillis=millis();
    
    if (walkStep==0){
      walkStep=1;
    }
    if (walkStep<=walkSteps){
      if (gaitFrame<GAIT_FRAMES){
        if (currentMillis-gaitFrameAppliedMillis>=FRAME_MS){
          apply_pose(GAIT[gaitFrame]);
          /*ReturnCode poseReturnCode= apply_pose(GAIT[gaitFrame]);
          if (poseReturnCode==Returncode::ERROR){
             currentCommandState.commandStatus=CommandStatus::FAILED;
          }*/
          gaitFrame++;
          gaitFrameAppliedMillis=millis();
          currentCommandStatus=CommandStatus::INPROGRESS;
        }

      }
      else{
        walkStep++;
        gaitFrame=0;
        currentCommandStatus=CommandStatus::INPROGRESS;

      }
    }
    else{
      command_cleanup();
      currentCommandStatus=CommandStatus::COMPLETED;
    }
    CommandState currentCommandState;
    currentCommandState.stateParam1=walkStep;
    currentCommandState.stateParam2=gaitFrame;
    currentCommandState.stateParam3=0;
    currentCommandState.stateParam4=0;
    currentCommandState.stateParam5=0;
    currentCommandState.commandStatus=currentCommandStatus;
    currentCommandState.commandImplMillis=gaitFrameAppliedMillis;
    return currentCommandState;
       
    
  
}


class VelocityVector{
  public:
    float forward=1;
    float lateral=0;
    VelocityVector(float x, float y){

        this->forward=x;
        this->lateral=y;
      }
    String validateVelocityVector(float forward, float lateral){
      if ((forward<=1) && (forward>=-1) && (lateral>=-1) && (lateral<=1)){
      return "VALID";
    }
    else{
      return "OUT_OF_BOUND_VALUES";
    }
    
     
    }
  
};

