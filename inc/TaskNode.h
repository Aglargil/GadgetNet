#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>

class TaskNode;
using TaskNodeSPtr = std::shared_ptr<TaskNode>;

class TaskNode :std::enable_shared_from_this<TaskNode> {

public:
    TaskNode(std::string name, std::function<void()> func) 
        : taskFunc_(func)
        , taskName_(name)
        , isStarted_(false)
        , isFinished_(false)
    {}

    ~TaskNode();
    
    void main();

    void wait();

    const std::vector<TaskNodeSPtr> getNext() const {return next_;}
    const std::string& getName() const {return taskName_;}

    void setNext(TaskNodeSPtr node) {
        next_.push_back(node);
    }

    void setLast(TaskNodeSPtr node) {
        last_.push_back(node);
    }

    void setName(const std::string& name) {taskName_ = name;}


private:
    std::vector<TaskNodeSPtr> last_;
    std::vector<TaskNodeSPtr> next_;
    std::function<void()> taskFunc_;
    std::future<void> future_;
    std::string taskName_;
    bool isStarted_;
    bool isFinished_;
};

inline TaskNodeSPtr operator>>(TaskNodeSPtr left, TaskNodeSPtr right) {
    left->setNext(right);
    right->setLast(left);
    return right;
}
