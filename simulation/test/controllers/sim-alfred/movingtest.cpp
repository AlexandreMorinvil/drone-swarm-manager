#include <gtest/gtest.h>
#include "controllers/sim-alfred/moving.cpp"

TEST(movingtest, basic_case_1) {
    /*      
        (1)
         |
         v


         ^
         |
        (2)

    (1) = currentRobot
    (2) = otherRobot
    */
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(0, 3, 0);
    float speed[3] = {0, -0.5, 0};
    float angle = cMoving.GetAngleToAvoidCollision(packetP2P, *cPos, speed);

    EXPECT_EQ(
        angle,
        0.0);
}

TEST(movingtest, basic_case_2) {
    /*      
               (1)  
               /  
              /   
                

         ^
         |
        (2)

    (1) = currentRobot
    (2) = otherRobot
    */
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(2, 3, 0);
    float speed[3] = {-0.5, -0.5, 0};
    float angle = cMoving.GetAngleToAvoidCollision(packetP2P, *cPos, speed);
    int angleToInt = angle * 100;

    EXPECT_EQ(
        angleToInt,
        -64);
}

TEST(movingtest, basic_case_3) {
    /*      
     (1)         
       \       
        \      
                

         ^
         |
        (2)

    (1) = currentRobot
    (2) = otherRobot
    */
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(-2, 3, 0);
    float speed[3] = {0.5, -0.5, 0};
    float angle = cMoving.GetAngleToAvoidCollision(packetP2P, *cPos, speed);
    int angleToInt = angle * 100;

    EXPECT_EQ(
        angleToInt,
        64);
}

TEST(movingtest, basic_case_4) {
   /*      
        (1)
         ^
         |


         |
         v
        (2)

    (1) = currentRobot
    (2) = otherRobot
    */
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = -50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(-2, 3, 0);
    float speed[3] = {0, 0.5, 0};
    float angle = cMoving.GetAngleToAvoidCollision(packetP2P, *cPos, speed);


    EXPECT_EQ(
        angle,
        NO_COLLISION);
}

TEST(movingtest, basic_case_5) {
    /*        <-----(1)
     
         ^  
         |
        (2)

    (1) = currentRobot
    (2) = otherRobot
    */
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(2, 0, 0);
    float speed[3] = {-0.5, 0, 0};
    float angle = cMoving.GetAngleToAvoidCollision(packetP2P, *cPos, speed);
    int angleToInt = angle * 100;

    EXPECT_EQ(
        angleToInt,
        438);
}

TEST(movingtest, basic_case_6) {
    /*        ----->(1)
     
         ^  
         |
        (2)

    (1) = currentRobot
    (2) = otherRobot
    */
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(2, 0, 0);
    float speed[3] = {0.5, 0, 0};
    float angle = cMoving.GetAngleToAvoidCollision(packetP2P, *cPos, speed);

    EXPECT_EQ(
        angle,
        NO_COLLISION);
}

// Test for distance
TEST(movingtest, basic_case_7) {
    
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 0;
    packetP2P.y = 0;

    // Current robot
    CVector3* cPos = new CVector3(3, 4, 0);
    float distance = cMoving.computeDistanceBetweenPoints(packetP2P, *cPos);

    EXPECT_EQ(
        distance,
        5.0);
}

TEST(movingtest, basic_case_8) {
    
    CMoving cMoving;

    // Other robot
    PacketP2P packetP2P;
    packetP2P.id = 1;
    packetP2P.vx = 0;
    packetP2P.vy = 50;
    packetP2P.x = 1;
    packetP2P.y = -2;

    // Current robot
    CVector3* cPos = new CVector3(-2, 2, 0);
    float distance = cMoving.computeDistanceBetweenPoints(packetP2P, *cPos);

    EXPECT_EQ(
        distance,
        5.0);
}

TEST(movingtest, basic_case_9) {
    
    CMoving cMoving;

    // Other robot
    CVector3* objective = new CVector3(3, -1, 0);

    // Current robot
    CVector3* cPos = new CVector3(2, 0, 0);
    
    float angle = cMoving.computeAngleToFollow(*objective, *cPos);
    int angleToInt = angle * 100;
    EXPECT_EQ(
        angleToInt,
        392);
}

TEST(movingtest, basic_case_10) {
    
    CMoving cMoving;

    // Other robot
    CVector3* objective = new CVector3(3, 2, 0);

    // Current robot
    CVector3* cPos = new CVector3(-2, 1, 0);
    
    float angle = cMoving.computeAngleToFollow(*objective, *cPos);
    int angleToInt = angle * 100;
    EXPECT_EQ(
        angleToInt,
        -137);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
