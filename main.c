#include "stm32f10x.h"
#include "LCDFunctions.h"
#include "delays.h"


//#define navbuttonport GPIOA;
//#define navbuttonpin  1;
//#define selectbuttonport GPIOA;
//#define selectbuttonpin 0;
//#define backbuttonport GPIOA;
//#define backbuttonpin 2;


int i=2; //Default Screen - Home
int MIN = 1; //Don't Change
int MAX = 4; //Number of Menus + 1
int Current = 2;

int main()
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		//PORTA PINS 2, 1 and 0 set to input pushpull
		GPIOA->CRL &= ~(GPIO_CRL_MODE0| GPIO_CRL_MODE1);
		GPIOA->CRL |= (GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1|GPIO_CRL_CNF2_1);
		GPIOA->CRL &= ~(GPIO_CRL_CNF0_0|GPIO_CRL_CNF1_0|GPIO_CRL_CNF2_0);	
		
		
		LCDBegin();
		LCDSendString("WELCOME");
		LCDSetCursorPosition(0,1);
		LCDSendString("Default PIN A6");
		DelayMs(2000);
		LCDClear();
		
		while(1)
			{		
				while(i == 2)
					{
						DelayMs(300);						
						//LCDClear();
						LCDSetCursorPosition(0,0);
						LCDSendString("LED -> OFF ");
						LCDSetCursorPosition(0,1);
						LCDSendString("    NAVIGATE  >>");
						Current = 2;
						while(i == 2)
							{
								
								if(GPIOA -> IDR & GPIO_IDR_IDR0)
									{
										i = i + 1;
										if(i > MAX) i = MAX;
									}
		
								if(GPIOA -> IDR & GPIO_IDR_IDR2)
									{
										i = i - 1;
										if(i <= MIN)
										i = 2;
									}
									
								if(GPIOA -> IDR & GPIO_IDR_IDR1)
									{
										GPIOA->CRL |= GPIO_CRL_MODE6; //Pin6 Mode to OUTPUT
										GPIOA->CRL &= ~GPIO_CRL_CNF6; // CNF is General PUSHPULL
										GPIOA->BSRR |= GPIO_BSRR_BS6;
										LCDSetCursorPosition(0,0);
										LCDSendString("LED -> ON ");
										
										LCDSetCursorPosition(0,1);
										LCDSendString("<<  OFF         ");
										while((GPIOA -> IDR & GPIO_IDR_IDR2) == 0)
											{
												
											}
										GPIOA->BRR |= GPIO_BRR_BR6;	
										LCDSetCursorPosition(0,0);
										LCDSendString("LED -> OFF ");
										LCDSetCursorPosition(0,1);
										LCDSendString("    NAVIGATE  >>");			
										DelayMs(300);
									}
									
																			
								}	
						//DelayMs(500);	
					}
			
				while(i == 3)
					{
						DelayMs(300);						
						//LCDClear();
						LCDSetCursorPosition(0,0);
						LCDSendString("ADC -> OFF            ");
						LCDSetCursorPosition(0,1);
						LCDSendString("<<  NAVIGATE  >>");
						Current = 3;
					
						while(i == 3)
						{
							//DelayMs(300);
							if(GPIOA -> IDR & GPIO_IDR_IDR0)
								{
									i = i + 1;
									if(i > MAX) i = MAX;
								}
		
							if(GPIOA -> IDR & GPIO_IDR_IDR2)
								{
									i = i - 1;
									if(i <= MIN) i = 2;
								}
							//DelayMs(500);	
						
							if(GPIOA -> IDR & GPIO_IDR_IDR1)
									{
										//int temp=0;
										
										LCDSetCursorPosition(0,0);
										LCDSendString("ADC ->          ");
										LCDSetCursorPosition(0,1);
										LCDSendString("<< OFF PRES&HOLD");
																							
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
												
										while(1)
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
													
												DelayMs(1000);
												if(GPIOA -> IDR & GPIO_IDR_IDR2)	
													{
														ADC1->CR2 |= ADC_CR2_ADON; 
														break;
													}			
											}
										
										LCDSetCursorPosition(0,0);
										LCDSendString("ADC -> OFF            ");
										LCDSetCursorPosition(0,1);
										LCDSendString("<<  NAVIGATE  >>");	
										DelayMs(300);
									}	
						
						}
						
					}		
					
					
				while(i == 4)
					{
						DelayMs(300);						
						//LCDClear();
						LCDSetCursorPosition(0,0);
						LCDSendString("EXIT             ");
						LCDSetCursorPosition(0,1);
						LCDSendString("<<  NAVIGATE    ");
						Current = 4;
						while(i == 4)
							{
								//DelayMs(300);
								if(GPIOA -> IDR & GPIO_IDR_IDR0)
									{
										i = i + 1;
										if(i > MAX) i = MAX;
									}
		
								if(GPIOA -> IDR & GPIO_IDR_IDR2)
									{
										i = i - 1;
										if(i <= MIN) i = 2;
									}
						//DelayMs(500);	

							}
						}
							
			
		}
		
		
		
		
		
	}
	
	
	

	
