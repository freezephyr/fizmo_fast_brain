#pragma once

class AbstractCommand{
  public:
  virtual CommandState start(ParsedCommand currentParsedCommand, CommandState previousState);

};




