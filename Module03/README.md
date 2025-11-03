# Module 03: RGB & Advanced Timers

## Overview

Control RGB LEDs using PWM to create millions of colors through red, green, and blue mixing.

## Theoretical Concepts

**RGB LED**: Three LEDs (R, G, B) in one package. Mix intensities for any color.  
**Common Anode/Cathode**: Shared positive (LOW = on) or shared ground (HIGH = on)  
**PWM**: Rapidly switch HIGH/LOW to simulate analog brightness  
**Frequency vs Duty**: Frequency = cycle speed (>60Hz for no flicker), Duty = % time HIGH (brightness)  
**Color Wheel**: Smooth hue transitions by varying two components while keeping third at 0 or max

---

## Exercise 00: Red Green Blue

**Goal**: Basic RGB control with digital signals.

**Requirements**: Cycle RGB LED D5 through Red → Green → Blue every 1s.

**Key Concept**: Each color = separate GPIO pin. Digital ON/OFF creates only primary colors.

---

## Exercise 01: 3-bit RGB Color

**Goal**: Binary color encoding.

**Requirements**: Display 8 colors sequentially (Red, Green, Blue, Yellow, Cyan, Magenta, White) every 1s.

**Key Concept**: 3 bits = 2³ = 8 colors. Yellow = red+green, Cyan = green+blue, Magenta = red+blue.

---

## Exercise 02: 24-bit RGB Color

**Goal**: PWM-based smooth color gradients.

**Requirements**: Write `init_rgb()` and `set_rgb(uint8_t r, uint8_t g, uint8_t b)`. Display color wheel animation using provided `wheel()` function.

**Provided Function**:

```c
void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}
```

**Key Concepts**:

- 8 bits/color = 256³ ≈ 16.7M colors
- `f_PWM = F_CPU / (Prescaler × 256)`. 16MHz/256 = 244Hz (no flicker)
- Duty cycle = brightness: OCR0A=128 = 50% brightness

---

## Exercise 03: UART to RGB

**Goal**: Integrate UART + PWM, parse hex colors.

**Requirements**: Listen on serial for `#RRGGBB` format (e.g., `#FF0000` = red). Set LED D5 to received color.

**Key Concept**: Parse two hex chars per component. Combines Module 02 (UART) + Module 03 (PWM).

**Examples**: `#FF0000`=red, `#00FF00`=green, `#FFFFFF`=white, `#808080`=gray

---

## Skills Developed

- Multi-channel PWM
- Color theory and RGB mixing
- Smooth animations
- Hex parsing
- System integration (UART + Timers)

## Formulas

```
Fast PWM: f_PWM = F_CPU / (Prescaler × 256)
Duty Cycle: (OCRnx / 255) × 100%
```
