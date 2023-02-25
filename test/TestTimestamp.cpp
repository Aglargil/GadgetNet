#include <gtest/gtest.h>
#include "Timestamp.h"

// Timestamp::now()能否正常输出
TEST(TestTimestamp, NowTime) {
    std::cout << Timestamp::now().toString() << std::endl;
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

