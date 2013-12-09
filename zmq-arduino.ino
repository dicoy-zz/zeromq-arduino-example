/*

ZeroMQ Arduino Ethernet Shield example

created 09 Dec 2013 
by Mariano Vassallo

based on code by Tom Igoe / Usman Haque / Joe Saavedra / Scott Fitzgerald

You need an Ethernet Shield and (optionally) some sensors to be read on analog pins 0 and 1

Sends one ZeroMQ message every second to a ZeroMQ PULL socket on 192.168.11.3 port 7000
This means this code implements the simplest PUSH message.
It does not implement a full ZeroMQ stack. You shoul use it as a mean of delivering messages to a more stable
router running on a server.

 */

#include <SPI.h>
#include <Ethernet.h>

void sendData(String thisData) ;

  byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192,168,11,3);

// initialize the library instance:
EthernetClient client;

IPAddress server(192,168,11,2);

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 1000;  //delay between updates (in milliseconds)

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection:
  /*if (Ethernet.begin(mac) == 0) {*/
    // DHCP failed, so use a fixed IP address:
    Ethernet.begin(mac, ip);
  //}
}

void loop() {
  // read the analog sensor:
  int sensorReading = analogRead(A0);   
  // convert the data to a String to send it:

  String dataString = "sensor1,";
  dataString += sensorReading;

  int otherSensorReading = analogRead(A1);
  dataString += " sensor2,";
  dataString += otherSensorReading;

   // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and 1 second has passed since
  // your last connection, then connect again and send data: 
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    sendData(dataString);
    //sendData("BBB");
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void sendData(String thisData) {
  // if there's a successful connection:
  client.flush();
  if (client.connect(server, 7000)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
        /*echo -e $(printf '\\x01\\x00\\x%02x\\x00%s' $((1 + ${#m})) "$m") | nc $@ >/dev/null*/
    client.write((uint8_t)1);
    client.write((uint8_t)0);
    // imprimir len + 1
    //imprimir el string
    client.write((uint8_t)(thisData.length() + 1));
    client.write((uint8_t)0);

    // here's the actual content of the PUT request:
    client.print(thisData);
    client.flush();
    client.stop();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  // note the time that the connection was made or attempted:
  lastConnectionTime = millis();
}
