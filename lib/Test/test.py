from canmodule import *
ID_CAN_PANTILT = 64

canModule = Can()
for i in range(0, 180, 10):
    canModule.sendDeg(i, 180-i, ID_CAN_PANTILT)
