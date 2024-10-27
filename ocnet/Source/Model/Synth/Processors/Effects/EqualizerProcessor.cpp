/*
  ==============================================================================

    EqualizerProcessor.cpp
    Created: 31 Aug 2024 7:24:45pm
    Author:  TecNo

  ==============================================================================
*/

#include "EqualizerProcessor.h"

EqualizerProcessor::EqualizerProcessor(int id) : gainLValue(0.0f), cutOffLValue(0.0f), resonanceLValue(0.0f), sampleRate(44100.0f), selectedLowCutSlope(0), selectedHighCutSlope(0)
{
    setId(id);
}

EqualizerProcessor::~EqualizerProcessor()
{
}

void EqualizerProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;

    juce::dsp::ProcessSpec monoSpec{};

    monoSpec.sampleRate = spec.sampleRate;
    monoSpec.numChannels = spec.numChannels;
    monoSpec.maximumBlockSize = spec.maximumBlockSize;

    leftChain.prepare(monoSpec);
    rightChain.prepare(monoSpec);

    gainLValue.reset(sampleRate, 0.0005);
    gainMValue.reset(sampleRate, 0.0005);
    gainHValue.reset(sampleRate, 0.0005);

    cutOffLValue.reset(sampleRate, 0.0005);
    cutOffMValue.reset(sampleRate, 0.0005);
    cutOffHValue.reset(sampleRate, 0.0005);

    resonanceLValue.reset(sampleRate, 0.0005);
    resonanceMValue.reset(sampleRate, 0.0005);
    resonanceHValue.reset(sampleRate, 0.0005);
}

void EqualizerProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void EqualizerProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void EqualizerProcessor::updateParameterValues()
{
    gainLValue.setTargetValue(gainLParameter->getModulatedValue(8, -24.0f, 24.0f));
    cutOffLValue.setTargetValue(cutOffLParameter->getModulatedValue(8, 20.0f, 20000.0f));
    resonanceLValue.setTargetValue(resonanceLParameter->getModulatedValue(8, 0.0f, 10.0f));

    gainMValue.setTargetValue(gainLParameter->getModulatedValue(8, -24.0f, 24.0f));
    cutOffMValue.setTargetValue(cutOffMParameter->getModulatedValue(8, 20.0f, 20000.0f));
    resonanceMValue.setTargetValue(resonanceMParameter->getModulatedValue(8, 0.0f, 10.0f));

    gainHValue.setTargetValue(gainLParameter->getModulatedValue(8, -24.0f, 24.0f));
    cutOffHValue.setTargetValue(cutOffHParameter->getModulatedValue(8, 20.0f, 20000.0f));
    resonanceHValue.setTargetValue(resonanceHParameter->getModulatedValue(8, 0.0f, 10.0f));

    selectedLowCutSlope = selectedLowCutSlopeParameter->getCurrentIndex();
    selectedHighCutSlope = selectedHighCutSlopeParameter->getCurrentIndex();
}

float EqualizerProcessor::getNextSample(int sample)
{
    return 0.0f;
}

void EqualizerProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    gainLParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_gainL"));
    gainMParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_gainM"));
    gainHParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_gainH"));

    cutOffLParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_cutOffL"));
    cutOffMParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_cutOffM"));
    cutOffHParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_cutOffH"));

    resonanceLParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_resonanceL"));
    resonanceMParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_resonanceM"));
    resonanceHParameter = parameterHandler.syncWithSliderParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_resonanceH"));

    lowFreqParameter = parameterHandler.syncWithButtonParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_lowFreq"));
    midFreqParameter = parameterHandler.syncWithButtonParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_midFreq"));
    highFreqParameter = parameterHandler.syncWithButtonParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_highFreq"));

    selectedLowCutSlopeParameter = parameterHandler.syncWithComboBoxParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_lowCutSlope"));
    selectedHighCutSlopeParameter = parameterHandler.syncWithComboBoxParam(juce::String("Equalizer_") + juce::String(getId()) + juce::String("_highCutSlope"));
}

void EqualizerProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    updateLowCutCoeffs();
    updatePeekCoeffs();
    updateHighCutCoeffs();

    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);

}

float EqualizerProcessor::getNextSample(float currentSampleValue)
{
    return 0.0f;
}

bool EqualizerProcessor::isActive()
{
    return false;
}

void EqualizerProcessor::updateLowCutCoeffs()
{
    auto cutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(cutOffLValue.getTargetValue(), sampleRate, 2 * (selectedLowCutSlope + 1));

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

    applySlope(leftLowCut, selectedLowCutSlope, cutCoefficients);
    applySlope(rightLowCut, selectedLowCutSlope, cutCoefficients);
}

void EqualizerProcessor::updatePeekCoeffs()
{
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, cutOffMValue.getTargetValue(), resonanceMValue.getTargetValue(), juce::Decibels::decibelsToGain(gainMValue.getTargetValue()));

    *leftChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
}

void EqualizerProcessor::updateHighCutCoeffs()
{
    auto cutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(cutOffHValue.getTargetValue(), sampleRate, 2 * (selectedHighCutSlope + 1));

    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();

    applySlope(leftHighCut, selectedHighCutSlope, cutCoefficients);
    applySlope(rightHighCut, selectedHighCutSlope, cutCoefficients);
}

void EqualizerProcessor::applySlope(EqualizerProcessor::CutFilter& cutFilter, int slope, juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>> coeffs)
{
    cutFilter.setBypassed<0>(true);
    cutFilter.setBypassed<1>(true);
    cutFilter.setBypassed<2>(true);
    cutFilter.setBypassed<3>(true);

    switch (slope)
    {
    case Slope_12:
        *cutFilter.get<Slope_12>().coefficients = *coeffs[Slope_12];
        cutFilter.setBypassed<Slope_12>(false);
        break;

    case Slope_24:
        *cutFilter.get<Slope_12>().coefficients = *coeffs[Slope_12];
        cutFilter.setBypassed<Slope_12>(false);
        *cutFilter.get<Slope_24>().coefficients = *coeffs[Slope_24];
        cutFilter.setBypassed<Slope_24>(false);
        break;

    case Slope_36:
        *cutFilter.get<Slope_12>().coefficients = *coeffs[Slope_12];
        cutFilter.setBypassed<Slope_12>(false);
        *cutFilter.get<Slope_24>().coefficients = *coeffs[Slope_24];
        cutFilter.setBypassed<Slope_24>(false);
        *cutFilter.get<Slope_36>().coefficients = *coeffs[Slope_36];
        cutFilter.setBypassed<Slope_36>(false);
        break;

    case Slope_48:
        *cutFilter.get<Slope_12>().coefficients = *coeffs[Slope_12];
        cutFilter.setBypassed<Slope_12>(false);
        *cutFilter.get<Slope_24>().coefficients = *coeffs[Slope_24];
        cutFilter.setBypassed<Slope_24>(false);
        *cutFilter.get<Slope_36>().coefficients = *coeffs[Slope_36];
        cutFilter.setBypassed<Slope_36>(false);
        *cutFilter.get<Slope_48>().coefficients = *coeffs[Slope_48];
        cutFilter.setBypassed<Slope_48>(false);
        break;

    default:
        break;
    }
}


