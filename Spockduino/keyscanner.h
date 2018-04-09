#pragma once
#include "sx1509.h"

// This file holds the code that scans the keyboard matrix.
// It uses an SX1509 IO expander to read the matrix for the
// right hand side and an Adafruit Feather M0 for the left
// hand side.  The readMatrix() function is the main function
// of interest.

// NOTE: if you change the row or column mappings here, you
// will probably also need to change the KEYMAP macro defined
// in keymap.h.
// These are the pin assignments to the Feather M0 Express.
// The array lists the c0-c6 column assignments to the
// header block.
static const int colPins[] = {5,6,9,10,11,12,13};
// These are r0-r5 assignments
static const int rowPins[] = {14,15,16,17,18,19};

// These are pin assignments to the sx1509 IO multiplexer.
// The column pins are attached to c0-c6, and the row
// pins to r0-r5
static const int expColPins[] = {8,9,10,11,12,13,14};
static const int expRowPins[] = {0,1,2,3,4,5};

static struct matrix_t debounceMatrix;
static struct matrix_t localMatrix;
static constexpr int kDebounceIterations = 1;
static int debouncing = kDebounceIterations;
static SX1509 expander;

void initKeyScanner() {
  expander.init();
  memset(&debounceMatrix, 0, sizeof(debounceMatrix));
  memset(&localMatrix, 0, sizeof(localMatrix));
  debouncing = kDebounceIterations;

  // Set all the rows to output-high
  for (const auto &pin : rowPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  // 0 is output
  expander.directionA(0b11000000);
  // 1 is high
  expander.dataA(0b00111111);

  // Set all the columns to input-pullup
  for (const auto &pin : colPins) {
    pinMode(pin, INPUT_PULLUP);
  }

  // 1 is input
  expander.directionB(0b01111111);
  // 1 is pull up enabled
  expander.pullupB(0b01111111);
}

void scanMatrix() {
  for (int rowNum = 0; rowNum < sizeof(rowPins) / sizeof(rowPins[0]);
       ++rowNum) {
    // Set just this row to LOW in the expander; the rest are set HIGH
    expander.dataA(~(1<<rowNum));
    digitalWrite(rowPins[rowNum], LOW);
    delayMicroseconds(25);

    uint16_t rowBits = 0;
    uint8_t expanderBits;

    // Read all the columns from the expander in a single IO op
    if (!expander.readDataB(expanderBits)) {
      // Pretend that they are all high if there is a comms error
      expanderBits = 0xff;
    }

    for (int colNum = 0; colNum < sizeof(colPins) / sizeof(colPins[0]);
         ++colNum) {
      if (!digitalRead(colPins[colNum])) {
        rowBits |= 1 << (colNum);
      }
      if ((expanderBits & (1<<colNum)) == 0) {
        rowBits |= 1 << (colNum + 7);
      }
    }

    if (rowBits != debounceMatrix.rows[rowNum]) {
      // Switch status changed, reset the debounce counter
      debouncing = kDebounceIterations;
      debounceMatrix.rows[rowNum] = rowBits;
    }

    digitalWrite(rowPins[rowNum], HIGH);
  }

  if (debouncing) {
    if (--debouncing) {
      delay(1);
    } else {
      // We got a stable result, transfer the data to the matrix
      localMatrix = debounceMatrix;
    }
  }
}

struct matrix_t readMatrix() {
  scanMatrix();
  return localMatrix;
}
