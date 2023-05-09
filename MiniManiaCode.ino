/*
 *  Andrew Enelow
 *  
 * Mini Mania Code
 * GE 1502 1:35 PM
 * 4/21/21
 * 
 * This circuit contains:
 * 1) LCD (digital pins 13, 12, 11, 10, 8, 7) [Output]
 * 2) 4 Buttons (analog pins A1, A2, A4, A5) [Input]
 * 3) Buzzer (digital pin 2) [Output]
 * 4) Potentiometer (analog pin 3) [Input]
 * 5) RBG LED (digital pins 3,5,6) [Output]
 * 
 * This sketch was written by Andrew Enelow with Ethan Baram, Jacob Lieber, and Ellery Sledesky.
 * This sketch and its acompanying SparkFun circuit plays multiple educational mini games. 
 * 
 * Enter menu function from void loop
 * Select Options from Menu
 * Game Select:
 *    Select games from Game select
 *    Run game function
 *    Calculate Game Score
 * Add Players:
 *    Add players to game
 *    Assign Game code to host
 * Settings:
 *    Select scoring system
 *    Adjust Volume
 */

#include <LiquidCrystal.h>                  //the liquid crystal library contains commands for printing to the display
LiquidCrystal lcd(13, 12, 11, 10, 8, 7);    // tell the RedBoard what pins are connected to the display
int Contrast=80;

int RedPin = 3;   //PWM~ for analogWrite commands to achieve mixed colors & reduced light intensity
int GreenPin = 5; //PWM~ for analogWrite commands to achieve mixed colors & reduced light intensity
int BluePin = 6;  //PWM~ for analogWrite commands to achieve mixed colors & reduced light intensity

int button[] = {A1, A2, A4, A5};               //red is button[0], yellow is button[1], green is button[2], blue is button[3]
int tones[] = {262, 330, 392, 494};   //tones to play with each button (c, e, g, b)

int speakerPin = 2;  //set speaker to digital pin 2
int potPosition;       //this variable will hold a value based on the position of the potentiometer
int p = potPosition/100;
int setpot;

int roundsToWin = 16;         //number of rounds the player has to play before they win the game (the array can only hold up to 16 rounds)
int buttonSequence[16];       //make an array of numbers that will be the sequence that the player needs to remember
int gamecode[] = {1, 2, 3, 4};
int player[] = {1, 2, 3, 4};
int score[] = {0, 0, 0, 0};
int roundscore[] = {0, 0, 0, 0};
int rankedscore[] = {0, 0, 0, 0};
int temp1,temp2;
int control(1);


String colors[] = {"Red", "Yellow", "Green", "Blue"};
String menusecs[] = {"Start", "Add Players", "Settings", "Exit"};
String settingsecs[] = {"Scoring", "Controls", "Back"};
String game[] = {"Fast Math", "Simon Hears", "React", "BlackJack", "Krazy Mode", "Back"};
String cards[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
int cardvals[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
String mathsym[] = {"+","-","x","/"};
//int volume = 350;

long mathstarttime(0),mathtime(0),mathscore,mathendtime(0),recentscore,randomtime(0);

long startTime = 0;           //timer variable for time limit on button press
long shufflestarttime = 0;
long timeLimit = 6000;        //time limit to hit a button
long reacttimelimit = 2000;
double avgtime,timesum;

long reacttime1,reacttime2;
boolean reactloop = false;

boolean exitmod = false;
boolean exitmod2 = false;
boolean gameStarted = false;      //variable to tell the game whether or not to play the start sequence
boolean host = false;
boolean krazyrnd = true;
boolean comp = true;
int players(1),user(1);
int a(0),b(0),c(0),i(0),j(0),n(0),o(0),o2(0),t(0),t2(0),h(0),h2(0),n1(0),n2(0),symb(0),mathans(0),pcard1,pcard2,dcard1,dcard2,pcardsum,dcardsum,pot(10),playeraces(0),dealeraces(0),roundCounter(1),pressedButton(4),krazyrounds,reactrnd,reactanswer(0),volume(50);



void setup() {
  Serial.begin(9600);                       //begin setup
  randomSeed(analogRead(0));
  analogWrite(9,Contrast);
  potPosition = analogRead(A3);    //set potPosition to a number between 0 and 1023 based on how far the knob is turned
  lcd.begin(16, 2);                         //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                              //clear the display
  noTone(speakerPin);

//set all of the button pins to input_pullup (use the built-in pull-up resistors)
  pinMode(button[0], INPUT_PULLUP);
  pinMode(button[1], INPUT_PULLUP);
  pinMode(button[2], INPUT_PULLUP);
  pinMode(button[3], INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);   //set the buzzer pin to output
}

void loop() {  
  lcd.clear();                              //clear the LCD
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Mini Mania");                      //print label Seed
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("Blue to Start");             //print user intructions

  exitmod = false;
  if (digitalRead(button[3]) == LOW){
    menu();
  }
  if (digitalRead(button[1]) == LOW && Contrast < 200 ){
    Contrast = Contrast + 5;
    analogWrite(9,Contrast);
    delay(250);
  }
  if (digitalRead(button[0]) == LOW && Contrast > 20){
    Contrast = Contrast - 5;
    analogWrite(9,Contrast);
    delay(250);
  }
}

void menu() {
  i = 0;
  lcd.clear();                              //clear the LCD
  delay(1000);
  while(exitmod == false){
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Menu:");                      //print label Seed
    lcd.setCursor(6, 0);
    lcd.print(menusecs[i]);                      //print label Seed
    if(digitalRead(button[3]) == LOW){
      if(i == 0){
        gameselect();
      }
      if(i == 1){
        addplayers();
      }
      if(i == 2){
        settings();
      }
      if(i == 3){
        i = 0;
        exitmod = true;
        return 0;
      }
    }
    if (digitalRead(button[0]) == LOW){
      delay(250);
      lcd.clear();
      if(i < 3){
        i++;
      }
      else{
        i -= 3;
      }
    }
    if (digitalRead(button[1]) == LOW){
      delay(250);
      lcd.clear();
      if(i > 0){
        i--;
      }
      else{
        i += 3;
      }
    }
  }
}

void gameselect(){
  j = 0;
  delay(250);
  lcd.clear();                              //clear the LCD
  do{
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Game Select:");                      //print label Seed
    lcd.setCursor(0, 1);
    lcd.print(game[j]);                      //print label Seed
    if(digitalRead(button[3]) == LOW){
      if(j == 0){
        mathgame();
      }
      if(j == 1){
        simonhears();
      }
      if(j == 2){
        react();
      }
      if(j == 3){
        blackjack();
      }
      if(j == 4){
        krazymode();
      }
      if(j == 5){
        menu();
      }
    }
    if (digitalRead(button[0]) == LOW){
      delay(250);
      lcd.clear();
      if(j < 5){
        j++;
      }
      else{
        j -= 5;
      }
    }
    if (digitalRead(button[1]) == LOW){
      delay(250);
      lcd.clear();
      if(j > 0){
        j--;
      }
      else{
        j += 5;
      }}
      }while (digitalRead(button[3]) == HIGH);
      lcd.clear();
      }

void krazymode () {
  delay(250);
  lcd.clear();                              //clear the LCD
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Welcome to");                      //print label Seed
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("Krazy Mode");             //print user intructions
  delay(3000);
  lcd.clear();                              //clear the LCD
  addplayers();
  if(host == true){ 
      for(int i = 0; i < 3; i++){
        int r = random(0,3);
        int temp3 = gamecode[i];
        gamecode[i] = gamecode[r];
        gamecode[r] = temp3;
      }
      do{
        lcd.setCursor(0,0);
        lcd.print("Gamecode: ");
        lcd.print(gamecode[0]);
        lcd.print(gamecode[1]);
        lcd.print(gamecode[2]);
        lcd.print(gamecode[3]);
        delay(250);
    }while(digitalRead(button[3]) == HIGH);
    }
  else {
    lcd.setCursor(0,0);
    lcd.print("Enter");
    lcd.setCursor(0,1);
    lcd.print("Gamecode: ");
    for(int i = 0; i < 4; i++){
        do{
          lcd.setCursor(10+i,0);
          lcd.print(gamecode[i]);
          if (digitalRead(button[0]) == LOW){
            if(gamecode[i] < 4){
              gamecode[i]++;
            }
            else if (gamecode[i] == 4){
              gamecode[i] = 1;
            }
            delay(250);
          }
          if (digitalRead(button[1]) == LOW){
            if(gamecode[i] > 1){
              gamecode[i]--;
            }
            else if (gamecode[i] == 1){
              gamecode[i] = 4;
            }
            delay(250);
        }
       }while (digitalRead(button[3]) == HIGH);
       delay(250);
      }
  }
  lcd.clear();
  delay(500);
  do{
    lcd.setCursor(0,0);
    lcd.print("Waiting for");
    lcd.setCursor(0,1);
    lcd.print("Other Players");
  }while (digitalRead(button[3]) == HIGH);
  delay(250);
  for (int i = 0; i < 4; i++){
    krazyrounds = i + 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game ");
    lcd.print(i+1);
    lcd.print(": ");
    lcd.setCursor(0,1);
    if(gamecode[i] == 1){
      lcd.print(game[gamecode[i]-1]);
      delay(3000);
      mathgame();
    }
    if(gamecode[i] == 2){
      lcd.print(game[gamecode[i]-1]);
      delay(3000);
      simonhears();
    }
    if(gamecode[i] == 3){
      lcd.print(game[gamecode[i]-1]);
      delay(3000);
      react();
    }
    if(gamecode[i] == 4){
      lcd.print(game[gamecode[i]-1]);
      delay(3000);
      blackjack();
    }
  }
  lcd.clear(); 
  for(int i = 0; i < players - 1; i++){
    for(int j = i+1; j < players; j++){
      if(score[i] < score[j]){
        temp1 = score[i];
        score[i] = score[j];
        score[j] = temp1;
        temp2 = player[i];
        player[i] = player[j];
        player[j] = temp2;
      }
    }
  }
  if(user == player[0]){
    for(int i = 0; i < 10; i++){
      lcd.setCursor(5,0);
      lcd.print("WINNER!");
      delay(500);
      lcd.clear();
      delay(500);
    }}
   else{
    lcd.setCursor(5,0);
    lcd.print("YOU LOSE");
    lcd.setCursor(0,1);
    lcd.print("Player ");
    lcd.print(player[0]);
    lcd.print(" Won");
    delay(10000);
   }    
}  

void addplayers() {
  lcd.clear();                              //clear the LCD
  delay(250);
  do{
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Players: ");                      //print label Seed
    lcd.print(players);                            //print user intructions
    if (digitalRead(button[0]) == LOW){
      players = players + 1;
      delay(250);
    }
    if (digitalRead(button[1]) == LOW){
      players = players - 1;
      delay(250);
    }
  }while(digitalRead(button[3]) == HIGH);
  delay(250);
  lcd.clear();                              //clear the LCD
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Players: ");                      //print label Seed
  lcd.print(players);                            //print user intructions
  do{
  lcd.setCursor(0, 1);                      //set the cursor to the top left position
  lcd.print("Your Player:");                      //print label Seed
  lcd.print(user);                            //print user intructions
  if (digitalRead(button[0]) == LOW){
    if(user < players){
      user++;
    }
    else if (user == players){
      user = 1;
    }
    delay(250);
  }
  if (digitalRead(button[1]) == LOW){
    if(user > 1){
      user--;
    }
    else if (user == 1){
      user = players;
    }
    delay(250);
  }
  }while (digitalRead(button[3]) == HIGH);
  delay(250);
  if (user == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You Are the Host");
    host = true;
    delay(3000);
  }
  else {
    host = false;
  }
  lcd.clear();
}

void scorecounter(int x) {
    n = 0;
    h = 0;
    t = 0;
    o = 0;
  for (int i = 0; i < players; i++){
  lcd.clear();                              //clear the LCD
  delay(500);
  if(user == i+1){
    roundscore[i] = x;
  }
  else{
  if (control == 0){
  while (digitalRead(button[3]) == HIGH){
    lcd.setCursor(0, 0);                      //set the cursor to the bottom left position
    lcd.print("Enter P");
    lcd.print(i+1);
    lcd.print("'s score");
    lcd.setCursor(0, 1);                      //set the cursor to the top left position

    lcd.print(h);                             //print label Seed
    lcd.print(t);                             //print label Seed
    lcd.print(o);                             //print label Seed
    
    if (digitalRead(button[0]) == LOW){
      if(t < 9){
        n = n + 10;
        h = h + 1;
        delay(250);
      }
      else{
        n = n - 90;
        h = h - 9;
        delay(250);
      }
    }
    if (digitalRead(button[1]) == LOW){
      if(t < 9){
        n = n + 10;
        t = t + 1;
        delay(250);
      }
      else{
        n = n - 90;
        t = t - 9;
        delay(250);
      }
    }
    if (digitalRead(button[2]) == LOW){
      if(o < 9){
        n = n + 1;
        o = o + 1;
        delay(250);
      }
      else{
        n = n - 9;
        o = o - 9;
        delay(250);
      }
    }
    }}
    else if (control == 1){
      setpot = analogRead(A5);
      while (digitalRead(button[3]) == HIGH){
        lcd.setCursor(0, 0);                      //set the cursor to the bottom left position
        lcd.print("Enter P");
        lcd.print(i+1);
        lcd.print("'s score");
        potPosition = analogRead(A3);
        if (n < 10){
          lcd.setCursor(13,1);
          lcd.print("  ");
          lcd.setCursor(15, 1);                      //set the cursor to the top left position
        }
        else if (n < 100){
          lcd.setCursor(13,1);
          lcd.print(" ");
          lcd.setCursor(14, 1);                      //set the cursor to the top left position
        }
        else {
           lcd.setCursor(13, 1);                      //set the cursor to the top left position
        }
       n = (1023-potPosition)/20;
       lcd.print(n);                             //print label Seed
        if(potPosition =! setpot){
          lcd.clear();
          setpot = potPosition;
        }
    }}
  roundscore[i] = n;
  }
  score[i] = score[i] + roundscore[i];
  }
  
  lcd.clear(); 
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Scoreboard:");                      //print label Seed
  delay(3000);
  if (players >= 1){
    lcd.clear(); 
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("P1:");                      //print label Seed
    lcd.print(score[0]);
  }
  if(players >= 2){
    lcd.setCursor(8, 0);                      //set the cursor to the bottom left position
    lcd.print("P2:");                      //print label Seed
    lcd.print(score[1]);
  }
  if (players >= 3){
    lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
    lcd.print("P3:");                      //print label Seed
    lcd.print(score[2]);
  }  
  if (players >= 4){
    lcd.setCursor(8, 1);                      //set the cursor to the bottom left position
    lcd.print("P4:");                      //print label Seed
    lcd.print(score[3]);
  }
  delay(5000);
  lcd.clear();
}

void mathgame() {
  mathscore = 0;
  lcd.clear();
  lcd.noBlink();
  while(digitalRead(button[3]) == HIGH){
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Welcome to");                      //print label Seed
    lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
    lcd.print("Fast Math");             //print user intructions
  }
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Ready");                      //print label Seed
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("3");                      //print label Seed
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("2");                      //print label Seed
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("1");                      //print label Seed
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("GO!");                      //print label Seed
  delay(1000);
  mathstarttime = millis();                 //record the start time
  for (int i = 0; i<10; i++){
    h = 0;
    t = 0;
    o = 0;
    n = 0;
    n1 = random(1,21);
    n2 = random(1,21);
    symb = random(0,4);
    lcd.clear();
    if (control == 0){
    do{
    if (symb == 0){
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print(n1);
      lcd.print(mathsym[0]);
      lcd.print(n2);
      lcd.print("?");
      mathans = n1 + n2;
    }
    else if (symb == 1){
      if (n1 > n2){
        lcd.setCursor(0, 0);                      //set the cursor to the top left position
        lcd.print(n1);
        lcd.print(mathsym[1]);
        lcd.print(n2);
        lcd.print("?");
        mathans = n1 - n2;
      }
      else {
        lcd.setCursor(0, 0);                      //set the cursor to the top left position
        lcd.print(n2);
        lcd.print(mathsym[1]);
        lcd.print(n1);
        lcd.print("?");
        mathans = n2 - n1;
      }
    }
    else if (symb == 2){
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print(n1);
      lcd.print(mathsym[2]);
      lcd.print(n2);
      lcd.print("?");
      mathans = n1 * n2;
    }
    else if (symb == 3){
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print(n1*n2);
      lcd.print(mathsym[3]);
      lcd.print(n1);
      lcd.print("?");
      mathans = n2;
    }
    lcd.setCursor(0, 1);                      //set the cursor to the top left position
    lcd.print(h);                             //print label Seed
    lcd.print(t);                             //print label Seed
    lcd.print(o);                             //print label Seed
    
    if (digitalRead(button[0]) == LOW){
    n = n + 100;
    h = h + 1;
    delay(250);
    }
    if (digitalRead(button[1]) == LOW){
      if(t < 9){
        n = n + 10;
        t = t + 1;
        delay(250);
      }
      else{
        n = n - 90;
        t = t - 9;
        delay(250);
      }
    }
    if (digitalRead(button[2]) == LOW){
      if(o < 9){
        n = n + 1;
        o = o + 1;
        delay(250);
      }
      else{
        n = n - 9;
        o = o - 9;
        delay(250);
      }
    }
    }while(digitalRead(button[3]) == HIGH);
    }
    else if (control = 1){
      setpot = analogRead(A5);
      do{
        potPosition = analogRead(A3);
        if (symb == 0){
          lcd.setCursor(0, 0);                      //set the cursor to the top left position
          lcd.print(n1);
          lcd.print(mathsym[0]);
          lcd.print(n2);
          lcd.print("?");
          mathans = n1 + n2;
        }
        else if (symb == 1){
          if (n1 > n2){
            lcd.setCursor(0, 0);                      //set the cursor to the top left position
            lcd.print(n1);
            lcd.print(mathsym[1]);
            lcd.print(n2);
            lcd.print("?");
            mathans = n1 - n2;
          }
          else {
            lcd.setCursor(0, 0);                      //set the cursor to the top left position
            lcd.print(n2);
            lcd.print(mathsym[1]);
            lcd.print(n1);
            lcd.print("?");
            mathans = n2 - n1;
          }
        }
        else if (symb == 2){
          lcd.setCursor(0, 0);                      //set the cursor to the top left position
          lcd.print(n1);
          lcd.print(mathsym[2]);
          lcd.print(n2);
          lcd.print("?");
          mathans = n1 * n2;
        }
        else if (symb == 3){
       
          lcd.setCursor(0, 0);                      //set the cursor to the top left position
          lcd.print(n1*n2);
          lcd.print(mathsym[3]);
          lcd.print(n1);
          lcd.print("?");
          mathans = n2;
        }
        if (n < 10){
          lcd.setCursor(13,0);
          lcd.print("  ");
          lcd.setCursor(15, 0);                      //set the cursor to the top left position
        }
        else if (n < 100){
          lcd.setCursor(13,0);
          lcd.print(" ");
          lcd.setCursor(14, 0);                      //set the cursor to the top left position
        }
        else {
           lcd.setCursor(13, 0);                      //set the cursor to the top left position
        }
        lcd.print(n);                             //print label Seed
        if(mathans < 50){
          n = (1023-potPosition)/20;
        }
        else if(mathans < 100){
          n = (1023-potPosition)/10;
        }
        else if(mathans < 200){
          n = (1023-potPosition)/5;
        }
        else {
          n = (1023-potPosition)/4 + 100;
        }
        if(potPosition =! setpot){
          lcd.clear();
          setpot = potPosition;
        }
        
        }while(digitalRead(button[3]) == HIGH);
    }
    delay(250);
    if(n == mathans){
      green();
      lcd.clear();
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print("Correct");
      lcd.setCursor(0, 1);                      //set the cursor to the top left position
      lcd.print(9-i);
      lcd.setCursor(2, 1);                      //set the cursor to the top left position
      lcd.print("more rounds");
      tone(speakerPin, 3135, 500);   //G7
      delay(500);
      noTone(speakerPin);
      delay(500);
      turnOff();
    }
    else{
      red();
      lcd.clear();
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print("Incorrect");
      lcd.setCursor(0, 1);                      //set the cursor to the top left position
      lcd.print(10-i);
      lcd.setCursor(2, 1);                      //set the cursor to the top left position
      lcd.print("more rounds");
      tone(speakerPin, 65, 150);   //E7
      delay(175);
      tone(speakerPin, 65, 150);   //E7
      delay(175);
      noTone(speakerPin);
      delay(750);
      i = i - 1;
      turnOff();
    }
  }
  mathendtime = millis();
  mathtime = (mathendtime-mathstarttime)/1000;
  mathscore = 1000/mathtime;
  lcd.clear();
  lcd.setCursor(0, 0);                      //set the cursor to the bottom left position
  lcd.print("Time: ");             //print user intructions
  lcd.print(mathtime);
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("Score: ");             //print user intructions
  lcd.print(mathscore);
  recentscore = mathscore;
  delay(10000);
  if(comp == true){
  scorecounter(recentscore);
  }
}

void simonhears() {
  lcd.clear();                              //clear the LCD
  delay(1000);
  while(digitalRead(button[3]) == HIGH){
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Welcome to");                      //print label Seed
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("Simon Hears");             //print user intructions
  }
  lcd.clear();                              //clear the LCD
  do{
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Sound Test");                      //print label Seed
    lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
    lcd.print("Blue to Start");             //print user intructions
    
    if (digitalRead(button[0]) == LOW) {      //if the first key is pressed
    tone(speakerPin, tones[0]);                     //play the frequency for c
    }
    else if (digitalRead(button[1]) == LOW) { //if the second key is pressed
      tone(speakerPin, tones[1]);                     //play the frequency for e
    }
    else if (digitalRead(button[2]) == LOW) { //if the third key is pressed
      tone(speakerPin, tones[2]);                     //play the frequency for g
    }
    else {
      noTone(speakerPin);                        //if no key is pressed turn the buzzer off
    }
  }while(digitalRead(button[3]) == HIGH);
  lcd.clear();
  //Actual Code
  delay(250);
  if (gameStarted == false) {   //if the game hasn't started yet
    startSequence();            //flash the start sequence
    roundCounter = 0;           //reset the round counter
    delay(1500);                //wait a second and a half
    gameStarted = true;         //set gameStarted to true so that this sequence doesn't start again
  }

  do{
    lcd.setCursor(0, 0);  
    lcd.print("Round: ");                 //Print a label for the data
    lcd.print(roundCounter+1);                      //print the degrees Fahrenheit
    lcd.print("/");
    lcd.print(roundsToWin);
  for (int i = 0; i <= roundCounter; i++) { //go through the array up to the current round number
    flashLED(buttonSequence[i]); 
   // {pinMode(button[i], INPUT_PULLUP);      //if the first key is pressed
   // tone(speakerPin, 262) 
    //turn on the LED for that array position and play the sound
    delay(250);                           //wait
    allLEDoff();
    delay(125);
    
  }

  //then start going through the sequence one at a time and see if the user presses the correct button
  for (int i = 0; i <= roundCounter; i++) { //for each button to be pressed in the sequence
    startTime = millis();                 //record the start time
    while (gameStarted == true) { //loop until the player presses a button or the time limit is up (the time limit check is in an if statement)
      pressedButton = buttonCheck();      //every loop check to see which button is pressed
      if (pressedButton < 4) {            //if a button is pressed... (4 means that no button is pressed)
        flashLED(pressedButton);          //flash the LED for the button that was pressed
        if (pressedButton == buttonSequence[i]) { //if the button matches the button in the sequence
          delay(250);                   //leave the LED light on for a moment
          allLEDoff();                  //then turn off all of the lights and
          break;                        //end the while loop (this will go to the next number in the for loop)
        } else {                          //if the button doesn't match the button in the sequence
          loseSequence();               //play the lose sequence (the loose sequence stops the program)
          break;                        //when the program gets back from the lose sequence, break the while loop so that the game can start over
        }
      } else {                            //if no button is pressed
        allLEDoff();                      //turn all the LEDs off
      }
      //check to see if the time limit is up
      if (millis() - startTime > timeLimit) { //if the time limit is up
        loseSequence();                       //play the lose sequence
        break;                                //when the program gets back from the lose sequence, break the while loop so that the game can start over
      }
    }
  }

  if (gameStarted == true) {
    roundCounter = roundCounter + 1;      //increase the round number by 1
    lcd.setCursor(0, 0);  
    lcd.print("Round: ");                 //Print a label for the data
    lcd.print(roundCounter);                      //print the degrees Fahrenheit
    lcd.print("/");
    lcd.print(roundsToWin);

    if (roundCounter >= roundsToWin) {              //if the player has gotten to the 16th round
      winSequence();                      //play the winning song
    }

    delay(500);                           //wait for half a second between rounds
  }
  }while(gameStarted == true);
}

void flashLED (int ledNumber) {
  tone(speakerPin, tones[ledNumber]);
}

void allLEDoff () {
 
  //turn the buzzer off
  noTone(speakerPin);
}

int buttonCheck() {
  //check if any buttons are being pressed
  if (digitalRead(button[0]) == LOW) {
    return 0;
  } else if (digitalRead(button[1]) == LOW) {
    return 1;
  } else if (digitalRead(button[2]) == LOW) {
    return 2;
  } else if (digitalRead(button[3]) == LOW) {
    return 3;
  } else {
    return 4; //this will be the value for no button being pressed
  }
}

void startSequence() {

  randomSeed(analogRead(A0));   //make sure the random numbers are really random
  for (int i = 0; i <= roundsToWin; i++) {
    buttonSequence[i] = round(random(0, 4));
  }
  for (int i = 0; i <= 3; i++) {
    tone(speakerPin, tones[i], 200); //play one of the 4 tones
    delay(200);         //wait for a moment
  }
}

void winSequence() {

  //turn all the LEDs on
  for (int j = 0; j <= 3; j++) {
   
  }

  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("You Win!");
  
  //play the 1Up noise
  tone(speakerPin, 1318, 150);   //E6
  delay(175);
  tone(speakerPin, 1567, 150);   //G6
  delay(175);
  tone(speakerPin, 2637, 150);   //E7
  delay(175);
  tone(speakerPin, 2093, 150);   //C7
  delay(175);
  tone(speakerPin, 2349, 150);   //D7
  delay(175);
  tone(speakerPin, 3135, 500);   //G7
  delay(500);

  //wait until a button is pressed
  delay(5000);
  recentscore = roundCounter;
  gameStarted = false;   //reset the game so that the start sequence will play again.
  if(comp == true){
  scorecounter(recentscore);
  }

}

void loseSequence() {

  //turn all the LEDs on
  for (int j = 0; j <= 3; j++) {
    
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Score: ");
  lcd.setCursor(0,1);
  lcd.print(roundCounter);
  lcd.print(" rounds");
  
  //play the 1Up noise
  tone(speakerPin, 130, 250);   //E6
  delay(275);
  tone(speakerPin, 73, 250);   //G6
  delay(275);
  tone(speakerPin, 65, 150);   //E7
  delay(175);
  tone(speakerPin, 98, 500);   //C7
  delay(500);
  recentscore = roundCounter;
 delay(5000);
  
  gameStarted = false;   //reset the game so that the start sequence will play again.
  if(comp == true){
  scorecounter(recentscore);
  }
}

void react() {
  lcd.clear();                              //clear the LCD
  delay(1000);
  while(digitalRead(button[3]) == HIGH){
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Welcome to");                      //print label Seed
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("React");             //print user intructions
  }
  for (int i = 0; i<10; i++){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Round ");
    lcd.print(i+1);
    lcd.print("/10");
    delay(3000);
    randomtime = random(3000,5001);
    reactrnd = random(0,4);
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Wait");
    delay(randomtime);
    lcd.clear();
    reactanswer = 0;
    reacttime1 = millis();
    reactloop = true;
    while(reactloop == true){
      lcd.setCursor(6,0);
      lcd.print(colors[reactrnd]);
      if(reactrnd == 0){
        red();
      }
      if(reactrnd ==1){
        yellow();
      }
      if(reactrnd == 2){
        green();
      }
      if(reactrnd == 3){
        blue();
      }
      pressedButton = buttonCheck();      //every loop check to see which button is pressed
      if (pressedButton < 4) {            //if a button is pressed... (4 means that no button is pressed)
        reactloop = false;
        if (pressedButton == reactrnd) { //if the button matches the button in the sequence
          reactanswer = 1;
          reacttime2 = millis();
        } else {                          //if the button doesn't match the button in the sequence
          reactanswer = 2;                        //when the program gets back from the lose sequence, break the while loop so that the game can start over
        }
      }
      if (millis() - reacttime1 > reacttimelimit) { //if the time limit is up
        reactloop == false;
        reactanswer = 3;                                //when the program gets back from the lose sequence, break the while loop so that the game can start over
      }
    }
    if(reactanswer == 1){
      turnOff();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Time: ");
      lcd.print(reacttime2 - reacttime1 - 250);
      timesum = timesum + (reacttime2 - reacttime1);
      tone(speakerPin, 3135, 500);   //G7
      delay(500);
      noTone(speakerPin);
      delay(3000);
       
    }
    else if(reactanswer == 2){
      turnOff();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Wrong Button");
      timesum = timesum + reacttimelimit;
      tone(speakerPin, 65, 150);   //E7
      delay(175);
      tone(speakerPin, 65, 150);   //E7
      delay(175);
      noTone(speakerPin);
      delay(3000); 
    }
    else if(reactanswer == 3){
      turnOff();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Out of Time");
      timesum = timesum + reacttimelimit;
      tone(speakerPin, 65, 150);   //E7
      delay(175);
      tone(speakerPin, 65, 150);   //E7
      delay(175);
      noTone(speakerPin);
      delay(3000); 
    }
  }
  avgtime = timesum/10;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AVG Time: ");
  lcd.print(avgtime);
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(5000/avgtime);
  recentscore = 5000/avgtime;
  delay(10000);
  if(comp == true){
  scorecounter(recentscore);
  }
}

void blackjack () {
  lcd.clear();                              //clear the LCD
  delay(250);
  while(digitalRead(button[3]) == HIGH){
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Welcome to");                      //print label Seed
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("Blackjack");             //print user intructions
  }
  for (int i = 0; i<5; i++){
     pcard1 = 0;
     pcard2 = 0;
     dcard1 = 0;
     dcard2 = 0;
     pcardsum = 0;
     dcardsum = 0;
     playeraces = 0;
     dealeraces = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Round ");
  lcd.print(i+1);
  lcd.print("/5");
  lcd.setCursor(0,1);
  lcd.print("Pot:");
  lcd.print(pot);
  delay(3000);
  lcd.clear();
  int rndp1 = random(0,12);
  if(rndp1 == 0){
    playeraces++;
  }
  int rndp2 = random(0,12);
  if(rndp2 == 0){
    playeraces++;
  }
  int rndd1 = random(0,12);
  if(rndd1 == 0){
    dealeraces++;
  }
  int rndd2 = random(0,12);
  if(rndd2 == 0){
    dealeraces++;
  }
  if(playeraces =! 2) {
    pcardsum = cardvals[rndp1] + cardvals[rndp2];
  }
  else{
    pcardsum = cardvals[rndp1] + cardvals[rndp2] - 10;
  }
  if(dealeraces =! 2) {
    dcardsum = cardvals[rndd1] + cardvals[rndd2];
  }
  else {
    dcardsum = cardvals[rndd1] + cardvals[rndd2] - 10;
  }
  dcardsum = cardvals[rndd1] + cardvals[rndd2];
  if(pcardsum == 21 && dcardsum == 21){
    lcd.clear();
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("D:");
    lcd.print(cards[rndd1]);
    lcd.print(",");
    lcd.print(cards[rndd2]);
    lcd.print("  P:");
    lcd.print(cards[rndp1]);
    lcd.print(",");
    lcd.print(cards[rndp2]);
    lcd.setCursor(0, 1);                      //set the cursor to the top left position
    lcd.print("Double Blackjack");
    delay(5000);
  }
  else if(dcardsum == 21){
    lcd.clear();
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("D:");
    lcd.print(cards[rndd1]);
    lcd.print(",");
    lcd.print(cards[rndd2]);
    lcd.print("  P:");
    lcd.print(cards[rndp1]);
    lcd.print(",");
    lcd.print(cards[rndp2]);
    lcd.setCursor(0, 1);                      //set the cursor to the top left position
    lcd.print("Dealer Blackjack");
    delay(5000);
    pot = pot - 1;
  }
  else if(pcardsum == 21){
    lcd.clear();
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("D:");
    lcd.print(cards[rndd1]);
    lcd.print(",");
    lcd.print(cards[rndd2]);
    lcd.print("  P:");
    lcd.print(cards[rndp1]);
    lcd.print(",");
    lcd.print(cards[rndp2]);
    lcd.setCursor(0, 1);                      //set the cursor to the top left position
    lcd.print("Blackjack");
    delay(5000);
    pot = pot + 1;;
  }
  else{
    lcd.clear();
      do{
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print("D:");
      lcd.print(cards[rndd1]);
      lcd.print(",?");
      lcd.print("  P:");
      lcd.print(cards[rndp1]);
      lcd.print(",");
      lcd.print(cards[rndp2]);
      lcd.print("-");
      lcd.print(pcardsum);
      lcd.setCursor(0, 1);
      lcd.print("Hit or stand");
      if(digitalRead(button[0]) == LOW){
        delay(250);
        int rndpnextcard;
        lcd.clear();
        lcd.print("D:");
        lcd.print(cards[rndd1]);
        lcd.print(",?");
        lcd.print("  P:");
        lcd.print(cards[rndp1]);
        lcd.print(",");
        lcd.print(cards[rndp2]);
        lcd.print("-");
        lcd.print(pcardsum);
        lcd.setCursor(0, 1);
        lcd.print("Next Card:");
        rndpnextcard = random(0,12);
        if (rndpnextcard == 0){
          playeraces++;
        }
        lcd.print(cards[rndpnextcard]);
        pcardsum = pcardsum + cardvals[rndpnextcard];
        if(pcardsum > 21 && playeraces > 0){
          pcardsum = pcardsum - 10;
          playeraces = playeraces - 1;
        }
        delay(2000);
      }
      }while(digitalRead(button[1]) == HIGH && pcardsum < 21);
    if (pcardsum > 21){
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Bust");
      delay(5000);
      pot = pot - 1;
    }
    else {
      lcd.clear();
      while(dcardsum < 17){
      int rnddnextcard = random(0,12);
      if(rnddnextcard == 0){
        dealeraces++;
      }
      lcd.setCursor(0, 0);                      //set the cursor to the top left position
      lcd.print("P:");
      lcd.print(pcardsum);
      lcd.print(", D:");
      lcd.print(cards[rndd1]);
      lcd.print(",");
      lcd.print(cards[rndd2]);
      lcd.print("-");
      lcd.print(dcardsum);
      lcd.setCursor(10, 1);
      lcd.print("  ");
      lcd.setCursor(0, 1);                      //set the cursor to the top left position
      lcd.print("Next Card:");
      lcd.print(cards[rnddnextcard]);
      dcardsum = dcardsum + cardvals[rnddnextcard];
      if(dcardsum > 21 && dealeraces > 0){
        dcardsum = dcardsum - 10;
        dealeraces = dealeraces - 1;
      }
      delay(3000);
    }
    if(dcardsum > 21){
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Dealer Bust");
      pot = pot + 1;
      delay(5000);
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("P:");
      lcd.print(pcardsum);
      lcd.print(",");
      lcd.print("D:");
      lcd.print(dcardsum);
      if(pcardsum > dcardsum){
        lcd.setCursor(0,1);
        lcd.print("You Win");
        delay(5000);
        pot = pot + 1;
      }
      if(pcardsum < dcardsum){
        lcd.setCursor(0,1);
        lcd.print("You Lose");
        delay(5000);
        pot = pot - 1;
      }
      if(pcardsum == dcardsum){
        lcd.setCursor(0,1);
        lcd.print("Draw");
        delay(5000);
      }
    }}}}
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Final Pot: ");
  lcd.print(pot);
  delay(10000);
  if(comp == true){
  scorecounter(recentscore);
  }
}

void settings() {
  lcd.clear();                              //clear the LCD
  setpot = analogRead(A5);
  while (digitalRead(button[3]) == HIGH){
  delay(1000);
  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Settings:");                      //print label Seed
  lcd.setCursor(0, 1);                      //set the cursor to the bottom left position
  lcd.print("");             //print user intructions
  delay(3000);
  }
  c = 0;
  lcd.clear();                              //clear the LCD
  delay(1000);
  exitmod2 = false;
  while(exitmod2 == false){
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Settings: ");                      //print label Seed
    lcd.setCursor(0, 1);
    lcd.print(settingsecs[c]);                      //print label Seed
    if(digitalRead(button[3]) == LOW){
      if(c == 0){
        compselect();
      }
      if(c == 1){
        controls();
      }
      if(c == 2){
        c = 0;
        exitmod2 = true;
        menu();
      }
    }
    if(digitalRead(button[2]) == LOW){
      exitmod2 = true;
        menu();
    }
      if (digitalRead(button[0]) == LOW){
        delay(250);
        lcd.clear();
        if(c < 2){
          c++;
        }
        else{
          c -= 2;
        }
      }
      if (digitalRead(button[1]) == LOW){
        delay(250);
        lcd.clear();
        if(c > 0){
          c--;
        }
        else{
          i += 2;
        }
      }
  }
}

void compselect (){
  lcd.clear();
  while(digitalRead(button[3]) == HIGH){
    lcd.setCursor(0,0);
    lcd.print("Keep Score: ");
    if(comp == true){
      lcd.setCursor(12,0);
      lcd.print("YES");
    }
    else{
      lcd.setCursor(12,0);
      lcd.print("   ");
      lcd.setCursor(12,0);
      lcd.print("NO");
    }
    if (digitalRead(button[0]) == LOW){
      delay(250);
      lcd.setCursor(12,0);
      lcd.print("   ");
      comp = true;
    }
    if (digitalRead(button[1]) == LOW){
      delay(250);
      lcd.setCursor(12,0);
      lcd.print("   ");
      comp = false;
    }
  }
}

void controls() 
{
  lcd.clear();
  do{
    lcd.setCursor(0, 0);                      //set the cursor to the top left position
    lcd.print("Controls:");                      //print label Seed
    if(control == 0){
      lcd.setCursor(12,0);
      lcd.print("Dial");
      lcd.setCursor(9,1);
      lcd.print("BUTTONS");
      
    }
    else if(control == 1){
      lcd.setCursor(12,0);
      lcd.print("DIAL");
      lcd.setCursor(9,1);
      lcd.print("Buttons");
    }
    if(digitalRead(button[0]) == LOW){
          control = 1;
        }
        if(digitalRead(button[1]) == LOW){
          control = 0;
        }
        if(digitalRead(button[2]) == LOW){
          lcd.clear();
          return 0;
        }
}while(digitalRead(button[2]) == HIGH);
}

void red ()
{
  //set the LED pins to values that make red
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
}

void yellow ()
{
  //set the LED pins to values that make yellow
  analogWrite(RedPin, 50);
  analogWrite(GreenPin, 25);
  analogWrite(BluePin, 0);
}

void green ()
{
  //set the LED pins to values that make green
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 0);
}

void blue ()
{
  //set the LED pins to values that make blue
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 100);
}

void turnOff () 
{
  //set all three LED pins to 0 or OFF
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
}
