/*
 * main.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Variables

static constexpr uint32_t SPEAKER_PWM_FREQUENCY = 156250;
static constexpr uint8_t SPEAKER_PWM_RESOLUTION_BITS = 8;
static constexpr uint8_t SPEAKER_LEDC_CHANNEL = 0;

extern const unsigned char shupo_8k_u8_raw[];
extern const unsigned int shupo_8k_u8_raw_len;

////////////////////////////////////////////////////////////////////////////////
// setup and loop

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    ledcAttachPin(D0, SPEAKER_LEDC_CHANNEL);
    const uint32_t frequency = ledcSetup(SPEAKER_LEDC_CHANNEL, SPEAKER_PWM_FREQUENCY, SPEAKER_PWM_RESOLUTION_BITS);
    if (frequency == 0) abort();

    delay(1000);
    for (unsigned int i = 0; i < shupo_8k_u8_raw_len; ++i)
    {
        ledcWrite(SPEAKER_LEDC_CHANNEL, shupo_8k_u8_raw[i]);
        delayMicroseconds(125);
    }
}

void loop()
{
    vTaskSuspend(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
