/*
  ==============================================================================

    SamplerProcessor.cpp
    Created: 26 Aug 2024 5:16:14pm
    Author:  TecNo

  ==============================================================================
*/

#include "SamplerProcessor.h"


SamplerProcessor::SamplerProcessor(int id) : selectedSampleID(0), currentSample(0), sampleSamples()
{
    setId(id);
    mFormatManager.registerBasicFormats();
}

SamplerProcessor::~SamplerProcessor()
{
    mTransportSource.stop();
    mTransportSource.setSource(nullptr);
    mFormatReaderSource = nullptr;
}

void SamplerProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    mTransportSource.prepareToPlay(spec.maximumBlockSize, spec.sampleRate);
}

void SamplerProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    mTransportSource.setPosition(0.0);
    mTransportSource.start();
}

void SamplerProcessor::stopNote(float velocity, bool allowTailOff)
{
    //mTransportSource.stop();
}

void SamplerProcessor::updateParameterValues()
{
    oscGain = gainParameter->getValue();
    oscGainModulationBuffer = gainParameter->getModulationBuffer(getVoiceNumberId());

    panning = panningParameter->getValue();
    panningModulationBuffer = panningParameter->getModulationBuffer(getVoiceNumberId());
}

float SamplerProcessor::getNextSample(int sample)
{
    return 0.0f;
}

void SamplerProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    gainParameter = parameterHandler.syncWithSliderParam(juce::String("Sampler_") + juce::String(getId()) + juce::String("_volume"));
    panningParameter = parameterHandler.syncWithSliderParam(juce::String("Sampler_") + juce::String(getId()) + juce::String("_panning"));
}

void SamplerProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    const auto globalPanAngle = (panning + panningModulationBuffer[0]) * juce::MathConstants<float>::halfPi;
    const auto globalPanningLeft = std::cos(globalPanAngle);
    const auto globalPanningRight = std::sin(globalPanAngle);

    auto* leftChannelBuffer = buffer.getWritePointer(0);
    auto* rightChannelBuffer = buffer.getWritePointer(1);

    juce::AudioSourceChannelInfo bufferToFill(&buffer, 0, buffer.getNumSamples());
    mTransportSource.setGain(oscGain);
    mTransportSource.getNextAudioBlock(bufferToFill);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        leftChannelBuffer[i] *= globalPanningLeft;
        rightChannelBuffer[i] *= globalPanningRight;
    }
}



void SamplerProcessor::loadAudioFile(const juce::String& filename)
{
    juce::File file(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Ocnet").getChildFile("Sampler").getChildFile(filename));
    auto* reader = mFormatManager.createReaderFor(file);

    if (reader != nullptr)
    {
        // Crear un buffer para almacenar todos los samples
        juce::AudioBuffer<float> tempBuffer(reader->numChannels, static_cast<int>(reader->lengthInSamples));

        // Leer los samples desde el archivo al buffer temporal
        reader->read(&tempBuffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);

        // Copiar los samples en un juce::Array
        sampleSamples.clear();
        for (int channel = 0; channel < tempBuffer.getNumChannels(); ++channel)
        {
            for (int i = 0; i < tempBuffer.getNumSamples(); ++i)
            {
                sampleSamples.add(tempBuffer.getSample(channel, i));
            }
        }

        // Configurar el AudioTransportSource para la reproducción, si es necesario
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        mTransportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        mFormatReaderSource.reset(newSource.release());
    }
}

juce::Array<float> SamplerProcessor::getSamplerSampleSamples()
{
    return sampleSamples;
}
