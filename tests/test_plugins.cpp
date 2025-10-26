#include <gtest/gtest.h>
#include "ExpressionParser.h"
#include "PluginManager.h"
#include <cmath>

using namespace calculator;

class PluginTest : public ::testing::Test {
protected:
    void SetUp() override {
        pluginMgr = std::make_unique<PluginManager>();
        pluginMgr->load("./plugins");
        parser = std::make_unique<ExpressionParser>(pluginMgr.get());
    }

    std::unique_ptr<PluginManager> pluginMgr;
    std::unique_ptr<ExpressionParser> parser;
};

TEST_F(PluginTest, SinFunction) {
    EXPECT_NEAR(parser->parse("sin(0)"), 0.0, 0.01);
    EXPECT_NEAR(parser->parse("sin(90)"), 1.0, 0.01);
    EXPECT_NEAR(parser->parse("sin(30)"), 0.5, 0.01);
}

TEST_F(PluginTest, CosFunction) {
    EXPECT_NEAR(parser->parse("cos(0)"), 1.0, 0.01);
    EXPECT_NEAR(parser->parse("cos(90)"), 0.0, 0.01);
    EXPECT_NEAR(parser->parse("cos(60)"), 0.5, 0.01);
}

TEST_F(PluginTest, SqrtFunction) {
    EXPECT_DOUBLE_EQ(parser->parse("sqrt(16)"), 4.0);
    EXPECT_DOUBLE_EQ(parser->parse("sqrt(25)"), 5.0);
    EXPECT_DOUBLE_EQ(parser->parse("sqrt(0)"), 0.0);
}

TEST_F(PluginTest, SqrtNegative) {
    EXPECT_THROW(parser->parse("sqrt(-4)"), std::runtime_error);
}

TEST_F(PluginTest, LnFunction) {
    EXPECT_NEAR(parser->parse("ln(2.718281828)"), 1.0, 0.01);
    EXPECT_NEAR(parser->parse("ln(1)"), 0.0, 0.01);
}

TEST_F(PluginTest, LnNegative) {
    EXPECT_THROW(parser->parse("ln(-100)"), std::runtime_error);
    EXPECT_THROW(parser->parse("ln(0)"), std::runtime_error);
}

TEST_F(PluginTest, AbsFunction) {
    EXPECT_DOUBLE_EQ(parser->parse("abs(-5)"), 5.0);
    EXPECT_DOUBLE_EQ(parser->parse("abs(5)"), 5.0);
    EXPECT_DOUBLE_EQ(parser->parse("abs(0)"), 0.0);
}

TEST_F(PluginTest, ExpFunction) {
    EXPECT_NEAR(parser->parse("exp(0)"), 1.0, 0.01);
    EXPECT_NEAR(parser->parse("exp(1)"), 2.718281828, 0.01);
}

TEST_F(PluginTest, CombinedFunctions) {
    EXPECT_NEAR(parser->parse("sqrt(16) + abs(-5)"), 9.0, 0.01);
    EXPECT_NEAR(parser->parse("sin(90) + cos(0)"), 2.0, 0.01);
}

TEST_F(PluginTest, FunctionInExpression) {
    EXPECT_NEAR(parser->parse("2 * sqrt(4) + 3"), 7.0, 0.01);
    EXPECT_NEAR(parser->parse("10 + sin(30)"), 10.5, 0.01);
}

TEST_F(PluginTest, UnknownFunction) {
    EXPECT_THROW(parser->parse("unknown(10)"), std::runtime_error);
}
