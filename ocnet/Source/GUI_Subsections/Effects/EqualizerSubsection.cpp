/*
  ==============================================================================

    EqualizerSubsection.cpp
    Created: 31 Aug 2024 12:11:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "EqualizerSubsection.h"

EqualizerSubsection::EqualizerSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler, id, "Equalizer")
{
    gainParameterID = createParameterID("gain");
    cutOffParameterID = createParameterID("cutOff");
    resonanceParameterID = createParameterID("resonance");
    lowFreqParameterID = createParameterID("lowFreq");
    midFreqParameterID = createParameterID("midFreq");
    highFreqParameterID = createParameterID("highFreq");

    gainKnob = std::make_unique<Knob1>(gainParameterID, eventHandler, "Gain");
    cutOffKnob = std::make_unique<Knob1>(cutOffParameterID, eventHandler, "CutOff");
    resonanceKnob = std::make_unique<Knob1>(resonanceParameterID, eventHandler, "Resonance");

    lowFreqButton.setButtonText("LOW");
    midFreqButton.setButtonText("MID");
    highFreqButton.setButtonText("HIGH");

    lowFreqButton.setLookAndFeel(&lookAndFeelEqButtons);
    midFreqButton.setLookAndFeel(&lookAndFeelEqButtons);
    highFreqButton.setLookAndFeel(&lookAndFeelEqButtons);

    lowFreqButton.setToggleState(true, juce::dontSendNotification);

    lowFreqButton.setName(lowFreqParameterID);
    midFreqButton.setName(lowFreqParameterID);
    highFreqButton.setName(lowFreqParameterID);

    gainKnob->setRange(0.0f, 1.0f, 0.01f);
    cutOffKnob->setRange(0.0f, 1.0f, 0.01f);
    resonanceKnob->setRange(0.0f, 1.0f, 0.01f);

    lowFreqButton.setToggleable(true);
    midFreqButton.setToggleable(true);
    highFreqButton.setToggleable(true);

    lowFreqButton.addListener(this);
    midFreqButton.addListener(this);
    highFreqButton.addListener(this);

    addAndMakeVisible(*gainKnob);
    addAndMakeVisible(*cutOffKnob);
    addAndMakeVisible(*resonanceKnob);
    addAndMakeVisible(lowFreqButton);
    addAndMakeVisible(midFreqButton);
    addAndMakeVisible(highFreqButton);

}

void EqualizerSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    gainParameterAttachment = std::make_unique<OcnetSliderAttachment>(*gainKnob, *parameterHandler.getSliderParameter(gainParameterID)->get());
    cutOffParameterAttachment = std::make_unique<OcnetSliderAttachment>(*cutOffKnob, *parameterHandler.getSliderParameter(cutOffParameterID)->get());
    resonanceParameterAttachment = std::make_unique<OcnetSliderAttachment>(*resonanceKnob, *parameterHandler.getSliderParameter(resonanceParameterID)->get());

    lowFreqButtonAttachment = std::make_unique<ButtonParameterAttachment>(lowFreqButton, *parameterHandler.getButtonParameter(lowFreqParameterID)->get());
    midFreqButtonAttachment = std::make_unique<ButtonParameterAttachment>(midFreqButton, *parameterHandler.getButtonParameter(midFreqParameterID)->get());
    highFreqButtonAttachment = std::make_unique<ButtonParameterAttachment>(highFreqButton, *parameterHandler.getButtonParameter(highFreqParameterID)->get());
}

void EqualizerSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(gainParameterID, std::make_shared<SliderParameter>("gain", 0.5f));
    parameterHandler.addSliderParameter(cutOffParameterID, std::make_shared<SliderParameter>("cutOff", 0.5f));
    parameterHandler.addSliderParameter(resonanceParameterID, std::make_shared<SliderParameter>("resonance", 0.5f));

    parameterHandler.addButtonParameter(lowFreqParameterID, std::make_shared<ButtonParameter>("lowFreq", false));
    parameterHandler.addButtonParameter(midFreqParameterID, std::make_shared<ButtonParameter>("midFreq", false));
    parameterHandler.addButtonParameter(highFreqParameterID, std::make_shared<ButtonParameter>("highFreq", false));
}

void EqualizerSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 0;
    gainKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    cutOffKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    resonanceKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    lowFreqButton.setBounds(posX, 20, 60, 40);
    posX += 60;

    midFreqButton.setBounds(posX, 20, 60, 40);
    posX += 60;

    highFreqButton.setBounds(posX, 20, 60, 40);
    posX += 60;

    gainKnob->showLabel(*this, *gainKnob);
    cutOffKnob->showLabel(*this, *cutOffKnob);
    resonanceKnob->showLabel(*this, *resonanceKnob);
}

void EqualizerSubsection::buttonClickedCalled(juce::Button* clickedButton)
{
    if (clickedButton == &lowFreqButton) {
        lowFreqButton.setToggleState(true, juce::dontSendNotification);
        midFreqButton.setToggleState(false, juce::dontSendNotification);
        highFreqButton.setToggleState(false, juce::dontSendNotification);
    }

    if (clickedButton == &midFreqButton) {
        lowFreqButton.setToggleState(false, juce::dontSendNotification);
        midFreqButton.setToggleState(true, juce::dontSendNotification);
        highFreqButton.setToggleState(false, juce::dontSendNotification);
    }

    if (clickedButton == &highFreqButton) {
        lowFreqButton.setToggleState(false, juce::dontSendNotification);
        midFreqButton.setToggleState(false, juce::dontSendNotification);
        highFreqButton.setToggleState(true, juce::dontSendNotification);
    }
}
