#pragma once
#include<config.h>
#include<commands.h>
#include<walk.h>
CommandState commandState;
ParsedCommand currentCommand;
RegisteredCommand currentRunningCommand;
DeviceStatus deviceStatus;
CommandStatus currentCommandStatus;
int match_command(String command){
  for ( int i =0;i<numCommands;i++){
    if (command==commandNames[i]){
      return i;
    }

  }
  return 9;
}

ParsedCommand parse_command(String userCommand){
  String token[7];
  String inputString=userCommand;
  ParsedCommand parsedCommand;
  int posDelim=0;
  for (int i =0;i<5;i++){
    posDelim=inputString.indexOf(",");
    token[i]=inputString.substring(0,posDelim);
    inputString=inputString.substring(posDelim+1);
    
  }
  token[5]=inputString;

  parsedCommand.commandName=(RegisteredCommand)match_command(token[0]);
  parsedCommand.param1=token[1];
  parsedCommand.param2=token[2];
  parsedCommand.param3=token[3];
  parsedCommand.param4=token[4];
  parsedCommand.param5=token[5];
  
  return parsedCommand;

}



void initialize_command_state( ){
  commandState.stateParam1=0;
  commandState.stateParam2=0;
  commandState.stateParam3=0;
  commandState.stateParam4=0;
  commandState.stateParam5=0;
  commandState.commandStatus=CommandStatus::READY;
  commandState.commandImplMillis=millis();
}

void save_Command_state( CommandState currentCommandState){
  commandState=currentCommandState;
}

CommandState read_command_state(){
  return commandState;
}
void command_cleanup(){
  deviceStatus=DeviceStatus::DEVICE_READY;
  currentRunningCommand=RegisteredCommand::NA;
  currentCommandStatus=CommandStatus::READY;
  initialize_command_state();  
}

void recover_to_normal_pose(){
  return;
}




