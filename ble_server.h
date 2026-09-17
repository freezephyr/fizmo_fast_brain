#pragma once

/* NimBLE is deliberately absent from this header: nothing that includes it
   should depend on the radio library. All NimBLE types live in ble_server.cpp.
   Named BleServer, not BLEServer, because NimBLEDevice.h defines BLEServer as
   an alias for NimBLEServer. */
class BleServer {
  public:
    BleServer(){}
    void initialize();
    void start();
};

extern BleServer bleServer;
