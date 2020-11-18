#include <LiquidCrystal_I2C.h>

#include <LedControl.h>
#include <Wire.h> 


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int y1  = 0;
int y2 = 7;
bool b1 = false;
long int b1time = 0;
long int b2time = 0;
bool b2 = false;
int by1;
int by2;
volatile int screen[8][8];
bool move1 = true;
bool move2 = true;
int score1 = 0;
int score2 = 0;
long  last_right = 0;
long last_left = 0;
long last_left2 = 0;
long last_right2 = 0;
long last_shot = 0;
long last_shot2 = 0;

LedControl lc=LedControl(12,11,10,1);        


void setup()
{
    //SET INTERRUPTS AND PINS
    lc.shutdown(0,false);
  /* Set the brightness to a medium values */
    lc.setIntensity(0,8);
  /* and clear the display */
    lc.clearDisplay(0);
    int score1 = 0;
    int score2 = 0;
    lcd.init();
    lcd.backlight();
    init1();
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(19), left1, RISING);
    attachInterrupt(digitalPinToInterrupt(18), right1, RISING);
    pinMode(6, INPUT); 
   
    attachInterrupt(digitalPinToInterrupt(2), left2, RISING);
    pinMode(7, INPUT);
    attachInterrupt(digitalPinToInterrupt(3), right2, RISING);
    
    
}
void loop()
{
    update();
    for(int i = 0; i < 50; i++)
    {
      if(digitalRead(6) == HIGH)
      {
        shoot1();
        
      }
      if(digitalRead(7) == HIGH)
      {
        shoot2();
      }
      delay(1);
    }
    
}











void init1()
{
    y1  = 0;
    y2 = 7;
    b1 = false;
    b2 = false;
    by1 = 0;
    by2 = 0;
    move1 = true;
    move2 = true;
    delay(500);
    for( int i = 0; i < 8; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            screen[i][n] = 0;
        }
    }
    update();
    //clear
    //startinug animation
    scores(score1, score2);
    for (int i = 400; i > 50; i -= (i / 5))
    {
        
        for(int i = 0; i < 8; i++)
        {
          for (int n = 0; n < 8; n++) 
          {
            if (screen[i][n] == 1)
            {
              lc.setLed(0, i, n, true);
            }
          }
        }
        delay(i); 
        lc.clearDisplay(0);
        delay(i);
    }
//    y1  = 0;
//    int y2 = 7;
//    b1 = false;
//    bool b2 = false;
//    by1 = 0;
//    int by2 = 0;
//    update();
//    

}
void collide_bullets()
{ //see if bullets collides
    if ((b1 == true && b2 == true) && (by1 == by2))
    {
      if (b1time > b2time)
      { //if 1 was shot before b2, disable only b2, in effect killing p2
          b1 = false;
          Serial.println(2);
      }
      else
      {   if(b1time<b2time)
          { //if 2 was shot first, disable only b1, in effect killing p1 later on this frame
             b2 = false;
             Serial.println(1);
          }
          else
          { //if they were at the same time, no one dies and both are diables
            b1 = false;
            b2 = false;   
            Serial.println("both");
          }
      }    
    }
}
void collide1()
{
    if (y1 == by2 && b2 == true)
    { // see if p1 dies
        score2++;
        Serial.print(score1);
        Serial.print(" ");
        Serial.println(score2);

        delay(500);
        lc.clearDisplay(0);
        scores(score1, score2);
        for (int i = 500; i > 50; i -= (i/5))
        {
          lc.setLed(0, 0, y1, true);
          delay(i);
          lc.setLed(0, 0, y1, false); 
          delay(i);
        }
        init1();

    }

}
void collide2()
{
    if (y2 == by1 && b1 == true)
    { // se if p2 dies
        score1++;
        Serial.print(score1);
        Serial.print(" ");
        Serial.println(score2);
        delay(500);
        lc.clearDisplay(0);
        scores(score1, score2);
        for (int i = 500; i > 50; i -= (i/5))
        {
          lc.setLed(0, 7, y2, true);
          delay(i);
          lc.setLed(0, 7, y2, false); 
          delay(i);
        }
        init1();

    }
}
void left1()
{ // button bounce
    if ((micros() - last_left) >= 100000)
    { 
        if (move1 == true && y1 > 0)
        { // move
            y1--;
            move1 = false;
            last_left = micros();
        }
    }   
}
void right1()
{ //button bounce
    if ((micros() - last_right) >= 100000)
      {
        if (move1 == true && y1 < 7)
        { // move
            y1++;
            move1 = false;
            last_right = micros();
        }
      }  
}
void right2()
{ //button bounce
    if ((micros() - last_right2) >= 100000)
    { 
        if (move2 == true && y2 > 0)
        { //move
            y2--;
            move2 = false;
            last_right2 = micros();
        }
    }   
}
void left2()
{ //button bounce
    if ((micros() - last_left2) >= 100000)
      {
        if (move2 == true && y2 < 7)
        { //move
            y2++;
            move2 = false;
            last_left2 = micros();
        }
      }  
}

void shoot1()
{
    //button bounce
    if ((micros() - last_shot) >= 500000)
    { //is shooting allowed
        if (b1 == false)
        { //shoot and set bullet to true
            b1 = true;
            by1 = y1;
            b1time = millis();
            last_shot = micros();
        }
    }   
}
void shoot2()
{
    //button bounce
    if ((micros() - last_shot2) >= 500000)
    {
        // is shooting allowed
        if (b2 == false)
        {
          //shoot and set bullet to true
            b2 = true;
            by2 = y2;
            b2time = millis();
            last_shot2 = micros();
        }
    } 
}
void update()
{
   //see if the bullets collide
    collide_bullets();
    //reset internal representation of game
    for( int i = 0; i < 8; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            screen[i][n] = 0;
        }
    }

    for(int i = 0; i < 8; i++)
    {
        if (b1 == true)
        {
            screen[i][by1] = 1;
        } 
        if (b2 == true)
        {
            screen[i][by2] = 1;
        }  
    }
    screen[0][y1] = 1;
    screen[7][y2] = 1;
    lc.clearDisplay(0);
    for(int i = 0; i < 8; i++)
    {
      for (int n = 0; n < 8; n++) 
      {
        if (screen[i][n] == 1)
        {
          lc.setLed(0, i, n, true);
        }
      }
    }
    //
    ///DO THE MATRIX CODE HERE USING "SETLED"
    //
    //if either player moved, display the current state in the serial
    if (move1 == false || move2 == false)
    {
      for (int i = 0; i < 8; i++)
      {
        move1 = true;
        for (int n = 0; n < 8; n++)
        {
          if (screen[i][n] == 0)
          {
            Serial.print("_");
          }
          else
          {
            Serial.print("#");
          }
        }
        Serial.println();
      }
      
      Serial.println(b1);
      Serial.println();
    }
    //make the bullets stay for 3/4 seconds after shotx
    if ((millis() - b1time) >= 750 && b1 == true)
    {
      b1 = false;
      
    }
    if ((millis() - b2time) >=  750 && b2 == true)
    {
        b2 = false; 
    }
    //let movement happen and check if either playe dies
    move1 = true;
    move2 = true;
    collide1();
    collide2();

}

void scores(int score1, int score2)
{
    lcd.setCursor(0,0);
    lcd.print("Player1 Score:");
    lcd.setCursor(15,0);
    lcd.print(score1);
    lcd.setCursor(0,1);
    lcd.print("Player2 Score:");
    lcd.setCursor(15,1);
    lcd.print(score2);
}
