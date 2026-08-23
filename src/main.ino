#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int pos = 0;

const int red = 2;
const int green = 3;
const int bazzer = 4;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6,7,8,9};
byte colPins[COLS] = {10,11,12,13};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myservo;

const String Password ="1234";
String inputPin = "";
byte failedPassword = 0;

void resetUI();
void checkPassword();
void lockoutSystem();

void setup() {

  myservo.attach(5);
  myservo.write(0);


  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(bazzer, OUTPUT);

  Serial.begin(9600);
  
  Wire.begin();       
  lcd.begin(16, 2);    
  lcd.backlight();

  resetUI();
}

void loop() {

  char key=keypad.getKey();

  if(key)
  {
    if(key >= '0' && key <= '9')
    {
      if(inputPin.length() < 4)
      {
        inputPin += key;
        lcd.print("*");
      }
    }
    else if (key == '#')
    {
      checkPassword();
    }
    else if (key == '*')
    {
      resetUI();
    } 
  }
}

void resetUI (){

  inputPin = "";
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(bazzer, LOW);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN :");
  lcd.setCursor(0,1);

}


void checkPassword(){

  // IF IT IS CORRECT PASSWORED
  if (inputPin == Password)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Access Granted!");

   digitalWrite(green,HIGH);
   myservo.write(90);//OPEN GEAT
   
   delay(5000);

   myservo.write(0);// CLOSSE GEAT 

   resetUI();// REST SYSTEM

  }
  else
  {
   failedPassword++; //COUNT FAILED

   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Wrong Password!");
   digitalWrite(red,HIGH);

  // MAKE  A SOUNED 

   digitalWrite(bazzer,HIGH);
   delay(500);
   digitalWrite(bazzer,LOW);
   delay(1000);

   //IF HE FAILED 3 TIMES LOCK SYSTEM

   if(failedPassword >= 3)
   {
    lockoutSystem();
   }
   else 
   {
   resetUI();// REST SYSTEM
   }
  }
}

void lockoutSystem(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SYSTEM LOCKED!");
  
  // WAIT 15s
  for(int i = 15; i >= 0; i--)
  {
    lcd.setCursor(0,1);
    lcd.print("wait: ");
    lcd.print(i);
    lcd.print("s ");

    digitalWrite(red, !digitalRead(red));
    digitalWrite(bazzer, HIGH);
    delay(200);
    digitalWrite(bazzer,LOW);
    delay(800);
  }

  failedPassword = 0;   // REST FAILED
  resetUI();// REST SYSTEM

}
