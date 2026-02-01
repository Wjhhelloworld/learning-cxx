#include "../exercise.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
        std::cout << "Recorded '" << record << "' to current resource" << std::endl;
    }

    ~Resource() {
        std::cout << "Resource destructor called, records: \"" << _records << "\"" << std::endl;
        RECORDS.push_back(_records);
    }
};

using Unique = std::unique_ptr<Resource>;
Unique reset(Unique ptr) {
    if (ptr) {
        std::cout << "reset: recording 'r'" << std::endl;
        ptr->record('r');
    } else {
        std::cout << "reset: ptr is null" << std::endl;
    }
    return std::make_unique<Resource>();
}
Unique drop(Unique ptr) {
    if (ptr) {
        std::cout << "drop: recording 'd'" << std::endl;
        ptr->record('d');
    } else {
        std::cout << "drop: ptr is null" << std::endl;
    }
    return nullptr;
}
Unique forward(Unique ptr) {
    if (ptr) {
        std::cout << "forward: recording 'f'" << std::endl;
        ptr->record('f');
    } else {
        std::cout << "forward: ptr is null" << std::endl;
    }
    return ptr;
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3];

    std::cout << "=== Problem 1 ===" << std::endl;
    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);
    std::cout << "Problem 1 records: ";
    for (const auto& r : problems[0]) std::cout << "\"" << r << "\" ";
    std::cout << std::endl;

    std::cout << "\n=== Problem 2 ===" << std::endl;
    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);
    std::cout << "Problem 2 records: ";
    for (const auto& r : problems[1]) std::cout << "\"" << r << "\" ";
    std::cout << std::endl;

    std::cout << "\n=== Problem 3 ===" << std::endl;
    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);
    std::cout << "Problem 3 records: ";
    for (const auto& r : problems[2]) std::cout << "\"" << r << "\" ";
    std::cout << std::endl;

    // ---- 不要修改以上代码 ----

    std::vector<const char *> answers[]{
        {"fd"},  // 问题1
        // TODO: 分析 problems[1] 中资源的生命周期，将记录填入 `std::vector`
        // NOTICE: 此题结果依赖对象析构逻辑，平台相关，提交时以 CI 实际运行平台为准
        {"ffr", "d"},  // 问题2
        {"r", "d", "d"},  // 问题3
    };

    // ---- 不要修改以下代码 ----

    for (auto i = 0; i < 3; ++i) {
        ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (auto j = 0; j < problems[i].size(); ++j) {
            ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    std::cout << "\nAll tests passed!" << std::endl;

    return 0;
}