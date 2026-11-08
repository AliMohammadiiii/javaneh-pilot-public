from lib.general import can_send_check, now
initial_deg_x = 90
initial_deg_y = 90

def get_middle(position):
    x = (position[0][0] + position[1][0] + position[2][0] + position[3][0]) / 4
    y = (position[0][1] + position[1][1] + position[2][1] + position[3][1]) / 4
    return x, y


class PanTilt:
    def __init__(self, initialDegX, initialDegY):
        self.degX =  initialDegX
        self.degY = initialDegY
        self.middleVideoX = 320
        self.middleVideoY = 240
        self.ldx = 0
        self.ldy = 0
        self.detectTime = 0
        
    def finder(self):
        if can_send_check(300,self.detectTime):
            self.detectTime = now()
            self.ldy = initial_deg_y
            self.ldx = initial_deg_x
            self.degX = initial_deg_x
            self.degY = initial_deg_y
            return self.degX, self.degY
        return -1, -1

    def checkCanSend(self):
        return True
        
    def calculateDegree(self, x, y):
        if x is not None:        
            deg_x = self.setMotorDeg(int(x-self.middleVideoX))
            deg_y = self.setMotorDeg(int(y-self.middleVideoY))
            if deg_x!=0 or deg_y!=0:
                    self.ldy=deg_y
                    self.degX -=deg_x
                    self.degY -=deg_y
            return self.degX, self.degY
            
        else:
            return self.finder()
            
    def setMotorDeg(self, distance):
        diffrence = distance // 10
        #print("distance: ", distance, "diffrence: ", diffrence)
        if distance > 0:
            modDistance = distance % 10
            if modDistance > 5:
               diffrence +=  1
        else:
            modDistance = abs(distance) % 10
            if modDistance < 5:
                diffrence += 1
        return diffrence

