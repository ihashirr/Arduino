int ledPins[] = {11, 10, 9, 8, 7, 6, 5}; // Array of LED pins
int buttonPin = 13; // Button pin
bool ledsState = false; // Current state of LEDs, initially off
unsigned long lastDebounceTime = 0; // The last time the button state was toggled
unsigned long debounceDelay = 50; // Debounce delay time in milliseconds

void setup() 
{
  for (int i = 0; i < 7; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);  
}

void loop() 
{
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) 
  {
    unsigned long currentTime = millis();
    if ((currentTime - lastDebounceTime) > debounceDelay) 
    {
      ledsState = !ledsState; // Toggle state
      for (int i = 0; i < 7; i++) {
        digitalWrite(ledPins[i], ledsState ? HIGH : LOW); // Set LEDs according to the new state
      }
      lastDebounceTime = currentTime; // Update the last debounce time
    }
  }
}
