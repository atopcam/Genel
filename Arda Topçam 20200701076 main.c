/*
Arda Topçam
20200701076

Drive the pins from your name and state them in your main.c as comment.
I chose pins 3 and 4
As a comment, I modded it with 5 from the ascii alphabet and determined it

*/

#include "stm32f4xx.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#define OUTPUT_PIN GPIO_PIN_3
#define OUTPUT_PORT GPIOA
#define INPUT_PIN GPIO_PIN_4
#define INPUT_PORT GPIOA

void GPIO_Init(void);
void USB_SendCharacter(uint8_t character);

int main(void)
{
    HAL_Init();
    GPIO_Init();
    MX_USB_DEVICE_Init();

    uint8_t receivechar = 0;

    while (1)
    {
        if (CDC_ReceiveByte(&receivechar) == USBD_OK)
        {
            if (receivechar > 0)
                HAL_GPIO_WritePin(OUTPUT_PORT, OUTPUT_PIN, GPIO_PIN_SET);
            else
                HAL_GPIO_WritePin(OUTPUT_PORT, OUTPUT_PIN, GPIO_PIN_RESET);

            uint8_t state = HAL_GPIO_ReadPin(INPUT_PORT, INPUT_PIN);

            charsendUSB(state);
            
            HAL_Delay(2000);
        }
    }
}

void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = OUTPUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OUTPUT_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = INPUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(INPUT_PORT, &GPIO_InitStruct);
}

void charsendUSB(uint8_t character)
{
    CDC_Transmit_FS(&character, 1);
}
