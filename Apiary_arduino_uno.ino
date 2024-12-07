#include <SPI.h>
#include <SD.h>
#include <DHT.h>

// Configuración de pines para los sensores
#define DHT22_PIN 7  // Pin del DHT22 (interno)
#define DHT11_PIN 2  // Pin del DHT11 (externo)
#define RAIN_SENSOR_PIN A0  // Pin analógico para el sensor de lluvia

// Configuración del módulo SD
#define SD_CS_PIN 10  // Pin CS para el módulo SD

// Inicialización de sensores DHT
DHT dht22(DHT22_PIN, DHT22);
DHT dht11(DHT11_PIN, DHT11);

// Variables para almacenar datos
float tempInterna, humInterna;
float tempExterna, humExterna;
int valorLluvia;
String estadoLluvia;

// Objeto para manejar archivos en la tarjeta SD
File archivo;

void setup() {
  Serial.begin(9600);
  
  // Inicializar sensores DHT
  dht22.begin();
  dht11.begin();
  
  // Configurar el módulo SD
  Serial.println("Inicializando tarjeta SD...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error al inicializar la tarjeta SD");
    while (true);  // Detener el programa si falla la inicialización
  }
  Serial.println("Tarjeta SD inicializada correctamente");

  // Crear o abrir el archivo para almacenar datos
  archivo = SD.open("datos.txt", FILE_WRITE);
  if (archivo) {
    archivo.println("Hora,Temperatura Interna,Humedad Interna,Temperatura Externa,Humedad Externa,Estado de Lluvia");
    archivo.close();
  } else {
    Serial.println("Error al crear o abrir el archivo");
  }
}

void loop() {
  // Leer datos de los sensores
  tempInterna = dht22.readTemperature();
  humInterna = dht22.readHumidity();
  tempExterna = dht11.readTemperature();
  humExterna = dht11.readHumidity();
  valorLluvia = analogRead(RAIN_SENSOR_PIN);

  // Determinar el estado de lluvia
  if (valorLluvia < 500) {
    estadoLluvia = "Lluvia detectada";
  } else {
    estadoLluvia = "Sin lluvia detectada";
  }

  // Imprimir datos en el monitor serie
  Serial.print("Temp Interna: "); Serial.print(tempInterna); Serial.println(" °C");
  Serial.print("Hum Interna: "); Serial.print(humInterna); Serial.println(" %");
  Serial.print("Temp Externa: "); Serial.print(tempExterna); Serial.println(" °C");
  Serial.print("Hum Externa: "); Serial.print(humExterna); Serial.println(" %");
  Serial.print("Estado de Lluvia: "); Serial.println(estadoLluvia);

  // Guardar datos en la tarjeta SD
  archivo = SD.open("datos.txt", FILE_WRITE);
  if (archivo) {
    archivo.print(millis() / 60000); archivo.print(" min,");
    archivo.print(tempInterna); archivo.print(",");
    archivo.print(humInterna); archivo.print(",");
    archivo.print(tempExterna); archivo.print(",");
    archivo.print(humExterna); archivo.print(",");
    archivo.println(estadoLluvia);
    archivo.close();
    Serial.println("Datos guardados en la tarjeta SD");
  } else {
    Serial.println("Error al escribir en la tarjeta SD");
  }

  // Intervalo de 2 minutos antes de la próxima medición
  delay(120000);
}