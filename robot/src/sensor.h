#define CRITICAL_VALUE 70.0f
#define PI_VALUE 3.14
#define PI_DIVIDE_TWO 1.52
#define PI_DIVIDE_FOUR 0.8
#define SPEED_SRAIGHT 0.2
#define SPEED_SIDE 0.3
#define MAGIC 1.84

#include<stdio.h>
#include <fcntl.h>

typedef enum SensorSide { 
    kLeft, kBack, kRight, kFront, kDefault
    } SensorSide;

typedef struct Sensor {
    float m_pcDistance; //Must be changed to CCI_CrazyflieDistanceScannerSensor
    float leftDist, backDist, frontDist, rightDist;
} Sensor;

enum SensorSide FreeSide(Sensor *sensor, float sensorValues[4]);
enum SensorSide ReturningSide(Sensor *sensor, float sensorValues[4], float angle);
enum SensorSide CriticalProximity(Sensor *sensor, float sensorValues[4]);

