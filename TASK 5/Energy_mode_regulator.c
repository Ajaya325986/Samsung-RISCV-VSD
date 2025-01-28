#include <ch32v00x.h>

#define MOTOR_PULSE_PIN GPIO_Pin_2 // Pulse from analog counter
#define RELAY_SOLAR_PIN GPIO_Pin_3 // Relay for solar
#define RELAY_BATTERY_PIN GPIO_Pin_4 // Relay for battery
#define LED_INDICATOR_PIN GPIO_Pin_5 // Status indicator LED

volatile uint16_t rotation_count = 0; // Tracks motor rotations

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    // Configure motor pulse input
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = MOTOR_PULSE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure relay output pins
    GPIO_InitStructure.GPIO_Pin = RELAY_SOLAR_PIN | RELAY_BATTERY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure LED output pin
    GPIO_InitStructure.GPIO_Pin = LED_INDICATOR_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void EXTI_Config(void) {
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    
    // Configure external interrupt for motor pulse pin
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Enable interrupt in NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI2_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
        rotation_count++;
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

int main(void) {
    SystemCoreClockUpdate();
    GPIO_Config();
    EXTI_Config();

    while (1) {
        if (rotation_count >= 10) {
            // Switch to battery power
            GPIO_WriteBit(GPIOB, RELAY_SOLAR_PIN, Bit_RESET);
            GPIO_WriteBit(GPIOB, RELAY_BATTERY_PIN, Bit_SET);
            GPIO_WriteBit(GPIOB, LED_INDICATOR_PIN, Bit_SET); // Indicate battery active
        } else {
            // Stay on solar power
            GPIO_WriteBit(GPIOB, RELAY_SOLAR_PIN, Bit_SET);
            GPIO_WriteBit(GPIOB, RELAY_BATTERY_PIN, Bit_RESET);
            GPIO_WriteBit(GPIOB, LED_INDICATOR_PIN, Bit_RESET); // Indicate solar active
        }
    }
}
