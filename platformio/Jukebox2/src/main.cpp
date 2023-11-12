/*
 * main.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <Arduino.h>
#include "NoteEngine.hpp"
#include "Melody.hpp"

////////////////////////////////////////////////////////////////////////////////
// Constants

static constexpr int ItsyBitsySpiderBeat = 4;
static constexpr int ItsyBitsySpiderTempo = 120;
static Note const ItsyBitsySpiderMelody[] =
    {
        Note::MakeDotted(NotePitch::F4, 8),
        Note::Make(NotePitch::F4, 16),
        Note::MakeDotted(NotePitch::F4, 8),
        Note::Make(NotePitch::G4, 16),
        Note::MakeDotted(NotePitch::A4, 8),
        Note::Make(NotePitch::A4, 16),
        Note::MakeDotted(NotePitch::A4, 8),
        Note::Make(NotePitch::A4, 16),
        Note::MakeDotted(NotePitch::G4, 8),
        Note::Make(NotePitch::F4, 16),
        Note::MakeDotted(NotePitch::G4, 8),
        Note::Make(NotePitch::A4, 16),
        Note::MakeDotted(NotePitch::F4, 8),
        Note::Make(NotePitch::F4, 16),
        Note::Make(NotePitch::C4, 4),

        Note::MakeDotted(NotePitch::A4, 8),
        Note::Make(NotePitch::A4, 16),
        Note::MakeDotted(NotePitch::A4, 8),
        Note::Make(NotePitch::B4b, 16),
        Note::MakeDotted(NotePitch::C5, 8),
        Note::Make(NotePitch::C5, 16),
        Note::MakeDotted(NotePitch::C5, 8),
        Note::Make(NotePitch::C5, 16),
        Note::MakeDotted(NotePitch::B4b, 8),
        Note::Make(NotePitch::A4, 16),
        Note::MakeDotted(NotePitch::B4b, 8),
        Note::Make(NotePitch::C5, 16),
        Note::Make(NotePitch::A4, 4),
        Note::Make(NotePitch::Rest, 8),
        Note::Make(NotePitch::C5, 8),

        Note::Make(NotePitch::A4, 4),
        Note::Make(NotePitch::Rest, 8),
        Note::Make(NotePitch::C5, 8),
        Note::Make(NotePitch::A4, 4),
        Note::Make(NotePitch::Rest, 8),
        Note::Make(NotePitch::C5, 8),
        Note::Make(NotePitch::A4, 8),
        Note::Make(NotePitch::C5, 8),
        Note::Make(NotePitch::A4, 8),
        Note::Make(NotePitch::C5, 8),
        Note::Make(NotePitch::A4, 4),
        Note::Make(NotePitch::Rest, 4),
};

////////////////////////////////////////////////////////////////////////////////
// setup and loop

void setup(void)
{
    // USBシリアル通信を初期化。
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();

    // 再生エンジンを初期化。
    BeginNote();

    // メロディを再生。
    for (size_t i = 0; i < std::extent<decltype(ItsyBitsySpiderMelody)>::value; ++i)
    {
        Note const &note = ItsyBitsySpiderMelody[i];
        NoteOn(static_cast<int>(note.pitch), 127);
        delay((60000.0 * ItsyBitsySpiderBeat / ItsyBitsySpiderTempo) * (note.duration.dotted ? 1.5 : 1) / note.duration.note);
    }
    NoteOn(0, 127);
}

void loop(void)
{
    vTaskSuspend(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
