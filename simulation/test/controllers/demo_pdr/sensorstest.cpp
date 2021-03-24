#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controllers/demo_pdr/sensors.cpp"


TEST(sensorstest, freeSide_basic_test_left) {
    /*
             1
             |
             |
    (L)5----(X)----2(R)
             |
             |
             3
    */
    float sensorValues[4] = {5.0, 3.0, 2.0, 1.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kLeft);
}

TEST(sensorstest, freeSide_basic_test_back) {
    /*
             1
             |
             |
    (L)2----(X)----2(R)
             |
             |
             3
    */
    float sensorValues[4] = {2.0, 3.0, 2.0, 1.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kBack);
}

TEST(sensorstest, freeSide_basic_test_right) {
    /*
             1
             |
             |
    (L)3----(X)----4(R)
             |
             |
             3
    */
    float sensorValues[4] = {3.0, 3.0, 4.0, 1.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kRight);
}

TEST(sensorstest, freeSide_basic_test_front) {
    /*
             9
             |
             |
    (L)3----(X)----4(R)
             |
             |
             3
    */
    float sensorValues[4] = {3.0, 3.0, 4.0, 9.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kFront);
}

TEST(sensorstest, freeSide_test_front_free) {
    /*
            -2
             |
             |
    (L)3----(X)----4(R)
             |
             |
             3
    */
    float sensorValues[4] = {3.0, 3.0, 4.0, -2.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kFront);
}

TEST(sensorstest, freeSide_test_front_and_back_free) {
    /*
            -2
             |
             |
    (L)3----(X)----4(R)
             |
             |
            -2
    */
    float sensorValues[4] = {3.0, -2.0, 4.0, -2.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kFront);
}

TEST(sensorstest, CriticalProximity_basic_test_front) {
    /*
             1
             |
             |
    (L)5----(X)----2(R)
             |
             |
             3
    */
    float sensorValues[4] = {5.0, 3.0, 2.0, 1.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.CriticalProximity(sensorValues), SensorSide::kFront);
}

TEST(sensorstest, CriticalProximity_basic_test_front2) {
    /*
             1
             |
             |
    (L)2----(X)----2(R)
             |
             |
             3
    */
    float sensorValues[4] = {2.0, 3.0, 2.0, 1.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.CriticalProximity(sensorValues), SensorSide::kFront);
}


TEST(sensorstest, CriticalProximity_basic_test_left) {
    /*
             6
             |
             |
    (L)3----(X)----4(R)
             |
             |
             4
    */
    float sensorValues[4] = {3.0, 4.0, 4.0, 6.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.CriticalProximity(sensorValues), SensorSide::kLeft);
}

TEST(sensorstest, CriticalProximity_basic_test_back) {
    /*
             9
             |
             |
    (L)4----(X)----4(R)
             |
             |
             3
    */
    float sensorValues[4] = {4.0, 3.0, 4.0, 9.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.CriticalProximity(sensorValues), SensorSide::kBack);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
