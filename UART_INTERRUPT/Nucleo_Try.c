#include "stm32f446xx.h"
#define PA9_AF_MODER 0x00080000
#define PA10_AF_MODER 0x00200000

void UART_INIT(void);


int main()
{
 	
SystemCoreClockUpdate();	 // Clock = 16 Mhz

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enabling GPIOA clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enabling USART1 clock
	
	GPIOA->MODER |= PA9_AF_MODER | PA10_AF_MODER ; // GPIOA moder register alternate-function active
  GPIOA->OSPEEDR |= (12<<16) | (3<<20); // PA9 & PA10 both at high speed
	GPIOA->PUPDR |= (1<<18); // PA9 set as Pull-up
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;  // GPIO alternate function high register AF7(0111)for both tx,rx
  GPIOA->AFR[1] |= 	GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2;
	UART_INIT();	
	
	
	
	
	return 0;
}


void UART_INIT(void)
	
{
	USART1->BRR |= 0x680;	
	               
	
	               /*An interrupt is generated 
	               if the TXEIE bit =1 
	              in the USART_CR1 register.
               	It is cleared by a write to the USART_DR register.
                                */           	
                USART1->CR1 |= USART_CR1_TXEIE                    | USART_CR1_RXNEIE;  // if (Read data register not empty =1) interrupt is generated
	
	USART1->CR1 |= USART_CR1_UE|USART_CR1_RE|USART_CR1_TE;
     
  NVIC_EnableIRQ(USART1_IRQn);
	while(1)
	{



	}

	
	}

	
void USART1_IRQHandler()
{

char temp;

// We are here because of RXNEIE interrupt	
if(USART1->SR & USART_SR_RXNE)
	   {
	    temp=USART1->DR; // reading Data register to clear RXNE bit
	    for(int i=0;i<800;i++);
			 USART1->DR = temp;  // transmit over what was recieved
     while (!(USART1->SR & USART_SR_TC));  // Wait for Transmission to complete
		
	    }

			
// We  are here because of TXEIE interrupt		
			
if(USART1->SR & USART_SR_TXE)  // If transmit data register is empty 
  
  {  char A='S';
		 USART1->DR = A;  // Writing in Data register to clear TXEIE bit
		
	}


}	
