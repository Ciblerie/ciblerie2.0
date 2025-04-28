
/*
esp32 - WebScocket , code ok le 28.04.25 à 13H30
*/

#include <WiFi.h>
#include <Arduino.h>
#include <WebSocketsServer.h>

// Configuration WiFi
const char* ssid = "Livebox-9410";
const char* password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Configuration du port série pour la communication avec l'Arduino Mega
HardwareSerial SerialMega(1);

void setup() {
  // Initialisation des ports série
  Serial.begin(115200);                       // Logs pour le PC
  SerialMega.begin(9600, SERIAL_8N1, 16, 17); // RX/TX pour l'Arduino Mega

  // Connexion au réseau WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Démarrage du serveur WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  // Boucle WebSocket
  webSocket.loop();

  // Lecture des données de l'Arduino Mega
  if (SerialMega.available()) {
    String messageRecu = SerialMega.readStringUntil('\n'); // Lecture
    Serial.println("Données reçues : " + messageRecu);

    // Transmission des données via WebSocket
    webSocket.broadcastTXT(messageRecu);
  }
}

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("Message reçu via WebSocket : %s\n", payload);
  }
}