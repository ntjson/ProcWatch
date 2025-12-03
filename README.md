

# ProcWatch 📊

**ProcWatch** is a lightweight, real-time system monitor for Linux (and WSL) written in pure C++. 

It mimics the functionality of tools like `top` or `htop` by interacting directly with the Linux Kernel via the `/proc` virtual filesystem. It demonstrates how to parse system files, calculate CPU deltas, and manage terminal output using ANSI escape codes.

## Features
*   **Real-time Monitoring:** Updates system stats every second.
*   **Zero Dependencies:** Written using only the C++ Standard Library (`<iostream>`, `<fstream>`, `<sstream>`).
*   **Lightweight:** Minimal CPU footprint (it sleeps when not updating).
*   **Under the Hood:** Reads directly from raw kernel files:
    *   `/proc/stat` for CPU ticks.
    *   `/proc/meminfo` for RAM calculations.

## How It Works
Unlike Windows, Linux treats everything as a file. This program works by:
1.  **CPU Calculation:** It takes two snapshots of `/proc/stat` (CPU tick counters). By comparing the difference between the "Total" ticks and "Idle" ticks over a 1-second interval, it calculates the precise CPU load percentage.
2.  **RAM Calculation:** It parses `/proc/meminfo` to find `MemTotal` and `MemAvailable`, providing a more accurate usage metric than simply looking at "Free" memory (which ignores caching).
3.  **Visuals:** It uses ANSI escape codes (`\033[2J`) to refresh the terminal screen smoothly without scrolling.

##  Installation & Usage

### Prerequisites
*   A Linux environment (Ubuntu, Fedora, Arch, etc.) or **WSL** (Windows Subsystem for Linux).
*   A C++ Compiler (`g++` or `clang`).

### Build and Run
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/ntjson/ProcWatch.git
    cd ProcWatch
    ```

2.  **Compile the code:**
    ```bash
    g++ main.cpp -o procwatch
    ```

3.  **Run the monitor:**
    ```bash
    ./procwatch
    ```

4.  **To Exit:**
    Press `Ctrl + C`.

##  Preview
```text
CPU Usage: 12.5%
RAM Usage: 45.2%
```

##  Future Improvements
*   Add a bar chart visualization using terminal characters (e.g., `[|||||.....]`).
*   Add support for monitoring individual CPU cores.
*   Add Network upload/download speed monitoring.

##  License
This project is open-source and available under the MIT License.
