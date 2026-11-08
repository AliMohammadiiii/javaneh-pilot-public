
#include <ezButton.h>
#define VRX_PIN  A0 
#define VRY_PIN  A1 
#define SW_PIN   26

#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

int xValue = 0 ; 
int yValue = 0 ; 
int bValue = 0;
int command = COMMAND_NO;

// ezButton button(SW_PIN);
void setup() {
  Serial.begin(9600) ;
  //button.setDebounceTime(50);
  pinMode(SW_PIN,INPUT);
}

void loop() {
  // button.loop();
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  // bValue = button.getState();

  // if (button.isPressed()) {
  //   Serial.println("The button is pressed");
  // }

  // if (button.isReleased()) {
  //   Serial.println("The button is released");
  // }
  bValue = digitalRead(SW_PIN);
  Serial.println(bValue);
  delay(50);
  command = COMMAND_NO;

  if (xValue < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;

  if (command & COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
  }

  if (command & COMMAND_RIGHT) {
    Serial.println("COMMAND RIGHT");
  }

  if (command & COMMAND_UP) {
    Serial.println("COMMAND UP");
  }

  if (command & COMMAND_DOWN) {
    Serial.println("COMMAND DOWN");
  }
}
