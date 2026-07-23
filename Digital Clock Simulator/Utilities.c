#include "Clock.h"

static volatile sig_atomic_t timerRunning = 1;

//setvbuf(stdout, NULL, _IONBF, 0);

static void handleTimerExit(int sig) {
    timerRunning = 0;
}



/* ── runTimer ─────────────────────────────────────────────────────────────
   Countdown timer. Smooth update without screen flashing.                  */
void runTimer(int totalSeconds) {
    int remaining = totalSeconds;

    timerRunning = 1;
    signal(SIGINT, handleTimerExit);

    logMessage("Time Counting");

    clearScreen();
    while (remaining >= 0 && timerRunning) {
        int h = remaining / 3600;
        int m = (remaining % 3600) / 60;
        int s = remaining % 60;

        printf("\033[H");
        // Use \033[K before \n to ensure empty lines clear old text completely
        printf("\033[K\n\033[K\n");
        printf(CYAN "  ==============================\033[K\n");
        printf("         COUNTDOWN TIMER        \033[K\n");
        printf("  ==============================\033[K\n");
        printf("\033[K\n");
        
        // \033[K added below to erase trailing equal signs
        printf("         %02d : %02d : %02d\033[K\n", h, m, s);
        
        printf("\033[K\n");
        printf("  ==============================\033[K\n");
        printf("  [Ctrl+C to cancel timer]     \033[K");
        printf(COLOR_RESET);
        fflush(stdout); 

        if (remaining == 0) {
            printf(YELLOW "\n  *** TIME IS UP! ***\n" COLOR_RESET);
            logMessage("Times Up");
            for (int i = 0; i < 5 && timerRunning; i++) {
                printf("\a");
                fflush(stdout);
                sleepMilliseconds(1000);
            }
            break;
        }

        remaining--;
        sleepMilliseconds(1000);
    }

    if (!timerRunning) {
        clearScreen();
        printf(YELLOW "\n  Timer cancelled. Returning to clock...\n" COLOR_RESET);
        logMessage("Timer cancelled. Returning to clock...");
        sleepMilliseconds(1000);
    }

    signal(SIGINT, SIG_DFL);
    timerRunning = 1;
}

/* ── runAlarm ─────────────────────────────────────────────────────────────
   Alarm clock. Smooth update matching the timer.                          */
void runAlarm(int targetHour, int targetMin, bool isPM, bool is12HourFormat) {

    timerRunning = 1;
    signal(SIGINT, handleTimerExit);

    int targetHour24 = targetHour;
    if (is12HourFormat) {
        if (isPM && targetHour < 12) {
            targetHour24 = targetHour + 12;
        } else if (!isPM && targetHour == 12) {
            targetHour24 = 0;
        } else if (isPM && targetHour == 12) {
            targetHour24 = 12;
        }
    }

    clearScreen();
    printf("\n\n");
    printf(GREEN "  ==============================\n");
    printf("          ALARM CLOCK           \n");
    printf("  ==============================\n\n");
    
    if (is12HourFormat) {
        printf("   Alarm set for  %02d:%02d %s\n", targetHour, targetMin, isPM ? "PM" : "AM");
    } else {
        printf("   Alarm set for  %02d:%02d (24-hr)\n", targetHour24, targetMin);
    }
    
    printf("\n  ==============================\n" COLOR_RESET);
    sleepMilliseconds(1000);

    logMessage("Alarm Running");

    clearScreen();
    while (timerRunning) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        int currentDisplayHour = t->tm_hour;
        char *currentAMPM = "";
        
        if (is12HourFormat) {
            currentAMPM = (t->tm_hour >= 12) ? "PM" : "AM";
            currentDisplayHour = (t->tm_hour % 12 == 0) ? 12 : t->tm_hour % 12;
        }

        printf("\033[H");
        printf("\033[K\n\033[K\n");
        printf(CYAN "  ==============================\033[K\n");
        printf("          ALARM CLOCK           \033[K\n");
        printf("  ==============================\033[K\n");
        printf("\033[K\n");
        
        if (is12HourFormat) {
            printf("   Current :  %02d:%02d:%02d %s\033[K\n",
                   currentDisplayHour, t->tm_min, t->tm_sec, currentAMPM);
            printf("   Alarm   :  %02d:%02d:00 %s\033[K\n",
                   targetHour, targetMin, isPM ? "PM" : "AM");
        } else {
            printf("   Current :  %02d:%02d:%02d\033[K\n",
                   t->tm_hour, t->tm_min, t->tm_sec);
            printf("   Alarm   :  %02d:%02d:00\033[K\n",
                   targetHour24, targetMin);
        }

        printf("\033[K\n");
        printf("  ==============================\033[K\n");
        printf("  [Ctrl+C to cancel alarm]     \033[K");
        printf(COLOR_RESET);
        fflush(stdout); 

        if (t->tm_hour == targetHour24 && t->tm_min == targetMin) {
            printf(YELLOW "\n  *** ALARM RINGING! ***\n" COLOR_RESET);
            logMessage("*** ALARM RINGING! ***");
            for (int i = 0; i < 5 && timerRunning; i++) {
                printf("\a");
                fflush(stdout);
                sleepMilliseconds(1000);
            }
            break;
        }

        sleepMilliseconds(1000);
    }

    if (!timerRunning) {
        clearScreen();
        printf(YELLOW "\n Alarm cancelled. Returning to clock... \n" COLOR_RESET);
        logMessage("Alarm cancelled. Returning to clock...");
        sleepMilliseconds(1000);
    }
    
    signal(SIGINT, SIG_DFL);
    timerRunning = 1;
}