#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void run_cmd(const char* cmd) {
    pid_t pid = fork();
    if (pid == 0) {
        execl("/system/bin/sh", "sh", "-c", cmd, (char*)NULL);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("[MegaMobox] Booting sequence initiated...\n");

    printf("[MegaMobox] Installing dependencies...\n");
    run_cmd("pkg update -y");
    run_cmd("pkg install x11-repo -y");
    run_cmd("pkg install termux-x11-nightly pulseaudio clang xorg-xeyes -y");

    printf("[MegaMobox] Launching Termux-X11 server...\n");
    run_cmd("termux-x11 :0 &");
    sleep(3);

    printf("[MegaMobox] Opening Termux-X11 App...\n");
    run_cmd("am start --user 0 -n com.termux.x11/.MainActivity");

    printf("[MegaMobox] Preparing Wine environment...\n");
    setenv("DISPLAY", ":0", 1);
    setenv("PULSE_SERVER", "127.0.0.1", 1);

    printf("[MegaMobox] Launching test process (xeyes)...\n");
    run_cmd("xeyes &");

    printf("[MegaMobox] Handing over control...\n");

    return EXIT_SUCCESS;
}
