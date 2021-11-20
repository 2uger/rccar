#include <HardwareSerial.h>
#include "driver/gpio.h"

#include "gpio.h"

// Forward and backward
gpio_num_t GPIO_MOVEMENT_OUT_1 = GPIO_NUM_18;
gpio_num_t GPIO_MOVEMENT_OUT_2 = GPIO_NUM_19;
// Right and left
gpio_num_t GPIO_MOVEMENT_OUT_34 = GPIO_NUM_34;
gpio_num_t GPIO_MOVEMENT_OUT_35 = GPIO_NUM_35;

#define GPIO_OUT_PINSEL ((1ULL<<GPIO_MOVEMENT_OUT_1) | (1ULL<<GPIO_MOVEMENT_OUT_2))

void gpio_init(void) {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUT_PINSEL;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

void move_forward(void) {
    gpio_set_level(GPIO_MOVEMENT_OUT_1, 0);
    gpio_set_level(GPIO_MOVEMENT_OUT_2, 1);
}

void move_backward(void) {
    gpio_set_level(GPIO_MOVEMENT_OUT_1, 1);
    gpio_set_level(GPIO_MOVEMENT_OUT_2, 0);
}

void move_stop(void) {
    gpio_set_level(GPIO_MOVEMENT_OUT_1, 0);
    gpio_set_level(GPIO_MOVEMENT_OUT_2, 0);
}

void rotate_left(void) {
    //gpio_set_level(GPIO_ROTATION_OUT_34, 1);
    //gpio_set_level(GPIO_ROTATION_OUT_35, 0);
}

void rotate_right(void) {
    //gpio_set_level(GPIO_ROTATION_OUT_34, 0);
    //gpio_set_level(GPIO_ROTATION_OUT_35, 1);
}
