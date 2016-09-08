#include "Tlc5940.h"///make sure to import this library

////make sure to edit the tlc_config.h file to define how many tlc5940s are in use
////change #define NUM_TLCS    1 to reflect:
///#define NUM_TLCS    2


int rgbChannels = 30;//total channels used on the TLC5940's
int rgb[30]; ///should be the same as the number of channels
int rgbLights = 10;
int colorArray[10];

void setup() {
  // put your setup code here, to run once
  Tlc.init(0); // Initiates the TLC5940 and set all channels off
  Serial.begin(250000);
  Serial.println("Total Channels: " + String(rgbChannels) + "  Total RGB Ligts: " + String(rgbLights));
  float divisor = 360 / (rgbChannels / 3); //degrees of color to display divided by the number of rgb lights
  Serial.println("Divisor: " + String(divisor) );
  float Step = divisor;
  for (int i = 0; i < rgbLights; i++) {
    colorArray[i] = Step;
    Serial.println("colorArray[" + String(i) + "]: " + String(colorArray[i]));
    Step = Step + divisor;
  }
}

void ledColor(int channel, int red, int green, int blue)
{
  Tlc.set(channel, red);
  Tlc.set(channel + 1, green);
  Tlc.set(channel + 2, blue);
}

void hsi_to_rgb(int startChannel, float H, float S, float I) {
  int r, g, b;
  if (H > 360) {
    H = H - 360;
  }
  // Serial.println("H: "+String(H));
  H = fmod(H, 360); // cycle H around to 0-360 degrees
  H = 3.14159 * H / (float)180; // Convert to radians.
  S = S > 0 ? (S < 1 ? S : 1) : 0; // clamp S and I to interval [0,1]
  I = I > 0 ? (I < 1 ? I : 1) : 0;
  if (H < 2.09439) {
    r = 4095 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    g = 4095 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    b = 4095 * I / 3 * (1 - S);
  } else if (H < 4.188787) {
    H = H - 2.09439;
    g = 4095 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    b = 4095 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    r = 4095 * I / 3 * (1 - S);
  } else {
    H = H - 4.188787;
    b = 4095 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    r = 4095 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    g = 4095 * I / 3 * (1 - S);
  }
  rgb[0 + startChannel] = r;
  rgb[1 + startChannel] = g;
  rgb[2 + startChannel] = b;

}

void oneByoneRv() {
  for (int i = 30; i > 0; i = i - 3) {
    int color = 0;
    for (int j = 0; j < 360; j++) {
      hsi_to_rgb(i, j, 1, .4);
      ledColor(i, rgb[i], rgb[i + 1], rgb[i + 2]);
      delayMicroseconds(50);
      Tlc.update();
      Tlc.clear();
      delayMicroseconds(30); color++;
    }
    Tlc.init(0);
    Tlc.update();
    Tlc.clear();
    delayMicroseconds(10);
  }
}

void oneByone() {
  for (int i = 0; i < 30; i = i + 3) {
    int color = 0;
    for (int j = 0; j < 360; j++) {
      hsi_to_rgb(i, j, 1, .4);
      ledColor(i, rgb[i], rgb[i + 1], rgb[i + 2]);
      delayMicroseconds(200);
      Tlc.update();
      Tlc.clear();
      delayMicroseconds(30); color++;
    }
    Tlc.init(0);
    Tlc.update();
    Tlc.clear();
    delayMicroseconds(10);
  }
}

void rainbowShift() {
  float brightness = .4;
  float saturation = 1;
  for (int n = 0; n <= 360; n++) {
    for (int i = 0, j = 0; i < rgbLights; i++) {
      hsi_to_rgb(j, colorArray[i] + n, saturation, brightness);
      //Serial.println("rgb" +String(i)+": "+String(rgb[j])+","+String(rgb[j+1])+","+String(rgb[j+2]));
      ledColor(j, rgb[j], rgb[j + 1], rgb[j + 2]);
      j = j + 3;
    }
    Tlc.update();
    Tlc.clear();
    delayMicroseconds(500);
  }
}
void rainbowShiftRv() {
  float brightness = .4;
  float saturation = 1;
  for (int n = 360; n >= 0; n = n - 1) {
    for (int i = 0, j = 0; i < rgbLights; i++) {
      hsi_to_rgb(j, colorArray[i] + n, saturation, brightness);
      //Serial.println("rgb" +String(i)+": "+String(rgb[j])+","+String(rgb[j+1])+","+String(rgb[j+2]));
      ledColor(j, rgb[j], rgb[j + 1], rgb[j + 2]);
      j = j + 3;
    }
    Tlc.update();
    Tlc.clear();
    delayMicroseconds(500);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
rainbowShiftRv() ;
rainbowShiftRv() ;
rainbowShiftRv() ;
rainbowShiftRv() ;
rainbowShiftRv() ;
rainbowShift() ;
rainbowShift() ;
rainbowShift() ;
rainbowShift() ;

}





