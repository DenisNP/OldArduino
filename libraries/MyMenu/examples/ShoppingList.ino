#include <MyMenu.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(3, 5, 10, 11, 12, 13);

#define NUM_ITEMS 32 //length of items array include submenu headers

MItm items[NUM_ITEMS] = {
  MItm("SHOPPING LIST",0,0), //main header always 0,0
  MItm("Fruits",1), //1 is target submenu index
  MItm("Veggies",2),
  MItm("Other",3),
  MItm("FRUITS LIST",1,0), //this is the submenu with index 1 (0 is previous menu index)
  MItm("Apple",50), // is there is no "50" submenu callback will run
  MItm("Orange",60),
  MItm("Banana",70),
  MItm("Pear",80),
  MItm(" <<BACK",0), //means "go to submenu with index 0"
  MItm("VEGGIES LIST",2,0),
  MItm("Tomato",90),
  MItm("Potato",100),
  MItm("Smth else",110),
  MItm(" <<BACK",0),
  MItm("Other",3,0),
  MItm("Meat",4),
  MItm("Fish",5),
  MItm(" <<BACK",0),
  MItm("MEAT",4,3), // notice that 3 in prev-index now not zero
  MItm("Chicken",6),
  MItm("Bacon",7),
  MItm("Beef",8),
  MItm("SHOW FISH",5),
  MItm(" <<BACK",3),
  MItm(" <<MAIN",0),
  MItm("FISH",5,3), 
  MItm("Clownfish",9),
  MItm("Shark",10),
  MItm("SHOW MEAT",4),
  MItm(" <<BACK",3),
  MItm(" <<MAIN",0)
};

Menu menu(items,NUM_ITEMS,&lcd,menuCallback);

void setup() {
  //set button pins
  pinMode(42,INPUT);
  pinMode(44,INPUT);
  pinMode(46,INPUT);
  pinMode(48,INPUT);
  
  //initialize serial port for monitoring
  Serial.begin(9600);
  
  //initialize lcd screen
  lcd.begin(20,4);
  
  //show main menu on screen
  menu.goMain();
}

void loop() {
  if(digitalRead(42) == HIGH){
    menu.goBack();
    delay(100); 
  }
  if(digitalRead(44) == HIGH){
    menu.goDown();
    delay(100); 
  }
  if(digitalRead(46) == HIGH){
    menu.goUp();
    delay(100); 
  }
  if(digitalRead(48) == HIGH){
    menu.goNext();
    delay(100); 
  }  

delay(100);
}

void menuCallback(int idx){
  if(idx < 10){
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("NO MEAT n FISH!"); 
  }else if(idx == 10){ //select "shark"
    int i=16;
    while(i >= 0){
      lcd.clear();
      lcd.setCursor(0,2);
      lcd.print("-^-^-^-^-^-^-^-^-^-^");
      lcd.setCursor(i,1);
      lcd.print("/l");
      delay(300);
      i--;
    }
  }else{
    lcd.clear();
    lcd.setCursor(2,2);
    lcd.print(String(idx)+" pressed");
  }
  delay(1000);
  menu.goLast();
}
