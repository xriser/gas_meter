extern "C" {
#include "user_interface.h"
}

// Include the libraries we need
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DallasTemperature.h>

const char *INFLUXDB_HOST = "host";
const uint16_t INFLUXDB_PORT = 8086;

const char *DATABASE = "db";
const char *DB_USER = "";
const char *DB_PASSWORD = "";

#define ONE_WIRE_BUS 4 // Data wire is plugged into port 2 on the Arduino
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.
DeviceAddress insideThermometer =  {0x28, 0xFA, 0xAB, 0x8C, 0x06, 0x00, 0x00, 0xFA};

int Led = 15; // Definition LED Interface
int SENSOR = 2; // Define the Hall magnetic sensor interface
int val;// Defines a numeric variable val
int hall_state = 0;
int count = 0;

#define GLED      13
#define GLED_ON   digitalWrite(GLED, 1)
#define GLED_OFF   digitalWrite(GLED, 0)

const char* ssid = "ssid";
const char* password = "pass";
WiFiServer server(80);

void setup() {
  pinMode(GLED, OUTPUT);
  pinMode(Led, OUTPUT); // Definition LED as output interface
  pinMode(SENSOR, INPUT); // Defined the Hall magnetic sensor as output interface
  sensors.setResolution(insideThermometer, 11);

  // start serial port
  Serial.begin(115200);


  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop() {
  count++;

  if (count >= 2)
  {
    GLED_OFF;
  }

  sensors.requestTemperatures(); // Send the command to get temperatures
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.println("Temperature for the device 1 (index 0) is: ");
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temp C: ");
  Serial.println(tempC);


  val = digitalRead(SENSOR); // The digital interface is assigned a value of 3 to read val

  if ((val == LOW) and (hall_state == 0)) // When the Hall sensor detects a signal , flash led and send data
  {
    hall_state = 1;
    digitalWrite(Led, HIGH);
    Serial.println("Detect state change to LOW");

    //send data here

    HTTPClient http;    //Declare object of class HTTPClient

    http.begin("http://" + String(INFLUXDB_HOST) + ":" + String(INFLUXDB_PORT) + "/write?db=" + String(DATABASE));      //Specify request destination
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header

    int httpCode = http.POST("gas,host=home,region=ua value=1");   //Send the request
    String payload = http.getString();                  //Get the response payload

    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload

    http.end();  //Close connection

  }

  if ((val == HIGH) and (hall_state == 1)) //When Hall sensor release signal
  {
    digitalWrite(Led, LOW);
    hall_state = 0;
  }


  WiFiClient client = server.available();
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
  s += "temp: ";
  s += tempC;
  s += "C</html>\n";

  // Send the response to the client
  client.print(s);
  Serial.println();


  Serial.println(count);

  if (count > 10)
  {
    GLED_ON;
    Serial.println("GLED ON");
    count = 0;
  }

  delay(1000);
}

