#include <ezButton.h>
#include <BleMouse.h>

BleMouse bleMouse;

// ---- ADC PINS ----
#define VRX_PIN  32
#define VRY_PIN  33
#define SW_PIN   18

ezButton button(SW_PIN);


// ---- JOYSTICK CALIBRATION ----
const int xCenter = 1771;
const int yCenter = 1840;

const int deadZone = 120;       // small dead zone
const int maxAnalog = 4095;

// speed multiplier
int sensitivity = 3;

int valueX, valueY;
int mouseSpeedX, mouseSpeedY;

void setup() {
  BLEDevice::init("Joystick Mouse");

  Serial.begin(115200);
  Serial.println("Starting BLE...");

  bleMouse.begin();

  analogSetAttenuation(ADC_11db);
  button.setDebounceTime(50);
}

void loop() {
  button.loop();


  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  int xFromCenter = valueX - xCenter;
  int yFromCenter = valueY - yCenter;


  if (abs(xFromCenter) < deadZone) xFromCenter = 0;
  if (abs(yFromCenter) < deadZone) yFromCenter = 0;


  mouseSpeedX = map(xFromCenter, -2000, 2000, -7, 7);
  mouseSpeedY = map(yFromCenter, -2000, 2000, 7, -7);  // invert Y for mouse


  mouseSpeedX = constrain(mouseSpeedX, -7, 7);
  mouseSpeedY = constrain(mouseSpeedY, -7, 7);


  if (bleMouse.isConnected()) {

    if (button.isPressed()) {
      Serial.println("Button pressed");
      bleMouse.click(MOUSE_LEFT);
    }


    if (mouseSpeedX != 0 || mouseSpeedY != 0) {
      bleMouse.move(mouseSpeedX * sensitivity,
                    -mouseSpeedY * sensitivity);
    }
  }


  Serial.print("X:");
  Serial.print(valueX);
  Serial.print(" Y:"); 
  Serial.print(valueY);
  Serial.print(" -> SpeedX:");
  Serial.print(mouseSpeedX);
  Serial.print(" SpeedY:");
  Serial.println(mouseSpeedY);

  delay(10);
}
