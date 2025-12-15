#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <iomanip>

void getCpuStats(unsigned long long &idleTime, unsigned long long &totalTime) {
    std::ifstream cpuFile("/proc/stat");
    if (!cpuFile.is_open()) {
        std::cerr << "Error: Could not open /proc/stat" << std::endl;
        return;
    }
    std::string line;
    std::getline(cpuFile, line);
    std::stringstream ss(line);
    std::string label;

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    ss >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    idleTime = idle + iowait;
    totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
    cpuFile.close();
}

double getRamUsage() {
    std::ifstream ramFile("/proc/meminfo");
    if (!ramFile.is_open()) return 0.0;

    std::string line;
    double memTotal = 0.0;
    double memAvailable = 0.0;
    while (std::getline(ramFile, line)) {
        std::stringstream ss(line);
        std::string key;
        long value;
        std::string unit;
        ss >> key >> value >> unit;
        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemAvailable:") memAvailable = value;
        if (memTotal > 0 && memAvailable > 0) break;
    }
    ramFile.close();
    return ((memTotal - memAvailable) / memTotal) * 100.0;
}

void getNetworkStats(unsigned long long &totalRx, unsigned long long &totalTx) {
    std::ifstream networkFile("/proc/net/dev");
    if (!networkFile.is_open()) {
        std::cerr << "Error: Could not open /proc/net/dev" << std::endl;
        return;
    }

    std::string line;
    std::getline(networkFile, line);
    std::getline(networkFile, line);

    totalRx = 0;
    totalTx = 0;

    while (std::getline(networkFile, line)) {
        std::stringstream ss(line);
        std::string iface;
        unsigned long long rxBytes, txBytes;
        unsigned long long dummy;

        ss >> iface;

        if (iface != "lo:") {
            ss >> rxBytes;
            for (int i = 0; i < 7; i++) {
                ss >> dummy;
            }
            ss >> txBytes;

            totalRx += rxBytes;
            totalTx += txBytes;
        }
    }
    networkFile.close();
}

std::string getProgressBar(double percent) {
    int totalWidth = 20;
    std::string bars;

    int filledBars = static_cast<int>((percent / 100.0) * totalWidth);

    if (filledBars < 0) filledBars = 0;
    if (filledBars > totalWidth) filledBars = totalWidth;

    for (int i = 0; i < filledBars; i++) bars += "|";
    for (int i = filledBars; i < totalWidth; i++) bars += " ";

    std::string color;
    if (percent < 50) color = "\033[32m";
    else if (percent < 80) color = "\033[33m";
    else color = "\033[31m";

    return color + "[" + bars + "]" + "\033[0m";
}

int main() {
    unsigned long long prevIdle, prevTotal;
    unsigned long long currIdle, currTotal;
    unsigned long long prevRx, prevTx;
    unsigned long long currRx, currTx;

    getCpuStats(prevIdle, prevTotal);
    getNetworkStats(prevRx, prevTx);

    std::cout << "\033[?25l";

    while (true) {
        sleep(1);

        getCpuStats(currIdle, currTotal);
        getNetworkStats(currRx, currTx);

        double ramPercent = getRamUsage();

        long long totalDelta = currTotal - prevTotal;
        long long idleDelta = currIdle - prevIdle;

        double cpuPercent = 0.0;
        if (totalDelta > 0) {
            cpuPercent = static_cast<double>(totalDelta - idleDelta) / totalDelta * 100.0;
        }

        long long downloadSpeed = currRx - prevRx;
        long long uploadSpeed = currTx - prevTx;

        std::cout << "\033[2J\033[1;1H";

        std::cout << std::fixed << std::setprecision(1);

        std::cout << "CPU Usage:      " << getProgressBar(cpuPercent) << " " << cpuPercent << "%" << std::endl;
        std::cout << "RAM Usage:      " << getProgressBar(ramPercent) << " " << ramPercent << "%" << std::endl;

        std::cout << std::defaultfloat;
        std::cout << "Download Speed: " << downloadSpeed / 1024 << " KB/s" << std::endl;
        std::cout << "Upload Speed:   " << uploadSpeed / 1024 << " KB/s" << std::endl;

        prevIdle = currIdle;
        prevTotal = currTotal;
        prevRx = currRx;
        prevTx = currTx;
    }

    return 0;
}
