/*
  This game lights one of three LEDs at random. The player must press the corresponding 
  button before the time limit, or it's game over, and the LEDs will blink with disappointment.
  
  At the start of each round, all LEDs will flash to count the current level.

  At level 1, the time limit to press the correct button is 4 seconds. This decreases by 250ms
  every level, until 250ms at level 16. Beyond that, it gets 10% faster until game over!

  This program was written in 2016 by Joshua Scott, as part of a university course.
*/

const int button1 = 2;
const int button2 = 3;
const int button3 = 4;
const int led1 = 5;
const int led2 = 6;
const int led3 = 7;
int level = 1;

void setup() 
{
  // Specify LEDs and buttons as outputs and inputs
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() 
{
  Play();
}

void Play()
{
  FlashAllLeds();            // Flash LEDs to signal which level you're playing
  delay(1000);                // Wait one second before the fun begins

  int timeToReact = SetTimeToReact();  // Set timeToReact based on level

  // Pick random correct led/button, initialise variables
  int correctLed = random(5, 8);        // Pick led 1, 2, or 3 at random
  int correctButton = correctLed - 3;     // Identify corresponding button
  bool winner = false;
  // for verifying button presses:
  int buttonState;             // Current button reading
  int lastButtonState = LOW;   // Previous button reading
  unsigned long lastDebounceTime = 0;  // The last time the led was toggled
  unsigned long debounceDelay = 50;    // The debounce time; increase if the output flickers
  
  // Turn the correct LED on for the allowed reaction time, and continuously check if it's pressed. 
  unsigned long int startTime = millis();
  while(millis() < startTime + timeToReact)                     
  {
    digitalWrite(correctLed, HIGH);                         

    // Read the current button's state
    int reading = digitalRead(correctButton);
  
    // If state changed, reset the debouncing timer
    if (reading != lastButtonState) 
      lastDebounceTime = millis();
  
    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
      // Whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
  
      // If the button state has changed:                       
      if (reading != buttonState) 
      {
        buttonState = reading;
  
        // Set winner to true because correct button was pressed
        if (buttonState == HIGH) 
          winner = true;
      }
    }
    // Save current reading to check if it's changed on next loop
    lastButtonState = reading;
  }
  SwitchLedsOff();

  if (winner)               // If winner on that level, level up. Else, game over
    level++;
  else
    GameOver();       
}

void SwitchLedsOff()
{
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void FlashAllLeds()
{
  for (int i = 0; i < level; i++)   // Flash LEDs for half-second each level, to show level number
  {                   
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(350);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(150);
  }
}

int SetTimeToReact()
{
  int time = 4250 - (level * 250);  // Time starts at 4sec, decreases by 250ms every level down to 250ms (at lvl 16)

  if (level > 16)           // Above lvl 16, instead of going to 0ms, decrease by 10% per level
  {
    time = 250;
    int bonus = level - 16;     // e.g. if on level 19 (third 'bonus' level after 16) bonus is 3
    while(bonus > 0)            // Time is -10% for every level above 16
    {
      time *= 0.90;
      bonus--;
    }
  }
  return time;
}

void TwoLedsOn(int ledA, int ledB)
{
  digitalWrite(ledA, HIGH);
  digitalWrite(ledB, HIGH);
  delay(150);
  digitalWrite(ledA, LOW);
  digitalWrite(ledB, LOW);
  delay(150);
}

void GameOver()
{
  level = 1;                	// Reset level

  // Flash lights in a pattern to signal game over
  for (int i = 0; i < 4; i++)   // Each loop lasts 900ms total, so we run it 4 times = 3.6 secs between games
  { 
    TwoLedsOn(led1, led2);
    TwoLedsOn(led2, led3);
    TwoLedsOn(led3, led1);
  }
}
