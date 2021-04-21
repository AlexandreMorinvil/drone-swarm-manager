# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./log'))

# Add dependecies
from enum import Enum
from setup_logging import LogsConfig
from subprocess import call

class Mode(Enum):
    REAL = 0
    SIMULATION = 1

class Environment:

    # Singleton attributes
    mode = Mode.SIMULATION.value
    is_environment_initialized = False

    # Logger attribute
    logsConfig = None
    logger = None

    def __init__(self):
        if not Environment.is_environment_initialized:
            # Initialize the logger
            Environment.logsConfig = LogsConfig()
            Environment.logger = self.logsConfig.logger('Environment')

            # Initialize the envrionment
            Environment.set_mode()

            # Indicate the initialization as completed
            Environment.is_environment_initialized = True

    @classmethod
    def set_mode(cls, mode=Mode.SIMULATION):
        cls.mode = mode
        if (cls.mode == Mode.REAL):
            cls.logger.info('Mode realtime activated')
        else:
            cls.logger.info('Mode simulation activated')

    @classmethod
    def is_in_simulation(cls):
       return cls.mode == Mode.SIMULATION.value

    @classmethod
    def launch_simulation(cls, number_drones):
        print("This call exists")
        call(['scripts/start-simulation.sh', '{}'.format(number_drones)])