#include <Arduino.h>
#include <ETH.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define BOT_TOKEN "8797749793:AAGKdNO0U7v6pLk-D5oEnW4OSMsS7NsNO6Q"
#define CHAT_ID "5971411410"

#define ETH_PHY_ADDR  1
#define ETH_PHY_TYPE  ETH_PHY_LAN8720
#define ETH_PHY_MDC   23
#define ETH_PHY_MDIO  18
#define ETH_PHY_POWER 16
#define ETH_CLK_MODE  ETH_CLOCK_GPIO0_IN

const int RELAY_PIN = 14; 
bool eth_connected = false;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

unsigned long lastTimeBotRan;
const unsigned long botDelay = 1000; 

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) continue; 

    String text = bot.messages[i].text;
    Serial.println("Pesan diterima: " + text);

    if (text == "/on") {
      digitalWrite(RELAY_PIN, LOW); 
      bot.sendMessage(chat_id, "✅ Relay sudah NYALA", "");
      Serial.println("Status: Relay ON");
    } 
    else if (text == "/off") {
      digitalWrite(RELAY_PIN, HIGH);
      bot.sendMessage(chat_id, "❌ Relay sudah MATI", "");
      Serial.println("Status: Relay OFF");
    } 
    else if (text == "/status") {
      String status = (digitalRead(RELAY_PIN) == LOW) ? "NYALA" : "MATI";
      bot.sendMessage(chat_id, "Status saat ini: " + status, "");
    }
    else {
      bot.sendMessage(chat_id, "Gunakan perintah /on, /off, atau /status", "");
    }
  }
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("Ethernet Dimulai...");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("Kabel Ethernet Terhubung!");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("IP didapatkan: ");
      Serial.println(ETH.localIP());
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("Ethernet Terputus!");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 

  WiFi.onEvent(WiFiEvent);
  
  // Memulai Ethernet
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  
  client.setInsecure(); 
  Serial.println("Sistem Menunggu Koneksi...");
}

void loop() {
  if (eth_connected) {
    if (millis() > lastTimeBotRan + botDelay) {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      
      while (numNewMessages) {
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }
  }
}