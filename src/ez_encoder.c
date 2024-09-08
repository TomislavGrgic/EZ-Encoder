#include "ez_encoder.h"

static void encoder_read_signals(encoder_handle_t *encoder);

encoder_handle_t encoders[NUMBER_OF_ENCODERS] = {0};

void encoder_init(void){
    for(uint32_t i = 0; i < NUMBER_OF_ENCODERS; i++){
        DEFAULT_ENCODER_CONFIG(encoders[i]);
    }
}

void encoder_isr(void){
    static uint32_t count = 0;
    encoder_read_signals(&encoders[count++]);
    if(count >= NUMBER_OF_ENCODERS){
        count = 0;
    }
}

static void encoder_read_signals(encoder_handle_t *encoder){
    encoder->dt = encoder_read_gpio(encoder->clk_pin);
    encoder->clk = encoder_read_gpio(encoder->dt_pin);
    
    if ( encoder->clk_prev == encoder->clk ){
        encoder->debounce_curr = 0;
        return;
    }

    encoder->debounce_curr++;

    if ( encoder->debounce > encoder->debounce_curr ){
        return;
    }

    encoder->clk_prev = encoder->clk;

    if(encoder->clk != encoder->active_state){
        return;
    }

    encoder_disable_intr();
    encoder->count += encoder->dt != encoder->clk ? 1 : -1;
    encoder_enable_intr();
}

int16_t encoder_read_counter(int16_t encoder_id){
    assert(encoder_id < NUMBER_OF_ENCODERS);
    encoder_disable_intr();
    int16_t tmp = encoders[encoder_id].count;
    encoders[encoder_id].count = 0;
    encoder_enable_intr();
    return tmp;
}

void encoder_set_pins(uint16_t clk, uint16_t dt, int16_t encoder_id){
    assert(encoder_id < NUMBER_OF_ENCODERS);
    encoders[encoder_id].clk_pin = clk;
    encoders[encoder_id].dt_pin = dt;
}

void encoder_set_debounce_cycles(int16_t debounce, int16_t encoder_id){
    assert(encoder_id < NUMBER_OF_ENCODERS);
    encoders[encoder_id].debounce = debounce;
}