#include <Arduino.h>
#include "I2Cdev.h"
#include <Adafruit_MPU6050.h>
#include <Servo.h>

Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;
Servo servo;

#define START 0
#define FLIGHT 1
bool FALL = false;
#define LANDING 3

void servoMove() {
    servo.write(45);
    Serial.println("SERVO MOVING");
    delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(5,OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(11, OUTPUT);

    while (!Serial)
        delay(10); // will pause Zero, Leonardo, etc until serial console opens

    Serial.println("Adafruit MPU6050 test!");

    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }

    Serial.println("MPU6050 Found!");
    mpu_temp = mpu.getTemperatureSensor();
    mpu_temp->printSensorDetails();

    mpu_accel = mpu.getAccelerometerSensor();
    mpu_accel->printSensorDetails();

    mpu_gyro = mpu.getGyroSensor();
    mpu_gyro->printSensorDetails();
  servo.attach(31);
}

// в идеале запихать в отдельный файл как класс
bool button(bool btnState) {
  bool btnFlag = false;
  if(btnState && !btnFlag) {
    return btnFlag = true;
  }
  else {
    return btnFlag = false;
  }
}

void buzzer(bool buzzerState) {
  if(buzzerState) {
    tone(12, 2000, 50);
  }
}

void diod(bool diodState) {
  if(diodState) {
    digitalWrite(11, HIGH);
  }
  else {
    digitalWrite(11, LOW);
  }
}

void gy501() {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    mpu_temp->getEvent(&temp);
    mpu_accel->getEvent(&accel);
    mpu_gyro->getEvent(&gyro);

    Serial.print("\t\tTemperature ");
    Serial.print(temp.temperature);
    Serial.println(" deg C");

    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("\t\tAccel X: ");
    Serial.print(accel.acceleration.x);
    Serial.print(" \tY: ");
    Serial.print(accel.acceleration.y);
    Serial.print(" \tZ: ");
    Serial.print(accel.acceleration.z);
    Serial.println(" m/s^2 ");
    Serial.println(FALL);
    if(accel.acceleration.z < 0) {
        FALL = 1;
        Serial.println(FALL);
    }
    if(FALL == 1) {
        servoMove();
        FALL = 0;
    }
    delay(100);
}

int state;

void loop() {
    gy501();
//    if(accel.acceleration.z == 7) {
//        servoMove();
//    }

//  // сделать перебор с индексом, что бы не было говнокода
//  bool btnState1 = digitalRead(2);
//  bool btnState2 = digitalRead(3);
//  bool btnState3 = digitalRead(4);
//  buzzer(button(btnState1));
//  diod(button(btnState2));
//    switch (state)
//    {
//        case START:
//        {
//            if (alt > 10) state = FLIGHT;
//            servo.write(0);
//            break;
//        }
//        case FLIGHT:
//        {
//            if (spd_z < 0 or abs(gyro.readRadPerSecX()) > 1 or abs(gyro.readRadPerSecY()) > 1) state = FALL;
//            break;
//        }
//        case FALL:
//        {
//            servoMove();
//            if (alt<10) state = LANDING;
//            break;
//        }
//        case LANDING:
//        {
//            break;
//        }}
}