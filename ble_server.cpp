/*Bluetooth server uses NimBLE by h2zero for initiating server.*/
#include "ble_server.h"
#include <Arduino.h>
#include "config.h"
#include <NimBLEDevice.h>

BleServer bleServer;

static NimBLEServer* server = nullptr;

/* Runs in NimBLE's own task, not in loop(). Printing only: nothing here may
   touch motion state. */
class BleServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
      Serial.print("BLE connected: ");
      Serial.println(connInfo.getAddress().toString().c_str());
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
      Serial.print("BLE disconnected, reason ");
      Serial.println(reason);
    }
};

static BleServerCallbacks bleServerCallbacks;

void BleServer::initialize(){
  NimBLEDevice::init(BLE_SERVER_NAME);
}

void BleServer::start(){
  server = NimBLEDevice::createServer();
  server->setCallbacks(&bleServerCallbacks);

  NimBLEAdvertising* bleAdvertising = NimBLEDevice::getAdvertising();
  bleAdvertising->setName(BLE_SERVER_NAME);
  if (!bleAdvertising->start()){
    Serial.println("BLE advertising failed to start");
  }
}
