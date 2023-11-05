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
    A0 = 28,
    A0s = 29,
    B0b = 29,
    B0 = 31,
    C1 = 33,
    C1s = 35,
    D1b = 35,
    D1 = 37,
    D1s = 39,
    E1b = 39,
    E1 = 41,
    F1 = 44,
    F1s = 46,
    G1b = 46,
    G1 = 49,
    G1s = 52,
    A1b = 52,
    A1 = 55,
    A1s = 58,
    B1b = 58,
    B1 = 62,
    C2 = 65,
    C2s = 69,
    D2b = 69,
    D2 = 73,
    D2s = 78,
    E2b = 78,
    E2 = 82,
    F2 = 87,
    F2s = 93,
    G2b = 93,
    G2 = 98,
    G2s = 104,
    A2b = 104,
    A2 = 110,
    A2s = 117,
    B2b = 117,
    B2 = 123,
    C3 = 131,
    C3s = 139,
    D3b = 139,
    D3 = 147,
    D3s = 156,
    E3b = 156,
    E3 = 165,
    F3 = 175,
    F3s = 185,
    G3b = 185,
    G3 = 196,
    G3s = 208,
    A3b = 208,
    A3 = 220,
    A3s = 233,
    B3b = 233,
    B3 = 247,
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
    C6 = 1047,
    C6s = 1109,
    D6b = 1109,
    D6 = 1175,
    D6s = 1245,
    E6b = 1245,
    E6 = 1319,
    F6 = 1397,
    F6s = 1480,
    G6b = 1480,
    G6 = 1568,
    G6s = 1661,
    A6b = 1661,
    A6 = 1760,
    A6s = 1865,
    B6b = 1865,
    B6 = 1976,
    C7 = 2093,
    C7s = 2217,
    D7b = 2217,
    D7 = 2349,
    D7s = 2489,
    E7b = 2489,
    E7 = 2637,
    F7 = 2794,
    F7s = 2960,
    G7b = 2960,
    G7 = 3136,
    G7s = 3322,
    A7b = 3322,
    A7 = 3520,
    A7s = 3729,
    B7b = 3729,
    B7 = 3951,
    C8 = 4186,
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

static constexpr int ItsyBitsySpiderBeat = 4;
static constexpr int ItsyBitsySpiderTempo = 120;
static const Note ItsyBitsySpiderMelody[] =
{
    Note::MakeDotted(NotePitch::F4, 8), Note::Make(NotePitch::F4, 16), Note::MakeDotted(NotePitch::F4, 8), Note::Make(NotePitch::G4, 16),
    Note::MakeDotted(NotePitch::A4, 8), Note::Make(NotePitch::A4, 16), Note::MakeDotted(NotePitch::A4, 8), Note::Make(NotePitch::A4, 16),
    Note::MakeDotted(NotePitch::G4, 8), Note::Make(NotePitch::F4, 16), Note::MakeDotted(NotePitch::G4, 8), Note::Make(NotePitch::A4, 16),
    Note::MakeDotted(NotePitch::F4, 8), Note::Make(NotePitch::F4, 16), Note::Make(NotePitch::C4, 4),

    Note::MakeDotted(NotePitch::A4, 8), Note::Make(NotePitch::A4, 16), Note::MakeDotted(NotePitch::A4, 8), Note::Make(NotePitch::B4b, 16),
    Note::MakeDotted(NotePitch::C5, 8), Note::Make(NotePitch::C5, 16), Note::MakeDotted(NotePitch::C5, 8), Note::Make(NotePitch::C5, 16),
    Note::MakeDotted(NotePitch::B4b, 8), Note::Make(NotePitch::A4, 16), Note::MakeDotted(NotePitch::B4b, 8), Note::Make(NotePitch::C5, 16),
    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 8), Note::Make(NotePitch::C5, 8),

    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 8), Note::Make(NotePitch::C5, 8),
    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 8), Note::Make(NotePitch::C5, 8),
    Note::Make(NotePitch::A4, 8), Note::Make(NotePitch::C5, 8), Note::Make(NotePitch::A4, 8), Note::Make(NotePitch::C5, 8),
    Note::Make(NotePitch::A4, 4), Note::Make(NotePitch::Rest, 4),
};

void setup()
{
    // USBシリアル通信を初期化。
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    // XIAOGYANライブラリを初期化。
    Xiaogyan.begin();

    for (size_t i = 0; i < std::extent<decltype(ItsyBitsySpiderMelody)>::value; ++i)
    {
        const Note& note = ItsyBitsySpiderMelody[i];
        Xiaogyan.speaker.setTone(static_cast<int>(note.pitch));
        delay((60000.0 * ItsyBitsySpiderBeat / ItsyBitsySpiderTempo) * (note.duration.dotted ? 1.5 : 1) / note.duration.note - 10);
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
