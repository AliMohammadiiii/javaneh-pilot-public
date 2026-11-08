import cv2
import numpy as np
ARUCOCODE = cv2.aruco.DICT_4X4_1000
class Marker:
	def __init__(self, marker_code):
		self.dictionary = cv2.aruco.getPredefinedDictionary(ARUCOCODE)
		self.parameters =  cv2.aruco.DetectorParameters()
		self.detector = cv2.aruco.ArucoDetector(self.dictionary, self.parameters)
		self.markerCorners = 0
		self.markerIds = 0
		self.markerCode = marker_code
		self.rejectedCandidates = 0
        
	def getMarkerMiddle(self, frame):
		if self.getMarkerPosition(frame):
			x = (self.position[0][0] + self.position[1][0] + self.position[2][0] + self.position[3][0]) / 4
			y = (self.position[0][1] + self.position[1][1] + self.position[2][1] + self.position[3][1]) / 4
			return int(x), int(y)
		else:
			return None, None
        
	def getMarkerPosition(self, frame):
		self.markerCorners, self.markerIds, self.rejectedCandidates = self.detector.detectMarkers(frame)
		try:
			index = np.where(self.markerIds == self.markerCode)[0][0]
			self.position =  self.markerCorners[index][0]
			return True
		except:
			return False


