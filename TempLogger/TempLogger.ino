/*
  MySQL Temperature Logger
  Version 1.0.6
  Reading temperature with DHT22 sensor and logging to MySQL Database.
*/

// include the library code:
// Library for LCD Display
#include <LiquidCrystal.h>
// Library for Ethernet Shield
#include <SPI.h>
#include <Ethernet.h>
// Library for MySQL Connection
#include <sha1.h>
#include <mysql.h>
// Library for DHT22 Temperature/Humidity sensor
#include <DHT.h>

////          ////
//  Networking  //
////          ////

// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
//the IP address for the shield:
//byte ip[] = { 192, 168, 55, 5 };   

////     ////
//  MySQL  //
////     ////

// IP of MySQL Server
IPAddress server_addr(192, 168, 55, 10);
Connector my_conn;
char user[] = "temp_logger_user";
char password[] = "Password123";
// MySQL Query
char INSERT_SQL[] = "INSERT INTO temp_logger.log (log_temp_c, log_hum, log_sensor_id) VALUES (%s, %s, %d)";
char query[128];
char temperature[10];
char humidity[10];

////         ////
//  Variables  //
////         ////

// Initialise variables
String ver = "1.0.6";
int sensorID = 1001;
float temp = 0;
float hum = 0;
int numFails = 0;
int maxFails = 5;
// Initialise variables for LED pins
int red = 23;
int green = 25;
int blue = 27;

////   ////
//  LCD  //
////   ////

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

////   ////
//  DHT  //
////   ////

#define DHTPIN 29     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define fan 4
 
//int maxHum = 60;
//int maxTemp = 40;
 
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  // Set Up LCD

  // Set pin modes for LED
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  dht.begin();
  
  lcd.begin(16, 2);
  
  // Set up network connection
  //Ethernet.begin(mac);

  Serial.begin(9600);
  while (!Serial);
  //delay(1000);
  Serial.println("MySQL Temperature Logger");
  Serial.println("Version " + ver);
  Serial.println("Starting...");
  lcd.setCursor(0, 0);
  lcd.print("Temp Logger");
  lcd.setCursor(0, 1);
  lcd.print("Version " + ver);
  redLed();
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status:   V" + ver);
  Serial.println("Establishing network connection...");
  clearLine();
  lcd.print("Connecting LAN...");
  
  // Print IP address
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(5000);
    softReset();
  } else {
  // print your local IP address:
  Serial.println(Ethernet.localIP());
  greenLed();
  }
  
  //logData();
}

void logData(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status:   V" + ver);
  Serial.println("Connecting database...");
  clearLine();
  lcd.print("Connecting DB...");
  if (my_conn.mysql_connect(server_addr, 3306, user, password)){
    Serial.println("Connected!");
    clearLine();
    lcd.print("Connected!");
    blueLed();
    Serial.println("Inserting Data...");
    clearLine();
    lcd.print("Inserting Data...");
    
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    dtostrf(temp, 5, 2, temperature);
    dtostrf(hum, 5, 2, humidity);
    sprintf(query, INSERT_SQL, temperature, humidity, sensorID);
    Serial.println(query);
    my_conn.cmd_query(query);

    
    Serial.println("Data inserted.");
    clearLine();
    lcd.print("Data Inserted.");
    delay(1000);
    if (my_conn.is_connected()) {
      //Serial.println("Disconnecting...");
      //clearLine();
      //lcd.print("Disconnecting...");
      my_conn.disconnect();
      //Serial.println("Disconnected!");
      clearLine();
      lcd.print("Disconnected!");
      greenLed();
      delay (1000);
      numFails = 0;
    } else {
      //Serial.println("Already Disconnected.");
    }
  } else {
    Serial.println("Connection failed.");
    clearLine();
    lcd.print("Failed!");
    redLed();
    numFails++;
    if (numFails == maxFails) {
      Serial.println("Exceeded maximum connection faiilures! Rebooting...");
      clearLine();
      lcd.print("Reached maxFail!");
      delay(1000);
      clearLine();
      lcd.print("Rebooting...");
      delay(2000);
      softReset();
    }
  }
}

void softReset() {
  asm volatile("jmp 0");
} 

void clearLine(){
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);  
}

void redLed(){
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
}
void greenLed(){
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    digitalWrite(blue, LOW);
}
void blueLed(){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
}

void loop(){

logData();

clearLine();
lcd.print("T " + String(temp, 1) + char(223) + "C H " + String(hum, 1) + "%");
delay(10000);
clearLine();
lcd.print("Waiting...");

delay(284000); //5 mins total

  //printIPAddress();
  //delay(5000); // Loop delay
}
