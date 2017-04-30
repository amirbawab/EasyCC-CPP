#include <easycc/Grammar.h>
#include <gtest/gtest.h>

TEST(GrammarTest, GrammarTest_Multiple_Delimeter_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test1.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Continue_Line_No_LHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test2.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Definition_Line_No_LHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test3.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_LHS_Must_Be_Upper_Case_Only_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test4.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Wrong_Continue_Line_LHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test5.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Production_Empty_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test6.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Terminal_Alone_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test7.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Epsilon_Alone_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test8.txt"), std::runtime_error);
}

TEST(GrammarTest, GrammarTest_Empty_RHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar_test9.txt"), std::runtime_error);
}
