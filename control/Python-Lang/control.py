#!/usr/bin/python3

import math 

from lib.canmodule import *
import time

MOTOR_R = 0
MOTOR_L = 1
SPEED = 0
DIRECTION = 1
ID_CAN_MOTOR = 4
RADIUS = 15
WIDTH = 30
canModule = Can()
WHEELBASELEN = 32
MINIMUM_DISTANCE = 15

ID_CAN_MOVEMENT = 20
ID_LAYER_PLANNING = 2
ID_CHANGE_SPEED = 21
ID_CHANGE_ANGLE = 22
ID_BREAK = 23
ID_ULTRASONIC = 40

class RationalSteering:
    def __init__(self):
        self.trackIn = 0.27
        self.trackOut = 0.27
        self.wheelBaseIn = 0.11
        self.wheelBaseOut = 0.32
        self.tireDia = 0.1
        self.rMin = 0.35
        self.angle = 0
        self.v = 2
        self.r = self.rMin
        self.rWheel = [self.rMin, self.rMin]
        self.rpm = [0,0]
        self.deg = 0
        self.turnDirection = 0
        self.wheelDirection = 1
        self.motorValue = [[0,1],[0,0]]
    def rCalc(self):
        #print(self.deg)
        if self.deg < 0.02:
            self.r = 10000
        else:
            self.r = self.rMin * 10 * (1-self.deg) / 2 + self.rMin
            #print("calc: ", self.rMin, self.deg, self.rMin * 10 * (1-self.deg) / 2 + self.rMin)
        #print("r: ", self.r)
    def rWheelCalc(self):
        if self.turnDirection:
            self.rWheel[MOTOR_L] = math.sqrt(abs(pow(self.r + self.trackIn / 2, 2) + pow(self.wheelBaseOut/2,2)))
            self.rWheel[MOTOR_R] = math.sqrt(abs(pow(self.r - self.trackIn / 2, 2) + pow(self.wheelBaseOut/2,2)))
        else:
            self.rWheel[MOTOR_L] = math.sqrt(abs(pow(self.r - self.trackIn / 2, 2) + pow(self.wheelBaseOut/2,2)))
            self.rWheel[MOTOR_R] = math.sqrt(abs(pow(self.r + self.trackIn / 2, 2) + pow(self.wheelBaseOut/2,2)))
        #print(self.rWheel)
    def rpmCacl(self):
        v = [self.v / self.r * self.rWheel[MOTOR_R] , self.v / self.r * self.rWheel[MOTOR_L]]
        #print("V: ", v)
        self.rpm = [v[MOTOR_R] / 3.6 * 60 / math.pi / self.tireDia, v[MOTOR_L] / 3.6 * 60 / math.pi / self.tireDia]
        #print("self rpm: " ,self.rpm)
    def setV(self, v, direction):
        print("set V: ", v)
        self.v = v
        self.wheelDirection = direction
        self.rpmCacl()
        self.motorCal()
        self.canSend()
        print("voltage: ",self.getRpm())
    def setAngle(self, angle, direction):
        self.turnDirection = direction
        self.deg = angle / 100
        self.rCalc()
        self.rWheelCalc()
        self.rpmCacl()
        self.motorCal()
        self.canSend()
        print("voltage: ", self.getRpm())
    def motorCal(self):
        if self.wheelDirection:
            self.motorValue[MOTOR_L][DIRECTION] = 0
            self.motorValue[MOTOR_R][DIRECTION] = 1
        else:
            self.motorValue[MOTOR_L][DIRECTION] = 1
            self.motorValue[MOTOR_R][DIRECTION] = 0
        self.motorValue[MOTOR_R][SPEED] = round(self.rpm[MOTOR_R] * 255 / 120)
        self.motorValue[MOTOR_L][SPEED] = round(self.rpm[MOTOR_L] * 255 / 120)
        if self.motorValue[MOTOR_R][SPEED] > 255 and self.motorValue[MOTOR_L][SPEED] > 255:
            self.motorValue[MOTOR_L][SPEED] = 255
            self.motorValue[MOTOR_R][SPEED] = 255
        elif self.motorValue[MOTOR_R][SPEED] > 255:
            self.motorValue[MOTOR_L][SPEED] -= self.motorValue[MOTOR_R][SPEED] - 255
            self.motorValue[MOTOR_R][SPEED] = 255
        elif self.motorValue[MOTOR_L][SPEED] > 255:
            self.motorValue[MOTOR_R][SPEED] -= self.motorValue[MOTOR_L][SPEED] - 255
            self.motorValue[MOTOR_L][SPEED] = 255
    def getRpm(self):
        return self.motorValue

    def canSend(self):
        global canModule
        canModule.sendMessage([self.motorValue[MOTOR_R][SPEED], self.motorValue[MOTOR_R][DIRECTION],self.motorValue[MOTOR_L][SPEED],self.motorValue[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)

class Movement:
    def __init__(self):
        self.speed = [[0,1],[0,0]]
        self.ratioR = 1
        self.ratioL = 1
        self.lastSpeed = 0
        self.lastDirection = 1
    def changeSpeed(self, speed, direction):
        rpm = round(speed )#/ 2 / 3.14 / 0.05 / 60) # range 4700 and 1507
        if rpm > 250:
            rpm = 250
        if rpm < 40 and rpm > 0:
            rpm = 40
        self.lastSpeed = speed
        self.lastDirection = direction
        self.speed[MOTOR_L][SPEED] =  int(round(rpm * self.ratioL))
        self.speed[MOTOR_R][SPEED] =  int(round(rpm * self.ratioR))
        if direction:
            self.speed[MOTOR_L][DIRECTION] = 0
            self.speed[MOTOR_R][DIRECTION] = 1
        else:
            self.speed[MOTOR_L][DIRECTION] = 1
            self.speed[MOTOR_R][DIRECTION] = 0
        self.canSend()
    def changeAngle(self, deg, direction):
        rpm_L = (RADIUS + WIDTH) / RADIUS
        print(rpm_L)
        minRpm = self.speed[MOTOR_L][SPEED]
        if direction:
            self.speed[MOTOR_L][SPEED] = int(round(self.speed[MOTOR_L][SPEED] * rpm_L))
        else:
            self.speed[MOTOR_R][SPEED] = int(round(self.speed[MOTOR_R][SPEED] * rpm_L))
        if self.speed[MOTOR_L][SPEED] > 250:
            self.speed[MOTOR_L][SPEED] = 250
        if self.speed[MOTOR_R][SPEED] > 250:
            self.speed[MOTOR_R][SPEED] = 250
        self.canSend()
        sleepTime = round(RADIUS / 1  * deg / 360 * 60 / (115 * minRpm / 255), 3)
        print(sleepTime)
        time.sleep(sleepTime)
        self.speed[MOTOR_L][SPEED] = minRpm
        self.speed[MOTOR_R][SPEED] = minRpm
        #if direction:
        #    self.speed[MOTOR_L][SPEED] =  self.speed[MOTOR_R][SPEED]
        #else:
        #    self.speed[MOTOR_R][SPEED] =  self.speed[MOTOR_L][SPEED] 
        print(self.speed[MOTOR_L][SPEED], self.speed[MOTOR_R][SPEED])
        self.canSend()

    def changeSteering(self, deg, direction):
        if deg == 0:
            self.ratioR = 1
            self.ratioL = 1
        else:
            #radius = abs(WHEELBASELEN / math.tan(deg*0.0174533)) - WIDTH + abs(WHEELBASELEN / 2 * math.tan(deg * 0.0174533))
            #ratio = (radius + WIDTH) / radius
            #print(ratio)
            ratio = math.log(deg, 10)
            print(ratio)
            if direction:
                self.ratioL = ratio / 2
                self.ratioR = -ratio / 2
            else:
                self.ratioR = ratio / 2
                self.ratioL = -ratio / 2

        self.changeSpeed(self.lastSpeed, self.lastDirection)
    def breakCar(self):
        self.speed = [[0,1],[0,0]]
        self.lastSpeed = 0
        self.changeSpeed(0, self.lastDirection)

    def canSend(self):
        global canModule
        canModule.sendMessage([self.speed[MOTOR_R][SPEED], self.speed[MOTOR_R][DIRECTION],self.speed[MOTOR_L][SPEED],self.speed[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)


    def turnInPlace(self, speed):
        self.speed[MOTOR_L][SPEED] = speed
        self.speed[MOTOR_R][SPEED] = speed
        self.speed[MOTOR_L][DIRECTION] = 1
        self.speed[MOTOR_R][DIRECTION] = 1
        self.canSend()
        time.sleep(6)
        self.speed = [[0,1],[0,0]]
        self.canSend()

movement = Movement()
rationalSteering = RationalSteering()
def changeMovement(msg):
    if int(msg[0]) == ID_CHANGE_SPEED:
        # movement.changeSpeed(int(msg[1]),int(msg[2]))
        # print(int(msg[1]) / 250 ,int(msg[2]))
        rationalSteering.setV(int(msg[1]) / 100 / 1.2,int(msg[2]))
    elif int(msg[0]) == ID_CHANGE_ANGLE:
        #movement.changeSteering(int(msg[1]),int(msg[2]))
        print("angle", int(msg[1]),int(msg[2]))
        rationalSteering.setAngle(int(msg[1]),int(msg[2]))
    elif int(msg[0]) == ID_BREAK:
        rationalSteering.setV(0,0)
        movement.breakCar()
    elif int(msg[0]) == 25:
        movement.turnInPlace(int(msg[1]))
def ultraSonic(msg):
    if int(msg[0]) < MINIMUM_DISTANCE or int(msg[1]) < MINIMUM_DISTANCE:
        rationalSteering.setV(0,0)
while True:
    msg = canModule.read()
    command_id = msg.arbitration_id
    if command_id == ID_CAN_MOVEMENT:
        changeMovement(msg.data)
    if command_id == ID_ULTRASONIC:
        ultraSonic(msg.data)
   # x = int(input("1: change speed, 2: change angle, 3: break:  "))
   # if x == 1:
        #     speed = int(input("Speed: "))
   #     direction = int(input("1: direct 0: inverse : "))
   #     rationalSteering.setV(speed, direction)
   #     print(rationalSteering.getRpm())
   # if x == 2:
        #    deg = int(input("Deg: "))
    #    direction = int(input("1: Right 0: Left : "))
    #    rationalSteering.setAngle(deg, direction)
   #     print(rationalSteering.getRpm())

        







