import sqlite3

class DatabaseConnector:
    def __init__(self): 
        self._conn = sqlite3.connect('map.db')
        self._c = self._conn.cursor()

    def create_table(self):
        self._c.execute('''CREATE TABLE IF NOT EXISTS map 
             (mapid INTEGER PRIMARY KEY AUTOINCREMENT, name text)''')
        self._c.execute('''CREATE TABLE IF NOT EXISTS listOfPoints 
             (mapid INTEGER, x real, y real, z real,
            FOREIGN KEY(mapid) REFERENCES map (mapid) 
             )''')
        self._conn.commit()
        self._c.execute("SELECT name FROM sqlite_master WHERE type='table';")
        print(self._c.fetchall())

    def delete_all_table(self):
        self._c.execute('''DROP TABLE IF EXISTS map''')
        self._c.execute('''DROP TABLE IF EXISTS listOfPoints''')
        self._conn.commit()
        self._c.execute("SELECT name FROM sqlite_master WHERE type='table';")
        print(self._c.fetchall())

    def add_map(self, name):
        self._c.execute(''' INSERT INTO map(name) VALUES(?)''', (name,))
        self._conn.commit()
        self._c.execute('SELECT * FROM map')
        print(self._c.fetchall())
        return self._c.lastrowid
    
    def delete_map(self, mapid):
        self._c.execute('''DELETE FROM listOfPoints
                            WHERE mapid = ?''',(mapid,))
        self._c.execute('''DELETE FROM map
                            WHERE mapid = ?''',(mapid,))
        self._conn.commit()
    
    def delete_all(self):
        self._c.execute('DELETE * FROM map')
        self._conn.commit()
        return(self._c.fetchall())
    
    def update_map(self, mapId, data):
        for point in data:
            self._c.execute(''' INSERT INTO listOfPoints VALUES(?,?,?,?)''', (mapId, point.x,point.y,point.z))
        self._conn.commit()

    def show_content_map(self, mapid):
       self._c.execute('''SELECT * FROM listOfPoints WHERE mapid = ?''',(mapid,))
       print(self._c.fetchall()) 

    def get_map(self, mapid):
        self._c.execute('''SELECT x,y FROM listOfPoints WHERE mapid = ?''',(mapid,))
        dataMap = self._c.fetchall()
        return dataMap

    def get_all_maps(self):
        self._c.execute('''SELECT * FROM map''')
        return self._c.fetchall()
    





        
        


        