# oneup_exp

This repo captures reusable code and techniques extracted from the smart backpack project.
It is meant as a quick reference for reuse or for AI context loading.

## Reusable modules and techniques

1) UWB parsing and localization
- Binary frame parsing for MK8000 (F0 05 addrL addrH distL distH rssi AA)
- Robust fallback for ASCII data, CR/LF handling
- Baseline and tag offset calibration, distance scaling
- Angle calculation via atan2 and optional inversion/offset
- Files: src/uwb.cpp, src/uwb.h, src/config.h

2) Follow control loop
- Low-pass filtering (alpha), hysteresis on turn thresholds
- Distance gating (stop inside 1m, minimum distance safety)
- Command hold to reduce oscillation
- Files: src/follow.cpp, src/follow.h, src/config.h

3) Motor PWM driver (L298N)
- PWM on IN1/IN2/IN3/IN4 using LEDC channels
- Runtime speed profiles (forward/turn) per mode
- Files: src/motor.cpp, src/motor.h, src/config.h

4) Buzzer patterns (active-low)
- Non-blocking beep patterns (single, continuous, triple warn)
- Pattern state machine with cooldown
- Files: src/buzzer.cpp, src/buzzer.h, src/main.cpp

5) Button handler state machine
- Single click: mode cycle
- Double click: special mode (return)
- Long press: LED toggle
- Files: src/main.cpp, src/config.h

6) Path teaching and returning
- Record action transitions with durations
- Playback path for return mode
- Files: src/path.cpp, src/path.h

7) Bluetooth remote control
- Simple command protocol (W/A/S/D/X + F/B/L/R/X)
- Shared handler for Serial and BluetoothSerial
- Files: src/main.cpp, src/config.h

8) OLED UI
- Mode-specific screens for follow/carry/teach/return
- Files: src/display.cpp, src/display.h

9) Dual I2C buses (ESP32)
- Separate I2C0 for OLED and I2C1 for MPU6050
- Files: src/config.h, src/imu.cpp, src/display.cpp

10) WS2812 LED strip
- Adafruit NeoPixel integration and simple on/off toggle
- Files: src/led.cpp, src/led.h, src/config.h

## Configuration knobs worth reusing

- UWB geometry and angle: UWB_BASELINE, UWB_TAG_OFFSET, UWB_ANGLE_INVERT, UWB_ANGLE_OFFSET
- Follow control: FOLLOW_ENABLE_DISTANCE, FOLLOW_FILTER_ALPHA, FOLLOW_TURN_ON/OFF
- Motor speeds: MOTOR_SPEED_FOLLOW_FORWARD, MOTOR_SPEED_PATH_FORWARD, MOTOR_SPEED_TURN
- Buzzer warn: WEIGHT_WARNING_THRESHOLD, BUZZER_WARN_DURATION/INTERVAL

## Integration tips

- Keep the loop non-blocking: update sensors, then run mode logic, then update display
- Guard actions by connection state (UWB connected) to avoid motor runaway
- Use per-mode speed profiles for stable behavior
