#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include "hardware/clocks.h"

#define TEMPO_TROCA_SINAL 3000  // Tempo para troca do semáforo
#define PINO_BUZZER 21   
#define PINO_BOTAO 6           // Pino do botão

#define FREQ_BUZZER_VERMELHO 1000  // Frequência para o som do semáforo vermelho (1kHz)
#define FREQ_BUZZER_AMARELO 1500  // Frequência para o som do semáforo amarelo (1.5kHz)
#define FREQ_BUZZER_VERDE 2000    // Frequência para o som do semáforo verde (2kHz)

#define PWM_CLK_DIV 1  // Divisor para o PWM (ajuste conforme necessário)
uint slice_num;

typedef enum { VERMELHO, AMARELO, VERDE } EstadoSemaforo;

volatile EstadoSemaforo estado_semaforo = VERMELHO;
volatile uint8_t pinos_leds[3] = {13, 12, 11};  

bool buzzer_ativo = false;  // Flag para controlar se o buzzer está ativo ou não

void configurar_leds();
void configurar_buzzer();
void emitir_som(int frequencia, int duracao_ms);
void emitir_som_correspondente();
bool alterar_sinal(struct repeating_timer *t);

int main() {
    stdio_init_all();
    configurar_leds();
    configurar_buzzer();  // Chama a função para configurar o buzzer

    // Configura botão como entrada com pull-up interno
    gpio_init(PINO_BOTAO);
    gpio_set_dir(PINO_BOTAO, GPIO_IN);
    gpio_pull_up(PINO_BOTAO);

    struct repeating_timer temporizador;
    add_repeating_timer_ms(TEMPO_TROCA_SINAL, alterar_sinal, NULL, &temporizador);

    gpio_put(pinos_leds[VERMELHO], 1);  // Começa com vermelho aceso

    while (1) {
        sleep_ms(100);  // Pequeno delay para reduzir a carga no loop

        // Se o botão for pressionado e o buzzer não estiver ativo, emite som
        if (gpio_get(PINO_BOTAO) == 0 && !buzzer_ativo) {
            buzzer_ativo = true;  // Marca que o buzzer foi acionado
            printf("Botao pressionado! Emitindo som correspondente ao semáforo.\n");
            emitir_som_correspondente();
        }

        // Quando o botão não estiver pressionado, permite que o buzzer seja acionado novamente
        if (gpio_get(PINO_BOTAO) == 1) {
            buzzer_ativo = false;
        }
    }
}

void configurar_leds() {
    for (uint8_t i = 0; i < 3; i++) {
        gpio_init(pinos_leds[i]);
        gpio_set_dir(pinos_leds[i], GPIO_OUT);
        gpio_put(pinos_leds[i], 0);
    }
}

// Configura PWM no pino do buzzer para buzzer passivo
void configurar_buzzer() {
    gpio_set_function(PINO_BUZZER, GPIO_FUNC_PWM);  // Define o pino como PWM
    slice_num = pwm_gpio_to_slice_num(PINO_BUZZER); // Obtém o slice PWM
    pwm_set_enabled(slice_num, true);  // Ativa PWM
}

void emitir_som(int frequencia, int duracao_ms) {
    uint32_t clock_freq = clock_get_hz(clk_sys);  // Frequência do sistema (~125 MHz)
    uint16_t wrap = clock_freq / frequencia - 1;  // Define o período do PWM

    pwm_set_wrap(slice_num, wrap);
    pwm_set_gpio_level(PINO_BUZZER, wrap / 2);  // 50% de duty cycle para som audível
    sleep_ms(duracao_ms);

    // Desliga o som
    pwm_set_gpio_level(PINO_BUZZER, 0);
}

// Função para emitir som conforme a cor do semáforo
void emitir_som_correspondente() {
    switch (estado_semaforo) {
        case VERMELHO:
            // Frequência 1kHz para o semáforo vermelho
            emitir_som(FREQ_BUZZER_VERMELHO, 1000);
            break;

        case AMARELO:
            // Frequência 1.5kHz para o semáforo amarelo, com 3 bipes curtos
            for (int i = 0; i < 3; i++) {
                emitir_som(FREQ_BUZZER_AMARELO, 200);
                sleep_ms(100);
            }
            break;

        case VERDE:
            // Frequência 2kHz para o semáforo verde, com 5 bipes rápidos
            for (int i = 0; i < 5; i++) {
                emitir_som(FREQ_BUZZER_VERDE, 50);
                sleep_ms(100);
            }
            break;
    }
}

bool alterar_sinal(struct repeating_timer *t) {
    // Apaga o sinal atual
    for (uint8_t i = 0; i < 3; i++) {
        gpio_put(pinos_leds[i], 0);
    }

    // Altera o estado do semáforo
    if (estado_semaforo == VERMELHO) {
        estado_semaforo = AMARELO;
    } else if (estado_semaforo == AMARELO) {
        estado_semaforo = VERDE;
    } else {
        estado_semaforo = VERMELHO;  
    }

    // Acende o LED correspondente
    gpio_put(pinos_leds[estado_semaforo], 1);
    return true;
}
