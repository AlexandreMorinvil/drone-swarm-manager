import sqlite3

class DatabaseConnector:
    def __init__(self):   
        self._conn = sqlite3.connect('map.db')
        self._c = self._conn.cursor()

    def createTable(self):
        self._c.execute('''CREATE TABLE IF NOT EXISTS map 
             (mapid integer, name text)''')
        self._c.execute('''CREATE TABLE IF NOT EXISTS listOfPoints 
             (mapid integer, x real, y real, 
            FOREIGN KEY (mapid)
            REFERENCES drones (mapid) 
             )''')
        self._conn.commit()

    def addMap(self, mapid, name):
        self._c.execute(''' INSERT INTO map VALUES(?,?)''', (mapid, name,))
        self._conn.commit()
        #print(self._c.fetchall())
    
    def deleteMap(self, mapid):
        self._c.execute('''DELETE FROM listOfPoints
                            WHERE mapid = ?''',(mapid,))
        self._c.execute('''DELETE FROM map
                            WHERE mapid = ?''',(mapid,))
        self._conn.commit()
    
    def updateMap(self, mapId, data):
        for point in data:
            self._c.execute(''' INSERT INTO listOfPoints VALUES(?,?,?)''', (mapId, point.x,point.y,))

        self._conn.commit()

    def showContentMap(self, mapid):
       self._c.execute('''SELECT * FROM listOfPoints WHERE mapid = ?''',(mapid,))
       print(self._c.fetchall()) 

    def getMap(self, mapid):
        self._c.execute('''SELECT x,y FROM listOfPoints WHERE mapid = ?''',(mapid,))
        dataMap = self._c.fetchall()
        return dataMap
        


        