#include "DHT11Function.h"
#include "stm32f10x.h"
#include "delays.h"
#include "LCDFunctions.h"



short int DHTreceive(void)
{
	//sensor data capture bit start (50us low)(bit0(30us high)) (50us low)(bit1(70us high))  
	short int d=0;
	for(short int i = 0; i<8 ; i++)
		{
			while(!(GPIOA->IDR & GPIO_IDR_IDR6));
			DelayUs(35);
			if((GPIOA->IDR & GPIO_IDR_IDR6))
					{
						d = ((d<<1)|(0x01));	//bit 1
						while(GPIOA->IDR & GPIO_IDR_IDR6);					
					}
			else
			    {
						d = (d<<1); //bit 0
						
					}
			
		}
		
		return d;
}


//Put DHT11run function in while loop
void DHT11run(void)
{
	short	int x=0,IH=0,DH=0,IT=0,DT=0,CK=0;

			//LCDClear();
			
			//MC port output 1 default	
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			GPIOA->CRL |= GPIO_CRL_MODE6; //50MHz
			GPIOA->CRL &= ~GPIO_CRL_CNF6_1;  //opendrain///
			GPIOA->CRL |= GPIO_CRL_CNF6_0;
			
			//MC send out start signal for atleast 18ms (low)
			GPIOA->BSRR = GPIO_BSRR_BR6; //Pulls the line low
			DelayMs(30);
			
			//MC will pull up (20-40us)(high) and wait for response from dht sensor ///
			////MC port pin send 1 for 20-40us////
			GPIOA->BSRR = GPIO_BSRR_BS6;
			DelayUs(50);  
			
			
			if(GPIOA->IDR & GPIO_IDR_IDR6)
				{
					while(GPIOA->IDR & GPIO_IDR_IDR6);
				}

			//DHT sends out response signal(low)and keeps it for 80us 
			//set MC port pin set to input push pull and receive 0 for 80us
						
			if(!(GPIOA->IDR & GPIO_IDR_IDR6))
				{
					while(!(GPIOA->IDR & GPIO_IDR_IDR6));
					while(GPIOA->IDR & GPIO_IDR_IDR6);	

			//DHT pull up and get ready for sensor data for 80us
			//MC receives 1 from sensor for 80us

			/*40bit higherdata bite first Data format: 8bit integral RH data + 8bit decimal RH data + 8bit integral T data + 8bit decimal T
      data + 8bit check sum. If the data transmission is right, the check-sum should be the last 8bit of
      "8bit integral RH data + 8bit decimal RH data + 8bit integral T data + 8bit decimal T data".*/
					IH = DHTreceive();
					DH = DHTreceive();
					IT = DHTreceive();
					DT = DHTreceive();
					CK = DHTreceive();


					if(CK == (IH + DH + IT + DT))
						{
							LCDSetCursorPosition(0,0);
							LCDSendString("H:   %");
							LCDSetCursorPosition(2,0);
							LCDSendInteger(IH,3);
					
							LCDSetCursorPosition(6,0);
							LCDSendString("  T:   C");
							LCDSetCursorPosition(10,0);
							LCDSendInteger(IT,3);
						}else 
							{	
								LCDClear();
								LCDSendString("DATA ERROR"); 
								LCDSendInteger(x++,3);
							}	

					//sensor pulls down bus voltage
					DelayMs(3000);
				}else
						{
							LCDClear();
							LCDSendString("SENSOR NO RESPONSE"); 
						}			
	
}
