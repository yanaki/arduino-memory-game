#include <Button.h>
#include <Tone.h>
#include <Potentiometer.h>

const int NLEDS = 4;
const int LEDPINS[NLEDS] = {
  4, 5, 6, 7
};
const int sAudioPin = 13;

const int PAUSEB4SEQ = 500;
const int MINLEVEL = 4;
const int MAXLEVEL = 7;
int gameLevel;
int memorySeq[MAXLEVEL];
int SEQDELAY = 500;
Potentiometer potentiometer = Potentiometer(A1, 5);

Button SWITCHPINS[NLEDS] = {
  Button(8, PULLUP), Button(9, PULLUP), Button(10, PULLUP), Button(11, PULLUP)
};

void setup() {
  randomSeed(analogRead(0));
  gameLevel = MINLEVEL;
  for (byte l = 0; l < NLEDS; l++) {
    pinMode(LEDPINS[l], OUTPUT);
    //    digitalWrite(LEDPINS[l], OUTPUT);
  }
  Serial.begin(9600);

  playLoseSequence();
  playWinSequence();
}

void loop() {
  int done;
  initSequence(gameLevel);
  done = 0;
  while (!done) {
    readButton(); //Start
    delay(PAUSEB4SEQ);
    playSequence(gameLevel);
    if (playerGuess(gameLevel)) {
      playWinSequence();
      done = 1;
      if (gameLevel < MAXLEVEL) {
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
  for (int i = 0; i < gameLevel; i++) {
    memorySeq[i] = random(NLEDS);
  }
}

void playSequence(int gameLevel) {
  Serial.println("====Begin=====");
  for (int i = 0; i < gameLevel; i++) {
    setLed(memorySeq[i]); // Flash the LED in the sequence
    delay(SEQDELAY);
    Serial.println(memorySeq[i] + 1);
    setLed(-1); // turn all LEDs off
    delay(SEQDELAY);
  }
}

void setLed(int theLed) {
  // if not 0<=theLed<NLEDS (e.g. -1), turn all off
  for (int l = 0; l < NLEDS; l++) {
    digitalWrite(LEDPINS[l], l == theLed);
  }
}

int playerGuess(int gameLevel) {
  for (int i = 0 ; i < gameLevel ; i++) {
    int guess = readButton();

    digitalWrite(LEDPINS[guess], HIGH);
    delay(SEQDELAY);
    digitalWrite(LEDPINS[guess], LOW);

    if (guess != memorySeq[i]) {
      return 0;
    }
  }
  return 1;
}

int readButton() {
  int newDelay = SEQDELAY;
  while (true) {  
    newDelay = potentiometer.getSector()*200;
    if(newDelay!=SEQDELAY) {
      SEQDELAY = newDelay;
      Serial.println("New Delay:" + String(newDelay));
    }
    for (int i = 0; i < NLEDS; i++) {
      if (SWITCHPINS[i].uniquePress()) {
        return i;
      }
    }
  }
}


int playWinSequence() {
  Serial.println("====RIGHT====");
  int NOTE_SUSTAIN = 100;
  for (uint8_t nLoop = 0; nLoop < 2; nLoop ++)
  {
    tone(sAudioPin, NOTE_A5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_B5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_C5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_B5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_C5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_D5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_C5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_D5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_E5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_D5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_E5);
    delay(NOTE_SUSTAIN);
    tone(sAudioPin, NOTE_E5);
    delay(NOTE_SUSTAIN);
  }
  noTone(sAudioPin);
}

int playLoseSequence() {
  Serial.println("====Error====");
  tone(sAudioPin, NOTE_G4);
  delay(250);
  tone(sAudioPin, NOTE_C4);
  delay(500);
  noTone(sAudioPin);
}
