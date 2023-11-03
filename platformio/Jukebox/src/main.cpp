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
    REST = 0,
    C4 = 262,
    C4s = 277,
    D4b = 277,
    D4 = 294,
    D4s = 311,
    E4b = 311,
    E4 = 330,
    F4 = 349,
    F4s = 370,
    G4b = 370,
    G4 = 392,
    G4s = 415,
    A4b = 415,
    A4 = 440,
    A4s = 466,
    B4b = 466,
    B4 = 494,
    C5 = 523,
    C5s = 554,
    D5b = 554,
    D5 = 587,
    D5s = 622,
    E5b = 622,
    E5 = 659,
    F5 = 698,
    F5s = 740,
    G5b = 740,
    G5 = 784,
    G5s = 831,
    A5b = 831,
    A5 = 880,
    A5s = 932,
    B5b = 932,
    B5 = 988,
};

typedef int Duration;

struct Note
{
    Pitch pitch;
    Duration duration;
};

static const Note MelodyItsyBitsySpider[] =
{
    {Pitch::F4, 3}, {Pitch::F4, 1}, {Pitch::F4, 3}, {Pitch::G4, 1},
    {Pitch::A4, 3}, {Pitch::A4, 1}, {Pitch::A4, 3}, {Pitch::A4, 1},
    {Pitch::G4, 3}, {Pitch::F4, 1}, {Pitch::G4, 3}, {Pitch::A4, 1},
    {Pitch::F4, 3}, {Pitch::F4, 1}, {Pitch::C4, 4},

    {Pitch::A4, 3}, {Pitch::A4, 1}, {Pitch::A4, 3}, {Pitch::B4b, 1},
    {Pitch::C5, 3}, {Pitch::C5, 1}, {Pitch::C5, 3}, {Pitch::C5, 1},
    {Pitch::B4b, 3}, {Pitch::A4, 1}, {Pitch::B4b, 3}, {Pitch::C5, 1},
    {Pitch::A4, 4}, {Pitch::REST, 2}, {Pitch::C5, 2},

    {Pitch::A4, 4}, {Pitch::REST, 2}, {Pitch::C5, 2},
    {Pitch::A4, 4}, {Pitch::REST, 2}, {Pitch::C5, 2},
    {Pitch::A4, 2}, {Pitch::C5, 2}, {Pitch::A4, 2}, {Pitch::C5, 2},
    {Pitch::A4, 4}, {Pitch::REST, 4},
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
