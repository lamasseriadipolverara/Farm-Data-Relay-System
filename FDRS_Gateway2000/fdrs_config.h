//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000 Configuration
//  This is still in progress. Stay tuned!
#define RXD2 21
#define TXD2 22
#define UNIT_MAC 0x01// THIS UNIT
#define PREV_MAC 0x00// ESPNOW1 Address 
#define NEXT_MAC 0x03// ESPNOW2 Address

//#define USE_WIFI    //You cannot use ESP-NOW while WiFi is in use
#define WIFI_NET "Your SSID"
#define WIFI_PASS "Password"
#define MQTT_ADDR "192.168.0.8"

//#define USE_LORA      
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6


//Actions -- Define what happens when a packet arrives at each interface:
//Current function options are: sendESPNOW(interface),  sendSerial(), sendMQTT(), and sendLoRa().

#define ESPNOW1_DELAY  0
#define ESPNOW2_DELAY  0
#define ESPNOWG_DELAY  0
#define SERIAL_DELAY   0
#define MQTT_DELAY     0
#define LORA_DELAY     0

//Use these settings for a gateway that recieves ESP-NOW data and sends serial (UART).
#define ESPNOW1_ACT    sendSerial();
#define ESPNOW2_ACT    sendSerial();
#define ESPNOWG_ACT    sendSerial();
#define SERIAL_ACT     sendESPNOW(0);
#define MQTT_ACT       sendSerial();
#define LORA_ACT       sendSerial();

//Use these settings for a gateway that recieves serial (UART) data and sends MQTT.
//#define USE_WIFI
//#define ESPNOW1_ACT  
//#define ESPNOW2_ACT  
//#define ESPNOWG_ACT  
//#define SERIAL_ACT   sendMQTT();
//#define MQTT_ACT     sendSerial();
//#define LORA_ACT     sendSerial();
