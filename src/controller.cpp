#include <stdlib.h>
#include <stdio.h>

#include "controller.h"

// Main logic
// recv message from websocket and make pwm and gpio stuff here
// String from websocket would be a tuple like object
// First elem is move FORWARD(+) or BACKWARD(-)(pwm values) and second is
// turn RIGHT(+) or LEFT(-)(pwm value)
// Examples of input data:
// (100;100)
// ( 90; 87)
// Parse string coming from websocket connection
Motion parse_ws_input(const char * s) {
    Motion motion = {0, 0};
    char s_velocity [4];
    char s_rotation [4];
    int velocity_sign = 1;
    int rotation_sign = 1;
    while (*s != ';') {
        switch (*s) {
            case '(': {
                s++;
                if (*s == '-') {
                    velocity_sign = -1;
                    s++;
                }
                int i = 0;
                while (*s != ',') {
                    s_velocity[i] = *s;
                    s++;
                    i++;
                }
                s_velocity[i++] = '\0';
                break;
            }
            case ',': {
                s++;
                if (*s == '-') {
                    rotation_sign = -1;
                    s++;
                }
                int i = 0;
                while (*s != ')') {
                    s_rotation[i] = *s;
                    s++;
                    i++;
                }
                s_rotation[i++] = '\0';
                break;
            }
            default:
                s++;
                break;
        }
    }
    motion.velocity = atoi(s_velocity) * velocity_sign;
    motion.rotation = atoi(s_rotation) * rotation_sign;
    return motion;
}