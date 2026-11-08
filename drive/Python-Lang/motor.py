MOTOR_LF = 0
MOTOR_RF = 1
MOTOR_LB = 2
MOTOR_RB = 3
class Motor:
    def __init__(self, motorId):
        self.id = motorId
        self.speed = 0
        self.directon = motorId % 2
    def setSpeed(self, distance):
        if distance < 15 or distance > 120:
            self.speed = 0
        else:
            self.speed = distance #* 2
        return [self.speed, self.directon]


