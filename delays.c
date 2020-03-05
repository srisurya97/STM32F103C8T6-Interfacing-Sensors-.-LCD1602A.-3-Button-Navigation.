#include "stm32f10x.h"
#include "delays.h"


int mytick = 0;


void TIMER2init(void)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		
		TIM2->PSC = 0;
		TIM2->ARR = 150;   //72
		
		TIM2->CR1 |= TIM_CR1_URS;
		TIM2->DIER |= TIM_DIER_UIE;
		TIM2->EGR |= TIM_EGR_UG;
		
		NVIC_EnableIRQ(TIM2_IRQn);
	
	}


void TIM2_IRQHandler(void)
{
	mytick++;
	TIM2->SR &= ~TIM_SR_UIF;
}


void DelayMs(int Ms)
	{
		TIMER2init();
		TIM2->CR1 |= TIM_CR1_CEN;
		mytick=0;
		while(mytick<(Ms*1000));
		TIM2->CR1 &= ~TIM_CR1_CEN;
	}


void DelayUs(int Us)
	{
		TIMER2init();
		TIM2->CR1 |= TIM_CR1_CEN;
		mytick=0;
		while(mytick<Us);
		TIM2->CR1 &= ~TIM_CR1_CEN;
	}
