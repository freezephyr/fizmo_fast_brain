#pragma once
#include<commands.h>
#include<Arduino.h>
#include<config.h>
#include<abstractCommand.h>


/*changed to enable loop to check all incoming commands and not hold this.*/
class Walk : public AbstractCommand {

  private:
    void  apply_pose(const float pose[NUM_JOINTS]);
    int tick_from_lvl1_normalized(float norm, uint8_t jointSweepMin, uint8_t jointSweepMax);
  public:
  Walk(){} void init();
  CommandState start(ParsedCommand currentParsedCommand, CommandState previousState);
};

extern Walk walk;
