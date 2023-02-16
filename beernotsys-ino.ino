#include <WiFi.h>
#include <HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "Galaxy M62fc96";
const char* password = "dfbw3605";
// const char* ssid = "MiauHouse -2.4G";
// const char* password = "19192222";

short int httpResponseCode;
String httpRequestCode, payload, url;

const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

float temperature;

void conectaWifi();
void mantemConexoes();
void temperatura();
void enviar();

void setup() {
  Serial.begin(115200);
  sensors.begin();

  conectaWifi();
}

void loop() {
  mantemConexoes();
  temperatura();
  enviar();

  delay(5000);
}

void enviar() {
  HTTPClient http;
  http.addHeader("Content-Type", "text/plain");
  url = "http://192.168.180.136:8000/Temperatura/Enviar?Temperatura=" + (String)temperature + "&Garrafa=Prototipo01";
  http.begin(url.c_str());

  httpResponseCode = http.POST("");
  delay(300);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void temperatura() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  Serial.print(temperature);
  Serial.println("ºC");
}

void conectaWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Conectando-se na rede ");
  Serial.print(ssid);
  Serial.println(" aguarde!");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(ssid);

  Serial.print(" IP obtido: ");
  Serial.println(WiFi.localIP());
}

void mantemConexoes() {
  conectaWifi();
}