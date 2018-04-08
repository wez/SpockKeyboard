#include "HID.h"
#include "HIDTables.h"
#include "Keyboard.h"
#include "SparkFunSX1509.h"

#define HID_KEYBOARD_A HID_KEYBOARD_A_AND_A
#define HID_KEYBOARD_B HID_KEYBOARD_B_AND_B
#define HID_KEYBOARD_C HID_KEYBOARD_C_AND_C
#define HID_KEYBOARD_D HID_KEYBOARD_D_AND_D
#define HID_KEYBOARD_E HID_KEYBOARD_E_AND_E
#define HID_KEYBOARD_F HID_KEYBOARD_F_AND_F
#define HID_KEYBOARD_G HID_KEYBOARD_G_AND_G
#define HID_KEYBOARD_H HID_KEYBOARD_H_AND_H
#define HID_KEYBOARD_I HID_KEYBOARD_I_AND_I
#define HID_KEYBOARD_J HID_KEYBOARD_J_AND_J
#define HID_KEYBOARD_K HID_KEYBOARD_K_AND_K
#define HID_KEYBOARD_L HID_KEYBOARD_L_AND_L
#define HID_KEYBOARD_M HID_KEYBOARD_M_AND_M
#define HID_KEYBOARD_N HID_KEYBOARD_N_AND_N
#define HID_KEYBOARD_O HID_KEYBOARD_O_AND_O
#define HID_KEYBOARD_P HID_KEYBOARD_P_AND_P
#define HID_KEYBOARD_Q HID_KEYBOARD_Q_AND_Q
#define HID_KEYBOARD_R HID_KEYBOARD_R_AND_R
#define HID_KEYBOARD_S HID_KEYBOARD_S_AND_S
#define HID_KEYBOARD_T HID_KEYBOARD_T_AND_T
#define HID_KEYBOARD_U HID_KEYBOARD_U_AND_U
#define HID_KEYBOARD_V HID_KEYBOARD_V_AND_V
#define HID_KEYBOARD_W HID_KEYBOARD_W_AND_W
#define HID_KEYBOARD_X HID_KEYBOARD_X_AND_X
#define HID_KEYBOARD_Y HID_KEYBOARD_Y_AND_Y
#define HID_KEYBOARD_Z HID_KEYBOARD_Z_AND_Z

#define HID_KEYBOARD_BRACKET_RIGHT HID_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_CURLY_BRACE
#define HID_KEYBOARD_BRACKET_LEFT HID_KEYBOARD_LEFT_BRACKET_AND_LEFT_CURLY_BRACE
#define HID_KEYBOARD_APOSTROPHE HID_KEYBOARD_QUOTE_AND_DOUBLEQUOTE
#define HID_KEYBOARD_GRAVE HID_KEYBOARD_GRAVE_ACCENT_AND_TILDE

#define HID_KEYBOARD_1 HID_KEYBOARD_1_AND_EXCLAMATION_POINT
#define HID_KEYBOARD_2 HID_KEYBOARD_2_AND_AT
#define HID_KEYBOARD_3 HID_KEYBOARD_3_AND_POUND
#define HID_KEYBOARD_4 HID_KEYBOARD_4_AND_DOLLAR
#define HID_KEYBOARD_5 HID_KEYBOARD_5_AND_PERCENT
#define HID_KEYBOARD_6 HID_KEYBOARD_6_AND_CARAT
#define HID_KEYBOARD_7 HID_KEYBOARD_7_AND_AMPERSAND	
#define HID_KEYBOARD_8 HID_KEYBOARD_8_AND_ASTERISK	
#define HID_KEYBOARD_9 HID_KEYBOARD_9_AND_LEFT_PAREN	
#define HID_KEYBOARD_0 HID_KEYBOARD_0_AND_RIGHT_PAREN

#define KEYBOARD_MODIFIER_LEFTCTRL 1<<0
#define KEYBOARD_MODIFIER_LEFTSHIFT 1<<1
#define KEYBOARD_MODIFIER_LEFTALT 1<<2
#define KEYBOARD_MODIFIER_LEFTGUI 1<<3
#define KEYBOARD_MODIFIER_RIGHTCTRL 1<<4
#define KEYBOARD_MODIFIER_RIGHTSHIFT 1<<5
#define KEYBOARD_MODIFIER_RIGHTALT 1<<6
#define KEYBOARD_MODIFIER_RIGHTGUI 1<<7


#include "keymap.h"
#include "keyscanner.h"

void setup() 
{
//  Serial.begin(115200);
  Keyboard.begin();

  initKeyScanner();
  resetKeyMatrix();

 // Serial.println("made it here");
}

void loop() 
{
  applyMatrix();
}
