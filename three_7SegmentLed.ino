//this is the final code for 3 seven segment display using addressable led through serial port
#include <FastLED.h>
#define LED_PIN 7   // use pin 26 for esp32s2 and pin 7 for arduino
#define NUM_LEDS 123

String no;
CRGB selectedled[NUM_LEDS];
//int leds[0][41] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0};
// int leds0[41] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0};
// int leds1[41] = {0,0,0,0,0,0,0,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   0,0,0,0,0,   0,0,0,0,0};
// int leds2[41] = {1,0,0,0,0,0,1,1,1,1,1,1,1,    1,1,1,1,1,   1,0,0,0,0,0,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};
// int leds3[41] = {0,0,0,0,0,0,0,0,0,0,0,0,0,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};
// int leds4[41] = {1,1,1,1,1,1,1,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   0,0,0,0,0,   1,1,1,1,1};
// int leds5[41] = {1,1,1,1,1,1,1,0,0,0,0,0,0,    1,1,1,1,1,   1,1,1,1,1,1,1,0,0,0,0,0,1,   1,1,1,1,1,   1,1,1,1,1};
// int leds6[41] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,0,0,0,0,0,1,   1,1,1,1,1,   1,1,1,1,1};
// int leds7[41] = {1,0,0,0,0,0,0,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0};
// int leds8[41] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};
// int leds9[41] = {1,1,1,1,1,1,1,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};

int leds[10][41] = {
 {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   0,0,0,0,0,   0,0,0,0,0},
 {1,0,0,0,0,0,1,1,1,1,1,1,1,    1,1,1,1,1,   1,0,0,0,0,0,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1},
 {0,0,0,0,0,0,0,0,0,0,0,0,0,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1},
 {1,1,1,1,1,1,1,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   0,0,0,0,0,   1,1,1,1,1},
 {1,1,1,1,1,1,1,0,0,0,0,0,0,    1,1,1,1,1,   1,1,1,1,1,1,1,0,0,0,0,0,1,   1,1,1,1,1,   1,1,1,1,1},
 {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,0,0,0,0,0,1,   1,1,1,1,1,   1,1,1,1,1},
 {1,0,0,0,0,0,0,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0},
 {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1},
 {1,1,1,1,1,1,1,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1},
};

void printArray(int*arr){
  for(int i = 0; i<41 ; i++){
    Serial.print(arr[i]);
  }
}
// void copyArray100(CRGB*arr1, int*arr2){
//   for(int i = 0; i<41; i++){
//     //Serial.print(arr2[i]);
//     //Serial.print(" "); 
//     if(arr2[i])   
//       arr1[i] = 1;
//     else
//       arr1[i] = 0;
//   }
// }

void copyArray100(CRGB*arr1, int*arr2){
  // int i = 0;
  // int j = 0;
  // while(i<41 && j<41){
  //   if(arr2[i]){
  //     arr1[i] = 1;      
  //   }
  //   else
  //     arr1[i] = 0;
  //   i++;
  //   j++;
  // }
    int i = 0;
  int j = 0;
  while(j<41){
    arr1[i] = arr2[j];
    // if(arr2[i]){
    //   arr1[i] = 1;
    // }
    // else
    //   arr1[i] = 0;
    i++;
    j++;
  }
}
void copyArray10(CRGB*arr1, int*arr2){
  int i = 41;
  int j = 0;
  while(j<41){
    arr1[i] = arr2[j];
    i++;
    j++;
  }
}

void copyArray1(CRGB*arr1, int*arr2){
  int i = 82;
  int j = 0;
  while(j<41){
    arr1[i] = arr2[j];
    // if(arr2[i]){
    //   arr1[i] = 1;
    // }
    // else
    //   arr1[i] = 0;
    i++;
    j++;
  }
}
#define BRIGHTNESS  96
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(selectedled, NUM_LEDS);

  Serial.begin(9600);
  Serial.println("Type any number between 0 to 9");
}
void loop() {
  if(Serial.available())
  {
    // no = Serial.read();
    no = Serial.readString();

    char nos[4];
    no.toCharArray(nos, 4);

    int integerNo = atoi(nos);
    Serial.print("No is ");
    Serial.println(integerNo);

    int ones = integerNo % 10;
    int doubleDigit = integerNo /10;
    int tens = doubleDigit % 10;
    int hunderedth = doubleDigit /10;
    Serial.println(ones);
    Serial.println(tens);
    Serial.println(hunderedth);
    copyArray1(selectedled, leds[ones]);
    copyArray10(selectedled, leds[tens]);
    copyArray100(selectedled, leds[hunderedth]);
    printArray(leds[ones]);
    //return;
    // switch(no)
    // {
    //   case '0' :
    //   copyArray(selectedled, leds0);
    //   Serial.println(0);
    //   break;
    //   case '1' :
    //   copyArray(selectedled, leds1);
    //   Serial.println(1);
    //   break;
    //   case '2' :
    //   copyArray(selectedled, leds2);
    //   break;
    //   case '3' :
    //   copyArray(selectedled, leds3);
    //   break;
    //   case '4' :
    //   copyArray(selectedled, leds4);
    //   break;
    //   case '5' :
    //   copyArray(selectedled, leds5);
    //   break;
    //   case '6' :
    //   copyArray(selectedled, leds6);
    //   break;
    //   case '7' :
    //   copyArray(selectedled, leds7);
    //   break;
    //   case '8' :
    //   copyArray(selectedled, leds8);
    //   break;
    //   case '9' :
    //   copyArray(selectedled, leds9);
    //   break;
    //   default:
    //   Serial.println("verify");
    //   break;
    // }

    // switch(ones)
    // {
    //   case 0 :
    //   copyArray1(selectedled, leds0);
    //   Serial.print('a');
    //   break;
    //   case 1 :
    //   copyArray1(selectedled, leds1);
    //   //Serial.println(1);
    //   break;
    //   case 2 :
    //   copyArray1(selectedled, leds2);
    //   break;
    //   case 3 :
    //   copyArray1(selectedled, leds3);
    //   break;
    //   case 4 :
    //   copyArray1(selectedled, leds4);
    //   break;
    //   case 5 :
    //   copyArray1(selectedled, leds5);
    //   break;
    //   case 6 :
    //   copyArray1(selectedled, leds6);
    //   break;
    //   case 7 :
    //   copyArray1(selectedled, leds7);
    //   break;
    //   case 8 :
    //   copyArray1(selectedled, leds8);
    //   break;
    //   case 9 :
    //   copyArray1(selectedled, leds9);
    //   break;
    //   default:
    //   Serial.println("verify");
    //   break;
    // }
    // switch(tens)
    // {
    //   case 0 :
    //   copyArray10(selectedled, leds0);
    //   Serial.print('a');
    //   break;
    //   case 1 :
    //   copyArray10(selectedled, leds1);
    //   //Serial.println(1);
    //   break;
    //   case 2 :
    //   copyArray10(selectedled, leds2);
    //   break;
    //   case 3 :
    //   copyArray10(selectedled, leds3);
    //   break;
    //   case 4 :
    //   copyArray10(selectedled, leds4);
    //   break;
    //   case 5 :
    //   copyArray10(selectedled, leds5);
    //   break;
    //   case 6 :
    //   copyArray10(selectedled, leds6);
    //   break;
    //   case 7 :
    //   copyArray10(selectedled, leds7);
    //   break;
    //   case 8 :
    //   copyArray10(selectedled, leds8);
    //   break;
    //   case 9 :
    //   copyArray10(selectedled, leds9);
    //   break;
    //   default:
    //   Serial.println("verify");
    //   break;
    // }
    // switch(hunderedth)
    // {
    //   case 0 :
    //   copyArray100(selectedled, leds0);
    //   Serial.print('a');
    //   break;
    //   case 1 :
    //   copyArray100(selectedled, leds1);
    //   //Serial.println(1);
    //   break;
    //   case 2 :
    //   copyArray100(selectedled, leds2);
    //   break;
    //   case 3 :
    //   copyArray100(selectedled, leds3);
    //   break;
    //   case 4 :
    //   copyArray100(selectedled, leds4);
    //   break;
    //   case 5 :
    //   copyArray100(selectedled, leds5);
    //   break;
    //   case 6 :
    //   copyArray100(selectedled, leds6);
    //   break;
    //   case 7 :
    //   copyArray100(selectedled, leds7);
    //   break;
    //   case 8 :
    //   copyArray100(selectedled, leds8);
    //   break;
    //   case 9 :
    //   copyArray100(selectedled, leds9);
    //   break;
    //   default:
    //   Serial.println("verify");
    //   break;
    // }
  }
  FastLED.show();
}
