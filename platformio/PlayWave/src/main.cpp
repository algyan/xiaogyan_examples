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
// Variables

extern const unsigned char shupo_16k_u8_raw[];
extern const unsigned int shupo_16k_u8_raw_len;

////////////////////////////////////////////////////////////////////////////////
// setup and loop

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    Xiaogyan.begin();
    delay(1000);

    Xiaogyan.speaker.play(shupo_16k_u8_raw, shupo_16k_u8_raw_len, 16000);
}

void loop()
{
    vTaskSuspend(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
