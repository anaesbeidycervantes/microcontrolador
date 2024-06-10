#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>

Adafruit_MPU6050 mpu;

const char* ssid = "user";         // Reemplaza con tu SSID
const char* password = "0312484177"; // Reemplaza con tu contraseña
const char* serverName = "http://192.168.137.173:3002/api/values";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Construir la URL con los parámetros de consulta
    String url = serverName;
    url += "?x=" + String(a.acceleration.x, 6); // 6 dígitos de precisión
    url += "&y=" + String(a.acceleration.y, 6);
    url += "&z=" + String(a.acceleration.z, 6);

    // Imprimir los valores que se van a enviar
    Serial.print("Sending values: ");
    Serial.print("X = ");
    Serial.print(a.acceleration.x, 6);
    Serial.print(", Y = ");
    Serial.print(a.acceleration.y, 6);
    Serial.print(", Z = ");
    Serial.println(a.acceleration.z, 6);

    HTTPClient http;
    http.begin(url);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(500); // Espera 500 ms antes de volver a leer los datos y enviarlos
}
