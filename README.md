

***

# ProcWatch 📊

**ProcWatch** is a high-performance, lightweight system monitor for Linux and WSL (Windows Subsystem for Linux), written entirely in modern C++. 

It provides real-time tracking of CPU usage, Memory consumption, and Network traffic by parsing the Linux Kernel's virtual filesystem directly. It features a color-coded terminal interface with visual bar charts, designed to be a dependency-free alternative to tools like `htop`.

##  Preview

```text
CPU Usage:      [||||||              ] 32.5%
RAM Usage:      [||||||||||||||      ] 74.2%
Download Speed: 12054 KB/s
Upload Speed:   450 KB/s
```

##  Features

*   **Real-Time Monitoring:** Updates system metrics every second using efficient delta calculations.
*   **Zero Dependencies:** Built using only the C++ Standard Library (`<iostream>`, `<fstream>`, `<vector>`). No external libraries required.
*   **Kernel-Level Parsing:**
    *   **CPU:** Calculates usage percentages by analyzing processor ticks from `/proc/stat`.
    *   **RAM:** Determines accurate memory availability via `/proc/meminfo`.
    *   **Network:** Aggregates real-time upload/download speeds from `/proc/net/dev`.
*   **Visual Dashboard:**
    *   Dynamic bar charts that scale with usage.
    *   Color-coded indicators (Green < 50%, Yellow < 80%, Red > 80%).
    *   Flicker-free rendering using ANSI escape sequences.
##  How It Works

**ProcWatch** operates by reading the raw data exposed by the Linux Kernel in the `/proc` directory.

1.  **CPU Calculation:**
    The program takes two snapshots of `/proc/stat`. It sums the total Jiffies (time units) and the Idle Jiffies. By calculating the difference (Delta) between the two snapshots, it derives the precise CPU load percentage:
    
    $$ 
    \text{Usage} = \frac{\Delta\text{Total} - \Delta\text{Idle}}{\Delta\text{Total}}
    $$

2.  **Memory Analysis:**
    Instead of simply reading "Free RAM" (which is often misleading due to caching), ProcWatch parses `/proc/meminfo` to calculate `MemTotal` and `MemAvailable`, providing a true representation of usable memory.

3.  **Network Throughput:**
    It iterates through all network interfaces (excluding loopback) in `/proc/net/dev`, aggregating the total RX (Receive) and TX (Transmit) bytes. The speed is calculated by comparing the byte count growth over a 1-second interval.

##  Installation & Usage

### Prerequisites
*   A Linux environment (Ubuntu, Debian, Fedora, Arch, etc.) OR **WSL**.
*   A C++ compiler (`g++` or `clang++`).

### Build Instructions

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/YOUR_USERNAME/ProcWatch.git
    cd ProcWatch
    ```

2.  **Compile the source code:**
    ```bash
    g++ main.cpp -o procwatch
    ```

3.  **Run the application:**
    ```bash
    ./procwatch
    ```

4.  **To Exit:**
    Press `Ctrl + C`.

##  Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

##  License
This project is open-source and available under the MIT License.
