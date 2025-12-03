#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

void getCpuStats(long &idleTime, long &totalTime) {
    std::ifstream cpuFile("/proc/stat");

    if (!cpuFile.is_open()) {
        std::cerr << "Error: Could not open /proc/stat" << std::endl;
        return;
    }

    std::string line;
    std::getline(cpuFile, line);
    std::stringstream ss(line);

    std::string label;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    ss >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    idleTime = idle + iowait;
    totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    cpuFile.close();
}

double getRamUsage() {
    std::ifstream ramFile("/proc/meminfo");
    if (!ramFile.is_open()) {
        std::cerr << "Error: Could not open /proc/meminfo" << std::endl;
        return 0.0;
    }

    std::string line;
    double memTotal = 0.0;
    double memAvailable = 0.0;

    while (std::getline(ramFile, line)) {
        std::stringstream ss(line);
        std::string key;
        long value;
        std::string unit;

        ss >> key >> value >> unit;

        if (key == "MemTotal:") {
            memTotal = value;
        } else if (key == "MemAvailable:") {
            memAvailable = value;
        }

        if (memTotal > 0 && memAvailable > 0) break;
    }

    ramFile.close();

    return ((memTotal - memAvailable) / memTotal) * 100.0;
}

int main() {
    long prevIdle, prevTotal;
    long currIdle, currTotal;

    getCpuStats(prevIdle, prevTotal);

    std::cout << "\033[?25l";

    while (true) {
        sleep(1);

        getCpuStats(currIdle, currTotal);

        long totalDelta = currTotal - prevTotal;
        long idleDelta = currIdle - prevIdle;

        double cpuUsage = static_cast<double>(totalDelta - idleDelta) / totalDelta * 100.0;

        std::cout << "\033[2J\033[1;1H";
        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
        std::cout << "RAM Usage: " << getRamUsage() << "%" << std::endl;

        prevIdle = currIdle;
        prevTotal = currTotal;
    }
}
