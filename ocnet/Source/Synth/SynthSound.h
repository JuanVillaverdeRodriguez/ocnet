/*
  ==============================================================================

    SynthSound.h
    Created: 2 May 2024 1:45:43am
    Author:  TecNo

  ==============================================================================
*/

#pragma once
class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};