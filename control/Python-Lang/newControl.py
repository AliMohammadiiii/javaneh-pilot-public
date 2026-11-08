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
        print(self.rWheel)
    def rpmCacl(self):
        v = [self.v / self.r * self.rWheel[MOTOR_R] , self.v / self.r * self.rWheel[MOTOR_L]]
        print("V: ", v)
        self.rpm = [v[MOTOR_R] / 3.6 * 60 / math.pi / self.tireDia, v[MOTOR_L] / 3.6 * 60 / math.pi / self.tireDia]
        if self.rpm[MOTOR_R] > 120 and self.rpm[MOTOR_L] > 120:
            self.rpm[MOTOR_R] = 120
            self.rpm[MOTOR_L] = 120
        elif self.rpm[MOTOR_R] > 120:
            self.rpm[MOTOR_R] = 120
            print("old motor l rpm", self.rpm[MOTOR_L])
            self.rpm[MOTOR_L] = self.overFlowRpm()
        elif self.rpm[MOTOR_L] > 120:
            self.rpm[MOTOR_L] = 120
            print("old motor r rpm", self.rpm[MOTOR_R])
            self.rpm[MOTOR_R] = self.overFlowRpm()
        #print("self rpm: " ,self.rpm)
    def overFlowRpm(self):
        vmaxWheel = 120 * 3.6 * math.pi / 60 * self.tireDia
        rwheel = math.sqrt(abs(pow(self.r + self.trackIn / 2, 2) + pow(self.wheelBaseOut/2,2)))
        print(self.r)
        vcar = vmaxWheel * self.r / rwheel
        rwheel = math.sqrt(abs(pow(self.r - self.trackIn / 2, 2) + pow(self.wheelBaseOut/2,2)))
        print("vmax car in this deg", vcar)
        return vcar / self.r * rwheel/ 3.6 * 60 / math.pi / self.tireDia
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
        print("speed of motors", self.motorValue)
        canModule.sendMessage([self.motorValue[MOTOR_R][SPEED], self.motorValue[MOTOR_R][DIRECTION],self.motorValue[MOTOR_L][SPEED],self.motorValue[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)


rationalSteering = RationalSteering()
def changeMovement(msg):
    if int(msg[0]) == ID_CHANGE_SPEED:
        # movement.changeSpeed(int(msg[1]),int(msg[2]))
        # print(int(msg[1]) / 250 ,int(msg[2]))
        rationalSteering.setV(int(msg[1]) / 100 / 1.5,int(msg[2]))
    elif int(msg[0]) == ID_CHANGE_ANGLE:
        #movement.changeSteering(int(msg[1]),int(msg[2]))
        print("angle", int(msg[1]),int(msg[2]))
        rationalSteering.setAngle(int(msg[1]),int(msg[2]))
    elif int(msg[0]) == ID_BREAK:
        rationalSteering.setV(0,0)
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
    #if command_id == ID_ULTRASONIC:
    #    ultraSonic(msg.data)
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

        







