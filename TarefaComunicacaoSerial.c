#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "TarefaComunicacaoSerial.pio.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define button_0 5
#define button_1 6
#define LEDG_PIN 11
#define LEDB_PIN 12

#define IS_RGBW false
#define NUM_PIXELS 25
#define matrixRGB_PIN 7

// varialvel que armazena a cor e intensidade
uint8_t led_r = 1; // Intensidade do vermelho
uint8_t led_g = 1; // Intensidade do verde
uint8_t led_b = 1; // Intensidade do azul

static volatile uint32_t last_time = 0;

bool cor = true; // Agora é global
ssd1306_t ssd; // Agora é global

// Matriz para armazenar quais LEDs serão ligados
bool matrixLED[10][NUM_PIXELS] = {
    {// 0
    0, 1, 1, 1, 0, 
    1, 0, 0, 0, 1, 
    1, 0, 0, 0, 1, 
    1, 0, 0, 0, 1, 
    0, 1, 1, 1, 0
}, {// 1
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0
}, {// 2
    0, 1, 1, 1, 0, 
    0, 0, 1, 0, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 0, 1, 1, 0
}, {// 3
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
}, {// 4
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0
}, {// 5
    0, 0, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 0, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
}, {// 6
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
}, {// 7
    0, 0, 0, 1, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
}, {// 8
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
}, {// 9
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
}
};

void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 300000) { // 300ms de debouncing
        last_time = current_time;

        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);

        if (gpio == button_0) {
            gpio_put(LEDG_PIN, !gpio_get(LEDG_PIN));
            ssd1306_draw_string(&ssd, "LED RGB VERDE ", 11, 17);
            if(gpio_get(LEDG_PIN)== true){
                ssd1306_draw_string(&ssd, "LIGADO", 40, 37);
                printf("LED RGB verde acaba de ser ligado, pressione novamente para que ele seja desligado\n"); 
            }else{
                ssd1306_draw_string(&ssd, "DESLIGADO", 30, 37);
                printf("LED RGB verde acaba de ser desligado, pressione novamente para que ele seja ligado\n");  
            }
        } else if (gpio == button_1) {
            gpio_put(LEDB_PIN, !gpio_get(LEDB_PIN));
            ssd1306_draw_string(&ssd, "LED RGB AZUL ", 12, 17);
            if(gpio_get(LEDB_PIN)== true){
                ssd1306_draw_string(&ssd, "LIGADO", 40, 37); 
                printf("LED RGB azul acaba de ser ligado, pressione novamente para que ele seja desligado\n"); 
            }else{
                ssd1306_draw_string(&ssd, "DESLIGADO", 30, 37); 
                printf("LED RGB azul acaba de ser desligado, pressione novamente para que ele seja ligado\n"); 
            }

        }

        ssd1306_send_data(&ssd);
    }
}

//funções para utilização da matriz de led 
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// função que exibe numeros na matriz de led 
void set_one_led(uint8_t r, uint8_t g, uint8_t b, int numb)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (matrixLED[numb][i])
        {
            put_pixel(color); // Liga os ledss que estiverem com 1  
        }
        else
        {
            put_pixel(0);  // Desliga os leds que estiverem com 0
        }
    }
}

int main() {

    stdio_init_all(); // Inicializa comunicação USB CDC para monitor serial

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &TarefaComunicacaoSerial_program);

    TarefaComunicacaoSerial_program_init(pio, sm, offset, matrixRGB_PIN, 800000, IS_RGBW);

    // inicialização de ambos os leds e botões
    gpio_init(LEDG_PIN);              
    gpio_set_dir(LEDG_PIN, GPIO_OUT);

    gpio_init(LEDB_PIN);              
    gpio_set_dir(LEDB_PIN, GPIO_OUT);

    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN);
    gpio_pull_up(button_0);

    gpio_init(button_1);
    gpio_set_dir(button_1, GPIO_IN);
    gpio_pull_up(button_1);

    //inicialização da interrupção para os dois botões
    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_1, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    while (true) {
        if (stdio_usb_connected()) { // Certifica-se de que o USB está conectado
            char c;
            if (scanf(" %c", &c) == 1) {  // O espaço antes do %c ignora espaços em branco e '\n'
                // Verifica o caso de ';' para limpar e desenhar no display
                if (c == ';'){
                    ssd1306_fill(&ssd, !cor); // Limpa o display
                    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
                    ssd1306_draw_string(&ssd, "a b c d e f g", 8, 10);
                    ssd1306_draw_string(&ssd, "h i j k l m n", 8, 22);
                    ssd1306_draw_string(&ssd, "o p q r s t u", 8, 34);
                    ssd1306_draw_string(&ssd, "v w x y z", 25, 48);
                    ssd1306_send_data(&ssd); // Atualiza o display
                }
    
                // Controle das LEDs
                switch (c) {
                    case '0': set_one_led(led_r, led_g, led_b, 0); break;
                    case '1': set_one_led(led_r, led_g, led_b, 1); break;
                    case '2': set_one_led(led_r, led_g, led_b, 2); break;
                    case '3': set_one_led(led_r, led_g, led_b, 3); break;
                    case '4': set_one_led(led_r, led_g, led_b, 4); break;
                    case '5': set_one_led(led_r, led_g, led_b, 5); break;
                    case '6': set_one_led(led_r, led_g, led_b, 6); break;
                    case '7': set_one_led(led_r, led_g, led_b, 7); break;
                    case '8': set_one_led(led_r, led_g, led_b, 8); break;
                    case '9': set_one_led(led_r, led_g, led_b, 9); break;
                }
    
                // Código de desenho sempre executa se c for diferente de ;
                if (c != ';'){
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
                ssd1306_draw_char(&ssd, c, 60, 30); // Exibe a letra no centro
                ssd1306_send_data(&ssd); // Atualiza o display
                }
            }
        }
    }
    
    

    return 0;
    
}
