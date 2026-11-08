import sys
sys.path.append('./build/')
from lib.py.plot import TiroPlot, TiroPlotPoint
from lib.py.database import TiroDB
import grpc
import javaneh_pb2
import javaneh_pb2_grpc
# import javaneh_resources



import time
import json
import redis
import numpy as np
tirodb = TiroDB("build/test1.sqlite3")
# r = redis.Redis()
# p = r.pubsub(ignore_subscribe_messages=True)
# p.subscribe('movingData')

point = TiroPlotPoint()
points, area = point.loadFromDB(tirodb)

plot = TiroPlot(xmin=area[0] - 100, xmax=area[1] + 100, ymin=area[2] - 100, ymax=area[3] + 100)
plot.draw(points)
# moveDataPoint = TiroPlotPoint('blue')
# carDataPoint = TiroPlotPoint('green')
currntPoint = TiroPlotPoint('blue', temporary=True)
selectedPoint = TiroPlotPoint('green', temporary=True)
counter = 0
lat = 0
lon = 0
while(True):
    with grpc.insecure_channel("localhost:50053") as channel:
        stub = javaneh_pb2_grpc.VehicleStub(channel)
        p = javaneh_pb2.Point()
        feature = stub.Info(p)
        if feature.lat != lat or feature.lon != lon:
            print(feature.lat, feature.lon)
            currntPoint.point = (feature.lon, feature.lat)
            plot.add(currntPoint)
            plot.render()
            lat = feature.lat
            lon = feature.lon
    
    time.sleep(.01)
    

time.sleep(100)
    
    
