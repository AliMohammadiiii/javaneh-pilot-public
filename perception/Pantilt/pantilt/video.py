import cv2
class Video:
    def __init__(self):
        self.vid = cv2.VideoCapture(0)

        
    def read(self):
    	if self.vid.isOpened():
        	(self.ret, self.frame) = self.vid.read()
        
    def show(self):
        cv2.imshow('frame', self.frame)
        
    def drawPoint(self, x, y, color):
        for i in range(3):
            for j in range(3):
                self.frame[x+i,y+j] = color
                
    def release(self):
        self.vid.release()
        # Destroy all the windows
        cv2.destroyAllWindows()


