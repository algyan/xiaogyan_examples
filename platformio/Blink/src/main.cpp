/*
 * main.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <Arduino.h>
#include "Xiaogyan.hpp"

////////////////////////////////////////////////////////////////////////////////
// setup and loop

void setup()
{
    // USBシリアル通信を初期化。
    // Serial.begin(115200);
    // delay(1000);
    // Serial.println();
    // Serial.println();

    // XIAOGYANライブラリを初期化。
    Xiaogyan.begin();
}

void loop()
{
    // XIAOGYANライブラリを実行。
    Xiaogyan.doWork();

    // LEDを点滅する。
    // （200ミリ秒点灯、800ミリ秒消灯を繰り返す。）
    Xiaogyan.led.write(millis() % 1000 < 200 ? LOW : HIGH);
}

////////////////////////////////////////////////////////////////////////////////
