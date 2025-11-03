# Module 02: UART Protocol

## Overview

Learn UART serial communication for microcontroller-computer data exchange.

## Theoretical Concepts

**UART**: Asynchronous, serial, full-duplex protocol  
**115200 8N1**: 115200 baud, 8 data bits, no parity, 1 stop bit  
**Baud Rate**: `UBRR = (F_CPU / (16 × BAUD)) - 1` (normal) or `(F_CPU / (8 × BAUD)) - 1` (double speed)  
**Registers**: UBRRnH/L (baud), UCSRnA (status), UCSRnB (enable), UCSRnC (format), UDRn (data)

---

## Exercise 00: write(2)

**Goal**: Implement UART transmission.

**Requirements**: Write `uart_init()` and `uart_tx(char c)`. Configure 115200 8N1. Transmit 'Z' at 1Hz.

**Key Concept**: Wait for UDRE flag before transmitting.

---

## Exercise 01: print_str

**Goal**: String transmission with timers.

**Requirements**: Write `uart_printstr(const char* str)`. Print "Hello World!" every 2s. Main loop empty.

**Key Concept**: Iterate until `\0`, call `uart_tx()` for each character.

---

## Exercise 02: read(2)

**Goal**: UART reception and echo.

**Requirements**: Write `uart_rx(void)`. Echo received characters.

**Key Concept**: Check RXC flag before reading UDR.

---

## Exercise 03: Uppercase

**Goal**: Interrupt-driven reception with character transformation.

**Requirements**: Echo with case inversion using UART RX interrupt. Main loop empty.

**Key Concept**: Enable RXCIE in UCSRnB for automatic interrupt on data arrival.

---

## Exercise 04 (Bonus): Login

**Goal**: Complete interactive system.

**Requirements**: Username/password prompt, echo username visible, password as asterisks. Support Backspace and Enter. Blink LEDs on success.

**Key Concept**: State machine for login flow, consume escape sequences.

---

## Skills Developed

- UART configuration
- Interrupt-driven vs polling I/O
- String handling
- Terminal interfaces
- State machines
