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

    freqCutKnob = std::make_unique<Knob1>(freqParameterID, eventHandler, "Cutoff");
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
    highPassButton.setButtonText("High pass");
    setDesiredHeight(102);
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
    auto content = getContentArea();
    const int x0 = content.getX();
    const int y0 = content.getY();

    placeKnob(*freqCutKnob, x0, y0);
    placeKnob(*resonanceKnob, x0 + knobWidth, y0);

    highPassButton.setBounds(x0 + knobWidth * 2 + 14, y0 + (knobHeight - 26) / 2, 96, 26);

    placeKnob(*mixKnob, highPassButton.getRight() + 14, y0);
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
