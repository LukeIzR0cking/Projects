#ifndef CLOCK_H
#define CLOCK_H

#if !defined(_WIN32) && !defined(_DEFAULT_SOURCE)
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

/* Cross-platform library imports */
#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#else
    #include <unistd.h>
    #include <sys/time.h>
#endif

// ANSI Color Codes
#define GREY          "\x1b[37m"
#define RED           "\x1b[31m"
#define CYAN          "\x1b[36m"
#define GREEN         "\x1b[32m"
#define YELLOW        "\x1b[33m"
#define COLOR_RESET   "\x1b[0m"

void displayClock(bool is12HourFormat);
void promptTimer(void);
void promptAlarm(bool use12HourFormat);
void runTimer(int seconds);
void runAlarm(int targetHour, int targetMin, bool isPM, bool use12HourFormat);


static inline void initConsole(void) {
    // Windows requires manual enablement of ANSI escape codes for colors
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

static inline void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static inline void sleepMilliseconds(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000); // usleep takes microseconds, so multiply by 1000
#endif
}


static inline const char* getTimestamp(void) {
    // We use a 'static' buffer so the memory isn't destroyed when the function ends
    static char buffer[128]; 
    int hour, min, sec, ms;

#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    hour = st.wHour;
    min  = st.wMinute;
    sec  = st.wSecond;
    ms   = st.wMilliseconds * 100;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *ptm = localtime(&tv.tv_sec);
    hour = ptm->tm_hour;
    min  = ptm->tm_min;
    sec  = ptm->tm_sec;
    ms   = tv.tv_usec / 10; 
#endif

    // snprintf writes the formatted text INTO the buffer, rather than out to the screen
    snprintf(buffer, sizeof(buffer), "  " YELLOW "[%02d:%02d:%02d.%05d]" COLOR_RESET " ", hour, min, sec, ms);
    return buffer; // Return the pointer to the string
}

static inline void logMessage(const char *txt){
    FILE *file = fopen("ClockLog.txt", "a");
    if (file == NULL){
        printf("Error");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file, "[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, txt);
    fclose(file);
}

#endif // CLOCK_H