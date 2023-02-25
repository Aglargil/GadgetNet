#include <gtest/gtest.h>
#include "Logger.h"

TEST(TestLogger, format) {
    LOG_INFO("format %d %s %f", 1, "hello", 2.f);
}

TEST(TestLogger, loglevel) {
    LOG_INFO("info");
    LOG_ERROR("error");
    LOG_FATAL("fatal");
    LOG_DEBUG("debug");
    // Logger a();
    // Logger b = a;
}


int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

