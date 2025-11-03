# Module 01: Timers

## Overview

Learn hardware timers for precise timing without blocking code. Covers timer modes, prescalers, and PWM.

## Theoretical Concepts

**Hardware Timers**: Count independently (Timer0: 8-bit, Timer1: 16-bit, Timer2: 8-bit)  
**Prescalers**: Divide clock frequency (e.g., 16MHz/1024 = 15,625Hz)  
**Timer Modes**: Normal, CTC (Clear on Compare), PWM (Pulse Width Modulation)  
**Duty Cycle**: Percentage of time signal is HIGH (10% = HIGH for 10%, LOW for 90%)

---

## Exercise 00: Blinker

**Goal**: Create software delays and toggle without conditionals.

**Requirements**: Blink LED D2 (PB1) at ~1Hz using your own delay function. Toggle with XOR: `PORTX ^= (1 << PIN)`.

---

## Exercise 01: Timer1

**Goal**: Configure hardware timer to control outputs automatically.

**Requirements**: Blink LED D2 at exactly 1Hz using Timer1. Main loop empty, no PORTX.

**Key Formula**: `Compare_Value = (F_CPU / (Prescaler × Frequency × 2)) - 1`

---

## Exercise 02: Duty Cycle

**Goal**: Understand PWM and duty cycle.

**Requirements**: LED D2 blinks at 1Hz with 10% duty cycle using Timer1 PWM mode.

**Key Concept**: 10% duty = ON for 0.1s, OFF for 0.9s per cycle.

---

## Exercise 03: The Cycle of Life

**Goal**: Dynamic PWM control with user input.

**Requirements**: Variable duty cycle (10-100%) controlled by SW1 (increment) and SW2 (decrement). Delays only for debouncing.

**Key Concept**: Modify OCR1A to change duty cycle: `Duty = (OCR1A / TOP) × 100%`

---

## Skills Developed

- Timer configuration and modes
- Prescaler calculations
- PWM generation
- Real-time behavior without blocking

## Formulas

```
CTC: f_output = F_CPU / (2 × Prescaler × (1 + OCR1A))
PWM: f_PWM = F_CPU / (Prescaler × (1 + TOP))
Duty: Duty_Cycle = (OCR1A / TOP) × 100%
```
