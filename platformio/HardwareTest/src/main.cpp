/*
 * main.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <Arduino.h>
#include <elapsedMillis.h>
#include "Xiaogyan.hpp"

////////////////////////////////////////////////////////////////////////////////
// Variables

static int EncoderValue_ = 5;

////////////////////////////////////////////////////////////////////////////////
// setup and loop

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    ////////////////////////////////////////
    // Initialize

    // XIAOGYANライブラリを初期化。
    Xiaogyan.begin();

    // ロータリーエンコーダを回転したときに、EncoderValue_を増加/減少する。
    // （EncoderValue_は0～19の範囲内。）
    Xiaogyan.encoder.setRotatedHandler([](bool cw){
        const int value = EncoderValue_ + (cw ? -1 : 1);
        EncoderValue_ = constrain(value, 0, 19);
        Serial.println(EncoderValue_);
    });

    ////////////////////////////////////////
    // Startup Sequence

    // 「ド」音を鳴らす。
    Xiaogyan.speaker.setTone(262);  // C4
    // LEDマトリックスを全て赤色点灯。
    Xiaogyan.ledMatrix.setBrightness(2);
    Xiaogyan.ledMatrix.fillScreen(1);
    delay(200);

    // 音を止める。
    Xiaogyan.speaker.setTone(0);
    // LEDマトリックスを全て消灯。
    Xiaogyan.ledMatrix.fillScreen(0);
}

void loop()
{
    // XIAOGYANライブラリを実行。
    Xiaogyan.doWork();

    // LEDを点滅する。
    // （200ミリ秒点灯、800ミリ秒消灯を繰り返す。）
    Xiaogyan.led.write(millis() % 1000 < 200 ? LOW : HIGH);

    // Aボタン、Bボタンで「ド」「レ」「ミ」を鳴らす。
    static bool buttonA = false;
    static bool buttonB = false;
    bool preButtonA = buttonA;
    bool preButtonB = buttonB;
    buttonA = Xiaogyan.buttonA.read() == LOW;
    buttonB = Xiaogyan.buttonB.read() == LOW;
    if (preButtonA != buttonA || preButtonB != buttonB)
    {
        if      ( buttonA && !buttonB) Xiaogyan.speaker.setTone(262);   // C4
        else if (!buttonA &&  buttonB) Xiaogyan.speaker.setTone(294);   // D4
        else if ( buttonA &&  buttonB) Xiaogyan.speaker.setTone(330);   // E4
        else                           Xiaogyan.speaker.setTone(0);
    }

    // LEDマトリックスを流れるように点灯。
    static const int COLOR_MAP[] = { 1, 0, 2, 0, 3, 0, };
    static int x = 0;
    static int y = 0;
    static int colorIndex = 0;
    static elapsedMillis ledMatrixElapsed{ 0 };

    if (ledMatrixElapsed >= (EncoderValue_ + 1) * 10)
    {
        ledMatrixElapsed = 0;

        Xiaogyan.ledMatrix.drawPixel(x, y, COLOR_MAP[colorIndex]);

        if (++x >= Xiaogyan.ledMatrix.width())
        {
            x = 0;
            if (++colorIndex >= std::extent<decltype(COLOR_MAP)>::value)
            {
                colorIndex = 0;
                if (++y >= Xiaogyan.ledMatrix.height())
                {
                    y = 0;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
