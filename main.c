#include "stm32f10x.h"
#include "LCDFunctions.h"
#include "delays.h"
#include "ADCFunction.h"
#include "DHT11Function.h"


//#define navbuttonport GPIOA;
//#define navbuttonpin  1;
//#define selectbuttonport GPIOA;
//#define selectbuttonpin 0;
//#define backbuttonport GPIOA;
//#define backbuttonpin 2;


int i=2; //Default Screen - Home
int MIN = 1; //Don't Change
int MAX = 5; //Number of Menus + 1
int Current = 2;

//Menu1 current2 LED ON/OFF
//Menu2 current3 ADC
//Menu3 current4 DHT11
//Menu4 current5 Exit
//

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
		DelayMs(1500);
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
										LCDSendString("<< BACK/LEDOFF  ");
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
						
							if(GPIOA -> IDR & GPIO_IDR_IDR1)
									{										
										LCDSetCursorPosition(0,0);
										LCDSendString("ADC ->          ");
										LCDSetCursorPosition(0,1);
										LCDSendString("<< OFF PRES&HOLD");
																							
										ADCinit();		
										
										while(1)
											{
												ADCResult();

												if(GPIOA -> IDR & GPIO_IDR_IDR2)	
													{
														ADC1->CR2 &= ~ADC_CR2_ADON; 
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
						LCDSendString("DHT11->OFF      ");
						LCDSetCursorPosition(0,1);
						LCDSendString("<<  NAVIGATE  >>");
						Current = 4;
						while(i == 4)
							{
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
									
								if(GPIOA -> IDR & GPIO_IDR_IDR1)
									{
										LCDSetCursorPosition(0,1);
										LCDSendString("<<OFF HOLD 3SEC ");
										while(1)
										{
											DHT11run();
											if(GPIOA -> IDR & GPIO_IDR_IDR2)	
													{
														break;
													}
										}
										
										LCDSetCursorPosition(0,0);
										LCDSendString("DHT11->OFF      ");
										LCDSetCursorPosition(0,1);
										LCDSendString("<<  NAVIGATE  >>");
										DelayMs(1000);
									}
							}
						}
							
        while(i == 5)
					{
						DelayMs(300);						
						LCDSetCursorPosition(0,0);
						LCDSendString("EXIT             ");
						LCDSetCursorPosition(0,1);
						LCDSendString("<<  NAVIGATE    ");
						Current = 5;
						while(i == 5)
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

