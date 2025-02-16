#include <stdio.h>
#include "pico/stdlib.h"

#define A_BUTTON_PIN 5 // GPIO conectada ao botão A
#define I2C_PORT i2c1 // Porta I2C que será utilizada para a comunicação com o display
#define OLED_DISPLAY_SDA_PIN 14 // GPIO conectada ao pino SDA do display
#define OLED_DISPLAY_SCL_PIN 15 // GPIO conectada ao pino SCL do display
#define OLED_DISPLAY_ADDRESS 0x3C // Endereço I2C do Display
#define OLED_DISPLAY_CLOCK_FREQUENCY 400 * 1000 // Define a frequência de clock do display em KHz (400KHz)
#define RGB_LED_GREEN_PIN 11 // GPIO que controla o LED verde
#define RGB_LED_BLUE_PIN 12 // GPIO que controla o LED azul
#define RGB_LED_RED_PIN 13 // GPIO que controla o LED vermelho
#define JOYSTICK_X_PIN 26 // GPIO conectada ao eixo X do joystick
#define JOYSTICK_Y_PIN 27 // GPIO conectada ao eixo y do joystick
#define JOYSTICK_BUTTON_PIN 22 // GPIO conectada ao botão central do joystick
#define MAX_AXIS_VALUE 4095 // Valor máximo que o joystick captura em ambos os eixos
#define AXIS_CENTER_VALUE 2047 // Valor de ambos os eixos quando o joystick está centralizado
#define DEBOUNCE_INTERVAL 200000 // Define um intervalo de 200 milisssegundos para debounce

uint last_event_time;

int main()
{
    stdio_init_all(); // Inicializa a entrada e saída padrão

    while (true) {
        sleep_ms(50); // Aguarda 50 milissegundos
    }
}