import pandas as pd
import json  


FOLLOWERCSVPATH = './Follower.csv'
CANID = 'canId'
FOLLOWERID = 'Follower'
MESSAGENUMBER = 'Message Number'
TYPE = 'Type'
SENSOR = 'Sensor'
ACTUATOR = 'Actuator'
PATH = 'Signal Name'
SIGNALCSVPATH = './signal.json'





data = pd.read_csv(FOLLOWERCSVPATH)
data[CANID] = data[FOLLOWERID] * 32 + data[MESSAGENUMBER]

data = data.drop(columns=  FOLLOWERID)
data = data.drop(columns = MESSAGENUMBER)

sensor = data[data[TYPE] == SENSOR]
sensor.drop(columns = TYPE)
Actuator = data[data[TYPE] == ACTUATOR]
Actuator.drop(columns = TYPE)

json_str_sensor = sensor.to_json(orient='records')
json_str_sensor_path = set(sensor[PATH])
json_str_sensor_canID = set(sensor[CANID])
json_str_Actuator = Actuator.to_json(orient='records')
json_str_Actuator_path = set(Actuator[PATH])
json_str_Actuator_canID = set(Actuator[CANID])
json_str_sensor_value = {}

sensor = sensor.set_index(PATH)
Actuator = Actuator.set_index(PATH)

json_str_Actuator_value = {}

for i in range(len(sensor.index)):
    tmp_df = sensor.iloc[[i]]
    tmp_str = tmp_df.to_dict(orient='index')
    json_str_sensor_value.update(tmp_str)


for i in range(len(Actuator.index)):
    tmp_df = Actuator.iloc[[i]]
    tmp_str = tmp_df.to_dict(orient='index')
    json_str_Actuator_value.update(tmp_str)

signal = {
    "sensor": {
        "canIds": list(json_str_sensor_canID),
        "path": list(json_str_sensor_path),
        **json_str_sensor_value
        
    },
    "actuator": {
        "canIds": list(json_str_Actuator_canID),
        "path": list(json_str_Actuator_path),
        **json_str_Actuator_value
    }
}

with open(SIGNALCSVPATH, 'w') as fp:
    json.dump(signal, fp, indent = 4)
