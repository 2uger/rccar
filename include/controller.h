#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct {
    int velocity;
    int rotation;
} Motion;

Motion parse_ws_input(const char *);

#endif