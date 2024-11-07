/*
WiFi Web Server LED Blink
 
A simple web server that lets you blink an LED via the web.
This sketch will print the IP address of your WiFi Shield (once connected)
to the Serial monitor. From there, you can open that address in a web browser
to turn on and off the LED on pin 5.
 
If the IP address of your shield is yourAddress:
http://yourAddress/H turns the LED on
http://yourAddress/L turns it off
 
This example is written for a network using WPA2 encryption. For insecure
WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.
 
Circuit:
* WiFi shield attached
* LED attached to pin 5
 
created for arduino 25 Nov 2012
by Tom Igoe
 
ported for sparkfun esp32
31.01.2017 by Jan Hendrik Berlin
 
*/
 
#include <WiFi.h>
 
const char *ssid = "TheLantern";
const char *password = "whynotdoom";
const int motorpinA = 13;
const int motorpinB = 12;
const int forwardpinA = 33;
const int backwardpinA = 15;
const int forwardpinB = 32;
const int backwardpinB = 14;
 
NetworkServer server(80);
 
void setup() {
  Serial.begin(9600);
  pinMode(motorpinA, OUTPUT);  // set the LED pin mode
  pinMode(motorpinB, OUTPUT);
  digitalWrite(forwardpinA, 0);
  digitalWrite(backwardpinA,0);
  digitalWrite(forwardpinB, 0);
  digitalWrite(backwardpinB, 0); 
  delay(10);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  server.begin();
}
 
void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients
 
  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character
 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
 
            // the content of the HTTP response follows the header:
client.print("<style> body {background-color: BLACK;}</style>");
client.print("<center><div id=\"gifOverlay\" style=\"position: fixed; top: 0; left: 0; width: 100vw; height: 100vh; background-color: #f0f2f5; display: flex; align-items: center; justify-content: center; z-index: 9999;\">");
client.print("<img src=\"http://k33p.zapto.org:85/meme-reactions/comunion.gif\" style=\"width: 100%; max-width: 600px;\"></div>");
 
client.print("<div id=\"mainContent\" style=\"display: none;\">");
client.print("<h1 style=\"margin-bottom: 20px; color: #2d3436;\">BLESSED IS THE MACHINE</h1>");
 
client.print("<a href=\"/H\" style=\"display: inline-block; margin: 10px; padding: 20px 40px; font-size: 1.5em; background-color: #00cec9; color: #ffffff; text-decoration: none; border-radius: 12px; font-weight: bold;\">FAST</a><br>");
client.print("<a href=\"/M\" style=\"display: inline-block; margin: 10px; padding: 20px 40px; font-size: 1.5em; background-color: #0984e3; color: #ffffff; text-decoration: none; border-radius: 12px; font-weight: bold;\">MEDIUM</a><br>");
client.print("<a href=\"/L\" style=\"display: inline-block; margin: 10px; padding: 20px 40px; font-size: 1.5em; background-color: #fdcb6e; color: #ffffff; text-decoration: none; border-radius: 12px; font-weight: bold;\">SLOW</a><br>");
client.print("<a href=\"/O\" style=\"display: inline-block; margin: 10px; padding: 20px 40px; font-size: 1.5em; background-color: #d63031; color: #ffffff; text-decoration: none; border-radius: 12px; font-weight: bold;\">OFF</a><br>");
 
client.print("<div style=\"margin-top: 20px;\">");
client.print("<a href=\"/F\" style=\"display: inline-block; margin: 10px; padding: 20px 40px; font-size: 1.5em; background-color: #6c5ce7; color: #ffffff; text-decoration: none; border-radius: 12px; font-weight: bold;\">FORWARD</a><br>");
client.print("<a href=\"/B\" style=\"display: inline-block; margin: 10px; padding: 20px 40px; font-size: 1.5em; background-color: #b2bec3; color: #ffffff; text-decoration: none; border-radius: 12px; font-weight: bold;\">BACKWARD</a><br>");
client.print("</div></div>");
 
client.print("<script>");
client.print("window.onload = function() {");
client.print("setTimeout(function() {");
client.print("document.getElementById('gifOverlay').style.display = 'none';");
client.print("document.getElementById('mainContent').style.display = 'block';");
client.print("}, 3000);");
client.print("};");
client.print("</script></center>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
 
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          analogWrite(motorpinA, 255);
          analogWrite(motorpinB, 255);
        }
        if (currentLine.endsWith("GET /M")) {
          analogWrite(motorpinA, 155);
          analogWrite(motorpinB, 155);
        }
        if (currentLine.endsWith("GET /L")) {
          analogWrite(motorpinA, 105);
          analogWrite(motorpinB, 105);
        }
        if (currentLine.endsWith("GET /O")) {
          analogWrite(motorpinA, 0);
          analogWrite(motorpinB, 0);
        }
        if (currentLine.endsWith("GET /F")) {
          digitalWrite(forwardpinA, 1);
          digitalWrite(backwardpinA, 0);
          digitalWrite(forwardpinB, 1);
          digitalWrite(backwardpinB, 0);
        }
        if (currentLine.endsWith("GET /B")) {
          digitalWrite(forwardpinA, 0);
          digitalWrite(backwardpinA, 1);
          digitalWrite(forwardpinB, 0);
          digitalWrite(backwardpinB, 1);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
