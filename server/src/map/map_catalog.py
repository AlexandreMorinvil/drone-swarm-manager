from DBconnect import DatabaseConnector
import json

class MapCatalog:
    def __init__(self):
        self.db = DatabaseConnector()

    def get_map_list(self):
        return self.db.get_all_maps()

    def map_list_to_Json(self, data):
        return{
            'id': data[0],
            'name': data[1],
            'date': data[2]
        }
    
    def get_select_map(self, id):
        points = self.db.get_map(id)
        return json.dumps([self.map_points_toJson(point) for point in points])

    def map_points_toJson(self,points):
        return{
            'x': points[0],
            'y': points[1],
            'z': points[2]
        }

    def delete_map(self, id):
        self.db.delete_map(id)
    

