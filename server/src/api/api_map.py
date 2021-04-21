# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./map'))
sys.path.append(os.path.abspath('./log'))

# Add dependencies
import json
from map_catalog import MapCatalog
from map_handler import MapHandler
from setup_logging import LogsConfig

logsConfig = LogsConfig()
logger = logsConfig.logger('ApiMap')

def api_map_get_map_list():
    map_catalog = MapCatalog()
    maps = map_catalog.get_map_list()
    return json.dumps([map_catalog.map_list_to_Json(map) for map in maps])

def api_map_get_map_points(data):
    id = data['id']
    map_catalog = MapCatalog()
    return map_catalog.get_select_map(id)

def api_map_delete_map(data):
    id = data['id']
    map_catalog = MapCatalog()
    map_catalog.delete_map(id)
    maps = map_catalog.get_map_list()
    return json.dumps([map_catalog.map_list_to_Json(map) for map in maps])
    
    
