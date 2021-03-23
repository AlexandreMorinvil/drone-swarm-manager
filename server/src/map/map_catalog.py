from DBconnect import DatabaseConnector
class MapCatalog:
    def __init__(self):
        db = DatabaseConnector()

    def get_map_list(self):
        return db.get_all_maps()

    def toJson(self, data):
        return{
            'id': data[0],
            'name': data[1]
        }
