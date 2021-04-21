#!/bin/bash 
SCRIPT=$(readlink -f "$0") # Absolute path to this script
SCRIPTPATH=$(dirname "$SCRIPT") # absolute path to the dirrectory
RobotRelPath='../../../robot'


# start from here
cd $SCRIPTPATH
cd ../crazyflie-lib-python
source venv/bin/activate

cd $SCRIPTPATH
# copy files

# navigate to robot src
cd $RobotRelPath
sed -i -e "s/250K\/.*/250K\/\"$1\"/g" tools/make/config.mk
make

if [ $? -eq 0 ]
then
    make cload
    exit 0
else
    exit 1
fi

