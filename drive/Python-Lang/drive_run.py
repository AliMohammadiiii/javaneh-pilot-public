#!/usr/bin/python3

from lib.canmodule import *
from drive.motor import *
import time

MOTOR_LF = 0
MOTOR_RF = 1
MOTOR_LB = 2
MOTOR_RB = 3
ID_CAN_MOTOR = 4

canModule = Can()

motors = [Motor(MOTOR_LF), Motor(MOTOR_RF), Motor(MOTOR_LB), Motor(MOTOR_RB)]
while(True):
	busMsg = canModule.read()
	speed = []
	if True:
		for i in range(4):
			speed.append(motors[i].setSpeed(busMsg.data[i%2]))
		canModule.sendSpeed(speed, ID_CAN_MOTOR)
		time.sleep(1)
