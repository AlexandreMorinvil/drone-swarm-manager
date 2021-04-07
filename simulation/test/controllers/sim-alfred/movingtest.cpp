#include <gtest/gtest.h>


/*TEST(movingtest, wegfwe) {
    Real piOverSix = 0.52;
    CMoving cMoving;
    CVector3* actualVector = cMoving.GoInSpecifiedDirection(SensorSide::kFront,
        new CRadians(piOverSix));
    CVector3* expectedVector = new CVector3(0.20, 0.20, 0.0);
    EXPECT_EQ(
        actualVector->GetX(),
        expectedVector->GetX());
}*/

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
