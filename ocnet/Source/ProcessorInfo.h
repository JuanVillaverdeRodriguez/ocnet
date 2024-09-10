/*
  ==============================================================================

    ProcessorInfo.h
    Created: 7 Sep 2024 12:27:51pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

struct HostInfo {

};

struct LegatoInfo {
    int previousMidiNotePressed;
    float previousLegatoFreq = 0.0f;
    bool previousNoteIsBeingPlayed = true;
    float glideValue;
    bool legatoIsActive;
};

struct ProcessorInfo {
    void prepare(float sampleRate, int maxBlockSize) {
        samplesInMinute = sampleRate * 60;
        ppqPositions.resize(maxBlockSize);
    }

    float getPPQPerSample() const {
        auto samplesPerBeat = samplesInMinute / info.bpm;
        return 1.0 / samplesPerBeat; // 1 pulso por cada samplesPerBeat, es decir en un pulso o beat (1/4) hay samplesPerBeat samples
    }

    void process(juce::AudioPlayHead* playHead, int numSamples) {
        if (playHead != nullptr) {
            playHead->getCurrentPosition(info);
        }

        auto ppqPerSample = getPPQPerSample();
        float offset = 0.0f;

        for (int sample = 0; sample < numSamples; ++sample) {
            ppqPositions[sample] = info.ppqPosition + offset;
            offset += ppqPerSample;
        }
    }

    std::atomic<float> currentPosition{ 0 };
    float samplesInMinute = 0.0f;
    juce::AudioPlayHead::CurrentPositionInfo info;
    std::vector<float> ppqPositions;

    LegatoInfo legatoInfo;
};