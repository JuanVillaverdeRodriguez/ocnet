/*
  ==============================================================================

    ReverbProcessor.h
    Created: 27 Aug 2024 8:00:06pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Effector.h"
#include <random>
#include "../../../../Libraries/SignalSmithLibrary/dsp/mix.h"
#include "../../../../Libraries/SignalSmithLibrary/dsp/delay.h"
#include <immintrin.h> //SIMD
#include <omp.h>

using DelaySignalSmith = signalsmith::delay::Delay<float, signalsmith::delay::InterpolatorLinear>;


class ReverbProcessor : public Effector {
public:
    ReverbProcessor(int id);
    ~ReverbProcessor();

    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    float getNextSample(int sample) override;
    void syncParams(const ParameterHandler& parameterHandler) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    float getNextSample(float currentSampleValue) override;
	bool isActive();

private:
    //===================================================================================
    static float randomInRange(float low, float high) {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(low, high);
        return dist(rng);
    }

    template<int channels = 8>
    struct MultiChannelMixedFeedback {
        using Array = std::array<float, channels>;
        float delayMs = 150.0f;
        float decayGain = 0.85f;

        std::array<int, channels> delaySamples;
        std::array<std::vector<float>, channels> delayBuffers;
        std::array<int, channels> writeIndices;

        void configure(float sampleRate) {
            float delaySamplesBase = delayMs * 0.001f * sampleRate;
            for (int c = 0; c < channels; ++c) {
                float r = static_cast<float>(c) / channels;
                delaySamples[c] = static_cast<int>(std::pow(2.0f, r) * delaySamplesBase);

                int bufferSize = delaySamples[c] + 1;
                delayBuffers[c].resize(bufferSize, 0.0f);
                writeIndices[c] = 0;
            }
        }

        void clear() {
            for (auto& buffer : delayBuffers) {
                std::fill(buffer.begin(), buffer.end(), 0.0f);
            }
            writeIndices.fill(0);
        }

        Array process(const Array& input) {
            Array delayed;
            Array mixed;

            #pragma omp for simd
            for (int c = 0; c < channels; ++c) { // Leer retrasos y mezclar en un solo bucle
                int bufferSize = static_cast<int>(delayBuffers[c].size());
                int readIndex = writeIndices[c] - delaySamples[c];
                if (readIndex < 0)
                    readIndex += bufferSize;

                delayed[c] = delayBuffers[c][readIndex];
            }

            // Mezcla usando una matriz de Householder
            mixed = delayed;
            signalsmith::mix::Householder<float, channels>::inPlace(mixed.data());

            // Escribir en buffers de delay y preparar salida
            #pragma omp for simd
            for (int c = 0; c < channels; ++c) {
                float sum = input[c] + mixed[c] * decayGain;
                delayBuffers[c][writeIndices[c]] = sum;

                // Incrementar índice de escritura
                writeIndices[c]++;
                if (writeIndices[c] >= delayBuffers[c].size())
                    writeIndices[c] = 0;
            }

            return delayed;
        }

        void processBuffer(juce::AudioBuffer<float>& audioBuffer) {
            const int numSamples = audioBuffer.getNumSamples();

            #pragma omp parallel  // Crear un bloque de paralelización persistente
            for (int sample = 0; sample < numSamples; ++sample) {
                Array input;

                #pragma omp for simd
                for (int c = 0; c < channels; c+=2) {
                    input[c] = audioBuffer.getReadPointer(c)[sample];
                    input[c+1] = audioBuffer.getReadPointer(c+1)[sample];
                }

                Array delayed = process(input);

                #pragma omp for simd
                for (int c = 0; c < channels; c+=2) {
                    audioBuffer.getWritePointer(c)[sample] = delayed[c];
                    audioBuffer.getWritePointer(c+1)[sample] = delayed[c+1];
                }
            }
        }

    };

    template<int channels = 8>
    struct DiffusionStep {
        using Array = std::array<float, channels>;
        float delayMsRange = 50.0f;

        std::array<int, channels> delaySamples;
        std::array<std::vector<float>, channels> delayBuffers;
        std::array<int, channels> writeIndices;
        std::array<bool, channels> flipPolarity;

        void configure(float sampleRate) {
            float delaySamplesRange = delayMsRange * 0.001f * sampleRate;
            for (int c = 0; c < channels; ++c) {
                float rangeLow = delaySamplesRange * c / channels;
                float rangeHigh = delaySamplesRange * (c + 1) / channels;
                delaySamples[c] = static_cast<int>(randomInRange(rangeLow, rangeHigh));

                int bufferSize = delaySamples[c] + 1;
                delayBuffers[c].resize(bufferSize, 0.0f);
                writeIndices[c] = 0;

                flipPolarity[c] = rand() % 2;
            }
        }

        void clear() {
            for (auto& buffer : delayBuffers) {
                std::fill(buffer.begin(), buffer.end(), 0.0f);
            }
            writeIndices.fill(0);
        }

        inline void applyPolarity(Array& mixed) const {
            for (int c = 0; c < channels; c+=2) {
                if (flipPolarity[c]) mixed[c] = -mixed[c];
                if (flipPolarity[c+1]) mixed[c+1] = -mixed[c+1];
            }
        }

        void processBuffer(juce::AudioBuffer<float>& audioBuffer) {
            const int numSamples = audioBuffer.getNumSamples();

            #pragma omp parallel  // Crear un bloque de paralelización persistente
            for (int sample = 0; sample < numSamples; ++sample) {
                Array delayed;

                #pragma omp for // Paralelizar solo dentro de la región paralela
                for (int c = 0; c < channels; ++c) {
                    // Escribe el valor en el buffer de delay
                    delayBuffers[c][writeIndices[c]] = audioBuffer.getReadPointer(c)[sample];

                    // Calcula el índice de lectura
                    int bufferSize = static_cast<int>(delayBuffers[c].size());
                    int readIndex = writeIndices[c] - delaySamples[c];
                    if (readIndex < 0)
                        readIndex += bufferSize;

                    // Lee el valor retrasado
                    delayed[c] = delayBuffers[c][readIndex];

                    // Actualiza el índice de escritura
                    writeIndices[c]++;
                    if (writeIndices[c] >= bufferSize)
                        writeIndices[c] = 0;
                }

                // Mezcla Hadamard
                Array mixed = delayed;
                signalsmith::mix::Hadamard<float, channels>::inPlace(mixed.data());

                // Invierte polaridades si es necesario
                applyPolarity(mixed);

                // Escribe la salida
                for (int c = 0; c < channels; c+=2) {
                    audioBuffer.getWritePointer(c)[sample] = mixed[c];
                    audioBuffer.getWritePointer(c+1)[sample] = mixed[c+1];
                }
            }
        }
    };

    template<int channels = 8, int stepCount = 3>
    struct DiffuserHalfLengths {
        using Array = std::array<float, channels>;

        using Step = DiffusionStep<channels>;
        std::array<Step, stepCount> steps;

        DiffuserHalfLengths(float diffusionMs) {
            setDiffusionValue(diffusionMs);
        }

        void setDiffusionValue(float diffusionMs) {
            float currentDiffusion = diffusionMs;
            for (auto& step : steps) {
                step.delayMsRange = currentDiffusion;
                currentDiffusion *= 0.5f;
            }
        }

        void clear() {
            for (auto& step : steps)
                step.clear();
        }

        void configure(float sampleRate) {
            for (auto& step : steps)
                step.configure(sampleRate);
        }

        void processBuffer(juce::AudioBuffer<float>& audioBuffer) {
            for (auto& step : steps) {
                step.processBuffer(audioBuffer);
            }
        }
    };

    template<int channels = 8, int diffusionSteps = 3>
    struct BasicReverb {
        using Array = std::array<float, channels>;

        MultiChannelMixedFeedback<channels> feedback;
        DiffuserHalfLengths<channels, diffusionSteps> diffuser;
        float dry = 0.0f;
        float wet = 1.0f;

        BasicReverb(float roomSizeMs, float rt60, float dryMix = 0.0f, float wetMix = 1.0f)
            : diffuser(roomSizeMs), dry(dryMix), wet(wetMix) {
            setParameters(roomSizeMs, rt60, wetMix);
        }

        void configure(float sampleRate) {
            feedback.configure(sampleRate);
            diffuser.configure(sampleRate);
        }

        void setParameters(float delay, float decay, float mix) {
            dry = 1.0f - mix;
            wet = mix;

            feedback.delayMs = delay;

            // Cálculo de ganancia de decaimiento
            float typicalLoopMs = delay * 1.5f;
            float loopsPerRt60 = decay / (typicalLoopMs * 0.001f);
            float dbPerCycle = -60.0f / loopsPerRt60;

            feedback.decayGain = std::pow(10.0f, dbPerCycle * 0.05f);
        }

        void clear() {
            feedback.clear();
            diffuser.clear();
        }

        void processBuffer(juce::AudioBuffer<float>& audioBuffer) {
            const int numSamples = audioBuffer.getNumSamples();
            juce::AudioBuffer<float> inputBuffer;
            inputBuffer.makeCopyOf(audioBuffer);

            // Procesar difusión
            diffuser.processBuffer(audioBuffer);

            // Procesar feedback
            feedback.processBuffer(audioBuffer);

            // Mezcla Dry/Wet

            #pragma omp parallel for
            for (int c = 0; c < channels; ++c) {
                float* writePtr = audioBuffer.getWritePointer(c);
                const float* readPtr = inputBuffer.getReadPointer(c);

                for (int sample = 0; sample < numSamples; ++sample) {
                    writePtr[sample] = dry * readPtr[sample] + wet * writePtr[sample];
                }
            }
        }
    };
    //===================================================================================

	BasicReverb<6, 3> reverb;

    void splitChannels(juce::AudioBuffer<float>& inputBuffer, int numberOfOutputChannels);
    void mixChannels(juce::AudioBuffer<float>& inputBuffer, int numberOfOutputChannels);

	bool noteIsOff;
	bool isReverbActive;
    float sampleRate;
	float averageOutputValue;

	float maxDecayValue;
	float maxDelayValue;
	float maxMixValue;

    std::shared_ptr<SliderParameter> mixParameter;
    juce::Array<float> mixModulationBuffer;
    float mixValue;

    std::shared_ptr<SliderParameter> delayParameter;
    juce::Array<float> delayModulationBuffer;
    float delayValue;

    std::shared_ptr<SliderParameter> decayParameter;
    juce::Array<float> decayModulationBuffer;
    float decayValue;
};