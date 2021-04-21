import sys
import logging
from logging.config import dictConfig

class LogsConfig: 
    def __init__(self):
        self.logging_config = dict(
                version=1,
                disable_existing_loggers=False,
                formatters={
                    'verbose': {
                        'format': ("[%(asctime)s] - %(filename)s - %(levelname)s "
                                "[%(name)s - %(funcName)s - %(lineno)s] %(message)s"),
                        'datefmt': "%d/%b/%Y %H:%M:%S",
                    },
                    'simple': {
                        'format': '%(levelname)s %(message)s',
                    },
                },
                handlers={
                    'file_handler': {'class': 'logging.handlers.RotatingFileHandler',
                                    'formatter': 'verbose',
                                    'level': logging.DEBUG,
                                    'filename': '../logs/server.log',
                                    'maxBytes': 524128800,
                                    'backupCount': 7},
                    
                    'console': {
                        'class': 'logging.StreamHandler',
                        'level': logging.WARNING,
                        'formatter': 'simple',
                        'stream': sys.stdout,
                    },
                },
                root={
                    'handlers': ['file_handler', 'console'],
                    'level': logging.INFO,
                }
        )

    def logger(self, logger_name):
        self.logging_config['loggers'] = {logger_name: {
                    'handlers': ['file_handler'],
                    'level': logging.DEBUG,
                    'propagate': False }
        }
        dictConfig(self.logging_config)
        return logging.getLogger(logger_name)
