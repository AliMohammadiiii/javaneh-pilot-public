#!/usr/bin/python3

from flask import Flask, render_template, url_for, request, jsonify
from flask_sock import Sock
from lib.canmodule import Can 
import json
import time

app = Flask(__name__, static_folder='./static')
sock = Sock(app)
can = Can()


@app.route('/')
def index():
    return render_template('joy.html')

@sock.route('/echo')
def echo(sock):
    while True:
        data = sock.receive()
        sock.send(data)

@app.route('/drive', methods=['POST', 'GET'])
def drive():
    if request.method == 'POST':
        data = request.get_json()
        y = int(int(data['y']) * 2.5)
        if y > 0:
            speed_direction = 1
        else:
            speed_direction = 0 
            y = y * -1 
        
        
        
        print("send speed", y, speed_direction)
        can.sendSpeed(y, speed_direction)
        x = int(data['x'])
        if x > 0:
            ang_direction = 1
        else:
            ang_direction = 0 
            x = x * -1 
        print("send ang", x, ang_direction)
        time.sleep(.050)
        can.sendAngle(x, ang_direction)
        data = {'speed' : y, 'speed_direction' : speed_direction, 'ang' : x, 'ang_direction' : ang_direction}
    return jsonify(data)

if __name__ == "__main__":
        app.run(host='0.0.0.0')
