#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_intr_types.h"
#include "esp_intr_alloc.h"

#include "ez_encoder.h"

#define DT_PIN      GPIO_NUM_17
#define CLK_PIN     GPIO_NUM_16

#define ENCODER_0_ID    (0)

void encoder_isr_wrapper(void *arg){
    encoder_isr();
}

void app_main(void) {
    gpio_config_t cnf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1u<<CLK_PIN,
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&cnf);
    cnf.pin_bit_mask = 1u<<DT_PIN;
    gpio_config(&cnf);

    encoder_set_pins(CLK_PIN, DT_PIN, ENCODER_0_ID);
    encoder_set_debounce_cycles(3, ENCODER_0_ID);
    
    esp_timer_handle_t timer;
    esp_timer_create_args_t args = {
        .arg = NULL,
        .callback = encoder_isr_wrapper,
        .dispatch_method = 0,
        .name = "Encoder Test",
        .skip_unhandled_events = 0
    };
    esp_timer_create(&args, &timer);
    esp_timer_start_periodic(timer, 500);

    int count = 0;

    for(;;){
        count += encoder_read_counter(ENCODER_0_ID);
        printf("%d\r\n", count);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}