#ifndef __EZ_ENCODER_H__
#define __EZ_ENCODER_H__

#include <stdint.h>
#include "ez_encoder_user_setup.h"

#define DEFAULT_ENCODER_CONFIG(x)    do{x.clk_pin = 0; x.dt_pin = 0; x.clk = 1; x.clk_prev = 1; x.dt = 1; x.active_state = 0; x.count = 0;}while(0)

typedef struct ez_encoder_handle{
    uint8_t clk_pin;            /**< Stores the pin number of the encoder clock */
    uint8_t dt_pin;             /**< Stores the pin number of the encoder data */
    uint8_t clk : 1;            /**< Stores the current state of the clk pin */ 
    uint8_t clk_prev : 1;       /**< Stores the previous state of the clk pin */
    uint8_t dt : 1;             /**< Stores the current state of the dt pin */
    uint8_t active_state : 1;   /**< Determins if the signal is active low or active high*/
    int16_t count;              /**< Stores the current rotation value.*/
    int16_t debounce;           /**< Debounce threshold in interrput cycles. I tells the encoder for how many cycles the clock pin needs to holds its state for it to be a valid read. Setting this value to 0 will not preforme a debounce. Each cycle time lenght is calculated by the interrupt rate of the periodic timer used, number of encoders and the cycles set in the debounce member. All three should be multiplied.*/
    int16_t debounce_curr;      /**< Current debounce value. Increments only if clk and clk_prev are equal, resets otherwise.*/
}encoder_handle_t;

void encoder_init(void);
void encoder_isr(void);
int16_t encoder_read_counter(int16_t encoder_id);
void encoder_set_pins(uint16_t clk, uint16_t dt, int16_t encoder_id);
void encoder_set_debounce_cycles(int16_t debounce, int16_t encoder_id);

#endif /*__EZ_ENCODER_H__*/