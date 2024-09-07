/*
  ==============================================================================

    RandomizerSubsection.cpp
    Created: 31 Aug 2024 12:12:27pm
    Author:  TecNo

  ==============================================================================
*/

#include "RandomizerSubsection.h"

RandomizerSubsection::RandomizerSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler, id, "Randomizer")
{
    speedParameterID = createParameterID("freq");
    modeParameterID = createParameterID("mode");

    speedKnob = std::make_unique<Knob1>(speedParameterID, eventHandler, "Frequency");

    speedKnob->setRange(0.0f, 1.0f, 0.01f);

    //speedKnob->setValue(0.5f);

    this->addAndMakeVisible(*speedKnob);

    // Configuración del ComboBox
    modeComboBox.addItem("Perlin", 1);
    modeComboBox.addItem("Sample & Hold", 2);

    //fmFromComboBox.setSelectedId(1);
    modeComboBox.setName(modeParameterID);

    addAndMakeVisible(modeComboBox);

}

void RandomizerSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    speedParameterAttachment = std::make_unique<OcnetSliderAttachment>(*speedKnob, *parameterHandler.getSliderParameter(speedParameterID)->get());
    modeComboBoxAttachment = std::make_unique<OcnetComboBoxAttachment>(modeComboBox, *parameterHandler.getComboBoxParameter(modeParameterID)->get());

}

void RandomizerSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(speedParameterID, std::make_shared<SliderParameter>("freq", 0.5f));
    parameterHandler.addComboBoxParameter(modeParameterID, std::make_shared<ComboBoxParameter>("mode", juce::StringArray{ "Perlin", "Sample & Hold" }, 0));

}

void RandomizerSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 35;
    speedKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    speedKnob->showLabel(*this, *speedKnob);

    modeComboBox.setBounds(posX, 20, defaultKnobSize * 2, defaultKnobSize - 10);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }
}