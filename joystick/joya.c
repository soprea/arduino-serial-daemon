#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>

#include <arpa/inet.h>
char keypressed[10];

void sendKey(char *keypressed) {
    int sock; /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    char *servIP = "127.0.0.1"; /* Server IP address (dotted quad) */
    int value = 1;
    int echoServPort = 4000; /* server port */

    /* Create a reliable, stream socket using TCP */
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof (echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    echoServAddr.sin_port = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof (echoServAddr)) < 0)
        exit(0);

    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &value, sizeof (int)) < 0)
        exit(0);

    /* Give the server a chance */
    usleep(1000);
    /* Send this */
    if (send(sock, keypressed, sizeof (keypressed), 0) != sizeof (keypressed))
        exit(0);
    close(sock);
}

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
