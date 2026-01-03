#include <Arduino.h>
#include <BluetoothSerial.h>

#include "oneup_config.h"
#include "buzzer.h"
#include "motor.h"
#include "uwb.h"
#include "follow.h"
#include "path.h"
#include "led.h"
#include "button.h"
#include "display.h"
#include "imu.h"
#include "weight.h"

// ==================== 全局对象 ====================
BluetoothSerial SerialBT;
ButtonManager button;
WorkMode currentMode = MODE_STANDBY;

// 简单命令处理（串口/蓝牙共享）
void handleCommand(char cmd) {
    if (cmd == '\r' || cmd == '\n') return;

    bool recordAction = false;
    PathActionType actionRec = ACTION_STOP;

    switch (cmd) {
        case 'w': case 'W':
        case 'f': case 'F':
            motor.forward();
            actionRec = ACTION_FORWARD;
            recordAction = true;
            break;
        case 's': case 'S':
        case 'b': case 'B':
            motor.backward();
            actionRec = ACTION_BACKWARD;
            recordAction = true;
            break;
        case 'a': case 'A':
        case 'l': case 'L':
            motor.turnLeft();
            actionRec = ACTION_LEFT;
            recordAction = true;
            break;
        case 'd': case 'D':
        case 'r': case 'R':
            motor.turnRight();
            actionRec = ACTION_RIGHT;
            recordAction = true;
            break;
        case 'x': case 'X':
            motor.stop();
            actionRec = ACTION_STOP;
            recordAction = true;
            break;
        default:
            break;
    }

    // 示教模式下记录动作
    if (currentMode == MODE_TEACHING && path.isRecording() && recordAction) {
        path.recordStep(actionRec);
    }
}

void handleInput(Stream& stream) {
    while (stream.available()) {
        char cmd = (char)stream.read();
        handleCommand(cmd);
    }
}

void setMode(WorkMode nextMode) {
    motor.stop();
    buzzer.stopBeeping();

    // 按模式切换速度
    if (nextMode == MODE_FOLLOWING) {
        motor.setSpeed(MOTOR_SPEED_FOLLOW_FORWARD, MOTOR_SPEED_FOLLOW_TURN);
    } else if (nextMode == MODE_TEACHING || nextMode == MODE_RETURNING) {
        motor.setSpeed(MOTOR_SPEED_PATH_FORWARD, MOTOR_SPEED_PATH_TURN);
    } else {
        motor.setSpeed(MOTOR_SPEED_FORWARD, MOTOR_SPEED_TURN);
    }

    if (currentMode == MODE_TEACHING && path.isRecording()) {
        path.stopRecording();
    }
    if (currentMode == MODE_RETURNING && path.isReturning()) {
        path.cancelReturning();
    }

    currentMode = nextMode;

    if (currentMode == MODE_TEACHING) {
        path.startRecording();
    } else if (currentMode == MODE_RETURNING) {
        if (!path.startReturning()) {
            currentMode = MODE_STANDBY;
        }
    }

    Serial.print("切换模式: ");
    Serial.println(MODE_NAMES[currentMode]);
}

void setup() {
    Serial.begin(115200);
    delay(200);

    SerialBT.begin("oneup_template");

    // 初始化各模块
    buzzer.begin();
    ledStrip.begin();
    display.begin();
    motor.begin();
    imu.begin();
    weight.begin();
    uwb.begin();
    follow.begin();
    path.begin();

    // 按钮管理
    button.begin(BUTTON_PIN, true);

    motor.setSpeed(MOTOR_SPEED_FORWARD, MOTOR_SPEED_TURN);
}

void loop() {
    // 按钮事件（单击/双击/长按）
    button.update();
    ButtonEvent evt = button.getEvent();
    if (evt == BUTTON_EVENT_SINGLE) {
        // 待机->背负->跟随->示教->手拉->待机
        if (currentMode == MODE_STANDBY) setMode(MODE_CARRYING);
        else if (currentMode == MODE_CARRYING) setMode(MODE_FOLLOWING);
        else if (currentMode == MODE_FOLLOWING) setMode(MODE_TEACHING);
        else if (currentMode == MODE_TEACHING) setMode(MODE_PULLING);
        else setMode(MODE_STANDBY);
    } else if (evt == BUTTON_EVENT_DOUBLE) {
        setMode(MODE_RETURNING);
    } else if (evt == BUTTON_EVENT_LONG) {
        ledStrip.toggle();
    }

    // 处理串口/蓝牙命令
    handleInput(Serial);
    handleInput(SerialBT);

    // 传感器更新
    uwb.update();
    buzzer.update();
    ledStrip.update();

    static unsigned long lastSensor = 0;
    if (millis() - lastSensor > 100) {
        if (currentMode == MODE_CARRYING) imu.update();
        if (currentMode == MODE_STANDBY) weight.readWeight();
        lastSensor = millis();
    }

    // 模式逻辑
    if (currentMode == MODE_FOLLOWING) {
        if (uwb.isConnected()) {
            UWBData data = uwb.getData();
            follow.update(data.distance, data.angle);
        } else {
            motor.stop();
        }
    } else if (currentMode == MODE_CARRYING) {
        PostureWarning w = imu.checkPosture();
        if (w != POSTURE_OK) buzzer.startBeeping();
        else buzzer.stopBeeping();
    } else if (currentMode == MODE_RETURNING) {
        if (!path.updateReturning()) {
            setMode(MODE_STANDBY);
        }
    }

    // OLED 刷新（可按需裁剪）
    static unsigned long lastDisplay = 0;
    if (millis() - lastDisplay > 200) {
        if (currentMode == MODE_FOLLOWING) {
            UWBData ud = uwb.getData();
            display.showFollowScreen(currentMode, ud.distance, ud.angle, ud.d0, ud.d1);
        } else if (currentMode == MODE_CARRYING) {
            PostureWarning pw = imu.checkPosture();
            display.showCarryingScreen(currentMode, imu.getPitch(), imu.getRoll(), imu.getWarningText(pw));
        } else if (currentMode == MODE_RETURNING) {
            display.showReturningScreen(currentMode, path.getStepCount(), path.getRemainingSteps());
        } else if (currentMode == MODE_TEACHING) {
            display.showTeachingScreen(currentMode, path.getStepCount(), path.isRecording());
        } else if (currentMode == MODE_PULLING) {
            display.showPullingScreen(currentMode);
        }
        lastDisplay = millis();
    }
}
