// Benutzte Port:
// Schalter für Tests an Port 2 mit Pull Down Widerstand, d.h. HIGH entspricht AN
// Dauerlicht auf Port 3 und 4
// Ports 5 bis 69 werden genutzt
//
// Ports 0 und 1 werden nicht genutzt (wegen Kommunikation mit Serial Port)


// Konfiguration Periodendauer
unsigned long pause = 30000;      //  30s
unsigned long sequence = 270000;  // 270s



int testSwitch = 0;
int switchPort = 2;
bool testSwitchWasOn = false;

int startPort = 5;
int endPort = 69;

int ledState = LOW;

int i;

long lastSwitchTime;
long currentMillis;

unsigned long equalstep;
bool switchON = true;
bool switchOFF = false;

bool DEBUG = false;
long randNumber;
long randStep;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  if (DEBUG) {
    Serial.println(" Started Light Control");
    delay(250);
  }

  for (int i = startPort; i < endPort; i++) {
    pinMode(i, OUTPUT);
  }

  // Port 3,4 always active

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(3, 1);
  digitalWrite(4, 1);

  //  Port 2 -> switch all on
  int switchPort = 2;
  pinMode(switchPort, INPUT);

  i = startPort;


  equalstep = sequence / (endPort - startPort);
  randStep = equalstep;
  currentMillis = millis();
  lastSwitchTime = millis() + pause;  // for initial Pause

  // if (DEBUG) {
  //   Serial.print("lastSwitchTime : ");
  //   Serial.println((long)(lastSwitchTime));
  //   Serial.print("currentMillis : ");
  //   Serial.println((long)(currentMillis));
  //   delay(5000);
  // }
}

void loop() {

  currentMillis = millis();

  // if (DEBUG) {
  //   Serial.print("lastSwitchTime : ");
  //   Serial.println((long)(lastSwitchTime));
  //   Serial.print("currentMillis - lastSwitchTime : ");
  //   Serial.println((long)(currentMillis - lastSwitchTime));
  //   Serial.println("");
  //   delay(2000);
  // }


  // manual switch
  testSwitch = digitalRead(switchPort);

  if (testSwitch) {
    for (int i = startPort; i < endPort; i++) {
      digitalWrite(i, HIGH);
      testSwitchWasOn = true;

      if (DEBUG) {
        Serial.print(" Pressed Test Switch Activate Port:");
        Serial.println(i);
      }
    }
    i = startPort;
  } else {  // normal operation

    if (testSwitchWasOn) {
      for (int i = startPort; i < endPort; i++) {
        digitalWrite(i, LOW);
        testSwitchWasOn = false;
        lastSwitchTime = millis() + pause;
      }
    }


    // switch on

    if ((long)(currentMillis - lastSwitchTime) >= (long)(randStep) && switchON) {
      digitalWrite(i, HIGH);
      lastSwitchTime = millis();

      if (DEBUG) {
        Serial.print(" Set Port HIGH:");
        Serial.println(i);
        Serial.println(" ");
      }

      randNumber = random(1, 2 * equalstep );
      randStep = randNumber;
      if (DEBUG) {
        Serial.print(" Step:");
        Serial.println(randStep);
      }
i++;
      if (i >= endPort) {

        switchON = false;
        switchOFF = true;
        if (DEBUG) {
          Serial.print(" All Ports HIGH:");
          Serial.println(i);
        }

        i = startPort ;
        lastSwitchTime = millis() + pause;
      }
      
    }



    // switch off
    if ((long)(currentMillis - lastSwitchTime) >= (long)(randStep) && switchOFF) {
      digitalWrite(i, LOW);
      lastSwitchTime = millis();

      if (DEBUG) {
        Serial.print(" Set Port LOW:");
        Serial.println(i);
        Serial.println(" ");
      }
      randNumber = random(1, 2 * equalstep );
      randStep = randNumber;
      if (DEBUG) {
        Serial.print(" Step:");
        Serial.println(randStep);
      }
i++;
      if (i >= endPort) {

        switchON = true;
        switchOFF = false;
        if (DEBUG) {
          Serial.print(" All Ports LOW:");
          Serial.println(i);
        }
        i = startPort; 
        lastSwitchTime = millis() + pause;
      }
      
    }
  }
}
