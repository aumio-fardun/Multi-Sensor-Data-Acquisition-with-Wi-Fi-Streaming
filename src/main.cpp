#define IR_PIN 35
#define DHT_PIN 14
#define V_PIN 34
#define LED_PIN 23
#define BUZ_PIN 19
#define I_PIN 34
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);
Adafruit_MPU6050 mpu;

// Replace these with your network credentials
const String ssid = "SECL RnD LAB";
const String password = "SECL@2024";

// // Server IP and port
// const char* host = "192.168.1.100";  // Replace with the server's IP address
// const uint16_t port = 8080;          // Replace with the server's port number

WiFiServer server(80);
WiFiClient client;

void setup()
{

  //WiFiClient client = server.accept();
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  dht.begin();
  // Initialize I2C communication
  Wire.begin();

  // Initialize MPU6050
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
      ;
  }

  // Set MPU6050 to its default settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 initialized successfully");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZ_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(DHT_PIN, INPUT);
  pinMode(V_PIN, INPUT);
  pinMode(I_PIN, INPUT);
  digitalWrite(BUZ_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // // Connect to server
  // if (!client.connect(host, port)) {
  //   Serial.println("Connection to server failed");
  //   return;
  // }
  Serial.println("Connected to server");

  server.begin();
}

void loop()
{
  WiFiClient client = server.accept(); // listen for incoming clients

  Serial.println(client);



  int analogValue = analogRead(IR_PIN);
  int analogValue2 = analogRead(V_PIN);
  float voltage = analogValue2 * 20 / 4095.0;
  float current = analogRead(I_PIN);

  float temperature = dht.readTemperature();
  // Read the humidity
  float humidity = dht.readHumidity();
  // print out the values you read:

  // if (analogValue < 4000)
  // {
  //   digitalWrite(BUZ_PIN, HIGH);
  //   digitalWrite(LED_PIN, HIGH);
  // }
  // else
  // {
  //   digitalWrite(BUZ_PIN, LOW);
  //   digitalWrite(LED_PIN, LOW);
  // }
  digitalWrite(LED_PIN, HIGH);
  
  
  // Serial.printf("IR Sensor = %d\t", analogValue);
  // client.printf("IR Sensor = %d\t", analogValue);
  // Serial.printf("Voltage Sensor = %f\t", voltage);
  // client.printf("Voltage Sensor = %f\t", voltage);
  // Serial.printf("Current Sensor = %f\t", current);
  // client.printf("Current Sensor = %f\t", current);
  // Serial.printf("DHT Sensor = %f °C & %f %% \t", temperature, humidity);
  // client.printf("DHT Sensor = %f °C & %f %% \t", temperature, humidity);


String combined_string1 = "IR Sensor =" + String(analogValue) +", Voltage Sensor ="+String(voltage) +",Current Sensor =" +String(current) + ", DHT Sensor =" +String(temperature) + "°C & " +String(humidity) +" %\t";

// Print the combined string to both Serial and client
//Serial.println(combined_string1);
client.println(combined_string1);

  
  

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  String combined_string2 = "Accel_X: " + String(a.acceleration.x) + ", Accel_Y: " + String(a.acceleration.y) + ", Accel_Z:" +String(a.acceleration.z) + ", Gyro_X:" + String(g.gyro.x) + ", Gyro_Y:" + String(g.gyro.y) +", Gyro_Z:" + String(g.gyro.z);

// Print the combined string to both Serial and client
   //Serial.println(combined_string2);
   client.println(combined_string2);

 // Print data in key-value pair format for Serial Plotter

  // Serial.print("Accel_X:");
  // Serial.print(a.acceleration.x);
  // Serial.print("\t");
  // client.printf("Accel_X: %f \t", a.acceleration.x);

  // Serial.print("Accel_Y:");
  // Serial.print(a.acceleration.y);
  // Serial.print("\t");
  // client.printf("Accel_Y: %f \t", a.acceleration.y);

  // Serial.print("Accel_Z:");
  // Serial.print(a.acceleration.z);
  // Serial.print("\t");
  // client.printf("Accel_Z: %f \t", a.acceleration.z);

  // Serial.print("Gyro_X:");
  // Serial.print(g.gyro.x);
  // Serial.print("\t");
  // client.printf("Gyro_X: %f \t", g.gyro.x);

  // Serial.print("Gyro_Y:");
  // Serial.print(g.gyro.y);
  // Serial.print("\t");
  // client.printf("Gyro_Y: %f \t", g.gyro.y);

  // Serial.print("Gyro_Z:");
  // Serial.print(g.gyro.z);
  // client.printf("Gyro_Z: %f", g.gyro.z);
  // Serial.println();
  //delay(500);

  digitalWrite(LED_PIN,LOW);
 // delay in between reads for clear read from serial
}
