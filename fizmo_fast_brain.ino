//command line interpretter. this code reads serial command line and parses to and calls relevant tools.
#include<Wire.h>
#include <Adafruit_NeoPixel.h>
#include <commands.h>
#include <commands_impl.h>
#include <config.h>
#include <walk.h>
#include<commandRunner.h>
#include "ble_server.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  walk.init();
  bleServer.initialize();
  bleServer.start();
}

void loop() {
   
  // put your main code here, to run repeatedly:
  String raw_input=Serial.readStringUntil('\n');
  ParsedCommand requestedParsedCommand=parse_command(raw_input);
 
  CommandState state=commandRunner.run(requestedParsedCommand);
  save_Command_state(state);
  Serial.println(raw_input);
}
