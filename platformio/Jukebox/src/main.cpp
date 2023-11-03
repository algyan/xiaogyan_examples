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

enum class Pitch
{
    OFF = 0,
    C4 = 262,
    C4s = 277,
    D4f = 277,
    D4 = 294,
    D4s = 311,
    E4f = 311,
    E4 = 330,
    F4 = 349,
    F4s = 370,
    G4f = 370,
    G4 = 392,
    G4s = 415,
    A4f = 415,
    A4 = 440,
    A4s = 466,
    B4f = 466,
    B4 = 494,
    C5 = 523,
    C5s = 554,
    D5f = 554,
    D5 = 587,
    D5s = 622,
    E5f = 622,
    E5 = 659,
    F5 = 698,
    F5s = 740,
    G5f = 740,
    G5 = 784,
    G5s = 831,
    A5f = 831,
    A5 = 880,
    A5s = 932,
    B5f = 932,
    B5 = 988,
};

struct Note
{
    Pitch pitch;
    int duration;
};

static const Note MelodyItsyBitsySpider[] =
{
    {Pitch::F4, 3}, {Pitch::F4, 1}, {Pitch::F4, 3}, {Pitch::G4, 1},
    {Pitch::A4, 3}, {Pitch::A4, 1}, {Pitch::A4, 3}, {Pitch::A4, 1},
    {Pitch::G4, 3}, {Pitch::F4, 1}, {Pitch::G4, 3}, {Pitch::A4, 1},
    {Pitch::F4, 3}, {Pitch::F4, 1}, {Pitch::C4, 4},

    {Pitch::A4, 3}, {Pitch::A4, 1}, {Pitch::A4, 3}, {Pitch::B4f, 1},
    {Pitch::C5, 3}, {Pitch::C5, 1}, {Pitch::C5, 3}, {Pitch::C5, 1},
    {Pitch::B4f, 3}, {Pitch::A4, 1}, {Pitch::B4f, 3}, {Pitch::C5, 1},
    {Pitch::A4, 4}, {Pitch::OFF, 2}, {Pitch::C5, 2},

    {Pitch::A4, 4}, {Pitch::OFF, 2}, {Pitch::C5, 2},
    {Pitch::A4, 4}, {Pitch::OFF, 2}, {Pitch::C5, 2},
    {Pitch::A4, 2}, {Pitch::C5, 2}, {Pitch::A4, 2}, {Pitch::C5, 2},
    {Pitch::A4, 4}, {Pitch::OFF, 4},
};

static constexpr int Tempo = 150;

void setup()
{
    // XIAOGYANライブラリを初期化。
    Xiaogyan.begin();

    for (size_t i = 0; i < std::extent<decltype(MelodyItsyBitsySpider)>::value; ++i)
    {
        const Note& note = MelodyItsyBitsySpider[i];
        Xiaogyan.speaker.setTone(static_cast<int>(note.pitch));
        delay(note.duration * Tempo);
        Xiaogyan.speaker.setTone(0);
        delay(10);
    }

    Xiaogyan.speaker.setTone(0);
}

void loop()
{
    vTaskSuspend(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
