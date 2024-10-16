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
#include <immintrin.h> // Necesario para las instrucciones SIMD

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
		float unitRand = rand() / float(RAND_MAX);
		return low + unitRand * (high - low);
	}

	template<int channels = 8>
	struct MultiChannelMixedFeedback {
		using Array = std::array<float, channels>;
		float delayMs = 150;
		float decayGain = 0.85;

		std::array<int, channels> delaySamples;
		std::array<DelaySignalSmith, channels> delays;

		void configure(float sampleRate) {
			float delaySamplesBase = delayMs * 0.001 * sampleRate;
			for (int c = 0; c < channels; ++c) {
				float r = c * 1.0 / channels;
				delaySamples[c] = std::pow(2, r) * delaySamplesBase;
				delays[c].resize(delaySamples[c] + 1);
				delays[c].reset();
			}
		}

		void clear() {
			delaySamples.fill(0.0f);
		}

		Array process(Array input) {
			Array delayed;
			for (int c = 0; c < channels; ++c) {
				delayed[c] = delays[c].read(delaySamples[c]);
			}

			// Mix using a Householder matrix
			Array mixed = delayed;
			signalsmith::mix::Householder<float, channels>::inPlace(mixed.data());

			for (int c = 0; c < channels; ++c) {
				float sum = input[c] + mixed[c] * decayGain;
				delays[c].write(sum);
			}

			return delayed;
		}

		void process(juce::AudioBuffer<float>& audioBuffer, int sample) {
			Array delayed;
			for (int c = 0; c < channels; ++c) {
				delayed[c] = delays[c].read(delaySamples[c]);
			}

			// Mix using a Householder matrix
			Array mixed = delayed;
			signalsmith::mix::Householder<float, channels>::inPlace(mixed.data());

			for (int c = 0; c < channels; ++c) {
				float sum = audioBuffer.getReadPointer(c)[sample] + mixed[c] * decayGain;
				delays[c].write(sum);
			}

			for (int c = 0; c < channels; ++c) {
				delayed[c] = delays[c].read(delaySamples[c]);
				audioBuffer.getWritePointer(c)[sample] = delayed[c];
			}

		}
	};




	template<int channels = 8>
	struct DiffusionStep {
		using Array = std::array<float, channels>;
		float delayMsRange = 50;

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
			for (int c = 0; c < channels; ++c) {
				if (flipPolarity[c]) mixed[c] *= -1;
			}
		}

		void process(juce::AudioBuffer<float>& audioBuffer, int sample) {
			Array delayed;

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
				//writeIndices[c] = (writeIndices[c] + 1) % bufferSize;
				writeIndices[c]++;
				if (writeIndices[c] >= bufferSize)
					writeIndices[c] = 0;
			}

			// Mezcla Hadamard
			Array mixed = delayed;
			signalsmith::mix::Hadamard<float, channels>::inPlace(mixed.data());

			// Invierte polaridades si es necesario
			applyPolarity(mixed);

			for (int c = 0; c < channels; ++c) {
				audioBuffer.getWritePointer(c)[sample] = mixed[c];
			}
		}
	};
	template<int channels = 8, int stepCount = 4>
	struct DiffuserHalfLengths {
		using Array = std::array<float, channels>;

		using Step = DiffusionStep<channels>;
		std::array<Step, stepCount> steps;

		DiffuserHalfLengths(float diffusionMs) {
			for (auto& step : steps) {
				diffusionMs *= 0.5;
				step.delayMsRange = diffusionMs;
			}
		}

		void setDiffusionValue(float diffusionMs) {
			for (auto& step : steps) {
				diffusionMs *= 0.5;
				step.delayMsRange = diffusionMs;
			}
		}

		void clear() {
			for (auto& step : steps) step.clear();

		}

		void configure(float sampleRate) {
			for (auto& step : steps) step.configure(sampleRate);
		}

		Array process(Array samples) {
			for (auto& step : steps) {
				samples = step.process(samples);
			}
			return samples;
		}

		void process(juce::AudioBuffer<float>& audioBuffer, int sample) {
			for (auto& step : steps) {
				step.process(audioBuffer, sample);
			}

		}
	};

	template<int channels = 8, int diffusionSteps = 4>
	struct BasicReverb {
		using Array = std::array<float, channels>;

		MultiChannelMixedFeedback<channels> feedback;
		DiffuserHalfLengths<channels, diffusionSteps> diffuser;
		float dry, wet;

		BasicReverb(float roomSizeMs, float rt60, float dry = 0, float wet = 1) : diffuser(roomSizeMs), dry(dry), wet(wet) {
			feedback.delayMs = roomSizeMs;

			// How long does our signal take to go around the feedback loop?
			float typicalLoopMs = roomSizeMs * 1.5;
			// How many times will it do that during our RT60 period?
			float loopsPerRt60 = rt60 / (typicalLoopMs * 0.001);
			// This tells us how many dB to reduce per loop
			float dbPerCycle = -60 / loopsPerRt60;

			feedback.decayGain = std::pow(10, dbPerCycle * 0.05);

		}

		void configure(float sampleRate) {
			feedback.configure(sampleRate);
			diffuser.configure(sampleRate);
		}

		void setParameters(float delay, float decay, float mix) {
			dry = 1 - mix;
			wet = mix;

			//diffuser.setDiffusionValue(delay);
			feedback.delayMs = delay;

			// How long does our signal take to go around the feedback loop?
			float typicalLoopMs = delay * 1.5;
			// How many times will it do that during our RT60 period?
			float loopsPerRt60 = decay / (typicalLoopMs * 0.001);
			// This tells us how many dB to reduce per loop
			float dbPerCycle = -60 / loopsPerRt60;

			feedback.decayGain = std::pow(10, dbPerCycle * 0.05);
		}

		// Cuidado: Despues de llamar a este metodo, llamar a configure() de nuevo
		// Llena de 0s todos los buffers
		void clear() {
			feedback.clear();
			diffuser.clear();
		}

		Array process(Array input, int numSamples) {
			Array diffuse = diffuser.process(input);
			Array longLasting = feedback.process(diffuse);
			Array output;
			for (int c = 0; c < channels; ++c) {
				output[c] = dry * input[c] + wet * longLasting[c];
			}
			return output;
		}

		void process(juce::AudioBuffer<float>& audioBuffer) {
			juce::AudioBuffer<float> inputAudioBuffer;
			inputAudioBuffer.makeCopyOf(audioBuffer);

			const int numChannels = audioBuffer.getNumChannels();
			const int numSamples = audioBuffer.getNumSamples();

			for (int sample = 0; sample < numSamples; sample++) {
				diffuser.process(audioBuffer, sample);
				feedback.process(audioBuffer, sample);
				Array output;
				for (int c = 0; c < channels; ++c) {
					audioBuffer.getWritePointer(c)[sample] = dry * inputAudioBuffer.getReadPointer(c)[sample] + wet * audioBuffer.getReadPointer(c)[sample];
				}
			}

		}
	};
    //===================================================================================

	BasicReverb<8, 4> reverb;

    void splitChannels(juce::AudioBuffer<float>& inputBuffer, int numberOfOutputChannels);
    void mixChannels(juce::AudioBuffer<float>& inputBuffer, int numberOfOutputChannels);
    void diffuseStep(juce::AudioBuffer<float>& inputBuffer);
    void feedbackStep(juce::AudioBuffer<float>& inputBuffer);

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