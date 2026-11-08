import sqlite3


class TiroDB:
    connection = None
    cursor = None

    def __init__(self, address):
        self.connection = sqlite3.connect(address, check_same_thread=False)
        self.cursor = self.connection.cursor()

    def close(self):
        self.connection.close()

    def execute(self):
        pass

    def createTables(self):
        self.createMissionTable()
        self.createItterationTable()
        self.createCarDataTable()
        self.connection.commit()
        pass

    def createMissionTable(self):
        table = """
            CREATE TABLE IF NOT EXISTS mission(
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            name VARCHAR(32)
        );"""
        self.cursor.execute(table)

    def createItterationTable(self):
        table = """
            CREATE TABLE IF NOT EXISTS iteration(
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            iteration_id INTEGER NOT NULL,
            mission INTEGER NOT NULL
        );"""
        self.cursor.execute(table)

    def createCarDataTable(self):
        table = """ CREATE TABLE IF NOT EXISTS point (
             id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
             mission INTEGER NOT NULL,
             iteration INTEGER NOT NULL,
             lat INTEGER NOT NULL,
             lon INTEGER NOT NULL,
             safe_mode INTEGER NOT NULL,
             turn_in_place INTEGER NOT NULL,
             speed INTEGER NOT NULL,
             speed_direction INTEGER NOT NULL,
             angle INTEGER NOT NULL,
             steering_angle INTEGER NOT NULL,
             steering_angle_direction INTEGER NOT NULL,
             is_last INTEGER NOT NULL,
             start_lat INTEGER,
             start_lon INTEGER,
             end_lat INTEGER,
             end_lon INTEGER
         ); """
        self.cursor.execute(table)

    def read(self, cmd):
        self.cursor.execute(cmd)
        return self.cursor.fetchall()

    def write(self, cmd, data):
        self.cursor.execute(cmd, data)
        self.connection.commit()

    def addMission(self, name):
        self.write("INSERT INTO mission (name) VALUES (?)", [name])
        return self.cursor.lastrowid

    def readMissions(self):
        command = "SELECT * FROM mission"
        rows = self.read(command)
        pass

    def readMissionById(self, id):
        command = "SELECT * FROM mission WHERE id =" + str(id)
        rows = self.read(command)
        if len(rows) > 0:
            return rows[0]

    def readMissionByName(self, name):
        command = 'SELECT * FROM mission WHERE name = "%s"' % (name)
        rows = self.read(command)
        if len(rows) > 0:
            return rows[0]
        return None

    def addIteration(self, mission):
        command = (
            'SELECT iteration_id FROM iteration WHERE mission ="'
            + str(mission)
            + '" order by id desc'
        )
        rows = self.read(command)
        if len(rows) > 0:
            iteration_id = rows[0][0]
        else:
            iteration_id = 0
        self.write(
            "INSERT INTO iteration (mission, iteration_id) VALUES (?, ?)",
            [mission, iteration_id + 1],
        )
        return self.cursor.lastrowid
        pass

    def readIterations(self, mission = None, iteration_id=None, order='asc'):
        command = 'SELECT iteration_id FROM iteration WHERE'
        if mission:
            command += ' mission ="' + str(mission)
        if iteration_id:
            command += ' iteration_id = "' + str(iteration_id)
        command += '" order by id %s' % (order)
        rows = self.read(command)
        return rows

    def addCarData(self, data):
        self.write(
            "INSERT INTO point (mission, iteration, lat, lon, safe_mode, turn_in_place,speed, speed_direction, angle, steering_angle, steering_angle_direction, is_last) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            [
                data["mission"],
                data["iteration"],
                data["lat"],
                data["lon"],
                data["safe_mode"],
                data["turn_in_place"],
                data["speed"],
                data["speed_direction"],
                data["angle"],
                data["steering_angle"],
                data["steering_angle_direction"],
                data["is_last"]
            ],
        )
        return self.cursor.lastrowid

    def readCarData(self, mission = None, iteration=None, order="asc"):
        command = 'SELECT * FROM point'
        if iteration or mission:
            command += ' WHERE'
            if iteration:
                command += ' iteration = "' + str(iteration) + '"'
            if mission:
                command += ' mission = "' + str(mission) + '"'

        command += ' order by id %s' % (order)
        rows = self.read(command)
        return rows

    def updateCarLastData(self, data_id):
        cmd = "UPDATE point SET is_last = 1 where id = %s" % (data_id)
        self.cursor.execute(cmd)
        self.connection.commit()

    def updateCarLine(self, start_lat, start_lon, end_lat, end_lon, data_id):
        cmd = "UPDATE point SET start_lat = %d, start_lon = %d, end_lat = %d, end_lon = %d where id = %s" % (start_lat, start_lon, end_lat, end_lon, data_id)
        self.cursor.execute(cmd)
        self.connection.commit()
