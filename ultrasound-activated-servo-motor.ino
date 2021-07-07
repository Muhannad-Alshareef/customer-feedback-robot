// Arduino Ultrasoninc Sensor HC-SR04
#include<Servo.h>
#define echoPin 2         // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3         //attach pin D3 Arduino to pin Trig of HC-SR04
long duration;            // variable for the duration of sound wave travel
int distance;             // variable for the distance measurement
char Incoming_value = 0;  //Variable for storing Incoming_value
char outgoing_signal = 1; //variable for sending outgoing signals
long randomNumber;        //random number generated to choose between 3 welcoming gestures
int check=0;              //used to check if the person is still standing after 3 seconds to start the video

//first arm servos
Servo serv1;    //shoulder
Servo serv2;    //elbow
Servo serv3;    //wrist
//second arm servos
Servo serv4;    //shoulder
Servo serv5;    //elbow
Servo serv6;    //wrist

void setup() {
  // attaches the servo on pins to the servo object
  serv1.attach(5);  
  serv2.attach(6);
  serv3.attach(7);
  serv4.attach(8);
  serv5.attach(9);
  serv6.attach(10);

  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  
  //start all servos at 90 degrees
  serv1.write(90);
  serv2.write(90);
  serv3.write(90);
  serv4.write(90);
  serv5.write(90);
  serv6.write(90);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT
  Serial.begin(9600);       // Serial Communication is starting with 9600 of baudrate speed
}
void loop() {
  //ultrasonic detecion part
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;       // Speed of sound wave divided by 2 (go and back)
  
  // Displays the distance on the Serial Monitor
    //Serial.print("Distance: ");       //uncomment this part to see distance info
    //Serial.print(distance);
    //Serial.println(" cm");
  
  //servo movement part
  if(distance<=70){               //distance set for detection is 70cm
    randomNumber = random(1,4);   //generate a random number to choose a random welcome gesture 
    Serial.print(randomNumber);   //for testing
      switch(randomNumber){       //choose the gesture based on the random number generated
        
        case 1:  serv1.write(180);
                 serv2.write(90);
                 serv3.write(180);
                 serv4.write(180);
                 serv5.write(90);
                 serv6.write(180);
                break;
                
        case 2:  serv1.write(180);
                 serv2.write(180);
                 serv3.write(90);
                 serv4.write(180);
                 serv5.write(180);
                 serv6.write(90);
                break;
                
        case 3:  serv1.write(90);
                 serv2.write(0);
                 serv3.write(90);
                 serv4.write(90);
                 serv5.write(0);
                 serv6.write(90);
                break;
      }
  }
  delay(500);             //keep the pose for half a second
  //reset servo position 
  serv1.write(90);
  serv2.write(90);
  serv3.write(90);
  serv4.write(90);
  serv5.write(90);  
  serv6.write(90);

  //check if the person is still standing at 1 second intervals to avoid counting different people passing
  delay(1000);

  //read distance again
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
      
  if(distance<=70){
    check=1;
    delay(1000);
    //read distance again
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
      
      if(distance<=70){
        check =1;
        delay(1000);
        //read distance again
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = duration * 0.034 / 2;
        
        if(distance<=70){
          check =1;
        }
        else{
          check = 0;
        }
       }
      else{
        check = 0;
      }
    }  
  else{check = 0;}

  
  //bluetooth communication part
  if(Serial.available() > 0)  
  {
    if(check==1){                   //send a signal that a person is detected at 70cm or less
    Serial.print(outgoing_signal);  //send "1" to start the video
    Serial.print("\n");             //New line 
    while(distance<=70){            //keep the arduino busy until the person leaves
      //keep reading distance until the person leaves
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;    
    }           
    }
    else{}                          //if no person is detected restart the process 

  }
  
}
