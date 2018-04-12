#pragma once
// This file is responsible for translating the raw matrix status
// into USB HID key reports.
// The technique used here is a little different from most of the
// firmware implementations that I've seen.  Here we translate the
// matrix row/column into a scan code that identifies the key on
// the keyboard.  We can the track state for a compile-time defined
// number of keys at a time and remember the tapping and timing for
// those keys.  The keymap processing defines a simple rule per
// scan code (per layer) that translates the state for the scan
// code into a HID key report.

static constexpr uint32_t kMask = 0xf00;
static constexpr uint32_t kKeyPress = 0x100;
static constexpr uint32_t kModifier = 0x200;
static constexpr uint32_t kLayer = 0x300;
static constexpr uint32_t kTapHold = 0x400;
static constexpr uint32_t kToggleMod = 0x500;
static constexpr uint32_t kKeyAndMod = 0x600;
static constexpr uint32_t kMouseButton = 0x700;
static constexpr uint32_t kMacro = 0x800;
static constexpr uint32_t kTapInterval = 250; // milliseconds
typedef uint32_t action_t;

#define PASTE(a, b) a ## b

#define ___      0
#define CONS(a)  0  // FIXME
#define KEY(a)   kKeyPress | PASTE(HID_KEYBOARD_, a)
#define MOD(a)   kModifier | PASTE(KEYBOARD_MODIFIER_, a)
#define TMOD(a)  kToggleMod | PASTE(KEYBOARD_MODIFIER_, a)
#define TAPH(a, b) kTapHold | PASTE(HID_KEYBOARD_, a) | (PASTE(KEYBOARD_MODIFIER_, b) << 16)
#define KANDMOD(a, b) kKeyAndMod | PASTE(HID_KEYBOARD_, a) | (PASTE(KEYBOARD_MODIFIER_, b) << 16)
#define LAYER(n) kLayer | n
#define MACRO(n)  kMacro | n

// This maps the human perceived logical keycap layout to the less intuitive
// minimized hardware key matrix wiring.  This mapping is coupled with the
// row and column pin bindings in keyscanner.h
#define KEYMAP(\
               l00, l01, l02, l03, l04, l05, \
               l10, l11, l12, l13, l14, l15, \
               l20, l21, l22, l23, l24, l25, \
               l30, l31, l32, l33, l34, l35, \
               l40, l41, l42, l43, l44, l45, \
               l50, l51, l52, l53, l54, l55, \
               l60, l61, l62, l63, l64, l65, \
               r00, r01, r02, r03, r04, r05, \
               r10, r11, r12, r13, r14, r15, \
               r20, r21, r22, r23, r24, r25, \
               r30, r31, r32, r33, r34, r35, \
               r40, r41, r42, r43, r44, r45, \
               r50, r51, r52, r53, r54, r55, \
               r60, r61, r62, r63, r64, r65) \
      {\
             l10, l21, l03, l41, l04, l35, l61, r15, r24, r02, r44, r01, r30, r64, \
             l00, l31, l13, l42, l14, l45, l62, r05, r34, r12, r43, r11, r40, r63, \
             l01, l32, l23, l51, l15, l54, l52, r04, r33, r22, r54, r10, r51, r53, \
             l11, l22, l33, l50, l25, l55, l44, r14, r23, r32, r55, r20, r50, r41, \
             l12, l20, l43, l60, l24, l65, l53, r13, r25, r42, r65, r21, r60, r52, \
             l02, l30, l34, l40, l05, l64, l63, r03, r35, r31, r45, r00, r61, r62}

// Represents the state of some key
struct keystate {
  uint8_t scanCode;
  bool down;
  uint32_t lastChange;
  uint32_t priorChange;
  action_t action;
};
// We allow tracking the state of up to 16 keys.
// You can make this as large or small as makes sense to you.
// The constraint is available ram and fingers.
struct keystate keyStates[16];

// Allow for up to 8 layers to be stacked up
uint8_t layer_stack[8];
static uint8_t layer_pos = 0;

struct matrix_t {
  uint16_t rows[6];
};
struct matrix_t lastRead = {0,0,0,0,0,0};

// true if we synthesized a key tap on the last run; we will need
// to ensure that we follow up with a release event on the next loop
static bool last_was_tap = false;

struct matrix_t readMatrix();
void initKeyScanner();

void resetKeyMatrix() {
  layer_pos = 0;
  layer_stack[0] = 0;
  last_was_tap = false;
  memset(&lastRead, 0, sizeof(lastRead));
  memset(keyStates, 0xff, sizeof(keyStates));
  initKeyScanner();

  Keyboard.releaseAll();
}

void printState(struct keystate *state) {
  Serial.print("scanCode=");
  Serial.print(state->scanCode, HEX);
  Serial.print(" down=");
  Serial.print(state->down);
  Serial.print(" lastChange=");
  Serial.print(state->lastChange);
  Serial.print(" action=");
  Serial.print(state->action, HEX);
  Serial.println("");
}

struct keystate* stateSlot(uint8_t scanCode, uint32_t now) {
  struct keystate *vacant = nullptr;
  struct keystate *reap = nullptr;
  for (auto &s : keyStates) {
    if (s.scanCode == scanCode) {
      return &s;
    }
    if (!vacant && s.scanCode == 0xff) {
      vacant = &s;
      continue;
    }
    if (!s.down) {
      if (!reap) {
        reap = &s;
      } else if (now - s.lastChange > now - reap->lastChange) {
        // Idle longer than the other reapable candidate; choose
        // the eldest of them
        reap = &s;
      }
    }
  }
  if (vacant) {
    return vacant;
  }
  return reap;
}

const action_t keymap[2][84] = {
  // Layer 0
  KEYMAP(
    // LEFT
    KEY(ESCAPE),            KEY(PAUSE),     KEY(SCROLL_LOCK),           ___,           KEY(MINUS_AND_UNDERSCORE), KEY(EQUALS_AND_PLUS),
    KEY(GRAVE),             KEY(1),         KEY(2),                     KEY(3),        KEY(4),                    KEY(5),
    KEY(TAB),               KEY(Q),         KEY(W),                     KEY(E),        KEY(R),                    KEY(T),
    TAPH(ESCAPE, LEFTCTRL), KEY(A),         KEY(S),                     KEY(D),        KEY(F),                    KEY(G),
    MOD(LEFTSHIFT),         KEY(Z),         KEY(X),                     KEY(C),        KEY(V),                    KEY(B),
    ___,                    KEY(PAGE_UP),   KANDMOD(C, LEFTCTRL),       MOD(LEFTCTRL), MOD(LEFTALT),              MOD(LEFTGUI),
    ___,                    KEY(PAGE_DOWN), KANDMOD(INSERT, LEFTSHIFT), KEY(DELETE),   KEY(DELETE_FORWARD),       ___,

    // RIGHT
    KEY(BRACKET_LEFT), KEY(BRACKET_RIGHT), KEY(SCROLL_LOCK),         KEY(PRINTSCREEN),        ___,       ___,
    KEY(6),            KEY(7),             KEY(8),                   KEY(9),     KEY(0),    ___,
    KEY(Y),            KEY(U),             KEY(I),                   KEY(O),     KEY(P),    KEY(BACKSLASH_AND_PIPE),
    KEY(H),            KEY(J),             KEY(K),                   KEY(L),     KEY(SEMICOLON_AND_COLON),  KEY(APOSTROPHE),
    KEY(N),            KEY(M),             KEY(COMMA_AND_LESS_THAN), KEY(PERIOD_AND_GREATER_THAN), KEY(SLASH_AND_QUESTION_MARK), MOD(RIGHTSHIFT),
    MOD(RIGHTGUI),     MOD(RIGHTALT),      MOD(RIGHTCTRL),           ___,             KEY(UP_ARROW),   LAYER(1),
    ___,               KEY(ENTER),         KEY(SPACEBAR),            KEY(LEFT_ARROW), KEY(DOWN_ARROW), KEY(RIGHT_ARROW)


  ),
  // Layer 1
  KEYMAP(
    // LEFT
    ___,         ___,       KEY(F14),  KEY(F15),         ___,          ___,
    ___,         KEY(F1),   KEY(F2),   KEY(F3),      KEY(F4),     KEY(F5),
    ___,         ___,       ___,          ___,         ___,          ___,
    ___,         ___,       ___,          ___,         ___,          ___,
    ___,         ___,       ___,          ___,         ___,          ___,
    ___,         ___,       ___,          ___,         ___,          ___,
    ___,         ___,       ___,          ___,         ___,          ___,

    // RIGHT
    ___,         ___,       ___,          ___,         ___,          ___,
    KEY(F6),   KEY(F7),     KEY(F8),     KEY(F9),   KEY(F10),     ___,
    ___,         ___,       ___,          ___,         ___,       ___,
    ___,         ___,       ___,          ___,         ___,       ___,
    ___,         ___,       CONS(SCAN_PREVIOUS_TRACK), CONS(SCAN_PREVIOUS_TRACK), CONS(PLAY),       ___,
    ___,         ___,       ___,          ___,         KEY(VOLUME_UP),   ___,
    ___,         ___,       ___,          KEY(HOME),         KEY(VOLUME_DOWN), KEY(END)
  )
};

static uint32_t resolveActionForScanCodeOnActiveLayer(uint8_t scanCode) {
  int s = layer_pos;

  while (s >= 0 && keymap[layer_stack[s]][scanCode] == ___) {
    --s;
  }
  return keymap[layer_stack[s]][scanCode];
}

void performMacro(uint8_t n) {
  switch (n) {
    case 0:
      // enterSerialDfu();
      return;
  }
}

void applyMatrix() {
  bool keysChanged = false;

  auto down = readMatrix();
  auto now = millis();

  for (int rowNum = 0; rowNum < 6; ++rowNum) {
    for (int colNum = 0; colNum < 14; ++colNum) {
      auto scanCode = (rowNum * 14) + colNum;
      auto isDown = down.rows[rowNum] & (1 << colNum);
      auto wasDown = lastRead.rows[rowNum] & (1 << colNum);

      if (isDown == wasDown) {
        continue;
      }
      keysChanged = true;

      auto state = stateSlot(scanCode, now);
      if (isDown && !state) {
        // Silently drop this key; we're tracking too many
        // other keys right now
        continue;
      }
      //printState(state);

      bool isTransition = false;

      if (state) {
        if (state->scanCode == scanCode) {
          // Update the transition time, if any
          if (state->down != isDown) {
            state->priorChange = state->lastChange;
            state->lastChange = now;
            state->down = isDown;
            if (isDown) {
              state->action = resolveActionForScanCodeOnActiveLayer(scanCode);
            }
            isTransition = true;
          }
        } else {
          // We claimed a new slot, so set the transition
          // time to the current time.
          state->down = isDown;
          state->scanCode = scanCode;
          state->priorChange = now;
          state->lastChange = now;
          if (isDown) {
            state->action = resolveActionForScanCodeOnActiveLayer(scanCode);
          }
          isTransition = true;
        }

        if (isTransition) {
          switch (state->action & kMask) {
            case kLayer:
              if (state->down) {
                // Push the new layer stack position
                layer_stack[++layer_pos] = state->action & 0xff;
              } else {
                // Pop off the layer stack
                --layer_pos;
              }
              break;
          }
        }
      }
    }
  }

  if (keysChanged || last_was_tap) {
    KeyReport report;
    memset(&report, 0, sizeof(report));
    uint8_t repsize = 0;
    uint8_t mods = 0;
    uint8_t mouseButtons = 0;
    last_was_tap = false;

    for (auto &state: keyStates) {
      if (state.scanCode != 0xff && state.down) {
        switch (state.action & kMask) {
          case kTapHold:
            // Holding
            mods |= (state.action >> 16) & 0xff;
            break;
          case kKeyAndMod:
            {
              mods |= (state.action >> 16) & 0xff;
              auto key = state.action & 0xff;
              if (key != 0 && repsize < 6) {
                report.keys[repsize++] = key;
              }
            }
            break;
          case kKeyPress:
            {
              auto key = state.action & 0xff;
              if (key != 0 && repsize < 6) {
                report.keys[repsize++] = key;
              }
            }
            break;
          case kModifier:
            mods |= state.action & 0xff;
            break;
          case kToggleMod:
            mods ^= state.action & 0xff;
            break;

          case kMacro:
            performMacro(state.action & 0xff);
            break;
        }
      } else if (state.scanCode != 0xff && !state.down) {
        switch (state.action & kMask) {
          case kTapHold:
            if (state.lastChange == now && state.lastChange - state.priorChange <= kTapInterval) {
              // Tapped and just released.  FIXME: suppress this if we generated a
              // report with the hold portion of this together with another key?
              auto key = state.action & 0xff;
              if (key != 0 && repsize < 6) {
                report.keys[repsize++] = key;
                last_was_tap = true;
              }
            }
            break;
        }
      }
    }

    // HID().mouseButtonPress(mouseButtons);

#if WAT
    Serial.print("mods=");
    Serial.print(mods, HEX);
    Serial.print(" repsize=");
    Serial.print(repsize);
    for (int i = 0; i < repsize; i++) {
      Serial.print(" ");
      Serial.print(report.keys[i], HEX);
    }
    Serial.print("\r\n");
#endif

    report.modifiers = mods;
    Keyboard.sendReport(&report);
    lastRead = down;
  }
}
