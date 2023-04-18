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
static constexpr uint32_t SPEAKER_TIMER_FREQUENCY = 10000000;   // 10[MHz]

extern const unsigned char shupo_16k_u8_raw[];
extern const unsigned int shupo_16k_u8_raw_len;

////////////////////////////////////////////////////////////////////////////////
// Speaker2

class Speaker2
{
private:
    static Speaker2* instance_;

    int speakerPin_;
    int speakerEnablePin_;
    int ledControlChannel_;

    hw_timer_t* timer_;
    const uint8_t* wavePtr_;
    const uint8_t* waveEnd_;

private:
    static void timerIsr();

public:
    Speaker2(int speakerPin, int speakerEnablePin, int ledControlChannel);

    void begin();
    void play(const uint8_t* wave, size_t length, uint32_t frequency);

};

Speaker2* Speaker2::instance_ = nullptr;

void Speaker2::timerIsr()
{
    if (instance_->wavePtr_ != instance_->waveEnd_)
    {
        ledcWrite(instance_->ledControlChannel_, *instance_->wavePtr_++);
    }
    else
    {
        timerAlarmDisable(instance_->timer_);
    }
}

Speaker2::Speaker2(int speakerPin, int speakerEnablePin, int ledControlChannel) :
    speakerPin_{ speakerPin },
    speakerEnablePin_{ speakerEnablePin },
    ledControlChannel_{ ledControlChannel },
    timer_{ nullptr },
    wavePtr_{ nullptr },
    waveEnd_{ nullptr }
{
    if (instance_ != nullptr) abort();
    instance_ = this;
}

void Speaker2::begin()
{
    ledcAttachPin(speakerPin_, ledControlChannel_);
    const uint32_t frequency = ledcSetup(ledControlChannel_, SPEAKER_PWM_FREQUENCY, SPEAKER_PWM_RESOLUTION_BITS);
    if (frequency == 0) abort();

    timer_ = timerBegin(0, getApbFrequency() / SPEAKER_TIMER_FREQUENCY, true);
    timerAttachInterrupt(timer_, timerIsr, true);
}

void Speaker2::play(const uint8_t* wave, size_t length, uint32_t frequency)
{
    wavePtr_ = wave;
    waveEnd_ = wave + length;

    timerAlarmWrite(timer_, SPEAKER_TIMER_FREQUENCY / frequency, true);
    timerAlarmEnable(timer_);
}

////////////////////////////////////////////////////////////////////////////////
// setup and loop

static Speaker2 speaker{ D0, D3, 0 };

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    speaker.begin();
    delay(1000);

    speaker.play(shupo_16k_u8_raw, shupo_16k_u8_raw_len, 16000);
}

void loop()
{
    vTaskSuspend(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
