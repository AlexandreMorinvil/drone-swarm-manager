#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <controllers/demo_pdr/sensors.h>
#include <argos3/plugins/robots/crazyflie/control_interface/ci_crazyflie_distance_scanner_sensor.h>

/*class CCI_CrazyflieDistanceScannerSensorMock
: public CCI_CrazyflieDistanceScannerSensor {
 public:
    //MOCK_CONST_METHOD1(GetReadingsMap,
    //    const TReadingsMap&(const TReadingsMap& size));
    //MOCK_METHOD(TReadingsMap, GetReadingsMap, (), (const, override));
};

TEST(sensorstest, wegfwe) {
    CCI_CrazyflieDistanceScannerSensor* mock =
        new CCI_CrazyflieDistanceScannerSensor();
    CSensors* cSensors = new CSensors(mock);
    EXPECT_TRUE(true);
}*/



class NewSensors {
 public:
    NewSensors(float left, float back, float right, float front) {
        leftDist = left;
        rightDist = right;
        backDist = back;
        frontDist = front;
    }

    SensorSide FreeSide() {
        float sensor[4]  = {leftDist, backDist, rightDist, frontDist};
        SensorSide closeSens = CriticalProximity();
        if (closeSens == SensorSide::kDefault) return closeSens;
        SensorSide oppSens = (SensorSide) ((closeSens +2) % 4);
        if (sensor[oppSens] == -2 || sensor[oppSens] > 2 * CRITICAL_VALUE )
            return oppSens;
        float max   = sensor[closeSens];
        SensorSide maxSensor = SensorSide::kDefault;
        for (unsigned i = 0; i < 4; i++) {
            if (sensor[i] == -2 ) return (SensorSide) i;
            if (max < sensor[i]) {
                max = sensor[i];
                maxSensor = (SensorSide) i;
            }
        }
        return maxSensor;
    }

    SensorSide CriticalProximity() {
        float sensor[4]  = {leftDist, backDist, rightDist, frontDist};
        float min   = CRITICAL_VALUE;
        SensorSide minSensor = SensorSide::kDefault;

        for (unsigned i = 0; i < 4; i++) {
            if (min > sensor[i] && sensor[i] > 0.0) {
                min = sensor[i];
                minSensor = (SensorSide) i;
            }
        }

        if (((frontDist < 130.0 && frontDist > 0)
                && minSensor == SensorSide::kDefault)
            || (frontDist < min && frontDist > 0)) {
            minSensor = SensorSide::kFront;
        }

        return minSensor;
    }

 private:
    float leftDist, backDist, frontDist, rightDist;
};

TEST(sensorstest, wegfwe) {
    /*
             1
             |
             |
    (L)5----(X)----2(R)
             |
             |
             3
    */
    NewSensors* cSensors = new NewSensors(5.0, 3.0, 2.0, 1.0);
    EXPECT_EQ(cSensors->FreeSide(), SensorSide::kLeft);

    /*
             1
             |
             |
    (L)2----(X)----2(R)
             |
             |
             3
    */
    cSensors = new NewSensors(2.0, 3.0, 2.0, 1.0);
    EXPECT_EQ(cSensors->FreeSide(), SensorSide::kBack);


    /*
             1
             |
             |
    (L)3----(X)----4(R)
             |
             |
             3
    */
    cSensors = new NewSensors(3.0, 3.0, 4.0, 1.0);
    EXPECT_EQ(cSensors->FreeSide(), SensorSide::kRight);

    /*
             9
             |
             |
    (L)3----(X)----4(R)
             |
             |
             3
    */
    cSensors = new NewSensors(3.0, 3.0, 4.0, 9.0);
    EXPECT_EQ(cSensors->FreeSide(), SensorSide::kFront);
}




int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
