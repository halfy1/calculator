#include <gtest/gtest.h>
#include "ExpressionParser.h"
#include "PluginManager.h"

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        pluginMgr = std::make_unique<PluginManager>();
        pluginMgr->load("./plugins");
        parser = std::make_unique<ExpressionParser>(pluginMgr.get());
    }

    std::unique_ptr<PluginManager> pluginMgr;
    std::unique_ptr<ExpressionParser> parser;
};

TEST_F(ParserTest, SimpleAddition) {
    EXPECT_DOUBLE_EQ(parser->parse("10 + 20"), 30.0);
}

TEST_F(ParserTest, SimpleSubtraction) {
    EXPECT_DOUBLE_EQ(parser->parse("20 - 5"), 15.0);
}

TEST_F(ParserTest, SimpleMultiplication) {
    EXPECT_DOUBLE_EQ(parser->parse("5 * 4"), 20.0);
}

TEST_F(ParserTest, SimpleDivision) {
    EXPECT_DOUBLE_EQ(parser->parse("20 / 4"), 5.0);
}

TEST_F(ParserTest, Power) {
    EXPECT_DOUBLE_EQ(parser->parse("2 ^ 3"), 8.0);
}

TEST_F(ParserTest, Parentheses) {
    EXPECT_DOUBLE_EQ(parser->parse("(10 + 20) * 2"), 60.0);
}

TEST_F(ParserTest, ComplexExpression) {
    EXPECT_DOUBLE_EQ(parser->parse("16 + 4 * (3 - 1)"), 24.0);
}

TEST_F(ParserTest, FloatingPoint) {
    EXPECT_NEAR(parser->parse("3.14 + 2.86"), 6.0, 0.01);
}

TEST_F(ParserTest, DivisionByZero) {
    EXPECT_THROW(parser->parse("10 / 0"), std::runtime_error);
}

TEST_F(ParserTest, MismatchedParentheses) {
    EXPECT_THROW(parser->parse("(10 + 20"), std::runtime_error);
    EXPECT_THROW(parser->parse("10 + 20)"), std::runtime_error);
}

TEST_F(ParserTest, EmptyExpression) {
    EXPECT_THROW(parser->parse(""), std::runtime_error);
}

TEST_F(ParserTest, InvalidCharacter) {
    EXPECT_THROW(parser->parse("10 & 20"), std::runtime_error);
}