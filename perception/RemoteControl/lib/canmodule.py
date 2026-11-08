import can
from threading import *
import time
MOTOR_LF = 0
MOTOR_RF = 1
MOTOR_LB = 2
MOTOR_RB = 3
CAN_SPEED = 500000
ID_CAN_MARKER = 10
ID_CAN_MOVEMENT = 20
ID_LAYER_PLANNING = 2
ID_CHANGE_SPEED = 21
ID_CHANGE_ANGLE = 22
class Can:
    def __init__(self):
        self.bustype = 'socketcan'
        self.channel = 'can0'
        self.lastDegX = 0
        self.lastDegY = 0
        self.lastSpeed = [[0, 0], [0, 0],[ 0, 0],[ 0, 0]]
    def sendDeg(self, x,y, can_id):
        sign_x=0
        sign_y=0
        if self.lastDegX == x and self.lastDegY == y:
            return
        self.lastDegX = x
        self.lastDegY = y
        if x<0:
            x=x*-1
            sign_x=1
        if y<0:
            y=y*-1
            sign_y=1
        if x <256 and y<256:
            with can.interface.Bus(channel=self.channel, bustype=self.bustype,bitrate=CAN_SPEED) as bus2:
                msg= can.Message(arbitration_id=can_id,data=[x,y,sign_x,sign_y,0,0],is_extended_id=False)
                bus2.send(msg)

   # def sendSpeed(self, speed,can_id):
    #    self.lastSpeed = speed
     #   with can.interface.Bus(channel=self.channel, bustype=self.bustype,bitrate=CAN_SPEED) as bus2:
     #       msg= can.Message(arbitration_id=can_id,data=[speed[MOTOR_LF][0],speed[MOTOR_LF][1], speed[MOTOR_RF][0],speed[MOTOR_RF][1], speed[MOTOR_LB][0],speed[MOTOR_LB][1], speed[MOTOR_RB][0],speed[MOTOR_RB][1]],is_extended_id=False)
      #      bus2.send(msg)
    def read(self):
        with can.interface.Bus(channel=self.channel, bustype=self.bustype,bitrate=CAN_SPEED) as bus2:

            msg = bus2.recv()
        return msg

    def sendMessage(self, message, can_id):
        with can.interface.Bus(channel=self.channel, bustype=self.bustype,bitrate=CAN_SPEED) as bus2:
            msg = can.Message(arbitration_id=can_id,data = message, is_extended_id=False)
            bus2.send(msg)
    def sendSpeed(self, speed, direction):
        self.sendMessage([ID_CHANGE_SPEED, speed,direction, ID_LAYER_PLANNING], ID_CAN_MOVEMENT)
    def sendAngle(self, deg, direction):
        self.sendMessage([ID_CHANGE_ANGLE, deg, direction, ID_LAYER_PLANNING], ID_CAN_MOVEMENT)
