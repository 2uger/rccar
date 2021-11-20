#include "driver/mcpwm.h"

#include "pwm.h"

#define PWM_MOVEMENT_OUT 16
#define PWM_ROTATION_OUT 16


static void pwm_movement_init(void) {
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PWM_MOVEMENT_OUT);
}

static void pwm_rotation_init(void) {
    //mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PWM_ROTATION_OUT);
}

void pwm_movement_control(int pwm_val) {
    printf("Setup new pwm value %d\n", pwm_val);
    if (pwm_val == 0) {
        mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
    }
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, pwm_val);
    //mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

void pwm_rotation_control(int pwm_val) {
    //mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, pwm_val);
    //mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

void pwm_setup(void) {
    pwm_movement_init();
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 10000;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    gpio_init();
}