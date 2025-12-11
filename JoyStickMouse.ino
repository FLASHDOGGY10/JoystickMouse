#include <ezButton.h>
#include <BleMouse.h>

BleMouse bleMouse; //makes this a bleMouse

// joystick varibles
#define VRX_PIN  32
#define VRY_PIN  33
#define SW_PIN   18

//button decloration
#define CB_PIN 4

ezButton jsButton(SW_PIN);
ezButton cbButton(CB_PIN);

//joystick maths stuff
int xCentre = 1771;
int yCentre = 1840;
const int deadZone = 120;       
const int maxAnalog = 4095;
int sensitivity = 3;
int valueX, valueY;
int mouseSpeedX, mouseSpeedY;

void setup() {
  BLEDevice::init("Joystick Mouse"); // this makes the name 
  Serial.begin(115200);
  Serial.println("Starting BLE...");
  bleMouse.begin();

  analogSetAttenuation(ADC_11db); // does something, i dont really know

  //button debounce 
  jsButton.setDebounceTime(50);
  cbButton.setDebounceTime(50);
}

void loop() {
  //button loops
  jsButton.loop();
  cbButton.loop();

  //joystick finding varibles 
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);
  int xFromCentre = valueX - xCentre;
  int yFromCentre = valueY - yCentre;
  if (abs(xFromCentre) < deadZone) xFromCentre = 0;
  if (abs(yFromCentre) < deadZone) yFromCentre = 0;
  mouseSpeedX = map(xFromCentre, -2000, 2000, -7, 7);
  mouseSpeedY = map(yFromCentre, -2000, 2000, 7, -7);  
  mouseSpeedX = constrain(mouseSpeedX, -7, 7);
  mouseSpeedY = constrain(mouseSpeedY, -7, 7);


  if (bleMouse.isConnected()) {
  // big long if button pressed chain 
    if (jsButton.isPressed()) {
      bleMouse.click(MOUSE_LEFT);
    } else if (cbButton.isPressed()) 
    {
      calibrate();
    }
    if (mouseSpeedX != 0 || mouseSpeedY != 0) {
      bleMouse.move(mouseSpeedX * sensitivity, -mouseSpeedY * sensitivity);
    }
  }

  //lots for debugging
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

void calibrate()
{
  Serial.println("return mouse to centre");
  delay(1000);
  xCentre = analogRead(VRX_PIN);
  yCentre = analogRead(VRY_PIN);
  //centres mouse
  println("new X centre: " + xCentre);
  println("new Y centre: " + yCentre);
  
}
