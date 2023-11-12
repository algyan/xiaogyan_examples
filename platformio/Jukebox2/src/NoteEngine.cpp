/*
 * NoteEngine.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

#include <Arduino.h>
#include <driver/ledc.h>
#include <driver/timer.h>

static uint16_t const SinWave[] =
    {
        1024,
        1049,
        1074,
        1099,
        1124,
        1149,
        1174,
        1199,
        1224,
        1248,
        1273,
        1297,
        1321,
        1345,
        1369,
        1392,
        1415,
        1439,
        1461,
        1484,
        1506,
        1528,
        1550,
        1571,
        1592,
        1613,
        1633,
        1653,
        1673,
        1692,
        1711,
        1729,
        1747,
        1765,
        1782,
        1799,
        1815,
        1830,
        1846,
        1860,
        1875,
        1888,
        1901,
        1914,
        1926,
        1938,
        1949,
        1959,
        1969,
        1978,
        1987,
        1995,
        2003,
        2010,
        2016,
        2022,
        2027,
        2032,
        2036,
        2039,
        2042,
        2044,
        2046,
        2047,
        2047,
        2047,
        2046,
        2044,
        2042,
        2039,
        2036,
        2032,
        2027,
        2022,
        2016,
        2010,
        2003,
        1995,
        1987,
        1978,
        1969,
        1959,
        1949,
        1938,
        1926,
        1914,
        1901,
        1888,
        1875,
        1860,
        1846,
        1830,
        1815,
        1799,
        1782,
        1765,
        1747,
        1729,
        1711,
        1692,
        1673,
        1653,
        1633,
        1613,
        1592,
        1571,
        1550,
        1528,
        1506,
        1484,
        1461,
        1439,
        1415,
        1392,
        1369,
        1345,
        1321,
        1297,
        1273,
        1248,
        1224,
        1199,
        1174,
        1149,
        1124,
        1099,
        1074,
        1049,
        1024,
        999,
        974,
        949,
        924,
        899,
        874,
        849,
        824,
        800,
        775,
        751,
        727,
        703,
        679,
        656,
        633,
        609,
        587,
        564,
        542,
        520,
        498,
        477,
        456,
        435,
        415,
        395,
        375,
        356,
        337,
        319,
        301,
        283,
        266,
        249,
        233,
        218,
        202,
        188,
        173,
        160,
        147,
        134,
        122,
        110,
        99,
        89,
        79,
        70,
        61,
        53,
        45,
        38,
        32,
        26,
        21,
        16,
        12,
        9,
        6,
        4,
        2,
        1,
        1,
        1,
        2,
        4,
        6,
        9,
        12,
        16,
        21,
        26,
        32,
        38,
        45,
        53,
        61,
        70,
        79,
        89,
        99,
        110,
        122,
        134,
        147,
        160,
        173,
        188,
        202,
        218,
        233,
        249,
        266,
        283,
        301,
        319,
        337,
        356,
        375,
        395,
        415,
        435,
        456,
        477,
        498,
        520,
        542,
        564,
        587,
        609,
        633,
        656,
        679,
        703,
        727,
        751,
        775,
        800,
        824,
        849,
        874,
        899,
        924,
        949,
        974,
        999,
};

struct NoteOnMessage
{
    int Frequency;
    int Velocity;
};

static QueueHandle_t FreqQueue_;

static bool IRAM_ATTR TimerHandler(void *arg)
{
    BaseType_t taskWoken = pdFALSE;

    // int64_t time_since_boot = esp_timer_get_time();
    // Serial.println(time_since_boot);

    // Interval 12.5us
    // gpio_set_level(GPIO_NUM_21, 1);

    static int freq = 0;
    static int elapsed = 0;
    {
        NoteOnMessage msg;
        if (xQueueReceiveFromISR(FreqQueue_, &msg, &taskWoken))
        {
            freq = msg.Frequency;
            elapsed = 0;
        }
        else
        {
            ++elapsed;
        }
    }

    static int vco_x = 0;
    if (freq <= 0)
    {
        vco_x = 0;
    }
    else
    {
        vco_x += freq;
        if (vco_x >= 80000)
            vco_x -= 80000;
    }
    uint32_t const vco = SinWave[vco_x * std::extent<decltype(SinWave)>::value / 80000];

    uint32_t const duty = (static_cast<int>(vco) - 128) * (60000 - (elapsed < 60000 ? elapsed : 60000)) / 60000 + 128;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // gpio_set_level(GPIO_NUM_21, 0);

    return taskWoken == pdTRUE;
}

static StaticQueue_t FreqQueueStatic_;
static NoteOnMessage FreqQueueStorage_[1];

void BeginNote(void)
{
    // // Setup output
    // gpio_config_t const io_conf =
    //     {
    //         .pin_bit_mask = 1 << GPIO_NUM_21,
    //         .mode = GPIO_MODE_OUTPUT,
    //         .pull_up_en = GPIO_PULLUP_DISABLE,
    //         .pull_down_en = GPIO_PULLDOWN_DISABLE,
    //         .intr_type = GPIO_INTR_DISABLE,
    //     };
    // ESP_ERROR_CHECK(gpio_config(&io_conf));
    // ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_21, 0));

    // Create queue
    FreqQueue_ = xQueueCreateStatic(std::extent<decltype(FreqQueueStorage_)>::value, sizeof(FreqQueueStorage_[0]), reinterpret_cast<uint8_t *>(FreqQueueStorage_), &FreqQueueStatic_);
    configASSERT(FreqQueue_ != nullptr);

    // Setup LEDC
    ledc_timer_config_t const ledc_timer_conf =
        {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_11_BIT,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 39000,
            .clk_cfg = LEDC_USE_APB_CLK,
        };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_conf));

    ledc_channel_config_t const ledc_channel_conf =
        {
            .gpio_num = GPIO_NUM_2,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = LEDC_CHANNEL_0,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = 0,
            .hpoint = 0,
            .flags = {.output_invert = 0},
        };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_conf));

    // Setup timer
    timer_config_t const timer_conf =
        {
            .alarm_en = TIMER_ALARM_EN,
            .counter_en = TIMER_PAUSE,
            .intr_type = TIMER_INTR_LEVEL,
            .counter_dir = TIMER_COUNT_UP,
            .auto_reload = TIMER_AUTORELOAD_EN,
            .divider = 80000000 / 10000000,
        };
    ESP_ERROR_CHECK(timer_init(TIMER_GROUP_0, TIMER_0, &timer_conf));
    ESP_ERROR_CHECK(timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0));

    ESP_ERROR_CHECK(timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, TimerHandler, nullptr, ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 10000000 / 80000));

    ESP_ERROR_CHECK(timer_start(TIMER_GROUP_0, TIMER_0));
}

void NoteOn(int frequency, int velocity)
{
    NoteOnMessage const msg = {.Frequency = frequency, .Velocity = velocity};
    xQueueSendToBack(FreqQueue_, &msg, 0);
}
