#include <SPI.h>
#define SS   10
#define MOSI 11
#define MISO 12
#define SCK  13
 
//가속도 측정범위 -2.3g~+2.3g
//SCALE = 4.6/65536
#define SCALE 0.00007019
 
//가속도 값을 저장하기 위한 변수
double xAcc, yAcc, zAcc;
double xMax, yMax, zMax;
 
void setup() {
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
   
  SPI_SETUP();  //SPI 초기화
  Accelerometer_Setup();  //가속도 센서 초기화
}
 
void loop() {
  ReadAccVal(); //센서로 부터 가속도 값을 측정
 
  //측정한 가속도 값을 출력
  Serial.print("X=");
  Serial.print(xAcc, 1);
  Serial.print(" Y=");
  Serial.print(yAcc, 1);
  Serial.print(" Z=");
  Serial.println(zAcc, 1);
  //testMelody();
  if (xAcc > 2){
      tone(8, 220, 1000);
  }
  if (yAcc > 2){
      tone(8, 440, 1000);
  }
  if (zAcc > 2){
      tone(8, 880, 1000);
  }
  delay(100);
}

/*bool testMelody(){
  if (xAcc < 2 || yAcc < 2 || zAcc > 2){
      tone(8, 440, 1000);
      if (xAcc > 2 || yAcc < 2 || zAcc < 2){
        
      }
  }
}
}*/
 
void ReadAccVal(void){
  byte xAddressByteL = 0x28;      // Low Byte of X value (the first data register)
  byte readBit = B10000000;       // bit 0 (MSB) HIGH means read register
  byte incrementBit = B01000000;  // bit 1 HIGH means keep incrementing registers
  byte dataByte = xAddressByteL | readBit | incrementBit;
  byte b0 = 0x0;                  // an empty byte
 
  digitalWrite(SS, LOW);
  SPI.transfer(dataByte);
  byte xL = SPI.transfer(b0); // get the low byte of X data
  byte xH = SPI.transfer(b0); // get the high byte of X data
  byte yL = SPI.transfer(b0); // get the low byte of Y data
  byte yH = SPI.transfer(b0); // get the high byte of Y data
  byte zL = SPI.transfer(b0); // get the low byte of Z data
  byte zH = SPI.transfer(b0); // get the high byte of Z data
  digitalWrite(SS, HIGH);
 
  int xVal = (xL | (xH << 8));
  int yVal = (yL | (yH << 8));
  int zVal = (zL | (zH << 8));
 
  xAcc = xVal * SCALE;
  yAcc = yVal * SCALE;
  zAcc = zVal * SCALE;

  if(xAcc > xMax){
    xMax = xAcc;
  }
  if(yAcc > yMax){
    yMax = yAcc;
  }
  if(zAcc > zMax){
    zMax = zAcc;
  }
  
}
 
void SPI_SETUP()
{
  pinMode(SS, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}
 
void Accelerometer_Setup(void)
{
  byte addressByte = 0x21;
  byte ctrlRegByte = 0x00;
 
  digitalWrite(SS, LOW);
  SPI.transfer(addressByte);
  SPI.transfer(ctrlRegByte);
  digitalWrite(SS, HIGH);
 
  delay(100);
 
  addressByte = 0x22;
  ctrlRegByte = 0x00;
 
  digitalWrite(SS, LOW);
  SPI.transfer(addressByte);
  SPI.transfer(ctrlRegByte);
  digitalWrite(SS, HIGH);
  
  delay(100);
  
  addressByte = 0x20;
  ctrlRegByte = 0x47;
 
  digitalWrite(SS, LOW);
  SPI.transfer(addressByte);
  SPI.transfer(ctrlRegByte);
  digitalWrite(SS, HIGH);
}

