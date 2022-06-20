#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int RECV_PIN = A0;
IRrecv irrecv(RECV_PIN);
decode_results results;

int motorBlindD = 8; // 모터 블라인드 다운
int motorBlindU = 9; // 모터 블라인드 업                                       

int light = 10; // 전등

int speaker = 11; // 알람소리 (위험감지 소리)

int led1 = 12; // led 불빛 (초인종 및 위험감지 불빛) 

int sensor = A1; // 스마트폰 빛 감지

Servo servo;
int angle = 0;

void setup(){
  
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Hello!!");
  delay(1000);
  
  // 초기화
  pinMode(motorBlindD, OUTPUT);
  pinMode(motorBlindU, OUTPUT);
  
  pinMode(light, OUTPUT);
  digitalWrite(light, LOW);
  
  pinMode(speaker, OUTPUT);
  digitalWrite(speaker, LOW);
  
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  
  servo.attach(13);
  
}

void loop(){
  
  if(irrecv.decode(&results)){
    Serial.println(results.value, HEX); //리모콘 버튼값 HEX 값으로 읽기
    switch(results.value){
      case 0xFD30CF: lcd.clear(); // 0번 -  블라인드 DOWN
                     lcd.print("Blind Down");
      				 digitalWrite(motorBlindD, HIGH);
                     delay(5000);
       				 digitalWrite(motorBlindD, LOW);
                     break;
      case 0xFD08F7: lcd.clear(); // 1번 - 블라인드 UP
                     lcd.print("Blind Up");
            		 digitalWrite(motorBlindU, HIGH);
                     delay(5000);
       				 digitalWrite(motorBlindU, LOW);
                     break;
      case 0xFD8877: lcd.clear(); // 2번 - 전등 ON/OFF
                     if(digitalRead(light) == LOW){
                         lcd.print("Light On");
      					 digitalWrite(light, HIGH);
                     }else if(digitalRead(light) == HIGH){
                         lcd.print("Light Off");
      				     digitalWrite(light, LOW);
                     }
                     break;
      case 0xFD48B7: lcd.clear(); // 3번 - 알람 
                     lcd.print("Morning Call");
                     for(int i=0;i<5;i++){
                          digitalWrite(speaker, HIGH);
                          delay(500);
                          digitalWrite(speaker, LOW);
                          delay(500);
                     }
                     break;
      case 0xFD28D7: lcd.clear(); // 4번 - 아침모드(블라인드 UP + 전등 ON + 알람)
                     lcd.print("Morning Mode");
            		 digitalWrite(light, HIGH);
                     digitalWrite(motorBlindU, HIGH);
                     for(int i=0;i<5;i++){
                          digitalWrite(speaker, HIGH);
                          delay(500);
                          digitalWrite(speaker, LOW);
                          delay(500);
                     }
                     digitalWrite(motorBlindU, LOW);
                     break;
      case 0xFDA857: lcd.clear(); // 5번 - 나이트모드(블라인드 DOWN + 전등 OFF)
                     lcd.print("Night Mode");
            		 digitalWrite(light, LOW);
                     digitalWrite(motorBlindD, HIGH);
                     delay(5000);
                     digitalWrite(motorBlindD, LOW);
                     break;
      case 0xFD6897: lcd.clear(); // 6번 - 초인종 불빛 알림
                     lcd.print("DoorBell Ringing");
                     for(int i=0;i<10;i++){
                        digitalWrite(led1, HIGH);
                        delay(100);
                        digitalWrite(led1, LOW);
                        delay(100);
                     }
                     break;
      case 0xFD18E7: lcd.clear(); // 7번 - 위험 신호(경고 불빛 알림 + 소리 알림)
                     lcd.print("Warning!!");
                     for(int i=0;i<20;i++){
                        digitalWrite(led1, HIGH);
                        digitalWrite(speaker, HIGH);
                        delay(100);
                        digitalWrite(led1, LOW);
                        digitalWrite(speaker, LOW);
                        delay(100);
                     }
	                     break;
      case 0xFD9867: lcd.clear(); // 8번 - 스마트폰 밝기 이용한 확인
                     if(analogRead(sensor) < 86){
                        lcd.print("Phone Ringing");
                     }else{
                        lcd.print("Phone Sleep");
                     }
                     break;
      case 0xFD58A7: lcd.clear(); // 9번 - 문 Open/Close
                     if(angle == 90){
                        lcd.print("Door Open");
                        angle = 0;
                        servo.write(angle);
                     }else if(angle == 0){
                        lcd.print("Door Close");
                        angle = 90;
                        servo.write(angle);
                     }
                     break;
    }
    irrecv.resume();
  }
  
}