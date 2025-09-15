#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
 
int pot = 32;
int ledBatimento = 2;
int pote;
int tempoBatimento;
int velJogador = 33;
int velocidade;
int vel;
int bpm;
 
Adafruit_MPU6050 mpu;
 
 
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* apiKey = "KE2RFCNF16GIPBGZ";
const char* server = "http://api.thingspeak.com";
 
void setup() {
 
  pinMode(ledBatimento, OUTPUT);
 
  Serial.begin(115200);
  while (!Serial)
    delay(10);
 
  if (!mpu.begin()) {
    Serial.println("Falha ao encontrar o chip MPU6050");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Encontrado!");
 
  Serial.println("");
  delay(100);
 
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}
 
 
void loop() {
 
  vel = analogRead(velJogador);
 
  velocidade = map(vel, 0, 4095, 1, 60);
  Serial.print("Velocidade do jogador: ");
  Serial.print(velocidade);
  Serial.println(" Km/h");
 
  pote = analogRead(pot);
 
  tempoBatimento = map(pote, 0, 4095, 400, 1200);
  bpm = tempoBatimento > 0 ? 60000 / tempoBatimento : 0;
 
  digitalWrite(ledBatimento, HIGH);
  delay(100);
  digitalWrite(ledBatimento, LOW);
 
 
  delay(tempoBatimento);
 
  Serial.print("Batimentos: ");
  Serial.print(bpm);
  Serial.println(" bpm");
 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
  Serial.print("Aceleração X: ");
  Serial.println(a.acceleration.x);
 
  Serial.print("Temperatura: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");
 
  Serial.println("");
  delay(500);
 
 
 HTTPClient http;
    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(velocidade) + "&field2=" + String(bpm) + "&field3=" + String(a.acceleration.x) + "&field4=" + String(temp.temperature);
    http.begin(url);
 
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Dados enviados ao ThingSpeak.");
      Serial.print("Código HTTP: ");
      Serial.println(httpCode);
      Serial.println("Resposta: ");
      Serial.println(payload);
    } else {
      Serial.print("Erro ao enviar dados. Código HTTP: ");
      Serial.println(httpCode);
    }
   
    http.end();
}
