#include "TaskNode.h"
#include "Common.h"

TaskNode::~TaskNode() {
    future_.wait();
}

void TaskNode::main() {
    for (auto lastNode : last_) {
        lastNode->wait();
    }

    if (!isStarted_) {
        isStarted_ = true;
        future_ = reallyAsync(taskFunc_);
    }
    for (auto nextNode : next_) {
        nextNode->main();
    }
    wait();
}

void TaskNode::wait() {
    if (!isFinished_) {
        future_.wait();
        isFinished_ = true;
    }
}