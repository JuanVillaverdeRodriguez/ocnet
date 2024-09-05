/*
  ==============================================================================

    FilterSubsection.cpp
    Created: 5 Aug 2024 9:45:32pm
    Author:  TecNo

  ==============================================================================
*/

#include "FilterSubsection.h"

FilterSubsection::FilterSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler, id, "Filter")
{
    freqParameterID = createParameterID("Filter", getId(), "freqCut");
    resonanceParameterID = createParameterID("Filter", getId(), "resonance");
    highPassParameterID = createParameterID("Filter", getId(), "highPass");

    freqCutKnob = std::make_unique<Knob1>(freqParameterID, eventHandler, "Frequency");
    resonanceKnob = std::make_unique<Knob1>(resonanceParameterID, eventHandler, "Resonance");


    this->addAndMakeVisible(*freqCutKnob);
    this->addAndMakeVisible(*resonanceKnob);
    this->addAndMakeVisible(highPassButton);


    freqCutKnob->setRange(20.0f, 20000.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    resonanceKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    highPassButton.setToggleable(true);
    highPassButton.setLookAndFeel(&lookAndFeelButtons);
    highPassButton.addListener(this);
    highPassButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    highPassButton.setButtonText("HighPass");
    //freqCutKnob->setValue(20000.0f);

    freqCutKnob->setSkewFactor(0.2f);

    mixParameterID = createParameterID("mix");
    mixKnob = std::make_unique<Knob1>(mixParameterID, eventHandler, "Mix");

    this->addAndMakeVisible(*mixKnob);

    mixKnob->setRange(0.0f, 1.0f, 0.01f);
    //mixKnob->setValue(1.0f);

}

void FilterSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 0;
    freqCutKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    resonanceKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    highPassButton.setBounds(posX, 20, 50, defaultKnobSize);
    highPassButton.changeWidthToFitText();
    posX += highPassButton.getWidth();


    mixKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    freqCutKnob->showLabel(*this, *freqCutKnob);
    resonanceKnob->showLabel(*this, *resonanceKnob);
    mixKnob->showLabel(*this, *mixKnob);
}

void FilterSubsection::buttonClickedCalled(juce::Button* clickedButton)
{
    if (clickedButton == &highPassButton) {
        highPassButton.setToggleState(!highPassButton.getToggleState(), juce::NotificationType::dontSendNotification);
    }
}


void FilterSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    freqCutParameterAttachment = std::make_unique<OcnetSliderAttachment>(*freqCutKnob, *parameterHandler.getSliderParameter(freqParameterID)->get());
    resonanceParameterAttachment = std::make_unique<OcnetSliderAttachment>(*resonanceKnob, *parameterHandler.getSliderParameter(resonanceParameterID)->get());
    highPassButtonAttachment = std::make_unique<ButtonParameterAttachment>(highPassButton, *parameterHandler.getButtonParameter(highPassParameterID)->get());
}

void FilterSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(freqParameterID, std::make_shared<SliderParameter>("freqCut", 20000.0f));
    parameterHandler.addSliderParameter(resonanceParameterID, std::make_shared<SliderParameter>("resonance", 0.5f));
    parameterHandler.addButtonParameter(highPassParameterID, std::make_shared<ButtonParameter>("highPass", false));
}
