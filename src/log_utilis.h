#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>
#include <vector>

inline void writeLog(const std::string& message) {
    std::ofstream out("log.txt", std::ios::app);
    if (out) {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt)-1] = '\0'; // remove newline
        out << "[" << dt << "] " << message << std::endl;
    }
}

inline void monitorLog(int& linesSeen) {
    std::ifstream log("log.txt");
    std::string line;
    int currentLine = 0;

    while (std::getline(log, line)) {
        if (currentLine >= linesSeen) {
            std::cout << line << std::endl;
        }
        currentLine++;
    }

    linesSeen = currentLine;
}
inline std::vector<std::string> getLastLogLines(int n = 3) {
    std::ifstream log("log.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(log, line)) {
        lines.push_back(line);
    }
    int start = lines.size() > n ? lines.size() - n : 0;
    return std::vector<std::string>(lines.begin() + start, lines.end());
}
#endif
