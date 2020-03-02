#ifndef LCDFunctions_H
#define LCDFunctions_H

#include "stm32f10x.h"


#define LCDD0PIN 12
#define LCDD0PORT GPIOB
#define LCDD1PIN 13
#define LCDD1PORT GPIOB
#define LCDD2PIN 14
#define LCDD2PORT GPIOB
#define LCDD3PIN 15
#define LCDD3PORT GPIOB
#define LCDD4PIN 8 
#define LCDD4PORT GPIOA
#define LCDD5PIN 9
#define LCDD5PORT GPIOA
#define LCDD6PIN 10
#define LCDD6PORT GPIOA
#define LCDD7PIN 11
#define LCDD7PORT GPIOA
#define LCDRSPIN 10
#define LCDRSPORT GPIOB
#define LCDRWPIN 11
#define LCDRWPORT GPIOB
#define LCDEnablePIN 8
#define LCDEnablePORT GPIOB
#define DelayBeforeEnable 100 //40000 //100ns
#define DelayAfterEnable 300//80000 //300ns    200ns



/*void NotExactDelay(int delay);
void SetPinandPortForOutputPushPull(GPIO_TypeDef *port, int pinnumber);
void InitializingPortsForLCD(void);
void SendBitToPortandPin(GPIO_TypeDef *port, int pinnumber, uint8_t bitstate);
void LCDEnable(void);
void LCDSetToWrite(void);
void LCDSetToRead(void);
void LCDCommandMode(void);
void LCDCharacterMode(void);
void LCDByte(char character);
void LCDSendCharacter(char character);
void LCDSendCommand(char command);    */
void LCDClear(void);
void LCDBegin(void);
void LCDSendString(char *stringofcharacters);
void LCDSendInteger(int Integer ,uint8_t MaxLengthOfString);
void LCDSendFloat(float floatnumber ,uint8_t MaxLengthOfString);
//void LCDCursorPositionBitCheck(void);
void LCDSetCursorPosition(int X, int Y);
//

#endif

