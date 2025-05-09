#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "LABS_lib.h"

void auto_brake(int devid)
{
    uint16_t dist = 0;
    while(1){
        if ('Y' == ser_read(0) && 'Y' == ser_read(0)){
            uint8_t byte1 = ser_read(0);
            uint8_t byte2 = ser_read(0);
            dist = (byte2 << 8) | byte1;
            printf("Distance: %d cm\n", dist);
            gpio_write(GREEN_LED, OFF);
            gpio_write(RED_LED, OFF);
            gpio_write(BLUE_LED, OFF);

            if ( dist > 200){
                // safe distance, no brake only green
                gpio_write(GREEN_LED, ON);
                // gpio_write(RED_LED, OFF);
                // gpio_write(BLUE_LED, OFF);
            }else if (( dist > 100) && (dist < 200)){
                // brake lightly only yellow (red and green)
                gpio_write(GREEN_LED, ON);
                gpio_write(RED_LED, ON);
                // gpio_write(BLUE_LED, OFF);
            }else if((dist > 60) && (dist < 100)){
                // brake hard only red
                // gpio_write(GREEN_LED, OFF);
                gpio_write(RED_LED, ON);
                // gpio_write(BLUE_LED, OFF);
            }else if (dist < 60){
                // stop completely blink red using timer interrupt
                timer_setup(0, 100); // Set up timer 0 with 100ms interval
                timer_enable_interrupt(0); // Enable timer interrupt for timer 0
                while (dist < 60) {
                    if (timer_interrupt_flag(0)) { // Check if timer interrupt occurred
                        static int led_state = 0;
                        gpio_write(RED_LED, led_state); // Toggle RED LED
                        led_state = !led_state; // Flip the state
                        timer_clear_interrupt_flag(0); // Clear the interrupt flag
                    }
                }
                timer_disable_interrupt(0); // Disable timer interrupt after exiting loop
            }
        }
    }
}

int read_from_pi(int devid)
{
        // Configure UART interrupt for receiving data
        ser_enable_interrupt(devid);

        // Interrupt handler for UART
        void uart_interrupt_handler() {
            char data[8];
            ser_readline(devid, 8, data);
            sscanf(data, "%d", &received_angle);
            if (received_angle < 0) {
            received_angle = 0;
            }
        }

        // Attach the interrupt handler
        ser_attach_interrupt_handler(devid, uart_interrupt_handler);

        // Wait for the angle to be updated by the interrupt
        while (received_angle == 0);

        // Return the received angle
        return received_angle;
    }
}

void steering(int gpio, int pos)
{
    int duty_cycle = 544 + (pos * 2000-544) / 180; // Convert angle to duty cycle
    int remaining_time = 20000 - duty_cycle; // Calculate remaining time
    gpio_write(gpio, ON); // Set the GPIO pin high
    delay_usec(duty_cycle); // Wait for the duty cycle time
    gpio_write(gpio, OFF); // Set the GPIO pin low
    delay_usec(remaining_time); // Wait for the remaining time
    printf("pos %d\n", pos);

}


int main()
{
    // initialize UART channels
    ser_setup(0); // uart0
    ser_setup(1); // uart1
    int pi_to_hifive = 1; //The connection with Pi uses uart 1
    int lidar_to_hifive = 0; //the lidar uses uart 0
    
    printf("\nUsing UART %d for Pi -> HiFive", pi_to_hifive);
    printf("\nUsing UART %d for Lidar -> HiFive", lidar_to_hifive);
    
    //Initializing PINs
    gpio_mode(PIN_19, OUTPUT);
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    while (1) {
        auto_brake(lidar_to_hifive); // measuring distance using lidar and braking
        int angle = read_from_pi(pi_to_hifive); //getting turn direction from pi
        printf("\nangle=%d", angle) 
        int gpio = PIN_19; 
        for (int i = 0; i < 6; i++){
            // Here, we set the angle to 180 if the prediction from the DNN is a positive angle
            // and 0 if the prediction is a negative angle.
            // This is so that it is easier to see the movement of the servo.
            // If the servo function is written correctly, it should still work,
            // only the movements of the servo will be more subtle
            if(angle>0){
                steering(gpio, 180);
            }
            else {
                steering(gpio, angle + 90);
            }
        }

    }
    return 0;
}
