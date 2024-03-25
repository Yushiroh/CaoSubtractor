#include <Arduino.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
{'0','0','0'},
{'9','8','7'},
{'6','5','4'},
{'3','2','1'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {8, 7, 6};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int segPins[8] = {12, 11, A0, A2, A1, A5, A4, A3};
const int magic[11][8] = {

  {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH}, //1
  {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW, HIGH},    //2
  {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW, HIGH},    //3
  {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH},   //4
  {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW, HIGH},   //5
  {LOW, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH},    //6
  {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},    //7
  {LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH},         //8
  {LOW, LOW, LOW, LOW, HIGH, LOW, LOW, HIGH},        //9
  {LOW, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH},        //0
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},    //dot

};

const int button = 9;
const int regLed = 10;
bool var1State = false;
int storedVar = 0;
int currVar = 0;

void segmentDisplay(int inDigit);

void setup() {
  Serial.begin(9600);

  for(int i=0;i<8;i++){
    pinMode(segPins[i],OUTPUT);
  }
  segmentDisplay(10-1);
  pinMode(button,INPUT);
  pinMode(regLed,OUTPUT);
}


void loop() {

  char customKey = kpd.getKey();
  
  if (customKey){
    String msg = "";
    msg += customKey;
    msg = msg == "0" ? "10": msg;
    currVar = msg.toInt();
    currVar = currVar == 10? 0 : currVar;
    segmentDisplay(msg.toInt()-1);
  }else{
    int butState = digitalRead(button);
    delay(200);

    if(butState == 1 and !(var1State)){
      digitalWrite(regLed, HIGH);
      storedVar = currVar;
      var1State = true;
      segmentDisplay(10-1);
    }else if(butState == 1 and var1State){
      var1State = false;
      int finalAns = storedVar - currVar;
      finalAns = finalAns == 0 ? 10 : finalAns;
      Serial.println(finalAns);
      segmentDisplay(abs(finalAns) - 1);
      digitalWrite(regLed,LOW);
      if(finalAns < 0){
        digitalWrite(segPins[7], LOW);
      }
    }else{
      //No conditions met!
    }
    
  }
 
} 

void segmentDisplay(int inDigit){
    for(int i=0;i<8;i++){
      digitalWrite(segPins[i], magic[inDigit][i]);
    }
}