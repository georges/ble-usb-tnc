/*
 * Project ble-uart
 * Description:
 * Author:
 * Date:
 */

#include "Particle.h"

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
SYSTEM_MODE(MANUAL);

const size_t UART_TX_BUF_SIZE = 20;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

// These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.

const BleUuid serviceUuid("00000001-ba2a-46c9-ae49-01b0961f68bb");

const BleUuid rxUuid("00000003-ba2a-46c9-ae49-01b0961f68bb");
const BleUuid txUuid("00000002-ba2a-46c9-ae49-01b0961f68bb");

BleCharacteristic txCharacteristic("tx",  BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

//BleCharacteristic txCharacteristic("tx",  BleCharacteristicProperty::WRITE, txUuid, serviceUuid);
//BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::NOTIFY, rxUuid, serviceUuid, onDataReceived, NULL);


void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // Log.trace("Received data from: %02X:%02X:%02X:%02X:%02X:%02X:", peer.address()[0], peer.address()[1], peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);

    for (size_t ii = 0; ii < len; ii++) {
        Serial.write(data[ii]);
    }
}

void onConnected(const BlePeerDevice& peer, void* context)
{
  Serial.println("BLE Connected");
}

void onDisconnected(const BlePeerDevice& peer, void* context)
{
  Serial.println("BLE Disconnected");
}

void setup() {
    Serial.begin();

    BLE.on();
    BLE.onConnected(onConnected, NULL);
    BLE.onDisconnected(onDisconnected, NULL);
    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

    BleAdvertisingData data;
    data.appendLocalName("TNC");
    data.appendServiceUUID(serviceUuid);
    BLE.advertise(&data);
}

void loop() {
  if (BLE.connected()) {
      uint8_t txBuf[UART_TX_BUF_SIZE];
      size_t txLen = 0;

      while(Serial.available() && txLen < UART_TX_BUF_SIZE) {
          txBuf[txLen++] = Serial.read();
          //Serial.write(txBuf[txLen - 1]);
      }
      if (txLen > 0) {
          txCharacteristic.setValue(txBuf, txLen);
      }
  }
}