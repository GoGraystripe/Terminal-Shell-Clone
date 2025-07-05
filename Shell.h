#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#ifndef SHELL_H
#define SHELL_H
#endif

class Shell{
public:
    void run();

private:
    std::vector<std::string> splitInput(const std::string& input);
    void freeCommands(std::vector<char*>& commands);
};





