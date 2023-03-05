#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include "TaskNode.h"

TEST(TaskTaskNode, main) {
    int headOut;
    int in = 1;
    auto head = std::make_shared<TaskNode>("", 
    [in, &headOut]() {
        std::cout << "head:" << in <<std::endl;
        sleep(1);
        headOut = in;
    });

    auto parallelFunc = [](int headIn, int n, int& pOut) {
        std::cout << "headIn:" << headIn << " t:" << n <<std::endl;
        pOut = headIn;
    };

    int pOut1;
    auto p1 = std::make_shared<TaskNode>("", [&]{parallelFunc(headOut, 1, pOut1);});

    int pOut2;
    auto p2 = std::make_shared<TaskNode>("", [&]{parallelFunc(headOut, 2, pOut2);});
    head >> p1 >> p2;
    head >> p2;
    head->main();
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();   
}

