#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

/* LED configuration */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define tamanho 6
#define ID_KL25Z  0b10
#define NUM_CORES 3

static const struct gpio_dt_spec LEDs[NUM_CORES] =  {
     GPIO_DT_SPEC_GET(LED2_NODE, gpios), GPIO_DT_SPEC_GET(LED0_NODE, gpios), GPIO_DT_SPEC_GET(LED1_NODE, gpios),
};


static const struct device *uart0_dev;
int ID;
int R;
int G;
int B;
int ACAO;
int CHECK = 0;
int contador = 0;
uint8_t buffer[tamanho];
uint8_t cores [NUM_CORES];

void leds_usados (void) {
    int i;
    int estado;
    for(i=0; i<NUM_CORES; i++) {
        estado = cores[i];
    }
}

static void uart0_cb(const struct device *dev, void *user_data) {
    uint8_t leitura;
    if (!uart_irq_update(dev)) {
        return;
    }
    uart_err_check(dev);

    if (uart_irq_rx_ready(dev)) {
        while(uart_fifo_read(dev, &leitura, 1) == 1) {
            if(contador < tamanho) {
                buffer[contador] = leitura;
                contador++;
            }
        }
    }
    if(contador == tamanho) {
        if(buffer[1] != 0) {
            cores[0] = 1;
        } else {
            cores[0] = 0;
        }
        if(buffer[2] != 0) {
            cores[1] = 1;
        } else {
            cores[1] = 0;
        }
        if(buffer[3] != 0) {
            cores[2] = 1;
        } else {
            cores[2] = 0;
        } 
        if (buffer[0] != 0xFF && buffer[0] != 0x00) {
            CHECK = CHECK + 0b1;
        } 
        if (buffer[1] != 0xFF && buffer[1] != 0x00) {
            CHECK = CHECK + 0b10;
        }
        if (buffer[2] != 0xFF && buffer[2] != 0x00) {
            CHECK = CHECK + 0b100;
        }
        if (buffer[3] != 0xFF || buffer[3] != 0x00) {
            CHECK = CHECK + 0b1000;
        }
        if (buffer[4] != 0xFF || buffer[4] != 0x00) {
            CHECK = CHECK + 0b10000;
        }
        if(buffer[0] != ID_KL25Z) {
            printk("ERRO: ID não identificado!");
        } else if(CHECK != buffer[5]) {
            printk("ERRO: CHECK incorreto!");
        } else {
            if(buffer[4] == 1) {
                int i;
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_set_dt(&LEDs[i], 1);
                    }
                }
            } else if(buffer[4] == 2) {
                int i;
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_set_dt(&LEDs[i], 0);
                    }
                }
            } else if(buffer[4] == 3) {
                int i;
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_toggle_dt(&LEDs[i]);
                    }
                }
            } else if(buffer[4] == 4) {
                int i;
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_set_dt(&LEDs[i], 1);
                    }
                }
                k_msleep(1000);
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_set_dt(&LEDs[i], 0);
                    }
                }
            } else if(buffer[4] == 4) {
                int i;
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_set_dt(&LEDs[i], 1);
                    }
                }
                k_msleep(2000);
                for(i=0; i<NUM_CORES; i++) {
                    if(cores[i] == 1) {
                        gpio_pin_set_dt(&LEDs[i], 0);
                    }
                }
            }
        }
        contador = 0;
        CHECK = 0;
    }
}

void main(void) {
    /* Configure LED */
    if (!gpio_is_ready_dt(&LEDs[0])) {
        return;
    }
    gpio_pin_configure_dt(&LEDs[0], GPIO_OUTPUT_INACTIVE);
    if (!gpio_is_ready_dt(&LEDs[1])) {
        return;
    }
    gpio_pin_configure_dt(&LEDs[1], GPIO_OUTPUT_INACTIVE);
    if (!gpio_is_ready_dt(&LEDs[2])) {
        return;
    }
    gpio_pin_configure_dt(&LEDs[2], GPIO_OUTPUT_INACTIVE);

    /* Get uart0 */
    uart0_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

    if (!device_is_ready(uart0_dev)) {
        printk("uart0 não está pronta!\n");
        return;
    }

    /* Configure uart0 callback */
    uart_irq_callback_user_data_set(uart0_dev, uart0_cb, NULL);
    uart_irq_rx_enable(uart0_dev);

    while (1) {
        k_sleep(K_FOREVER);
    }
}
