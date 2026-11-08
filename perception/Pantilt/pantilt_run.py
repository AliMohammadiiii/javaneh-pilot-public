#!/usr/bin/python3

import time
from pantilt.video import Video
from pantilt.marker import *

from pantilt.pantilt import *

from lib.canmodule import *
from lib.general import can_send_check, now

ARUCOCODE = cv2.aruco.DICT_4X4_1000
initial_deg_x=90
initial_deg_y=90

MARKER_CODE = 645 

ID_CAN_PANTILT = 64

canModule = Can()

def panTilt(canModule):
    video = Video()
    pantilt = PanTilt(initial_deg_x, initial_deg_y)
    last_can_time = 0
    marker = Marker(MARKER_CODE)
    
    while(True):
        video.read()
        x, y = marker.getMarkerMiddle(video.frame)
        #if x:
        #    video.drawPoint(y, x, [0, 0, 255])
        if can_send_check(2, last_can_time):
            last_can_time = now()
            degX, degY = pantilt.calculateDegree(y, x)
            if degX != -1:
            	canModule.sendDeg(degX, degY, ID_CAN_PANTILT)
        #video.show()            
        if cv2.waitKey(1) & 0xFF == ord('q'):
        	break

panTilt(canModule)

