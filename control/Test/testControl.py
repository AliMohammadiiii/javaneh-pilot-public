ID_CAN_MARKER = 10
ID_CAN_MOVEMENT = 20
ID_LAYER_PLANNING = 2
ID_CHANGE_SPEED = 21
ID_CHANGE_ANGLE = 22
ID_BREAK = 23
SPEED = 0
DIRECTION =1
MOTOR_L = 1
MOTOR_R = 0
ID_CAN_MOTOR = 4

from lib.canmodule import *
canModule = Can()
import time
def testDirect():
    global canModule
    motorValue = [[118,1],[120,0]]
    canModule.sendMessage([motorValue[MOTOR_R][SPEED], motorValue[MOTOR_R][DIRECTION],motorValue[MOTOR_L][SPEED],motorValue[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)
    time.sleep(8)
    motorValue = [[0,0],[0,1]]
    canModule.sendMessage([motorValue[MOTOR_R][SPEED], motorValue[MOTOR_R][DIRECTION],motorValue[MOTOR_L][SPEED],motorValue[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)
    time.sleep(3)
    motorValue = [[115,0],[120,1]]
    canModule.sendMessage([motorValue[MOTOR_R][SPEED], motorValue[MOTOR_R][DIRECTION],motorValue[MOTOR_L][SPEED],motorValue[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)
    time.sleep(8)
    motorValue = [[0,0],[0,1]]
    canModule.sendMessage([motorValue[MOTOR_R][SPEED], motorValue[MOTOR_R][DIRECTION],motorValue[MOTOR_L][SPEED],motorValue[MOTOR_L][DIRECTION]], ID_CAN_MOTOR)

def testTurn():
    global canModule
    canModule.sendSpeed(120,1)
    time.sleep(3)
    canModule.sendAngle(100,1)
    time.sleep(3)
    canModule.sendSpeed(0,1)
    #time.sleep(5)
    #canModule.sendSpeed(117,0)
    #time.sleep(3)
    #canModule.sendAngle(0,0)
    #time.sleep(3)
    #canModule.sendSpeed(0,1)

while True:
    x = int(input("1: Direct, 2: turn"))
    if x == 1:
        testDirect()
    if x == 2:
        testTurn()
