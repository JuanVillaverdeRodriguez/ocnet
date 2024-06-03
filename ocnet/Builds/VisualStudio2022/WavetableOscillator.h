#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include <include_juce_audio_processors.cpp>


class WaveTableOscillator{

public:
	WaveTableOscillator();

	template <typename ProcessContext>
	void process(const ProcessContext& context);

private:

};

