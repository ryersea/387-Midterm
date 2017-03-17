
#include <elapsedMillis.h> // timer library
#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h> // ethernet library
#include <Twitter.h> // twitter library

// Ethernet Shield Settings
//mac = media access control
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Don't need to specify the IP address, DHCP is used

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("3494178987-W3isStL6pSv8RxNmWRGAaFhUyz0rLOsQlhGbsSw");
 
int ledPin = 13;                // choose the pin for the LED
int inputPin = A0;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
const int buzzer = 2;           // buzzer to arduino digital pin 2
int counter = 0;                // counter starts at zero
int motiontime = 0;             // time of motion
int flag = 0;                   // this keeps track of the time

// Message to post
//final twitter message to read "Motion was detected for (motion time value) seconds."
char msg[50];
char firstpart[] = "Motion was detected for";
char lastpart[] = "seconds.";
 
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 2 as an output
  Serial.begin(9600);
}

void tweet(){
  delay(1000);

  Ethernet.begin(mac);
  Serial.begin(9600);
  
  Serial.println("connecting ...");
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}
 
void loop(){
  val = analogRead(inputPin);  // read input value
  //Serial.println(val);
  if (val > 200) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON

    if(flag == 0)
    {
      counter = 0;
    }
    
    flag = 1;
    
//alarm only sounds if the motion has been detected for 30 seconds or more
  if (counter >= 30 && flag ==1 ){ 
 
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(100);        // ...for 1sec

}

    if (pirState == LOW) 
    {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } 
  else
  {
    digitalWrite(ledPin, LOW); // turn LED OFF

    if(flag == 1)
    {
        motiontime = counter - 8;
        sprintf(msg, "%s %i", firstpart, motiontime); //concatenates the first string message with the integer and forms a new message
        sprintf(msg, "%s %s", msg, lastpart); //concatenates the new messge with the last string to form the final message
        tweet();
    }

    flag = 0;
    
      if(pirState == HIGH)
      {
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

  delay(800);
  counter = counter + 1;
  Serial.println(counter);
  
}


