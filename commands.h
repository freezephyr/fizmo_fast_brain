#pragma once
#include<config.h>
#include<Arduino.h>



struct ParsedCommand{
  RegisteredCommand commandName;
  String param1;
  String param2;
  String param3;
  String param4;
  String param5;
};
void execute_command(ParsedCommand parsedCommand);
struct CommandState{
  long stateParam1;
  long stateParam2;
  long stateParam3;
  long stateParam4;
  long stateParam5;
  CommandStatus commandStatus; 
  long commandImplMillis; 
};

int match_command(String command);
ParsedCommand parse_command(String userCommand);
void initialize_command_state( );
void save_Command_state(CommandState currentCommandState);
CommandState read_command_state();
void command_cleanup();
CommandState command_runner(ParsedCommand parsedCommand);
void recover_to_normal_pose();

