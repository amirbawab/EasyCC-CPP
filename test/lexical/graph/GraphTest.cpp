#include <easycc/Graph.h>
#include <gtest/gtest.h>

TEST(GraphTest, GraphTest_Multiple_definition_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test1.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Order_ID_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test2.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_One_Initial_State_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test3.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Undefined_Type_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test4.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Undefined_From_State_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test5.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Undefined_To_State_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test6.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Initial_With_Incoming_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test7.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Final_With_Outgoing_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test8.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Multiple_Label_Definition_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test9.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_No_Initial_State_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test10.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Initial_State_With_Other_Transition_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test11.json"), std::runtime_error);
}

TEST(GraphTest, GraphTest_Normal_State_With_Other_Transition_Test) {
    ASSERT_THROW(ecc::Graph::buildGraph("resources/test/graph/graph_test12.json"), std::runtime_error);
}
