#include <TimedAction.h>
TimedAction DHT22SensAction = TimedAction(60000, &DHT22Sens);
TimedAction USonicSensAction = TimedAction(250, &USonicSens);
TimedAction PirSensAction = TimedAction(50, &PirSens);
TimedAction RobotAction = TimedAction(50, &Robot);

// DHT Humidity and temperature sensor
#include <DHT.h>
#define DHTPIN 46
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

// ____Ultrasonic HC-SR04____
#include <NewPing.h>
#define TRIGGER_PIN  22  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     23  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define USPOWER_PIN  2
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// ____PIR HC-SR501 sensor____
#define pirPin 3
int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 500;
boolean lockLow = true;
boolean takeLowTime;
int PIRValue = 0;

// ____Robot____
// Motor 1
int dir1PinA = 13;
int dir2PinA = 12;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 11;
int dir2PinB = 10;
int speedPinB = 8; // Needs to be a PWM pin to be able to control motor speed

// PanTilt
#include <Servo.h>
int PosCS1 = 100, PosCS2 = 100;
Servo CS1, CS2;

void setup() {
    Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
    
    // ____DHT22____
      dht.begin();

    // ____USonicSensor____
    pinMode(USPOWER_PIN, OUTPUT);
    digitalWrite(USPOWER_PIN, HIGH);

    // ____PIRSensor____
    pinMode(pirPin, INPUT);

    // ____Robot____
    // Define L298N Dual H-Bridge Motor Controller Pins
    pinMode(dir1PinA, OUTPUT);
    pinMode(dir2PinA, OUTPUT);
    pinMode(speedPinA, OUTPUT);
    pinMode(dir1PinB, OUTPUT);
    pinMode(dir2PinB, OUTPUT);
    pinMode(speedPinB, OUTPUT);

    // Define PanTilt Camera pins
    CS1.attach(53);
    CS2.attach(47);
    CS1.write(PosCS1);
    CS2.write(PosCS2);
}

void loop() {
    USonicSensAction.check();
    PirSensAction.check();
    RobotAction.check();
    DHT22SensAction.check();
}

void DHT22Sens(){
  float h = dht.readHumidity(); // Read data and store it to variables hum and temp
  float t = dht.readTemperature(); // Read temperature as Celsius
  float f = dht.readTemperature(false); // Read temperature as Fahrenheit (isFahrenheit = true)
    if (isnan(h) || isnan(t) || isnan(f)) { // Check if any reads failed and exit early (to try again).
      Serial.println("Failed to read from DHT sensor!");
    return;
    }
    float hic = dht.computeHeatIndex(t, h, false);
    //Print temp and humidity values to serial monitor
  Serial.print("Hum: ");
  Serial.print(h);
  Serial.print("% ");
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print("*C ");
  Serial.print("Feals: ");
  Serial.print(hic);
  Serial.println("*C ");
}

void USonicSens() {
    unsigned int cm = sonar.ping_cm();
    if ((cm == 0) || (cm > 40)) {
        delay(50);
    } else {
        Serial.print("Ping: ");
        Serial.print(cm); // Send ping, get distance in cm and print result (0 = outside set distance range)
        Serial.println("cm");
        delay(50); // Wait 30ms between pings (about 30 pings/sec). 29ms should be the shortest delay between pings.
    } if (cm == 0 && digitalRead(ECHO_PIN) == HIGH) {
        digitalWrite(USPOWER_PIN, LOW);
        delay(50); // 50 mS is the minimum Off time to get clean restart
        digitalWrite(USPOWER_PIN, HIGH); // Adjust this value if your sensors don't read after reset
        delay(60); // Some sensors throw out a very short false echo after timeout
        sonar.ping_cm(); // 5cm-10cm value.  If your sensors do not throw out this false echo,
        //  you can get rid of this delay and ping after power HIGH
    } // Conversely, if you still get a false echo, you may need to increase the delay.
}

void PirSens() {
    if (digitalRead(pirPin) == HIGH) {
        if (lockLow) {
            PIRValue = 1;
            lockLow = false;
            Serial.println("Motion detected.");
            delay(50);
        }
        takeLowTime = true;
    } if (digitalRead(pirPin) == LOW) {
        if (takeLowTime) {
            lowIn = millis();
            takeLowTime = false;
        }if (!lockLow && millis() - lowIn > pause) {
            PIRValue = 0;
            lockLow = true;
            Serial.println("Motion ended.");
            delay(50);
        }
    }
}

void Robot() {
    if (Serial.available()) {
        byte val = Serial.read();
        int speed; // Local variable

        //______________Motor 1______________

        if (val == 'F') { // Motor 1 Forward
            analogWrite(speedPinA, 255); //Sets speed variable via PWM 
            digitalWrite(dir1PinA, LOW);
            digitalWrite(dir2PinA, HIGH);
            analogWrite(speedPinB, 255);
            digitalWrite(dir1PinB, LOW);
            digitalWrite(dir2PinB, HIGH);
            Serial.println("Forward...");
        } if (val == 'T') { // Motor 1 Stop (Freespin)
            analogWrite(speedPinA, 0);
            digitalWrite(dir1PinA, LOW);
            digitalWrite(dir2PinA, LOW);
            analogWrite(speedPinB, 0);
            digitalWrite(dir1PinB, LOW);
            digitalWrite(dir2PinB, LOW);
            Serial.println("Stop");
        } if (val == 'B') { // Motor 1 Backward
            analogWrite(speedPinA, 255); //Sets speed variable via PWM 
            digitalWrite(dir1PinA, HIGH);
            digitalWrite(dir2PinA, LOW);
            analogWrite(speedPinB, 255);
            digitalWrite(dir1PinB, HIGH);
            digitalWrite(dir2PinB, LOW);
            Serial.println("Backward...");
        } if (val == 'R') { // Turn Right
            analogWrite(speedPinA, 255); //Sets speed variable via PWM 
            digitalWrite(dir1PinA, LOW);
            digitalWrite(dir2PinA, HIGH);
            analogWrite(speedPinB, 255);
            digitalWrite(dir1PinB, HIGH);
            digitalWrite(dir2PinB, LOW);
            Serial.println("Turn Right...");
        } if (val == 'L') { // Turn Left
            analogWrite(speedPinA, 255); //Sets speed variable via PWM 
            digitalWrite(dir1PinA, HIGH);
            digitalWrite(dir2PinA, LOW);
            analogWrite(speedPinB, 255);
            digitalWrite(dir1PinB, LOW);
            digitalWrite(dir2PinB, HIGH);
            Serial.println("Turn Left...");
        } if (val == 'A') { // Pan Left
            PosCS1 = PosCS1 + 5;
            CS1.write(PosCS1);
            Serial.println("A Pressed");
        } if (val == 'S') { // Tilt Down
            PosCS2 = PosCS2 + 5;
            CS2.write(PosCS2);
            Serial.println("S Pressed");
        } if (val == 'D') { // Pan Right
            PosCS1 = PosCS1 - 5;
            CS1.write(PosCS1);
            Serial.println("D Pressed");
        } if (val == 'W') { // Tilt Up
            PosCS2 = PosCS2 - 5;
            CS2.write(PosCS2);
            Serial.println("W Pressed");
        }
    }
}
