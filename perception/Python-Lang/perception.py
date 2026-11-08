#!/usr/bin/python3
import time
from pantilt.video import Video
from pantilt.marker import *

from pantilt.pantilt import *

from lib.canmodule import *
from lib.general import can_send_check, now

ARUCOCODE = cv2.aruco.DICT_4X4_1000
MARKER_CODE = 645

ID_CAN_MARKER = 10
ID_CAN_PANTILT = 64

class MarkerLocator:
    def __init__(self, marker_code):
        self.video = Video()
        self.marker = Marker(MARKER_CODE)
        self.last_can_time = 0
        self.x = 0
        self.y = 0
    def find_location(self):
        self.x, self.y = self.marker.getMarkerMiddle(self.video.frame)
        self.send_location()
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    def send_location(self):
        if can_send_check(2, last_can_time):
            last_can_time = now()
            canModule.sendMessage([ID_CAN_PANTILT, x, y], ID_CAN_MARKER)

markerLocator = MarkerLocator(MARKER_CODE)
while(True):
    markerLocator.find_location()
