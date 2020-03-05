#include "ADCFunction.h"
#include "stm32f10x.h"
#include "delays.h"
#include "LCDFunctions.h"

void ADCinit(void)
{
	
	// ADC and Alt function clock enable
										RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_AFIOEN;
										
										// PortA Pin 6 input PushPull 
										GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6_0);
										GPIOA->CRL |= GPIO_CRL_CNF6_1;
	 
										//Timer Settings
										TIM2->PSC = 0;
										TIM2->ARR = 72;
										TIM2->CR1 |= TIM_CR1_URS;
										TIM2->DIER |= TIM_DIER_UIE;
										TIM2->EGR |= TIM_EGR_UG;
		
										NVIC_EnableIRQ(TIM2_IRQn);
										//NVIC_EnableIRQ(ADC1_2_IRQn);
 
                    //ADC Settings	
										// Prescaler div by 8
										RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
		
										ADC1->CR1 |= ADC_CR1_EOCIE; //EOC Interrupt Enable
										ADC1->SMPR2 |= ADC_SMPR2_SMP6; //Sampling pin 6 to 236 cycles
										ADC1->SQR3 |= ADC_SQR3_SQ1_1 |ADC_SQR3_SQ1_2 ; //channel 6 in 1st sequence
										ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON; //Set to Continous and ADC ON for the 1st time.
										DelayUs(5);
												
										//ADC Calibration
										ADC1->CR2 |= ADC_CR2_ADON; //ADC ON 
										DelayUs(5);
										ADC1->CR2 |= ADC_CR2_CAL; //ADC Calibration
										while((ADC1->CR2 & ADC_CR2_CAL) == 1);   //While Calibration is being done
										//temp = ADC1->DR;
										LCDSetCursorPosition(8,0);
										LCDSendString("CALIBRATING");
										//LCDSendString("CAL ERROR:");
										//LCDSendInteger(ADC1->DR, 5);
										DelayMs(2000);


}


void ADCResult(void)
{
												ADC1->CR2 |= ADC_CR2_ADON;			
												while((ADC1->SR & ADC_SR_EOC) == 0);
												//LCDSendInteger(ADC1->DR, 5);
												LCDSetCursorPosition(7,0);
												LCDSendString("     ");
												LCDSetCursorPosition(7,0);
												LCDSendFloat((float)((ADC1->DR )*((float)(3.2)/4096)), 5);
												LCDSetCursorPosition(11,0);
												LCDSendString(" Volt");
}

