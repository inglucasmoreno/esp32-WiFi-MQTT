/* 
  Conexion wifi con ESP32 + MQTT
  Autor: Moreno Lucas Omar
  Año: 2020
*/

#include <WiFi.h>
#include <PubSubClient.h>

// Credenciales WiFi
const char* ssid = "Moreno";
const char* password = "pringles772";

// Credenciales MQTT
const char* mqtt_server = "192.168.2.113";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

// Funcion: Conexion/Reconexion a MQTT
void reconnect_mqtt(){
  Serial.println("");
  Serial.println("Intentando conexion a MQTT");
  String clientId = "iot_1_";
  clientId = clientId + String(random(0xffff, HEX));
  if(client.connect(clientId.c_str())){
    Serial.println("Conexion Exitosa...");
  }else{
    Serial.println("Fallo de conexion");
    Serial.println(client.state());
    Serial.println("Se intentara de nuevo en 2 segundos");
    delay(2000);
  }
}

// Funcion: Conexion/Reconexion a red WiFi
void reconnect_wifi(){
  digitalWrite(BUILTIN_LED, LOW);
  Serial.println("Conexion WiFi");
  Serial.print("Conectando a -> ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(250);
  }
  Serial.println("");
  Serial.println("Conexion Exitosa...");

  Serial.println("Mi IP es -> ");
  Serial.print(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  client.setServer(mqtt_server, mqtt_port);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  // Led - Funcionamiento normal
  if(WiFi.status() != WL_CONNECTED || !client.connected()) digitalWrite(BUILTIN_LED, LOW);
  else digitalWrite(BUILTIN_LED, HIGH);
  
  // Reconexion WiFi
  if(WiFi.status() != WL_CONNECTED) reconnect_wifi();

  // Reconexion MQTT 
  if(!client.connected()) reconnect_mqtt();
  client.loop();
}
