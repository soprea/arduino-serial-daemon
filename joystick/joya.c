#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include "../utils/utils.h"

#include <arpa/inet.h>
char keypressed[10]; 

int joy_open(char *device_name) {
    int fd = -1;

    if (device_name == NULL) {
        return fd;
    }

    fd = open(device_name, O_RDONLY | O_NONBLOCK);

    if (fd < 0) {
        printf("Could not locate joystick device %s\n", device_name);
        exit(1);
    }

    return fd;
}

void joy_info(int fd) {
    int axes = 0, buttons = 0;
    char name[128];

    ioctl(fd, JSIOCGAXES, &axes);
    ioctl(fd, JSIOCGBUTTONS, &buttons);
    ioctl(fd, JSIOCGNAME(sizeof (name)), &name);

    printf("%s\n  %d Axes %d Buttons\n", name, axes, buttons);
}

void joy_process(struct js_event jse) {
    if (jse.type == JS_EVENT_AXIS) {
        float v = jse.value;
        v = v / 32767;
        if (jse.number > 7 && jse.number < 14) {
        } else {
            if (jse.number > 6 && jse.number < 8) {
                if (jse.value < 0) {
                    char *keypressed = "F";
                    sendKey(keypressed);
                    printf("F was pressed\n");
                } else if (jse.value == 0) {
                    char *keypressed = "T";
                    printf("T was pressed\n");
                    sendKey(keypressed);
                } else if (jse.value > 0) {
                    char *keypressed = "B";
                    printf("B was pressed\n");
                    sendKey(keypressed);
                }
                printf("%d : Button %d\n", jse.time, jse.number);
            }
            if (jse.number > 5 && jse.number < 7) {
                if (jse.value < 0) {
                    char *keypressed = "L";
                    printf("L was pressed\n");
                    sendKey(keypressed);
                } else if (jse.value == 0) {
                    char *keypressed = "T";
                    printf("T was pressed\n");
                    sendKey(keypressed);
                } else if (jse.value > 0) {
                    char *keypressed = "R";
                    printf("R was pressed\n");
                    sendKey(keypressed);
                }
            if (jse.number == 7) {
                char *keypressed = "+";
                sendKey(keypressed);
            }
            if (jse.number == 6) {
                char *keypressed = "-";
                sendKey(keypressed);
            }
            if (jse.number == 5) {
                char *keypressed = ">";
                sendKey(keypressed);
            }
            if (jse.number == 4) {
                char *keypressed = "<";
                sendKey(keypressed);
            }
                printf("%d : Axis %d @ %0.2f\n", jse.time, jse.number, v);
            }
        }
    } else if (jse.type == JS_EVENT_BUTTON && jse.value > 0) {
        printf("%d : Button %d\n", jse.time, jse.number);
        if (jse.number > 2 && jse.number < 4) {
            char *keypressed = "W";
            printf("W was pressed\n");
            sendKey(keypressed);
        }
        if (jse.number == 0) {
            char *keypressed = "A";
            printf("A was pressed\n");
            sendKey(keypressed);
        }
        if (jse.number > 0 && jse.number < 2) {
            char *keypressed = "S";
            printf("S was pressed\n");
            sendKey(keypressed);
        }
        if (jse.number > 1 && jse.number < 3) {
            char *keypressed = "D";
            printf("D was pressed\n");
            sendKey(keypressed);
        }

    }
}

int main() {
    int fd;
    struct js_event jse;
    fd = joy_open("/dev/input/js0");
    joy_info(fd);
    while (1) {
        while (read(fd, &jse, sizeof (jse)) > 0) {
            joy_process(jse);
        }
    }
    return 0;
}
