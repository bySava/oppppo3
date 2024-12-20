#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class Wisdom {
public:
    std::string content;

    Wisdom(const std::string& c) : content(c) {}

    virtual std::string getType() const = 0;
    virtual bool matchesCondition(const std::string& condition) const = 0;

    virtual ~Wisdom() = default;
};

class Aphorism : public Wisdom {
public:
    std::string author;

    Aphorism(const std::string& c, const std::string& a) : Wisdom(c), author(a) {}

    std::string getType() const override {
        return "Aphorism";
    }

    bool matchesCondition(const std::string& condition) const override {
        return content.find(condition) != std::string::npos || author.find(condition) != std::string::npos;
    }
};

class Proverb : public Wisdom {
public:
    std::string country;

    Proverb(const std::string& c, const std::string& co) : Wisdom(c), country(co) {}

    std::string getType() const override {
        return "Proverb";
    }

    bool matchesCondition(const std::string& condition) const override {
        return content.find(condition) != std::string::npos || country.find(condition) != std::string::npos;
    }
};

class WisdomContainer {
private:
    std::vector<Wisdom*> wisdoms;

public:
    ~WisdomContainer() {
        for (auto wisdom : wisdoms) {
            delete wisdom;
        }
    }

    void add(Wisdom* wisdom) {
        wisdoms.push_back(wisdom);
    }

    void remove(const std::string& condition) {
        wisdoms.erase(std::remove_if(wisdoms.begin(), wisdoms.end(),
            [&](Wisdom* w) { return w->matchesCondition(condition); }), wisdoms.end());
    }

    void print() const {
        for (const auto& wisdom : wisdoms) {
            std::cout << wisdom->getType() << ": " << wisdom->content << std::endl;
        }
    }
};

void processCommand(WisdomContainer& container, const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "ADD") {
        std::string type;
        std::string content;
        std::string param;

        iss >> type;
        std::getline(iss, content);
        content.erase(0, content.find_first_not_of(" \t")); // Удаление пробелов
        content = content.substr(1); // Удаление первого пробела

        if (type == "Aphorism") {
            std::getline(iss, param);
            container.add(new Aphorism(content, param));
        }
        else if (type == "Proverb") {
            std::getline(iss, param);
            container.add(new Proverb(content, param));
        }
    }
    else if (cmd == "REM") {
        std::string condition;
        std::getline(iss, condition);
        container.remove(condition);
    }
    else if (cmd == "PRINT") {
        container.print();
    }
}

int main() {
    WisdomContainer container;
    std::string command;

    while (std::getline(std::cin, command)) {
        processCommand(container, command);
    }

    return 0;
}
