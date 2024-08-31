/*
  ==============================================================================

    ReverbProcessor.h
    Created: 27 Aug 2024 8:00:06pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Effector.h"
#include "../Source/SignalSmithLibrary/dsp/delay.h"
#include "../Source/SignalSmithLibrary/dsp/mix.h"
#include "../../../SignalSmithLibrary/dsp/delay.h"

using Delay = signalsmith::delay::Delay<double, signalsmith::delay::InterpolatorNearest>;


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
	static double randomInRange(double low, double high) {
		double unitRand = rand() / double(RAND_MAX);
		return low + unitRand * (high - low);
	}

	struct SingleChannelFeedback {
		double delayMs = 80;
		double decayGain = 0.85;

		int delaySamples;
		Delay delay;

		void configure(double sampleRate) {
			delaySamples = delayMs * 0.001 * sampleRate;
			delay.resize(delaySamples + 1);
			delay.reset(); // Start with all 0s
		}

		double process(double input) {
			double delayed = delay.read(delaySamples);

			double sum = input + delayed * decayGain;
			delay.write(sum);

			return delayed;
		}
	};


	template<int channels = 8>
	struct MultiChannelFeedback {
		using Array = std::array<double, channels>;

		double delayMs = 150;
		double decayGain = 0.85;

		std::array<int, channels> delaySamples;
		std::array<Delay, channels> delays;

		void configure(double sampleRate) {
			double delaySamplesBase = delayMs * 0.001 * sampleRate;
			for (int c = 0; c < channels; ++c) {
				// Distribute delay times exponentially between delayMs and 2*delayMs
				double r = c * 1.0 / channels;
				delaySamples[c] = std::pow(2, r) * delaySamplesBase;

				delays[c].resize(delaySamples[c] + 1);
				delays[c].reset();
			}
		}

		Array process(Array input) {
			Array delayed;
			for (int c = 0; c < channels; ++c) {
				delayed[c] = delays[c].read(delaySamples[c]);
			}

			for (int c = 0; c < channels; ++c) {
				double sum = input[c] + delayed[c] * decayGain;
				delays[c].write(sum);
			}

			return delayed;
		}
	};

	template<int channels = 8>
	struct MultiChannelMixedFeedback {
		using Array = std::array<double, channels>;
		double delayMs = 150;
		double decayGain = 0.85;

		std::array<int, channels> delaySamples;
		std::array<Delay, channels> delays;

		void configure(double sampleRate) {
			double delaySamplesBase = delayMs * 0.001 * sampleRate;
			for (int c = 0; c < channels; ++c) {
				double r = c * 1.0 / channels;
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
			signalsmith::mix::Householder<double, channels>::inPlace(mixed.data());

			for (int c = 0; c < channels; ++c) {
				double sum = input[c] + mixed[c] * decayGain;
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
			signalsmith::mix::Householder<double, channels>::inPlace(mixed.data());

			for (int c = 0; c < channels; ++c) {
				double sum = audioBuffer.getReadPointer(c)[sample] + mixed[c] * decayGain;
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
		std::array<Delay, channels> delays;
		std::array<bool, channels> flipPolarity;

		void configure(float sampleRate) {
			float delaySamplesRange = delayMsRange * 0.001 * sampleRate;
			for (int c = 0; c < channels; ++c) {
				float rangeLow = delaySamplesRange * c / channels;
				float rangeHigh = delaySamplesRange * (c + 1) / channels;
				delaySamples[c] = randomInRange(rangeLow, rangeHigh);
				delays[c].resize(delaySamples[c] + 1);
				delays[c].reset();
				flipPolarity[c] = rand() % 2;
			}
		}


		void clear() {
			delaySamples.fill(0.0f);
		}

		/*Array process(Array input) {
			// Delay
			Array delayed;
			for (int c = 0; c < channels; ++c) {
				delays[c].write(input[c]);
				delayed[c] = delays[c].read(delaySamples[c]);
			}

			// Mix with a Hadamard matrix
			Array mixed = delayed;
			signalsmith::mix::Hadamard<double, channels>::inPlace(mixed.data());

			// Flip some polarities
			for (int c = 0; c < channels; ++c) {
				if (flipPolarity[c]) mixed[c] *= -1;
			}

			return mixed;
		}*/

		inline void applyPolarity(Array& mixed) const {
			for (int c = 0; c < channels; ++c) {
				if (flipPolarity[c]) mixed[c] *= -1;
			}
		}

		void process(juce::AudioBuffer<float>& audioBuffer, int sample) {
			Array delayed;
			float* writePointers[channels];
			const float* readPointers[channels];

			for (int c = 0; c < channels; ++c) {
				readPointers[c] = audioBuffer.getReadPointer(c);
				writePointers[c] = audioBuffer.getWritePointer(c);
				delays[c].write(readPointers[c][sample]);  // Escribe el valor en el delay
				delayed[c] = delays[c].read(delaySamples[c]);  // Luego lee el valor
			}

			// Hadamard matrix mixing
			Array mixed = delayed;
			signalsmith::mix::Hadamard<float, channels>::inPlace(mixed.data());

			// Flip polarities if necessary
			applyPolarity(mixed);

			for (int c = 0; c < channels; ++c) {
				writePointers[c][sample] = mixed[c];
			}
		}
	};

	template<int channels = 8, int stepCount = 4>
	struct DiffuserEqualLengths {
		using Array = std::array<double, channels>;

		using Step = DiffusionStep<channels>;
		std::array<Step, stepCount> steps;

		DiffuserEqualLengths(double totalDiffusionMs) {
			for (auto& step : steps) {
				step.delayMsRange = totalDiffusionMs / stepCount;
			}
		}

		void configure(double sampleRate) {
			for (auto& step : steps) step.configure(sampleRate);
		}

		Array process(Array samples) {
			for (auto& step : steps) {
				samples = step.process(samples);
			}
			return samples;
		}
	};

	template<int channels = 8, int stepCount = 4>
	struct DiffuserHalfLengths {
		using Array = std::array<double, channels>;

		using Step = DiffusionStep<channels>;
		std::array<Step, stepCount> steps;

		DiffuserHalfLengths(double diffusionMs) {
			for (auto& step : steps) {
				diffusionMs *= 0.5;
				step.delayMsRange = diffusionMs;
			}
		}

		void setDiffusionValue(double diffusionMs) {
			for (auto& step : steps) {
				diffusionMs *= 0.5;
				step.delayMsRange = diffusionMs;
			}
		}

		void clear() {
			for (auto& step : steps) step.clear();

		}

		void configure(double sampleRate) {
			for (auto& step : steps) step.configure(sampleRate);
		}

		Array process(Array samples) {
			for (auto& step : steps) {
				samples = step.process(samples);
			}
			return samples;
		}

		void process(juce::AudioBuffer<float>& audioBuffer, int sample) {
			/*const int numChannels = buffer.getNumChannels();
			const int numSamples = buffer.getNumSamples();

			for (int channel = 0; channel < numChannels: channel++) {
				auto* data = upSampledBlock.getChannelPointer(channel);

				for (int sample = 0; sample < numSamples; sample++) {
					for (auto& step : steps) {
						samples = step.process(samples);
					}
				}
			}

			return samples;*/

			for (auto& step : steps) {
				step.process(audioBuffer, sample);
			}
			/*const int numChannels = audioBuffer.getNumChannels();
			for (int channel = 0; channel < numChannels; channel++) {

			}*/
		}
	};

	template<int channels = 8, int diffusionSteps = 4>
	struct BasicReverb {
		using Array = std::array<double, channels>;

		MultiChannelMixedFeedback<channels> feedback;
		DiffuserHalfLengths<channels, diffusionSteps> diffuser;
		double dry, wet;

		BasicReverb(double roomSizeMs, double rt60, double dry = 0, double wet = 1) : diffuser(roomSizeMs), dry(dry), wet(wet) {
			feedback.delayMs = roomSizeMs;

			// How long does our signal take to go around the feedback loop?
			double typicalLoopMs = roomSizeMs * 1.5;
			// How many times will it do that during our RT60 period?
			double loopsPerRt60 = rt60 / (typicalLoopMs * 0.001);
			// This tells us how many dB to reduce per loop
			double dbPerCycle = -60 / loopsPerRt60;

			feedback.decayGain = std::pow(10, dbPerCycle * 0.05);

		}

		void configure(double sampleRate) {
			feedback.configure(sampleRate);
			diffuser.configure(sampleRate);
		}

		void setParameters(float delay, float decay, float mix) {
			dry = 1 - mix;
			wet = mix;

			//diffuser.setDiffusionValue(delay);
			feedback.delayMs = delay;

			// How long does our signal take to go around the feedback loop?
			double typicalLoopMs = delay * 1.5;
			// How many times will it do that during our RT60 period?
			double loopsPerRt60 = decay / (typicalLoopMs * 0.001);
			// This tells us how many dB to reduce per loop
			double dbPerCycle = -60 / loopsPerRt60;

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