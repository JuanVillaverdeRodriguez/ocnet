/*
  ==============================================================================

    SamplerProcessor.h
    Created: 26 Aug 2024 5:16:14pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Processor.h"
#include "../../Oscillator/WavetableTypes.h"
#include "../../../ParameterHandler/SliderParameter.h"
#include <random>
#include <immintrin.h> // Header para operaciones SSE

class SamplerProcessor : public Processor {
public:
    SamplerProcessor(int id);
    ~SamplerProcessor();

    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    float getNextSample(int sample) override;
    void syncParams(const ParameterHandler& parameterHandler) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;

    void loadAudioFile(const juce::String& filename) override;
    juce::Array<float> getSamplerSampleSamples();

private:
    //void setCurrentAudioFile();
    juce::Array<float> sampleSamples;


    std::shared_ptr<SliderParameter> gainParameter;
    float oscGain;
    juce::Array<float> oscGainModulationBuffer;

    std::shared_ptr<SliderParameter> panningParameter;
    float panning;
    juce::Array<float> panningModulationBuffer;

    //juce::String samplesDirectoryName;
    int currentSample;
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader{ nullptr };
    std::unique_ptr<juce::AudioFormatReaderSource> mFormatReaderSource;
    juce::AudioTransportSource mTransportSource; 
    juce::Array<std::unique_ptr<juce::InputStream>> audioStreams;

    int selectedSampleID;

    juce::SamplerVoice samplerVoice;


};