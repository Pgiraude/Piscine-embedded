# Module 04: Interruptions

## Overview

Learn to handle interruptions on the microcontroller using Pin Change Interrupts (PCINT) and Timer Interrupts. Master event-driven programming where the microcontroller reacts instantly to external events without blocking the main execution loop.

## Theoretical Concepts

**Interrupts**: Automatic execution of code when specific events occur, without blocking the main program  
**Pin Change Interrupts (PCINT)**: Trigger on any state change (HIGH→LOW or LOW→HIGH) on configured pins  
**Timer Interrupts**: Periodic automatic execution based on timer overflow or compare match  
**ISR (Interrupt Service Routine)**: Function executed when interrupt occurs  
**Edge Detection**: Detect state transitions (rising/falling edge) using state comparison  
**Debouncing**: Delay after button press to filter mechanical noise and multiple triggers  
**Registers**: PCICR (enable PCINT groups), PCMSKx (select pins), PCIFR (clear flags), TIMSKx (enable timer interrupts)

---

## Exercise 00: External Interrupt

**Goal**: Implement basic Pin Change Interrupt to toggle LED on button press.

**Requirements**: Change LED D1 state when button SW1 is pressed. Use interrupts to read button state. Reading PINx registers is forbidden.

**Key Concept**: Enable PCINT group (PCICR), mask specific pin (PCMSKx), clear flag in ISR (PCIFR). Use `static` variables to maintain state between interrupt calls.

---

## Exercise 01: Timer0 Interrupt and PWM

**Goal**: Use Timer0 overflow interrupt to periodically update Timer1 PWM duty cycle for smooth LED brightness control.

**Requirements**: Configure Timer0 to trigger periodic interrupt that varies the duty cycle of LED PB1 controlled by Timer1. Timer1 frequency must be high enough to avoid visible flickering. Duty cycle must loop from 0% to 100% then 100% to 0% in 1 second.

**Key Concepts**:

- Timer0 overflow interrupt: `TIMSK0 |= (1 << TOIE0)`
- ISR updates PWM duty cycle: `OCR1A = new_value`
- Direction control: increment/decrement based on boundaries
- Timer0: Phase Correct PWM or Normal mode with prescaler
- Timer1: Fast PWM Mode 14, ICR1 = TOP, high frequency prescaler
- Frequency calculation: `f = F_CPU / (Prescaler × (TOP + 1))`

---

## Exercise 02: Binary Counter

**Goal**: Advanced Pin Change Interrupt with edge detection for dual-button counter with binary LED display.

**Requirements**: Increment value on SW1 press, decrement on SW2 press. Display value permanently on LEDs D1, D2, D3, D4 in binary. Use interrupts with empty main loop. Handle bit manipulation for LED D4 on PB4 (not PB3).

**Key Concept**: Edge detection using XOR state comparison (`changed = current ^ previous`). Detect falling edge (pull-up → pressed). Track state with `static` variable. Enable multiple PCINTs in same mask. Bit manipulation for LED mapping.

---

## Skills Developed

- Pin Change Interrupt configuration
- Timer overflow interrupts
- Edge detection algorithms
- State machine design in ISRs
- Debouncing techniques
- Multi-pin interrupt handling
- Interrupt-driven PWM control
- Bit manipulation for non-sequential GPIO mapping
- Event-driven programming architecture

---

## Formulas

```
Timer Frequency: f = F_CPU / (Prescaler × (TOP + 1))
Phase Correct PWM: f = F_CPU / (Prescaler × 510)
Fast PWM Mode 14: f = F_CPU / (Prescaler × (ICR1 + 1))
```
