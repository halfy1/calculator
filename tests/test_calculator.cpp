#include <gtest/gtest.h>
#include "PluginManager.h"
#include "ExpressionParser.h"

using namespace calculator;

class PluginManagerTest : public ::testing::Test {
protected:
    PluginManager pluginMgr;
};

TEST_F(PluginManagerTest, LoadPlugins) {
    EXPECT_TRUE(pluginMgr.load("./plugins"));
}

TEST_F(PluginManagerTest, HasFunction) {
    pluginMgr.load("./plugins");
    EXPECT_TRUE(pluginMgr.hasFunction("sin"));
    EXPECT_TRUE(pluginMgr.hasFunction("cos"));
    EXPECT_TRUE(pluginMgr.hasFunction("sqrt"));
    EXPECT_TRUE(pluginMgr.hasFunction("ln"));
    EXPECT_FALSE(pluginMgr.hasFunction("unknown"));
}

TEST_F(PluginManagerTest, CallFunction) {
    pluginMgr.load("./plugins");
    EXPECT_NEAR(pluginMgr.call("sqrt", 16.0), 4.0, 0.01);
    EXPECT_NEAR(pluginMgr.call("abs", -5.0), 5.0, 0.01);
}

TEST_F(PluginManagerTest, CallUnknownFunction) {
    pluginMgr.load("./plugins");
    EXPECT_THROW(pluginMgr.call("unknown", 10.0), std::runtime_error);
}

TEST_F(PluginManagerTest, LoadNonExistentDirectory) {
    EXPECT_FALSE(pluginMgr.load("./non_existent"));
}

TEST(IntegrationTest, CompleteCalculation) {
    PluginManager pm;
    pm.load("./plugins");
    ExpressionParser parser(&pm);
    
    EXPECT_DOUBLE_EQ(parser.parse("16 + 4 * (3 - 1)"), 24.0);
    EXPECT_NEAR(parser.parse("2^4 + sin(90)"), 17.0, 0.01);
    EXPECT_NEAR(parser.parse("sqrt(16) + abs(-5)"), 9.0, 0.01);
}