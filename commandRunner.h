#pragma once
#include<config.h>
#include<commands.h>

class CommandRunner{
  public:
    CommandRunner(){} void init();
    CommandStatus check_runner_status();
    CommandState run(ParsedCommand currentParsedCommand);
/*   CommandState pause();
    CommandState stop();
*/
};
extern CommandRunner commandRunner;