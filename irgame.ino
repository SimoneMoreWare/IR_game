#include <Wire.h> 
#include <IRremote.h> //libreria per utilizzare a pieno le potenzialità del ricevitore IR
#include <LiquidCrystal.h> //libreria per utilizzare a pieno le potenzialità del display

char up = 3;
char down = 1;
char ok = 2;

// Definizione di tre caratteri da utilizzare per il display LCD
byte up1[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte down1[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

byte ok1[8] = {
  B01110,
  B01010,
  B01110,
  B10000,
  B10100,
  B11000,
  B10100,
  B10010
};

int RECV_PIN = 2;  // Pin in cui è collegato il ricevitore IR

//Le varie scritte che compariranno sul display LCD le quali riportano le regole del gioco
char* howtoplay[23] = {
  "> FAST IR GAME <",
  "   Press key    ",
  "You have to",     
  "press the key,",
  "that you see on",
  "the screen, when",
  "it is beetween",
  "the fences (#).",
  "It ist getting",
  "faster and there",
  "will be symbols",
  "that you do not",
  "have to hit!",
  "Try to get to",
  "Level 10 and win",
  "the game. Before",
  "you start, you",
  "have to define",
  "the keys on your",
  "remote control",
  "to play the game", 
  "  Have fun !!",
  ""
};

int buzzer=13;

String keyNames[] = {"up", "down", "right", "left", "ok", "+", "-", "#", "*"}; //vettore nel quali sono riportati i tasti del telecomando da utilizzare
String keySymb[] = {"", "", "", "", "", "", "", "", ""}; 
long keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int keycount = 7;

int lev;
int xpos = 1;
int xadd = 1;
int xleft;
int xright;
int xstart = 0;
int xend = 15;

int actSym = 0;
int score;   //punteggio totale
int scorePerLev; //punteggio per livello
int scoreNextLev = 50;  // Punteggio dopo il quale si passera' al livello eccessivo

int gameState = -1;

bool pressed = false;

IRrecv irrecv(RECV_PIN);
decode_results results;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //i digital pin utilizzati dal display LCD
void setup()
{
  lcd.begin(16,2);  // colonne e righe che compongono il display
  lcd.createChar(up, up1);
  lcd.createChar(down, down1);
  lcd.createChar(ok, ok1);
  keySymb[0] = "\3";
  keySymb[1] = "\1";
  keySymb[2] = "\176";
  keySymb[3] = "\177";
  keySymb[4] = "\2";
  keySymb[5] = "+";
  keySymb[6] = "-";
  keySymb[7] = "#";
  keySymb[8] = "*";

  irrecv.enableIRIn(); // Si inizia con la ricezione
  
  info(); //funzione nella quale sono presente le istruzioni che permettono di leggere le regole
          //è possibile eliminare tale istruzione, in tal modo da saltare la presentazione delle regole
  randomSeed(analogRead(1));
}


void loop()
{

// definizione dei tasti da utilizzare 
  if (gameState == -1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Define Keys to");
    lcd.setCursor(0, 1);
    lcd.print("play the game...");
    tone(buzzer,440,500);
    delay(3000);
    pressed = false;
    for (int i = 0; i < keycount; i++) { 
      pressed = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Key for:");
      lcd.print(keyNames[i]);
      while (pressed != true) {
        if (irrecv.decode(&results)) {
          keys[i] = results.value;
          lcd.setCursor(0, 1);
          lcd.print(" ok!");
          Serial.println(keys[i]);
          delay(500);
          irrecv.resume(); //ricevo il prossimo valore
          pressed = true;
        }
      }
      pressed = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Repaet key!");
      lcd.print(keyNames[i]);
      irrecv.resume();
      while (pressed != true) {
        if (irrecv.decode(&results)) {
          if (keys[i] == results.value) {
            lcd.setCursor(0, 1);
            lcd.print("is the same!");
            tone(buzzer,440,500);
            delay(500);
            pressed = true;
          } else {
            lcd.setCursor(0, 1);
            lcd.print("wrong!");
            tone(buzzer,440,500);
            delay(500);
            lcd.setCursor(0, 1);
            lcd.print("       ");
          }
          irrecv.resume();
        }
      }
    }
    gameState = 0;  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Keys done!");
    tone(buzzer,440,500);
    delay(2000);
  }

  // Select Level
  if (gameState == 0) {
    lev = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select Level+ok");
    lcd.setCursor(0, 1);
    lcd.print("Level: ");
    lcd.print(lev);
    irrecv.resume();
    pressed = false;
    Serial.println("Level");
    Serial.println(pressed);
    while (pressed != true) {
      if (irrecv.decode(&results)) {
        Serial.println(results.value);
        if (results.value == keys[0]) lev++;
        if (results.value == keys[1]) lev--;
        if (results.value == keys[4]) pressed = true;
        if (lev < 1) lev = 1;
        if (lev > 10) lev = 10;
        lcd.setCursor(7, 1);
        lcd.print(lev);
        lcd.print(" ");
        irrecv.resume();
      }
      delay(250);
    }
    lcd.setCursor(0, 0);
    lcd.print("Ok! Play in    ");
    delay(2000);
    lcd.clear();
    gameState = 1; //gameplay principale
    score = 0;
    scorePerLev = 0;
    keycount = 7;
    xleft = 4;
    xright = 11;
    drawField("");
    irrecv.resume();
    Serial.println("Level Set");
  }

  // Main Game
  if (gameState == 1) {
    xpos = 0;
    xadd = 1;
    int k = 0;
    bool rightkey = false;
    pressed = false;
    actSym = floor(random(0, keycount));
    while (pressed != true) {
      Serial.println(xpos);
      if (irrecv.decode(&results)) {
        for (int i = 0; i < keycount; i++) {
          if (results.value == keys[i]) {
            rightkey = true;
            k = i;
          }
        }
        if (rightkey == true) {
          scorePerLev++;
          if (xpos <= xleft || xpos >= xright) {
            score = score - (4 + lev);
            tone(buzzer,880,300);
             delay(300);
          }
          if (actSym == k) {
            lcd.setCursor(xpos, 1);
            lcd.print(" ");
            score++;
            drawField("");
            changeDirection();
          } else {
            score = score - (2 + lev);
            drawField(" :(   ");
            tone(buzzer,880,300); 
            delay(300);
          }
          actSym = floor(random(0, keycount));
          rightkey = false;
        }
        delay(10);
        irrecv.resume();
        if (scorePerLev == scoreNextLev) {
          scorePerLev = 0;
          lev++;
          drawField("");
          if (lev < 11) {
            lcd.setCursor(0, 1);
            lcd.print("Next level!");
            waitForOK();
            // Check for score and display message here later
            lcd.setCursor(0, 1);
            lcd.print("           ");
          } else {
            gameState = 5;
            pressed = true;
          }
        }
      }
      lcd.setCursor(xpos, 1);
      lcd.print(" ");
      xpos = xpos + xadd;
      if (xpos == xend + 1 || xpos == xstart - 1) {
        if (actSym < 7) {
          score = score - (2 * (lev + 5));
          drawField(" :(   ");
        } else {
          drawField("");
        }
        changeDirection();
        actSym = floor(random(0, keycount));
        tone(buzzer,880,300); 
        delay(300);
      }
      lcd.setCursor(xpos, 1);
      lcd.print(keySymb[actSym]);
      delay(200 - (lev * 10));
      if (score < 0) {
        gameState = 9;
        pressed = true;
      }
    } // Main Game loop End

  }

  // Win
  // ##################
  if (gameState == 5) {
   tone(buzzer,440,300);
 delay(300);
    lcd.setCursor(0, 1);
    lcd.print("You win the Game");
    lcd.setCursor(0, 0);
    lcd.print("Bravo! ");
    tone(buzzer,440,500);
    waitForOK();
    gameState = 0;
  }

  // Game Over
  // ##################
  if (gameState == 9) {
    tone(buzzer,440,300); 
       delay(300);
    for (int i = 0; i < 5; i++) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("   Game over!   ");
      tone(buzzer,440,500);
      delay(300);
    }
    waitForOK();
    gameState = 0;
  }

}


void info () {
  int i = 0;
  while (howtoplay[i] != "") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(howtoplay[i]);
    lcd.setCursor(0, 1);
    lcd.print(howtoplay[i + 1]);
    tone(buzzer,440,500);
    delay(300);
    waitForKey();
    i++;
  }
  irrecv.resume();
}

void drawField(String empty) {
  Serial.println("drawField");
  int SCur;
  if (empty == "") empty = "      ";
  lcd.setCursor(0, 0);
  lcd.print("################");
  if (lev > 3) {
    xleft = floor(random(4, 7));
    xright = xleft + 7;
    if (lev > 6) xright = xleft + 6;
    if (lev > 9) xright = xleft + 5;
  }
  if (lev > 4) keycount = 8;
  if (lev > 6) keycount = 9;
  lcd.setCursor(xleft + 1, 0);
  lcd.print(empty.substring(0, xright - xleft - 1));
  lcd.setCursor(0, 0);
  lcd.print("L");
  lcd.print(lev);
  if (score < 1000) SCur = 13;
  if (score < 100) SCur = 14;
  if (score < 10) SCur = 15;
  if (score < 0) SCur = 14;
  lcd.setCursor(SCur, 0);
  lcd.print(score);
}

void changeDirection() {
  xpos = xstart;
  xadd = 1;
  if (lev > 3) {
    int dir = floor(random(0, 2));
    if (dir == 1) {
      xpos = xend;
      xadd = -1;
    }
  }
}

void waitForKey () {
  bool press = false;
  irrecv.resume();
  while (press == false) {
    if (irrecv.decode(&results)) {
      if (results.value != 0) press = true;
      irrecv.resume();
      delay(200);
    }
  }
}

void waitForOK()  {
  delay(1000);
  bool press = false;
  irrecv.resume();
  while (press == false) {
    if (irrecv.decode(&results)) {
      if (results.value == keys[4]) press = true;
      irrecv.resume();
      delay(200);
    }
  }
}
