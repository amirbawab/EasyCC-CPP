#include <easycc/Grammar.h>
#include <gtest/gtest.h>

TEST(GrammarParsingTest, GrammarParsingTest_Multiple_Delimeter_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test1.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Continue_Line_No_LHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test2.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Definition_Line_No_LHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test3.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_LHS_Must_Be_Upper_Case_Only_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test4.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Wrong_Continue_Line_LHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test5.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Production_Empty_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test6.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Terminal_Alone_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test7.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Epsilon_Alone_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test8.txt"), std::runtime_error);
}

TEST(GrammarParsingTest, GrammarParsingTest_Empty_RHS_Test) {
    ASSERT_THROW(ecc::Grammar("resources/test/grammar/grammar_test9.txt"), std::runtime_error);
}

TEST(GrammarFirstFollowTest, GrammarFirstFollowTest_First_set_1_Test) {
    ecc::Grammar grammar("resources/test/firstfollow/ff_test1.txt");

    std::string A = "A";
    std::string B = "B";
    std::string C = "C";
    std::string D = "D";
    std::string E = "E";
    std::string S = "S";
    std::string ClosePar = "T_CLOSE_PAR";
    std::string Divide = "T_DIVIDE";
    std::string Integer = "T_INTEGER";
    std::string Minus = "T_MINUS";
    std::string Multiply = "T_MULTIPLY";
    std::string OpenPar = "T_OPEN_PAR";
    std::string Plus = "T_PLUS";
    std::string Semicolon = "T_SEMICOLON";

    // Store tokens
    std::vector<std::string> tokens =
            {A,B,C,D,E,S,ClosePar,Divide,Integer,Minus,Multiply,OpenPar,Plus,Semicolon};

    std::set<std::string> FirstA;
    std::set<std::string> FirstB;
    std::set<std::string> FirstC;
    std::set<std::string> FirstD;
    std::set<std::string> FirstE;
    std::set<std::string> FirstS;
    std::set<std::string> FirstClosePar;
    std::set<std::string> FirstDivide;
    std::set<std::string> FirstInteger;
    std::set<std::string> FirstMinus;
    std::set<std::string> FirstMultiply;
    std::set<std::string> FirstOpenPar;
    std::set<std::string> FirstPlus;
    std::set<std::string> FirstSemicolon;

    // First(A)
    FirstA.insert("T_INTEGER");
    FirstA.insert("T_OPEN_PAR");

    // First(B)
    FirstB.insert("T_PLUS");
    FirstB.insert("T_MINUS");
    FirstB.insert("EPSILON");

    // First(C)
    FirstC.insert("T_INTEGER");
    FirstC.insert("T_OPEN_PAR");

    // First(D)
    FirstD.insert("T_MULTIPLY");
    FirstD.insert("T_DIVIDE");
    FirstD.insert("EPSILON");

    // First(E)
    FirstE.insert("T_INTEGER");
    FirstE.insert("T_OPEN_PAR");

    // First(S)
    FirstS.insert("EPSILON");
    FirstS.insert("T_INTEGER");
    FirstS.insert("T_OPEN_PAR");

    // First(T_CLOSE_PAR)
    FirstClosePar.insert("T_CLOSE_PAR");

    // First(T_DIVIDE)
    FirstDivide.insert("T_DIVIDE");

    // First(T_INTEGER)
    FirstInteger.insert("T_INTEGER");

    // First(T_MINUS)
    FirstMinus.insert("T_MINUS");

    // First(T_MULTIPLY)
    FirstMultiply.insert("T_MULTIPLY");

    // First(T_OPEN_PAR)
    FirstOpenPar.insert("T_OPEN_PAR");

    // First(T_PLUS)
    FirstPlus.insert("T_PLUS");

    // First(T_SEMICOLON)
    FirstSemicolon.insert("T_SEMICOLON");

    // Store sets
    std::vector<std::set<std::string>> sets = {FirstA,FirstB,FirstC,FirstD,FirstE,FirstS,FirstClosePar,
            FirstDivide,FirstInteger,FirstMinus,FirstMultiply,FirstOpenPar,FirstPlus,FirstSemicolon};

    for(int i=0; i < sets.size(); i++) {
        for(std::string first : *grammar.getFirstSet(tokens[i])) {
            if(sets[i].find(first) == sets[i].end()) {
                FAIL() << "Token " << first << " not in first set of " << tokens[i];
            }
        }
    }
}

TEST(GrammarFirstFollowTest, GrammarFirstFollowTest_Follow_set_1_Test) {
    ecc::Grammar grammar("resources/test/firstfollow/ff_test1.txt");

    std::string A = "A";
    std::string B = "B";
    std::string C = "C";
    std::string D = "D";
    std::string E = "E";
    std::string S = "S";
    std::string ClosePar = "T_CLOSE_PAR";
    std::string Divide = "T_DIVIDE";
    std::string Integer = "T_INTEGER";
    std::string Minus = "T_MINUS";
    std::string Multiply = "T_MULTIPLY";
    std::string OpenPar = "T_OPEN_PAR";
    std::string Plus = "T_PLUS";
    std::string Semicolon = "T_SEMICOLON";

    // Store tokens
    std::vector<std::string> tokens =
            {A,B,C,D,E,S,ClosePar,Divide,Integer,Minus,Multiply,OpenPar,Plus,Semicolon};

    std::set<std::string> FollowA;
    std::set<std::string> FollowB;
    std::set<std::string> FollowC;
    std::set<std::string> FollowD;
    std::set<std::string> FollowE;
    std::set<std::string> FollowS;
    std::set<std::string> FollowClosePar;
    std::set<std::string> FollowDivide;
    std::set<std::string> FollowInteger;
    std::set<std::string> FollowMinus;
    std::set<std::string> FollowMultiply;
    std::set<std::string> FollowOpenPar;
    std::set<std::string> FollowPlus;
    std::set<std::string> FollowSemicolon;

    // Follow(A)
    FollowA.insert("T_CLOSE_PAR");
    FollowA.insert("T_SEMICOLON");

    // Follow(B)
    FollowB.insert("T_CLOSE_PAR");
    FollowB.insert("T_SEMICOLON");

    // Follow(C)
    FollowC.insert("T_PLUS");
    FollowC.insert("T_MINUS");
    FollowC.insert("T_CLOSE_PAR");
    FollowC.insert("T_SEMICOLON");

    // Follow(D)
    FollowD.insert("T_PLUS");
    FollowD.insert("T_MINUS");
    FollowD.insert("T_CLOSE_PAR");
    FollowD.insert("T_SEMICOLON");

    // Follow(E)
    FollowE.insert("T_MULTIPLY");
    FollowE.insert("T_DIVIDE");
    FollowE.insert("T_PLUS");
    FollowE.insert("T_MINUS");
    FollowE.insert("T_CLOSE_PAR");
    FollowE.insert("T_SEMICOLON");

    // Follow(S)
    FollowS.insert("$");

    // Follow(T_CLOSE_PAR)
    FollowClosePar.insert("T_MULTIPLY");
    FollowClosePar.insert("T_DIVIDE");
    FollowClosePar.insert("T_PLUS");
    FollowClosePar.insert("T_MINUS");
    FollowClosePar.insert("T_CLOSE_PAR");
    FollowClosePar.insert("T_SEMICOLON");

    // Follow(T_DIVIDE)
    FollowDivide.insert("T_OPEN_PAR");
    FollowDivide.insert("T_INTEGER");

    // Follow(T_INTEGER)
    FollowInteger.insert("T_MULTIPLY");
    FollowInteger.insert("T_DIVIDE");
    FollowInteger.insert("T_PLUS");
    FollowInteger.insert("T_MINUS");
    FollowInteger.insert("T_CLOSE_PAR");
    FollowInteger.insert("T_SEMICOLON");

    // Follow(T_MINUS)
    FollowMinus.insert("T_OPEN_PAR");
    FollowMinus.insert("T_INTEGER");

    // Follow(T_MULTIPLY)
    FollowMultiply.insert("T_OPEN_PAR");
    FollowMultiply.insert("T_INTEGER");

    // Follow(T_OPEN_PAR)
    FollowOpenPar.insert("T_OPEN_PAR");
    FollowOpenPar.insert("T_INTEGER");

    // Follow(T_PLUS)
    FollowPlus.insert("T_OPEN_PAR");
    FollowPlus.insert("T_INTEGER");

    // Follow(T_SEMICOLON)
    FollowSemicolon.insert("T_OPEN_PAR");
    FollowSemicolon.insert("T_INTEGER");
    FollowSemicolon.insert("$");

    // Store sets
    std::vector<std::set<std::string>> sets = {FollowA,FollowB,FollowC,FollowD,FollowE,FollowS,
                                               FollowClosePar,FollowDivide,FollowInteger,FollowMinus,
                                               FollowMultiply,FollowOpenPar,FollowPlus,FollowSemicolon};

    for(int i=0; i < sets.size(); i++) {
        for(std::string follow : *grammar.getFollowSet(tokens[i])) {
            if(sets[i].find(follow) == sets[i].end()) {
                FAIL() << "Terminal " << follow << " not in follow set of " << tokens[i];
            }
        }
    }
}
