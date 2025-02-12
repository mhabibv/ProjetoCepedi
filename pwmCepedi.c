#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definição do pino para controle (LED ou Servomotor)
#define PWM_PIN_LED 12      // GPIO do LED
#define PWM_PIN_MOTOR 22    // GPIO do servomotor

#define USE_LED

// Definindo os valores de duty cycle para controle de posição do servomotor
#define MIN_PULSE 500       // 0 graus (2.5%)
#define MID_PULSE 1470      // 90 graus (7.35%)
#define MAX_PULSE 2400      // 180 graus (12%)

// Parâmetros de controle
#define PWM_PERIOD 20000    // 20ms para 50Hz
#define DIVISOR_CLK 125.0   // Divisor para alcançar 50Hz
#define INCREMENT 5         // Incremento do pulso (μs)

// Função para configurar o PWM
void setup_pwm(uint PWM_PIN) {
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint pwm_slice = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_clkdiv(pwm_slice, DIVISOR_CLK); // Configura divisor para 50Hz
    pwm_set_wrap(pwm_slice, PWM_PERIOD);
    pwm_set_enabled(pwm_slice, true);
}

int main() {
    stdio_init_all();

    // Alterna o pino PWM dependendo do controle
    #ifdef USE_LED
        uint PWM_PIN = PWM_PIN_LED; // Configuração para LED
    #else
        uint PWM_PIN = PWM_PIN_MOTOR; // Configuração para motor
    #endif

    setup_pwm(PWM_PIN); // Chama a função de configuração


    // Inicializa a posição do servomotor ou brilho do LED
    pwm_set_gpio_level(PWM_PIN, MIN_PULSE);
    sleep_ms(5000);
    pwm_set_gpio_level(PWM_PIN, MID_PULSE);
    sleep_ms(5000);
    pwm_set_gpio_level(PWM_PIN, MAX_PULSE);
    sleep_ms(5000);

    uint16_t pulse_width = MIN_PULSE;
    bool increasing = true;

    // Loop para movimento contínuo ou variação de brilho
    while(true) {
        pwm_set_gpio_level(PWM_PIN, pulse_width);
        sleep_ms(10);

        // Ajuste de incremento para a posição do motor ou intensidade do LED
        if(increasing) {
            pulse_width += INCREMENT;
            if(pulse_width >= MAX_PULSE) {
                increasing = false;
            }
        } else {
            pulse_width -= INCREMENT;
            if(pulse_width <= MIN_PULSE) {
                increasing = true;
            }
        }
    }

    return 0;
}
