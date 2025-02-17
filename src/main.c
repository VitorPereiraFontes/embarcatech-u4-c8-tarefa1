#include <stdio.h>
#include "pico/stdlib.h"
#include "pwm.h"
#include "joystick.h"
#include "interruption.h"
#include "ssd1306.h"

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
#define X_AXIS_CENTER_VALUE 2094 // Valor do eixo x quando o joystick está centralizado
#define Y_AXIS_CENTER_VALUE 2120 // Valor do eixo y quando o joystick está centralizado
#define AXIS_OFFSET 300 // Offset para regular a sensibilidade dos LED's
#define DEBOUNCE_INTERVAL 200000 // Define um intervalo de 200 milisssegundos para debounce

void draw_rectangle(uint top, uint left, uint width, uint height, bool visible);

uint last_event_time;
ssd1306_t display; // Inicializa a estrutura do display

void update_led_bright(){
    uint x_axis_value = get_joystick_x_value(); // Obtem o valor do eixo x
    uint y_axis_value = get_joystick_y_value(); // Obtém, o valor do eixo y

    printf("X: %d - Y: %d\n",x_axis_value,y_axis_value);

    if (x_axis_value > X_AXIS_CENTER_VALUE - AXIS_OFFSET && x_axis_value < X_AXIS_CENTER_VALUE + AXIS_OFFSET){
        update_duty_cycle(RGB_LED_RED_PIN,0); // Apaga o LED
    }else if(x_axis_value > X_AXIS_CENTER_VALUE){
        update_duty_cycle(RGB_LED_RED_PIN,x_axis_value); // Atualiza o duty cycle do LED vermelho com base no valor lido pelo joystick
    }else{
        update_duty_cycle(RGB_LED_RED_PIN,MAX_AXIS_VALUE - x_axis_value); // Atualiza o duty cycle do LED vermelho com base no valor lido pelo joystick, agora para valores menores que a posição central
    }

    if (y_axis_value > Y_AXIS_CENTER_VALUE - AXIS_OFFSET && y_axis_value < Y_AXIS_CENTER_VALUE + AXIS_OFFSET){
        update_duty_cycle(RGB_LED_BLUE_PIN,0); // Apaga o LED
    }else if(y_axis_value > Y_AXIS_CENTER_VALUE){
        update_duty_cycle(RGB_LED_BLUE_PIN,y_axis_value); // Atualiza o duty cycle do LED azul com base no valor lido pelo joystick
    }else{
        update_duty_cycle(RGB_LED_BLUE_PIN,MAX_AXIS_VALUE - y_axis_value); // Atualiza o duty cycle do LED azul com base no valor lido pelo joystick, agora para valores menores que a posição central
    }
}

void buttons_irq_handler(uint gpio, uint32_t events){
    uint64_t current_event_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos
    
    static bool pwm_state = true; // variavél de controle para habilitar/desabilitar o PWM

    static bool rectangle_state = false;
    
    // Implementação do debounce via software
    if (current_event_time - last_event_time > DEBOUNCE_INTERVAL){
        last_event_time = current_event_time;

        switch (gpio){
            case JOYSTICK_BUTTON_PIN:
                gpio_put(RGB_LED_GREEN_PIN,!gpio_get(RGB_LED_GREEN_PIN)); // ALterna o estado do LED verde

                rectangle_state = !rectangle_state; // Altera o valor da variável de true para false ou vice-versa
                
                draw_rectangle(0,0,WIDTH-1,HEIGHT-1, rectangle_state); // Desenha/esconde o retângulo externo
            break;

            case A_BUTTON_PIN:
                pwm_state = !pwm_state; // Altera o valor da variável de true para false ou vice-versa
                
                toggle_pwm(RGB_LED_BLUE_PIN,pwm_state); // Habilita/desabilita o PWM
                toggle_pwm(RGB_LED_RED_PIN,pwm_state); // Habilita/desabilita o PWM
            break;
            
            default:
            break;
        }
    }
}

void draw_square(uint top, uint left){
    ssd1306_rect(&display, top, left, 8, 8, true, true); // Desenha um quadrado
    ssd1306_send_data(&display); // Envia os dados para o display
}

void draw_rectangle(uint top, uint left, uint width, uint height,bool visible){
    ssd1306_rect(&display, top, left, width, height, visible, false); // Desenha um retângulo
    ssd1306_send_data(&display); // Envia os dados para o display
}

void clear_display(){
    ssd1306_fill(&display, false); // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_send_data(&display); // Envia os dados para o display
}

void init_display(){
    i2c_init(I2C_PORT, OLED_DISPLAY_CLOCK_FREQUENCY); // Inicializa a comunicação I2C
    gpio_set_function(OLED_DISPLAY_SDA_PIN,GPIO_FUNC_I2C); // Diz ao microcontrolador para usar o pino para comunicação I2C
    gpio_set_function(OLED_DISPLAY_SCL_PIN,GPIO_FUNC_I2C); // Diz ao microcontrolador para usar o pino para comunicação I2C
    gpio_pull_up(OLED_DISPLAY_SCL_PIN); // Configura um resistor de pull-up para o pino, conforme as intruções do protocolo I2C
    gpio_pull_up(OLED_DISPLAY_SDA_PIN); // Configura um resistor de pull-up para o pino, conforme as intruções do protocolo I2C

    ssd1306_init(&display, WIDTH, HEIGHT, false, OLED_DISPLAY_ADDRESS, I2C_PORT); // Inicializa o display
    ssd1306_config(&display); // Configura o display
    ssd1306_send_data(&display); // Envia os dados para o display
}

int main()
{
    stdio_init_all(); // Inicializa a entrada e saída padrão

    setup_joystick(JOYSTICK_X_PIN,JOYSTICK_Y_PIN); // Configura o joystick

    setup_pwm(RGB_LED_BLUE_PIN,1,MAX_AXIS_VALUE,0); // Configura o PWM para o LED azul
    setup_pwm(RGB_LED_RED_PIN,1,MAX_AXIS_VALUE,0); // Configura o PWM para o LED vermelho

    gpio_init(RGB_LED_GREEN_PIN); // Inicializa a GPIO conectada ao LED verde
    gpio_set_dir(RGB_LED_GREEN_PIN,GPIO_OUT); // Define a GPIO como saída
    gpio_put(RGB_LED_GREEN_PIN,0); // Inicializa a GPIO com o nível baixo

    initialize_buttons(A_BUTTON_PIN,JOYSTICK_BUTTON_PIN,buttons_irq_handler); // Configura a interrupção para o botão do joystick

    init_display(); //Inicializa o display OLED

    clear_display(); // Limpa o display OLED

    draw_rectangle(1,1,WIDTH-3,HEIGHT-3,true); // Desenha o retângulo interno

    while (true) {
        update_led_bright(); // Captura a entrada do joystick e atualiza a potência dos LED's
        
        sleep_ms(50); // Aguarda 50 milissegundos
    }
}