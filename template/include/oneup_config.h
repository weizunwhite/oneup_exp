#ifndef ONEUP_CONFIG_H
#define ONEUP_CONFIG_H

#include <Arduino.h>

// ==================== ???? ====================
#define DEBUG_ENABLED 1

#if DEBUG_ENABLED
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(...)
#endif

// ==================== ?????? ====================
enum WorkMode {
    MODE_STANDBY = 0,       // ????
    MODE_CARRYING = 1,      // ????
    MODE_FOLLOWING = 2,     // ????
    MODE_PULLING = 3,       // ????
    MODE_RETURNING = 4,     // ????
    MODE_TEACHING = 5,      // ????
    MODE_COUNT = 6
};

// ?????????????
static const char* const MODE_NAMES_CN[] = {
    "??", "??", "??", "??", "??", "??"
};

static const char* const MODE_NAMES[] = {
    "Standby", "Carrying", "Following", "Pulling", "Returning", "Teaching"
};

// ==================== ???? ====================
// I2C0 (OLED)
#define I2C0_SDA_PIN 21
#define I2C0_SCL_PIN 22

// I2C1 (MPU6050)
#define I2C1_SDA_PIN 25
#define I2C1_SCL_PIN 26

#define MPU6050_ADDR 0x68
#define SSD1306_ADDR 0x3C

// UWB ????
#define UWB0_RX_PIN 16
#define UWB0_TX_PIN 17
#define UWB1_RX_PIN 27
#define UWB1_TX_PIN 13
#define UWB_BAUD_RATE 115200

// ?????L298N?
#define MOTOR_LEFT_IN1  5
#define MOTOR_LEFT_IN2  14
#define MOTOR_RIGHT_IN1 32
#define MOTOR_RIGHT_IN2 33

// HX711 ??
#define HX711_DOUT_PIN 18
#define HX711_SCK_PIN  19
#define WEIGHT_CALIBRATION_FACTOR 420.0f

// ??
#define BUTTON_PIN 4
#define BUTTON_LONG_PRESS_TIME 1000  // ??????(ms)
#define BUTTON_DOUBLE_CLICK_MS 400   // ????(ms)
#define BUTTON_DEBOUNCE_MS 30        // ????(ms)

// ??????????
#define BUZZER_PIN 23
#define BUZZER_ACTIVE_LOW 1

// WS2812 ??
#define LED_STRIP_PIN 12
#define LED_STRIP_COUNT 30
#define LED_STRIP_ENABLED 1

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// ==================== ???? ====================
// UWB ??
#define UWB_BASELINE 28.0f
#define UWB_TAG_OFFSET 12.5f
#define UWB_DISTANCE_SCALE 1.0f
#define UWB_HEX_SCALE 0.1f
#define UWB_ANGLE_INVERT 1
#define UWB_ANGLE_OFFSET 0.0f

// ??????
#define FOLLOW_DIST_TARGET 80.0f
#define FOLLOW_DIST_DEADZONE 15.0f
#define FOLLOW_ANGLE_DEADZONE 15.0f
#define FOLLOW_ENABLE_DISTANCE 100.0f
#define FOLLOW_MIN_DISTANCE 40.0f
#define FOLLOW_FILTER_ALPHA 0.1f
#define FOLLOW_TURN_ON 35.0f
#define FOLLOW_TURN_OFF 15.0f
#define FOLLOW_CMD_HOLD_MS 300

// ?? PWM ??
#define MOTOR_PWM_FREQ 2000
#define MOTOR_PWM_RESOLUTION 8
#define MOTOR_SPEED_FORWARD 100
#define MOTOR_SPEED_TURN 80

// ??????
#define MOTOR_SPEED_FOLLOW_FORWARD 150
#define MOTOR_SPEED_FOLLOW_TURN 80
#define MOTOR_SPEED_PATH_FORWARD 204
#define MOTOR_SPEED_PATH_TURN 204

// ????
#define BEND_THRESHOLD 25.0f
#define SHOULDER_THRESHOLD 15.0f

// ????
#define WEIGHT_OVERLOAD_THRESHOLD 5000.0f
#define WEIGHT_WARNING_THRESHOLD 1000.0f
#define WEIGHT_WARNING_COOLDOWN_MS 3000

// ????
#define PATH_MAX_STEPS 100

// ?????
#define BUZZER_BEEP_INTERVAL 1000
#define BUZZER_WARN_DURATION 120
#define BUZZER_WARN_INTERVAL 120

// ???????
#define LED_BRIGHTNESS 128
#define LED_COLOR_R 255
#define LED_COLOR_G 0
#define LED_COLOR_B 0

#endif // ONEUP_CONFIG_H
