#!/usr/bin/python3
from lib.canmodule import *
ID_CAN_MARKER = 10
ID_CAN_MOVEMENT = 20
ID_LAYER_PLANNING = 2
ID_CHANGE_SPEED = 21
ID_CHANGE_ANGLE = 22
ID_BREAK = 23
#pantilt64 = PanTilt(initial_deg_x, initial_deg_y)

canModule = Can()
#class PanTiltPlanner:
#    def __init__(self):
#        self.pantilts = [PanTilt(initial_deg_x, initial_deg_y), PanTilt(initial_deg_x, initial_deg_y)]

 #   def changeAngle(self, panTilt_id, x, y):
 #       global canModule
 #       degX, degY = pantilt[panTilt_id%2].calculateDegree(y, x)
 #           if degX != -1:
 #               canModule.sendDeg(degX, degY,panTilt_id)


#panTiltPlanner = PanTiltPlanner()
while(True):
    #msg = canModule.read()
    #command_id = msg.arbitration_id
    #if command_id == ID_CAN_MARKER:
    #    panTiltPlanner.changeAngle(int(msg.data[0], int(msg.data[1]), int(msg.data[2]))
    x = int(input("1: change speed, 2: change angle, 3: break:  "))                                 
    if x == 1:
        speed = int(input("Speed: "))
        direction = int(input("1: direct 0: inverse : "))
        canModule.sendSpeed(speed, direction)
        #canModule.sendMessage([ID_CHANGE_SPEED, speed,direction, ID_LAYER_PLANNING], ID_CAN_MOVEMENT)
    if x == 2:
        deg = int(input("Deg: "))
        direction = int(input("1: Right 0: Left : "))
        canModule.sendAngle(deg,direction)
        #canModule.sendMessage([ID_CHANGE_ANGLE, deg, direction, ID_LAYER_PLANNING], ID_CAN_MOVEMENT)
    if x == 3:
        canModule.sendMessage([ID_BREAK,ID_LAYER_PLANNING], ID_CAN_MOVEMENT)
    if x == 4:
        canModule.sendMessage([25, 250 , ID_LAYER_PLANNING], ID_CAN_MOVEMENT)


