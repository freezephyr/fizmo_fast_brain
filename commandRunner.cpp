#pragma once
#include<config.h>
#include<commands.h>
#include <commandRunner.h>
#include<walk.h>
CommandRunner commandRunner;

CommandStatus CommandRunner::check_runner_status(){
  CommandStatus runningCommandStatus=read_command_state().commandStatus;
  return runningCommandStatus;

}


CommandState CommandRunner::run(ParsedCommand currentParsedCommand){
  CommandState prevState=read_command_state();
  CommandState  newState;
  CommandStatus runningCommandStatus=prevState.commandStatus;
  
  if (((runningCommandStatus==CommandStatus::INPROGRESS) || (runningCommandStatus==CommandStatus::PAUSED))
  &&((currentParsedCommand.commandName !=RegisteredCommand::STOP)&&(currentParsedCommand.commandName !=RegisteredCommand::PAUSE))){
    /*ignore if the previous command is already runnning and new command is neither stop nor paused.*/
    return prevState;
  }
  if (currentParsedCommand.commandName ==RegisteredCommand::STOP){
    /*stop resets the state.*/
    command_cleanup();
    recover_to_normal_pose();
    return read_command_state();
  }

  if (currentParsedCommand.commandName ==RegisteredCommand::PAUSE){
    prevState.commandStatus=CommandStatus::PAUSED;
    prevState.commandImplMillis=millis();
    return prevState;
  }
  if ((runningCommandStatus==CommandStatus::PAUSED) &&(currentParsedCommand.commandName ==RegisteredCommand::RESUME)){
    prevState.commandStatus=CommandStatus::INPROGRESS;
    prevState.commandImplMillis=millis();
  }
  switch(currentParsedCommand.commandName){
    case RegisteredCommand::WALK:
        newState=walk.start(currentParsedCommand,prevState);
        break;
      }
  return newState;
}
/* I am not sure if this is useful but keeping it if we need to make it work.
CommandState CommandRunner:stop(){
  CommandState state;
  return state
}

CommandState CommandRunner:pause(){
  CommandState state;
  return state
}
*/