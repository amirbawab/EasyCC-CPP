#include "../../../lexical/graph/State.h"
#include "../../../gtest-1.6.0/include/gtest/gtest.h"

TEST(StateTest, constructor_1) {
    ecc::State stateBckt(101, "type", "name", true);
    ecc::State stateNoBckt(101, "type", "name", false);
    EXPECT_EQ(stateBckt.getId(),101);
    EXPECT_EQ(stateBckt.getType(),"type");
    EXPECT_EQ(stateBckt.getTokenName(),"name");
    EXPECT_TRUE(stateBckt.mustBacktrack());
    EXPECT_FALSE(stateNoBckt.mustBacktrack());
}