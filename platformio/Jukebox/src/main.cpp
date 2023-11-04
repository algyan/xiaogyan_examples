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

enum class NotePitch
{
    Rest = 0,
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

struct NoteDuration
{
    int note;
    bool dotted;
    bool tie;
};

struct Note
{
    NotePitch pitch;
    NoteDuration duration;

    static constexpr Note Make(NotePitch pitch, int duration, bool tie = false)
    {
        return {pitch, {duration, false, tie}};
    }

    static constexpr Note MakeDotted(NotePitch pitch, int duration, bool tie = false)
    {
        return {pitch, {duration, true, tie}};
    }
};

static const Note MelodyItsyBitsySpider[] =
{
    Note::MakeDotted(NotePitch::F4, 2), Note::Make(NotePitch::F4, 1), Note::MakeDotted(NotePitch::F4, 2), Note::Make(NotePitch::G4, 1),
    Note::MakeDotted(NotePitch::A4, 2), Note::Make(NotePitch::A4, 1), Note::MakeDotted(NotePitch::A4, 2), Note::Make(NotePitch::A4, 1),
    Note::MakeDotted(NotePitch::G4, 2), Note::Make(NotePitch::F4, 1), Note::MakeDotted(NotePitch::G4, 2), Note::Make(NotePitch::A4, 1),
    Note::MakeDotted(NotePitch::F4, 2), Note::Make(NotePitch::F4, 1), Note::Make(NotePitch::C4, 4),

    Note::MakeDotted(NotePitch::A4, 2), Note::Make(NotePitch::A4, 1), Note::MakeDotted(NotePitch::A4, 2), Note::Make(NotePitch::B4b, 1),
    Note::MakeDotted(NotePitch::C5, 2), Note::Make(NotePitch::C5, 1), Note::MakeDotted(NotePitch::C5, 2), Note::Make(NotePitch::C5, 1),
    Note::MakeDotted(NotePitch::B4b, 2), Note::Make(NotePitch::A4, 1), Note::MakeDotted(NotePitch::B4b, 2), Note::Make(NotePitch::C5, 1),
    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 2), Note::Make(NotePitch::C5, 2),

    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 2), Note::Make(NotePitch::C5, 2),
    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 2), Note::Make(NotePitch::C5, 2),
    Note::Make(NotePitch::A4, 2), Note::Make(NotePitch::C5, 2), Note::Make(NotePitch::A4, 2), Note::Make(NotePitch::C5, 2),
    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 4),
};

static constexpr int Tempo = 150;   // TODO

void setup()
{
    // USBシリアル通信を初期化。
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    // XIAOGYANライブラリを初期化。
    Xiaogyan.begin();

    for (size_t i = 0; i < std::extent<decltype(MelodyItsyBitsySpider)>::value; ++i)
    {
        const Note& note = MelodyItsyBitsySpider[i];
        Xiaogyan.speaker.setTone(static_cast<int>(note.pitch));
        delay(note.duration.note * (note.duration.dotted ? 1.5 : 1) * Tempo);
        if (!note.duration.tie) Xiaogyan.speaker.setTone(0);
        delay(10);
    }

    Xiaogyan.speaker.setTone(0);
}

void loop()
{
    vTaskSuspend(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
