# Module 00: First Programs

## Overview

Introduction to ATMega328p and GPIO manipulation. Learn to control LEDs and read button states through direct register access.

## Theoretical Concepts

**GPIO Registers**: DDRx (direction), PORTx (output/pull-up), PINx (input)  
**Debouncing**: Filter button electrical noise with delays

---

## Exercise 00: Setup

**Goal**: Learn AVR toolchain (avr-gcc, avr-objcopy, avrdude) and create a Makefile for embedded compilation.

**Requirements**: Empty main(), Makefile with `all`, `hex`, `flash`, and `clean` rules.

---

## Exercise 01: A Glimmer of Hope

**Goal**: Configure GPIO output and control an LED.

**Requirements**: Turn on LED D1 (PB0) using only AVR registers.

**Key Concept**: DDR=1 sets output, PORT=1 sets HIGH.

---

## Exercise 02: Lumos

**Goal**: Read digital inputs and create interactive systems.

**Requirements**: LED D1 lights when SW1 (PD2) is pressed, off when released.

**Key Concept**: Enable internal pull-up; button grounds pin when pressed.

---

## Exercise 03: Day, Night, Day, Night

**Goal**: Implement edge detection and debouncing.

**Requirements**: Toggle LED PB0 on each button press (released→pressed transition).

**Key Concept**: Buttons "bounce"; add delay to avoid multiple triggers.

---

## Exercise 04 (Bonus): Binary Counter

**Goal**: Multiple I/O and bit manipulation.

**Requirements**: SW1 increments, SW2 decrements, display value on LEDs D1-D4 in binary.

**Key Concept**: D4 is on PB4 (not PB3), requires bit manipulation for non-contiguous mapping.

---

## Skills Developed

- Direct register manipulation
- Digital I/O fundamentals
- Debouncing and edge detection
- Bit manipulation
