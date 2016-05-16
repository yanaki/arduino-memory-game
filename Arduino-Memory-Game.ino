#include <Button.h>

Button button = Button(12,PULLUP);

const int NLEDS = 4; 
const Button LEDPINS[NLEDS] = {Button(12,PULLUP),Button(12,PULLUP),Button(12,PULLUP),Button(12,PULLUP)};
  
const int SWITCHPINS[NLEDS] = {
  8,9,10,11};
const int SEQDELAY = 200; // Millis between led flashes. Shorter is harder.
const int PAUSEB4SEQ = 1000; // Millis before starting the sequence.
const int MINLEVEL = 4;
const int MAXLEVEL = 16;
int gameLevel;
int simonSez[MAXLEVEL]; // sequence of 0..NLEDS-1

void setup() {
  // Analog in 0 should *not* be connected.
  // It's mama's little PRNG :)
  randomSeed(analogRead(0));
  gameLevel=MINLEVEL;
  for (byte l=0; l<NLEDS; l++) {
    pinMode(LEDPINS[l], OUTPUT);
  }
  //Serial.begin(9600);
  // Visual feedback after reset. Also good as a cable check :)
  playLoseSequence();
  playWinSequence(); 
}

void loop() {
  int done;
  initSequence(gameLevel);
  done = 0;
  while (!done) {
    getSwitchStroke();
    delay(PAUSEB4SEQ);
    playSequence(gameLevel);
    if (playerGuess(gameLevel)) {
      playWinSequence();
      done = 1;
      if (gameLevel<MAXLEVEL) {
        gameLevel++;
      }
    } 
    else {
      playLoseSequence();
    }
  }
}

void initSequence(int gameLevel) {
  // assertion: gameLevel<=MAXLEVEL
  for (int i=0; i<gameLevel; i++) {
    simonSez[i]=random(NLEDS);
  }
}

void playSequence(int gameLevel) {
  for (int i=0; i<gameLevel; i++) {
    setLed(simonSez[i]); // Flash the LED in the sequence
    delay(SEQDELAY);
    setLed(-1); // turn all LEDs off
    delay(SEQDELAY);
  }
}

void setLed(int theLed) {
  // if not 0<=theLed<NLEDS (e.g. -1), turn all off
  for (int l=0; l<NLEDS; l++) {
    digitalWrite(LEDPINS[l],l==theLed);
  }
}

int playerGuess(int gameLevel) {
  for (int i=0 ; i<gameLevel ; i++) {
    int guess=getSwitchStroke();
    // player feedback
    digitalWrite(LEDPINS[guess],HIGH);
    delay(250);
    digitalWrite(LEDPINS[guess],LOW);
    //Serial.print(guess,DEC);
    //Serial.print(",");
    //Serial.println(simonSez[i]);
    if (guess!=simonSez[i]) {
      return 0;
    }
  }
  return 1;
}

int playWinSequence() {
  for (int i=0; i<4*NLEDS; i++) {
    setLed(i%NLEDS); // Flash the LED in the sequence
    delay(50);
  }
  setLed(-1); // turn all LEDs off
}

int playLoseSequence() {
  for (int i=0; i<16; i++) {
    for (int l=0; l<NLEDS; l++) {
      digitalWrite(LEDPINS[l],i&1);
    }
    delay(50);
  }
  setLed(-1); // turn all LEDs off
}

int getSwitchStroke() {
  while (get1stLowSwitch()>=0) {
    // flush everything until no switch is pressed
    delay(50);
  }
  while (get1stLowSwitch()<0) {
    // wait for next press
    delay(50);
  }
  return get1stLowSwitch();
}

int get1stLowSwitch() {
  for (int i=0; i<NLEDS; i++) {
    if (!digitalRead(SWITCHPINS[i])) {
      return i;
    }
  }
  return -1;
}
