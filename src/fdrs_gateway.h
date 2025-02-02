//  FARM DATA RELAY SYSTEM
//
//  GATEWAY Main Functions
//  Developed by Timm Bogner (timmbogner@gmail.com)

#include "fdrs_datatypes.h"
#include "fdrs_globals.h"
#define FDRS_GATEWAY

#ifndef ESPNOWG_ACT
#define ESPNOWG_ACT
#endif
#ifndef ESPNOW1_ACT
#define ESPNOW1_ACT
#endif
#ifndef ESPNOW2_ACT
#define ESPNOW2_ACT
#endif
#ifndef SERIAL_ACT
#define SERIAL_ACT
#endif
#ifndef MQTT_ACT
#define MQTT_ACT
#endif
#ifndef LORAG_ACT
#define LORAG_ACT
#endif
#ifndef LORA1_ACT
#define LORA1_ACT
#endif
#ifndef LORA2_ACT
#define LORA2_ACT
#endif
#ifndef INTERNAL_ACT
#define INTERNAL_ACT
#endif
#ifdef USE_ETHERNET
#ifndef USE_WIFI
#define USE_WIFI
#endif
#endif // USE_ETHERNET

SystemPacket theCmd;
DataReading theData[256];
uint8_t ln;
uint8_t newData = event_clear;
uint8_t newCmd = cmd_clear;

DataReading fdrsData[256]; // buffer for loadFDRS()
uint8_t data_count = 0;

// Function Prototypes needed due to #ifdefs being moved outside of function definitions in header files 
void broadcastLoRa();
void sendLoRaNbr(uint8_t);
void timeFDRSLoRa(uint8_t *);
static uint16_t crc16_update(uint16_t, uint8_t);
void sendESPNowNbr(uint8_t);
void sendESPNowPeers();
void sendESPNow(uint8_t);

void sendMQTT();
void sendLog();
void resendLog();
void releaseLogBuffer();

#ifdef USE_OLED
  #include "fdrs_oled.h"
#endif
#include "fdrs_debug.h"
#include "fdrs_gateway_serial.h"
#include "fdrs_gateway_scheduler.h"
#ifdef USE_ESPNOW
  #include "fdrs_gateway_espnow.h"
#endif
#ifdef USE_LORA
  #include "fdrs_gateway_lora.h"
#endif
#ifdef USE_WIFI
  #include "fdrs_gateway_wifi.h"
  #include "fdrs_gateway_mqtt.h"
  #include "fdrs_gateway_ota.h"
#endif
#if defined(USE_FS_LOG) || defined(USE_SD_LOG)
  #include "fdrs_gateway_filesystem.h"
#endif
#ifdef DEBUG_CONFIG
  #include "fdrs_checkConfig.h"
#endif

void sendFDRS()
{
  for (int i = 0; i < data_count; i++)
  {
    theData[i].id = fdrsData[i].id;
    theData[i].t = fdrsData[i].t;
    theData[i].d = fdrsData[i].d;
  }
  ln = data_count;
  data_count = 0;
  newData = event_internal;
  DBG("Entered internal data.");
}

void loadFDRS(float d, uint8_t t, uint16_t id)
{
  DBG("Id: " + String(id) + " - Type: " + String(t) + " - Data loaded: " + String(d));
  DataReading dr;
  dr.id = id;
  dr.t = t;
  dr.d = d;
  fdrsData[data_count] = dr;
  data_count++;
}

void beginFDRS()
{
#if defined(ESP8266)
  Serial.begin(115200);
#elif defined(ESP32)
  Serial.begin(115200);
  UART_IF.begin(115200, SERIAL_8N1, RXD2, TXD2);
#endif
#ifdef USE_OLED
  init_oled();
  DBG("Display initialized!");
  DBG("Hello, World!");
#endif
  DBG("Address:" + String(UNIT_MAC, HEX));
#ifdef USE_LORA
  begin_lora();
  scheduleFDRS(asyncReleaseLoRaFirst, FDRS_LORA_INTERVAL);
#endif
#ifdef USE_WIFI
  begin_wifi();
  DBG("Connected.");
  begin_mqtt();
  begin_OTA();
#endif
#ifdef USE_ESPNOW
  begin_espnow();
#endif
#ifdef USE_SD_LOG
  begin_SD();
#endif
#ifdef USE_FS_LOG
  begin_FS();
#endif

#ifdef USE_WIFI
  client.publish(TOPIC_STATUS, "FDRS initialized");
#endif
}

void handleCommands()
{
  switch (theCmd.cmd)
  {
  case cmd_ping:
#ifdef USE_ESPNOW
    pingback_espnow();
#endif // USE_ESPNOW

    break;

  case cmd_add:
#ifdef USE_ESPNOW
    add_espnow_peer();
#endif // USE_ESPNOW

    break;
  }
  theCmd.cmd = cmd_clear;
  theCmd.param = 0;
}

void loopFDRS()
{
  handle_schedule();
  handleCommands();
#if defined(USE_SD_LOG) || defined(USE_FS_LOG)
  handleLogger();
#endif
  handleSerial();
#ifdef USE_LORA
  handleLoRa();
#endif
#ifdef USE_WIFI
  handleMQTT();
  handleOTA();
#endif
#ifdef USE_OLED
  drawPageOLED(true);
#endif
  if (newData != event_clear)
  {
    switch (newData)
    {
    case event_espnowg:
      ESPNOWG_ACT
      break;
    case event_espnow1:
      ESPNOW1_ACT
      break;
    case event_espnow2:
      ESPNOW2_ACT
      break;
    case event_serial:
      SERIAL_ACT
      break;
    case event_mqtt:
      MQTT_ACT
      break;
    case event_lorag:
      LORAG_ACT
      break;
    case event_lora1:
      LORA1_ACT
      break;
    case event_lora2:
      LORA2_ACT
      break;
    case event_internal:
      INTERNAL_ACT
      break;
    }
    newData = event_clear;
  }
}

// "Skeleton Functions related to FDRS Actions"
#ifndef USE_LORA
  void broadcastLoRa() {}
  void sendLoRaNbr(uint8_t address) {}
  void timeFDRSLoRa(uint8_t *address) {}
#endif
#ifndef USE_ESPNOW
  void sendESPNowNbr(uint8_t interface) {}
  void sendESPNowPeers() {}
  void sendESPNow(uint8_t address) {}
#endif
#ifndef USE_WIFI
  void sendMQTT() {}
#endif