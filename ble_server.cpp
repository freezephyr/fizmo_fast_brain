/*Bluetooth server uses NimBLE by h2zero for initiating server.*/
#include "ble_server.h"
#include <Arduino.h>
#include "config.h"
#include <NimBLEDevice.h>

BleServer bleServer;

static NimBLEServer* server = nullptr;
static NimBLECharacteristic* statusCharacteristic = nullptr;

/* Runs in NimBLE's own task, not in loop(). Printing only: nothing here may
   touch motion state. */
class BleServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
      Serial.print("BLE connected: ");
      Serial.print(connInfo.getAddress().toString().c_str());
      /* conn_itvl counts 1.25ms units. This is the latency quantum in both
         directions and has never been measured against the frame period. */
      Serial.print(", connection interval ");
      Serial.print(connInfo.getConnInterval() * 1.25f);
      Serial.println(" ms");
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
      Serial.print("BLE disconnected, reason ");
      Serial.println(reason);
    }

    void onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) override {
      Serial.print("BLE MTU negotiated: ");
      Serial.print(MTU);
      Serial.print(" bytes, ");
      Serial.print(MTU - 3);
      Serial.println(" usable");
    }
};

static BleServerCallbacks bleServerCallbacks;

class CommandCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& connInfo) override {
      NimBLEAttValue received = characteristic->getValue();
      Serial.print("BLE command: ");
      Serial.println(received.c_str());

      /* Temporary: proves the upward path end to end while nothing produces
         real state yet. Replaced when the runner is wired in. */
      if (statusCharacteristic != nullptr){
        statusCharacteristic->setValue(received.c_str());
        statusCharacteristic->notify();
      }
    }
};

static CommandCallbacks commandCallbacks;

void BleServer::initialize(){
  NimBLEDevice::init(BLE_SERVER_NAME);
}

void BleServer::start(){
  server = NimBLEDevice::createServer();
  server->setCallbacks(&bleServerCallbacks);
  /* NimBLE 2.5.1 initialises this to false, which leaves the board silent once
     a client disconnects: one connection per power cycle. */
  server->advertiseOnDisconnect(true);

  NimBLEService* commandService = server->createService(BLE_GATT_SERVICE_UUID);

  NimBLECharacteristic* commandCharacteristic =
      commandService->createCharacteristic(BLE_GATT_CHARACTERISTIC1_UUID, NIMBLE_PROPERTY::WRITE);
  commandCharacteristic->setCallbacks(&commandCallbacks);

  statusCharacteristic =
      commandService->createCharacteristic(BLE_GATT_CHARACTERISTIC2_UUID,
                                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  statusCharacteristic->setValue("0,IDLE");

  commandService->start();

  NimBLEAdvertising* bleAdvertising = NimBLEDevice::getAdvertising();
  bleAdvertising->setName(BLE_SERVER_NAME);
  if (!bleAdvertising->start()){
    Serial.println("BLE advertising failed to start");
  }
}
