#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controllers/demo_pdr/sensors.cpp"


TEST(sensorstest, freeSide_basic_test_back) {
    /*
               50
               |
               |
    (L)500----(X)----200(R)
               |
               |
              300
    */
    float sensorValues[4] = {500.0, 300.0, 200.0, 50.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kBack);
}

TEST(sensorstest, freeSide_two_sides_blocked) {
    /*
               15
               |
               |
    (L)200----(X)----20(R)
               |
               |
              300
    */
    float sensorValues[4] = {200.0, 300.0, 20.0, 15.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kBack);
}

TEST(sensorstest, freeSide_basic_test_right) {
    /*
              50
              |
              |
    (L)65----(X)----400(R)
              |
              |
              60
    */
    float sensorValues[4] = {65.0, 60.0, 400.0, 50.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kRight);
}

TEST(sensorstest, freeSide_basic_test_front) {
    /*
             300
              |
              |
    (L)30----(X)----40(R)
              |
              |
             30
    */
    float sensorValues[4] = {30.0, 30.0, 40.0, 300.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kFront);
}

TEST(sensorstest, freeSide_test_front_free) {
    /*
              -2
               |
               |
    (L)300----(X)----400(R)
               |
               |
              300
    */
    float sensorValues[4] = {300.0, 300.0, 400.0, -2.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.FreeSide(sensorValues), SensorSide::kDefault);
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

TEST(sensorstest, ReturningSide_basic_test_front_right) {
    /*
             200
              |    (O)
              |
    (L)40----(X)----40(R)
              |
              |
             200
    */
    float sensorValues[4] = {40.0, 200.0, 40.0, 200.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, 1.50), SensorSide::kFront);
}

TEST(sensorstest, ReturningSide_basic_test_front_left) {
    /*
             200
        (O)   |    
              |
    (L)40----(X)----40(R)
              |
              |
             200
    */
    float sensorValues[4] = {40.0, 200.0, 40.0, 200.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, 3), SensorSide::kFront);
}

TEST(sensorstest, ReturningSide_basic_test_back_left) {
    /*
             200
              |    
              |
    (L)40----(X)----40(R)
              |
        (O)   |
             200
    */
    float sensorValues[4] = {40.0, 200.0, 40.0, 200.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, -3), SensorSide::kBack);
}

TEST(sensorstest, ReturningSide_basic_test_back_right) {
    /*
             200
              |    
              |
    (L)40----(X)----40(R)
              |
              |   (O)
             200
    */
    float sensorValues[4] = {40.0, 200.0, 40.0, 200.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, -1.5), SensorSide::kBack);
}

/********************************************/

TEST(sensorstest, ReturningSide_continuous_sides_test_front_right) {
    /*
             40
              |    (O)
              |
    (L)200---(X)----40(R)
              |
              |
             200
    */
    float sensorValues[4] = {200.0, 200.0, 40.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, 1.50), SensorSide::kLeft);
}

TEST(sensorstest, ReturningSide_continuous_sides_test_front_left) {
    /*
             40
        (O)   |    
              |
    (L)200---(X)----40(R)
              |
              |
             200
    */
    float sensorValues[4] = {200.0, 200.0, 40.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, 3), SensorSide::kLeft);
}

TEST(sensorstest, ReturningSide_continuous_sides_test_back_left) {
    /*
             40
              |    
              |
    (L)200---(X)----40(R)
              |
        (O)   |
             200
    */
    float sensorValues[4] = {200.0, 200.0, 40.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, -3), SensorSide::kLeft);
}

TEST(sensorstest, ReturningSide_continuous_sides_test_back_right) {
    /*
             40
              |    
              |
    (L)200---(X)----40(R)
              |
              |   (O)
             200
    */
    float sensorValues[4] = {200.0, 200.0, 40.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, -1.5), SensorSide::kBack);
}

/********************************************/

TEST(sensorstest, ReturningSide_one_free_side_test_front_right) {
    /*
             40
              |    (O)
              |
    (L)200---(X)----200(R)
              |
              |
             200
    */
    float sensorValues[4] = {200.0, 200.0, 200.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, 1.50), SensorSide::kRight);
}

TEST(sensorstest, ReturningSide_one_free_side_test_front_left) {
    /*
             40
        (O)   |    
              |
    (L)200---(X)----200(R)
              |
              |
             200
    */
    float sensorValues[4] = {200.0, 200.0, 200.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, 3), SensorSide::kLeft);
}

TEST(sensorstest, ReturningSide_one_free_side_test_back_left) {
    /*
             40
              |    
              |
    (L)200---(X)----200(R)
              |
        (O)   |
             200
    */
    float sensorValues[4] = {200.0, 200.0, 200.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, -3), SensorSide::kLeft);
}

TEST(sensorstest, ReturningSide_one_free_side_test_back_right) {
    /*
             40
              |    
              |
    (L)200---(X)----200(R)
              |
              |   (O)
             200
    */
    float sensorValues[4] = {200.0, 200.0, 200.0, 40.0};
    CSensors cSensors;
    EXPECT_EQ(cSensors.ReturningSide(sensorValues, -1.5), SensorSide::kRight);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
