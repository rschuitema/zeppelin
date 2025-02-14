
// pin definitions
const int FIGURE8_LED_RED = 12;
const int FIGURE8_LED_GREEN1 = 11;
const int FIGURE8_LED_GREEN2 = 10;
const int FIGURE8_LED_GREEN3 = 9;
const int FIGURE8_MAGNET = 8;
const int FIGURE8_REED_SWITCH = 7;

const int SHUTTLE_LED_RED = 6;
const int SHUTTLE_LED_GREEN = 5;
const int SHUTTLE_MAGNET = 4;
const int SHUTTLE_REED_SWITCH = 3;

const int START_BUTTON = 2;

void setFigure8Leds(int mode) {
  switch (mode)
  {
    case 1:
    {
      digitalWrite(FIGURE8_LED_GREEN1, HIGH);
      digitalWrite(FIGURE8_LED_GREEN2, LOW);
      digitalWrite(FIGURE8_LED_GREEN3, LOW);
      digitalWrite(FIGURE8_LED_RED, LOW);
      break;
    }
    case 2:
    {
      digitalWrite(FIGURE8_LED_GREEN1, HIGH);
      digitalWrite(FIGURE8_LED_GREEN2, HIGH);
      digitalWrite(FIGURE8_LED_GREEN3, LOW);
      digitalWrite(FIGURE8_LED_RED, LOW);
      break;
    }
    case 3:
    {
      digitalWrite(FIGURE8_LED_GREEN1, HIGH);
      digitalWrite(FIGURE8_LED_GREEN2, HIGH);
      digitalWrite(FIGURE8_LED_GREEN3, HIGH);
      digitalWrite(FIGURE8_LED_RED, LOW);
      break;
    }
    case 4:
    {
      digitalWrite(FIGURE8_LED_GREEN1, LOW);
      digitalWrite(FIGURE8_LED_GREEN2, LOW);
      digitalWrite(FIGURE8_LED_GREEN3, LOW);
      digitalWrite(FIGURE8_LED_RED, HIGH);
      break;
    }
    default:
    {
      digitalWrite(FIGURE8_LED_GREEN1, LOW);
      digitalWrite(FIGURE8_LED_GREEN2, LOW);
      digitalWrite(FIGURE8_LED_GREEN3, LOW);
      digitalWrite(FIGURE8_LED_RED, HIGH);
      break;
    }
  }
}

void setShuttleLeds(int mode) {
  switch (mode)
  {
    case 1:
    case 2:
    {
      digitalWrite(SHUTTLE_LED_GREEN, HIGH);
      digitalWrite(SHUTTLE_LED_RED, LOW);
      break;
    }
    case 3:
    {
      digitalWrite(SHUTTLE_LED_GREEN, LOW);
      digitalWrite(SHUTTLE_LED_RED, HIGH);
      break;
    }
    default:
    {
      digitalWrite(SHUTTLE_LED_GREEN, LOW);
      digitalWrite(SHUTTLE_LED_RED, HIGH);
      break;
    }
  }

}

void turnOnFigure8Magnet() {
    digitalWrite(FIGURE8_MAGNET, HIGH);
}

void turnOffFigure8Magnet() {
    digitalWrite(FIGURE8_MAGNET, LOW);
}

void turnOnShuttleMagnet() {
    digitalWrite(SHUTTLE_MAGNET, HIGH);
}

void turnOffShuttleMagnet() {
    digitalWrite(SHUTTLE_MAGNET, LOW);
}

int figure8ReedSwitchState = 0;
int lastFigure8ReedSwitchState = 0;
int figure8LapCounter = 0;
// function to count the number of pulses of the figure 8 reed switch.
// it detects pulses from high to low.
int countFigure8Laps() {

  figure8ReedSwitchState = digitalRead(FIGURE8_REED_SWITCH);
  //Serial.println(figure8ReedSwitchState);
  // compare the buttonState to its previous state
  if (figure8ReedSwitchState != lastFigure8ReedSwitchState)
  {
    //Serial.println(figure8ReedSwitchState);
    // if the state has changed, increment the counter
    if (figure8ReedSwitchState == LOW)
    {
      // if the current state is LOW then the reed switch went from open to closed
      //Serial.println(figure8LapCounter);
      figure8LapCounter++;
    }
    delay(50);

    lastFigure8ReedSwitchState = figure8ReedSwitchState;
  }

  return figure8LapCounter;
}

int shuttleReedSwitchState = 0;
int lastShuttleReedSwitchState = 0;
int shuttleLapCounter = 0;
// function to count the number of pulses of the shuttle reed switch.
// it detects pulses from high to low.
int countShuttleLaps() 
{
  shuttleReedSwitchState = digitalRead(SHUTTLE_REED_SWITCH);

  // compare the buttonState to its previous state
  if (shuttleReedSwitchState != lastShuttleReedSwitchState)
  {
    // if the state has changed, increment the counter
    if (shuttleReedSwitchState == LOW)
    {
      // if the current state is LOW then the reed switch went from open to closed
      shuttleLapCounter++;
    }
    delay(50);

    lastShuttleReedSwitchState = shuttleReedSwitchState;
  }

  return shuttleLapCounter;
}

void setup() 
{
  // initialize leds for figure 8 signal post
  pinMode(FIGURE8_LED_RED, OUTPUT);
  pinMode(FIGURE8_LED_GREEN1, OUTPUT);
  pinMode(FIGURE8_LED_GREEN2, OUTPUT);
  pinMode(FIGURE8_LED_GREEN3, OUTPUT);

  // initialize magnet for figure 8
  pinMode(FIGURE8_MAGNET, OUTPUT);

  // initialzie reed switch input for the figure 8 using an internal pull up resistor
  pinMode(FIGURE8_REED_SWITCH, INPUT_PULLUP);

  // initialize the leds for the shuttle signal post
  pinMode(SHUTTLE_LED_RED, OUTPUT);
  pinMode(SHUTTLE_LED_GREEN, OUTPUT);

  // initialize the magnet for the shuttle
  pinMode(SHUTTLE_MAGNET, OUTPUT);

  //initialize the reed switch input for the shuttle using an internal pull up resistor
  pinMode(SHUTTLE_REED_SWITCH, INPUT_PULLUP);

  // initialize the input for the start button
  pinMode(START_BUTTON, INPUT_PULLUP);

  // initialize serial communication
  Serial.begin(9600);

  turnOnFigure8Magnet();
  turnOnShuttleMagnet();

  // turn on red led of figure 8 signalpost
  setFigure8Leds(4);

  //turn on red led of shuttle signal post
  setShuttleLeds(3);  // red led is on
}

typedef enum 
{
  TRAIN_NONE = 0,
  TRAIN_FIGURE8 = 1,
  TRAIN_SHUTTLE = 2
} train_t;

// define a variable that keeps track of which train is running
train_t currentTrain = TRAIN_NONE;


void loop() 
{
  switch(currentTrain)
  {
    case TRAIN_NONE:
    {
      int start = digitalRead(START_BUTTON);
      if (start == 0)
      {
        turnOffFigure8Magnet();
        figure8LapCounter = 1;
        currentTrain = TRAIN_FIGURE8;
      }
      break;
    }
    case TRAIN_FIGURE8:
    {
      figure8LapCounter = countFigure8Laps();
      // Serial.print("Figure8: ");
      // Serial.println(figure8LapCounter);
      setFigure8Leds(figure8LapCounter);
      if (figure8LapCounter == 4)
      {
        turnOnFigure8Magnet();
        turnOffShuttleMagnet();
        figure8LapCounter = 0;
        shuttleLapCounter = 1;
        currentTrain = TRAIN_SHUTTLE;
      }
      break;
    }
    case TRAIN_SHUTTLE:
    {
      shuttleLapCounter = countShuttleLaps();
      // Serial.print("Shuttle : ");
      // Serial.println(shuttleLapCounter);
      setShuttleLeds(shuttleLapCounter);
      if (shuttleLapCounter == 3)
      {
        turnOnShuttleMagnet();
        turnOffFigure8Magnet();
        shuttleLapCounter = 0;
        figure8LapCounter = 1;
        currentTrain = TRAIN_FIGURE8;
      }
    }
    default:
    {
      break;
    }
  }
}
