#include <string>
#include <unordered_map>
#include <mutex>
#include "TcpServer/TcpConnection.h"
#include "TcpServer/TcpServer.h"

class ErrorCode {

public:
    ErrorCode(int x, const std::string& y) : code(x), msg(y) {};
    int code;
    std::string msg;
    static const int SUCCEED = 0;
    static const int FAIL = 0;
};

class UserInfo;
using UserInfoSPtr = std::shared_ptr<UserInfo>;
class UserInfo {

public:
    UserInfo(const std::string& account, const std::string& password, TcpConnectionSPtr conn)
        : account_(account)
        , password_(password)
        , conn_(conn)
    {}

    static UserInfoSPtr create(const std::string& account, const std::string& password, TcpConnectionSPtr conn) {
        return std::make_shared<UserInfo>(account, password, conn);
    }

    const std::string getAccount() const {return account_;}
    const std::string getPassword() const {return password_;}
    const TcpConnectionSPtr getConn() const {return conn_;}

    void setAccount(const std::string& account) {account_ = account;}
    void setPassword(const std::string& password) {password_ = password;}
    void setConn(TcpConnectionSPtr conn) {conn_ = conn;}

private:
    std::string account_;
    std::string password_;
    TcpConnectionSPtr conn_;
};

class ClientManager {

public:
    ClientManager() {};
    ~ClientManager() {};

    ErrorCode registerAccount(const std::string& account, const std::string& password, TcpConnectionSPtr conn) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = accountUserInfoMap_.find(account);

        if (it == accountUserInfoMap_.end()) { // 用户不存在
            accountUserInfoMap_[account] = UserInfo::create(account, password, conn);
            return ErrorCode(ErrorCode::SUCCEED, "注册成功");
        } else { // 用户已经存在
            return ErrorCode(ErrorCode::FAIL, "用户已经存在");
        }
    }

    ErrorCode loginAccount(const std::string& account, const std::string& password, TcpConnectionSPtr conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = accountUserInfoMap_.find(account);
        if (it == accountUserInfoMap_.end()) { // 用户不存在
            return ErrorCode(ErrorCode::FAIL, "用户不存在, 请注册");
        } else { // 用户已经存在
            if (it->second->getPassword() == password) {
                it->second->setConn(conn);
                return ErrorCode(ErrorCode::SUCCEED, "登录成功!");
            } else {
                return ErrorCode(ErrorCode::FAIL, "密码错误, 登录失败!");
            }
        }
    }

    // 根据value找key, 或许需要设计为两个map, 暂行
    const std::string getAccountByConn(TcpConnectionSPtr conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it : accountUserInfoMap_) {
            if (it.second->getConn() == conn) {
                return it.second->getAccount();
            }
        }
        return "未知";
    }

private:
    std::mutex mutex_;
    std::unordered_map<std::string, UserInfoSPtr> accountUserInfoMap_;
};

class ChatServer {
public:
    ChatServer(std::string ip, uint16_t port) : tcpServer_(ip, port)
    {
        tcpServer_.setSubLoopsNum(5);
        tcpServer_.setMessageCallback(
        [this](TcpConnectionSPtr conn, const std::string& msg) {
            onMessage(conn, msg);
        });
    }

    ~ChatServer() = default;

    void start() {
        tcpServer_.start();
    }

private:
    void onMessage(TcpConnectionSPtr conn, const std::string& msg) {
        LOG_INFO("%s:%s", conn->getName().c_str(), msg.c_str());

        // 命令会以 "cmd|args" 的形式输入
        auto index = msg.find_first_of("|");
        if (index < msg.size()) {
            std::string cmd = msg.substr(0, index);
            std::string args = msg.substr(index+1, msg.size());
            processCommand(conn, cmd, args);
        } else {
            std::string name = clientManager_.getAccountByConn(conn);
            tcpServer_.foreachConnection(
            [&name, &msg, conn](TcpConnectionSPtr other) {
                if (conn == other) return; // 过滤自己发送的消息
                other->send(name + ":" + msg);
            });
        }
    }

    void processCommand(TcpConnectionSPtr conn, const std::string& cmd, const std::string& args) {
        LOG_INFO("cmd:%s args:%s", cmd.c_str(), args.c_str());

        if (cmd == "登录" || cmd == "注册") {
            // 登录命令的形式为 "登录|用户名|密码"
            // 注册命令的形式为 "注册|用户名|密码"
            auto parseAccountPassword = [&args](std::string& account, std::string& password) {
                auto index = args.find_first_of("|");
                if (index < args.size()) {
                    account = args.substr(0, index);
                    password = args.substr(index+1, args.size());
                    return ErrorCode(ErrorCode::SUCCEED, "解析成功");
                } else {
                    return ErrorCode(ErrorCode::FAIL, "解析失败");
                }
            };
            std::string account;
            std::string password;
            if (parseAccountPassword(account, password).code == ErrorCode::SUCCEED) {
                if (cmd == "登录") {
                    conn->send(clientManager_.loginAccount(account, password, conn).msg);
                } else if (cmd == "注册") {
                    conn->send(clientManager_.registerAccount(account, password, conn).msg);
                }
            } else {
                conn->send("用户名密码输入不完整");
            }
        } else {
            conn->send("消息内含有|会被认为是一条命令, 您输入了错误的命令");
        }
    }

    TcpServer tcpServer_;
    ClientManager clientManager_;
};

int main() {
    ChatServer server("0.0.0.0", 51121);
    server.start();
}