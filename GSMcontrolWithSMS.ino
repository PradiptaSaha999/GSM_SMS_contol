/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

char ch;
int8_t answer, i;
int light = 13;
SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
  pinMode(light, OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  digitalWrite(light, LOW);

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("AT+CMGF=1"); // Set SMS mode to text
  //delay(200);
  // mySerial.println("AT+CNMI=10,10,0,0"); // Set module to send SMS data to serial out upon receipt
  //delay(200);
  Serial.println("Hello, world?");
  do {
    answer = sendATcommand("AT+CMGF=1", "OK", 1000);
    if (answer == 1) {
      Serial.println("config done.");
    } else {
      Serial.println("Error");
    }
    delay(1000);
  } while (answer == 0);
}

void loop() // run over and over
{

  if (mySerial.available())
  {
    i = 0;
    answer = 0;
    int val;

    //    int val=mySerial.read();
    char response[200];
    do {
      if (mySerial.available() != 0) {
        val = mySerial.read();
        Serial.write(val);
        response[i] = val;
        i++;
        if (strstr(response, "L1") != NULL) {
          answer = 1;
        }
        if (strstr(response, "L0") != NULL) {
          answer = 2;
        }


      }
    } while ((answer == 0) && (val != 10));
    //    if(val==10)
    //    {
    //    Serial.println("enter");
    //    }
    //    else{
    //    Serial.write(val);
    //    }
    if (answer == 1)
    {
      Serial.println();
      Serial.println("got L1");
      digitalWrite(light, HIGH);
    }
    if (answer == 2)
    {
      Serial.println();
      Serial.println("got L0");
      digitalWrite(light, LOW);
    }
  }
  if (Serial.available())
    mySerial.write(Serial.read());



}
int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout) {
  uint8_t x = 0, answer = 0;
  char response[100];
  unsigned long previous;
  memset(response, '\0', 100); // initalize string
  delay(100);
  while (mySerial.available() > 0) {
    mySerial.read(); // clears the buffer
  }
  mySerial.println(ATcommand);
  // Serial.println(ATcommand);
  x = 0;
  previous = millis();
  do {
    if (mySerial.available() != 0) {
      response[x] = mySerial.read();
      x++;
      if (strstr(response, expected_answer) != NULL) {
        answer = 1;
      }
    }
  } while ((answer == 0) && ((millis() - previous) < timeout));
  Serial.println(response);
  return answer;
}

