from lib.canmodule import *
canModule = Can(0x011)
def converRadioMessage(msg):
    ch = int(msg[0])
    speed = int(msg[1])
    direction = int(msg[2])
    if ch == 2:
        print(ch, speed, direction)
        canModule.sendSpeed(speed, direction)
    if ch == 4:
        print(ch, speed, direction)
        deg = round(speed / 255 * 100)
        canModule.sendAngle(deg,direction)
while(True):
    msg = canModule.read()
    command_id = msg.arbitration_id
    #print(command_id)
    if command_id == 17:
        print(msg.data[0],msg.data[1],msg.data[2])
        #converRadioMessage(msg.data)

