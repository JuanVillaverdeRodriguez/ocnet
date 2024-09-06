/*
  ==============================================================================

    MacroSubsection.cpp
    Created: 31 Aug 2024 12:12:02pm
    Author:  TecNo

  ==============================================================================
*/

#include "MacroSubsection.h"

MacroSubsection::MacroSubsection(int id, int subMenuID, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler, id, "Macro"), selectedMacroID(subMenuID)
{
    macroParameterID = createParameterID("macro");
    selectedMacroParameterID = createParameterID("macroID");

    macroKnob = std::make_unique<Knob1>(macroParameterID, eventHandler, "Value");

    macroKnob->setRange(0.0f, 1.0f, 0.01f);

    addAndMakeVisible(*macroKnob);

}

void MacroSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    selectedMacroID = parameterHandler.getParameterValue(selectedMacroParameterID);
    macroParameterAttachment = std::make_unique<SliderAttachment>(parameterHandler.apvts, getSubType() + "_" + juce::String(selectedMacroID) + "_macro", *macroKnob.get());
}

void MacroSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addParameter(selectedMacroParameterID, selectedMacroID);
    parameterHandler.addSliderParameter(macroParameterID, std::make_shared<SliderParameter>("macro", 0.0f));
}

void MacroSubsection::subsectionResized()
{
    auto area = getLocalBounds();


    int posX = 35;
    macroKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    macroKnob->showLabel(*this, *macroKnob);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }


}

int MacroSubsection::getMacroID()
{
    return selectedMacroID;
}
