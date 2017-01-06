#include "../../../lexical/token/LexicalToken.h"
#include "../../../gtest-1.6.0/include/gtest/gtest.h"

TEST(LexicalTokenTest, constructorValues) {
    ecc::LexicalToken lexicalTokenNormal(ecc::LexicalToken::Type::NORMAL_TOKEN,"name", "value", 1, 2, 3);
    ecc::LexicalToken lexicalTokenError(ecc::LexicalToken::Type::ERROR_TOKEN,"name", "value", 1, 2, 3);
    EXPECT_EQ(lexicalTokenNormal.getLine(),1);
    EXPECT_EQ(lexicalTokenNormal.getColumn(),2);
    EXPECT_EQ(lexicalTokenNormal.getPosition(),3);
    EXPECT_EQ(lexicalTokenNormal.getName(),"name");
    EXPECT_EQ(lexicalTokenNormal.getValue(),"value");
    EXPECT_EQ(lexicalTokenNormal.getType(), ecc::LexicalToken::Type::NORMAL_TOKEN);
    EXPECT_EQ(lexicalTokenError.getType(), ecc::LexicalToken::Type::ERROR_TOKEN);
}