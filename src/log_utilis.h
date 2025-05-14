#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>

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

#endif
