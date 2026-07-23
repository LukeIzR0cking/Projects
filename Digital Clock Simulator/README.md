# Digital Clock Simulator

A lightweight, cross-platform terminal-based digital clock application written in C. The simulator features a high-visibility ASCII art time display, 12/24-hour dynamic format toggling, interactive countdown timer, configurable alarm system, anti-flicker UI updates, and real-time activity logging with millisecond timestamping.

---

## Features

* **Cross-Platform Compatibility**: Fully compatible with both Windows and POSIX (Linux / macOS) terminal environments through native conditional compilation.


* **ASCII Art Visual Clock**: Renders time dynamically in a large 5x5 ASCII art font using colorful ANSI terminal styling.


* **Interactive Clock Modes**:
* **12/24-Hour Format Toggle**: Seamlessly switch between 12-hour (with AM/PM indicators) and 24-hour formats on the fly.


* **Countdown Timer**: Interactive input in seconds with a live countdown display.


* **Alarm Clock**: Flexible parsing supporting inputs such as `8:30 PM`, `08:30 AM`, or `14:30`.


* **Audible Alerting**: Emits system audio alerts (`\a`) upon alarm or timer expiration.




* **Smooth Terminal Rendering**: Utilizes ANSI escape control sequences (`\033[H`, `\033[K`) to update the display smoothly without screen flickering.


* **Non-Blocking Asynchronous Input**: Reads keyboard strokes in real-time without pausing the background clock execution.


* **Signal Handling & Interrupt Safety**: Gracefully handles signals (`SIGINT`, `SIGTSTP`) so pressing `Ctrl+C` cancels active timers/alarms rather than closing the program abruptly.


* **Logging & Precision Timestamps**: Logs actions to `ClockLog.txt` and provides sub-second timestamping (millisecond resolution) for accurate event tracking.



---

## File Structure

| File | Description |
| --- | --- |
| `Clock.h` | Core header containing cross-platform macros, ANSI colors, library imports, function prototypes, and inline helper functions (`initConsole`, `clearScreen`, `sleepMilliseconds`, `getTimestamp`, `logMessage`).

 |
| `ClockMain.c` | Main application entry point (`main`), non-blocking keyboard event loop (`kbhit_unix`, `_kbhit`), and interrupt signal handlers.

 |
| `ClockSetup.c` | UI logic including 5x5 ASCII art rendering (`ascii_digits`), main clock display formatting, and interactive input prompts for alarm/timer setup.

 |
| `Utilities.c` | Execution engines for the countdown timer (`runTimer`) and alarm clock (`runAlarm`) featuring anti-flicker updates and alert sounds.

 |
| `Demo.c` | Demonstration program showcasing the millisecond-precision `getTimestamp()` tool inside an example prime calculation loop.

 |

---

## Controls

When running the main application, use the following single-key keyboard commands:

| Key | Action |
| --- | --- |
| `t` / `f` | Toggle between 12-hour and 24-hour time format.

 |
| `a` | Open the interactive Alarm setup prompt.

 |
| `s` | Open the interactive Timer setup prompt.

 |
| `q` | Quit the application safely.

 |
| `Ctrl+C` | Cancel an active alarm/timer or terminate the program cleanly.

 |

---

## Compilation & Usage

### Prerequisites

* GCC or Clang compiler toolchain installed.
* Terminal supporting ANSI escape codes (enabled automatically on Windows console via `ENABLE_VIRTUAL_TERMINAL_PROCESSING`).



### Compiling the Clock Application

#### Linux / macOS (GCC / Clang):

```bash
gcc -std=c99 -Wall ClockMain.c ClockSetup.c Utilities.c -o clock_simulator
./clock_simulator

```

#### Windows (GCC / MinGW):

```cmd
gcc -std=c99 ClockMain.c ClockSetup.c Utilities.c -o clock_simulator.exe
clock_simulator.exe

```

---

### Compiling and Running the Timestamp Utility Demo

A standalone demonstration (`Demo.c`) is included to highlight the millisecond timestamp logging system during execution:

```bash
# Compile Demo program
gcc -std=c99 -Wall Demo.c -o timestamp_demo -lm

# Run Demo
./timestamp_demo

```

---

## Technical Highlights

### Anti-Flicker Terminal Graphics

To prevent terminal flashing caused by repeated `system("clear")` calls, active loops position the cursor at the top left (`\033[H`) and erase to the end of each line (`\033[K`):

```c
printf("\033[H");
printf("         %02d : %02d : %02d\033[K\n", h, m, s);

```

### High-Precision Timestamping

`getTimestamp()` adapts to the host operating system to capture sub-second timing:

* Uses `GetLocalTime()` on Windows.


* Uses `gettimeofday()` on POSIX platforms.



### Logging

Key events such as setting or triggering alarms and timers are appended automatically to `ClockLog.txt` with formatted timestamps.
