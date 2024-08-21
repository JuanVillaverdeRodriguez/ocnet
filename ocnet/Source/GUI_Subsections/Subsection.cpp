/*
  ==============================================================================

    Subsection.cpp
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "Subsection.h"

Subsection::Subsection(GUI_EventHandler& eventHandler, int id, const juce::String& type, const juce::String& subType) 
    : eventHandler(eventHandler), defaultKnobSize(45), subType(subType), id(id)
{
    bypassParameterID = createParameterID("bypass");

    bypassed = false;

    this->addAndMakeVisible(removeButton);
    this->addAndMakeVisible(moveUpButton);
    this->addAndMakeVisible(moveDownButton);
    this->addAndMakeVisible(subsectionName);
    this->addAndMakeVisible(bypassButton);

    removeButton.setButtonText("X");
    moveUpButton.setButtonText("<");
    moveDownButton.setButtonText(">");
    bypassButton.setButtonText("");

    removeButton.addListener(this);
    moveUpButton.addListener(this);
    moveDownButton.addListener(this);
    bypassButton.addListener(this);

    bypassButton.setToggleable(true);
    bypassButton.setName(bypassParameterID);
}

void Subsection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &removeButton) {
        eventHandler.onDeleteSubsection(*this);
    }
    if (clickedButton == &moveUpButton) {
        eventHandler.onMoveSubsection(*this, -1);
    }
    if (clickedButton == &moveDownButton) {
        eventHandler.onMoveSubsection(*this, 1);
    }
    if (clickedButton == &bypassButton) {
        bypassButton.setToggleState(!bypassed, juce::NotificationType::dontSendNotification);
        eventHandler.onBypassChanged(*this, bypassButton.getToggleState());
    }
}

juce::String Subsection::getType()
{
    return type;
}

juce::String Subsection::getSubType()
{
    return subType;
}


void Subsection::onPostInitialization()
{
}

void Subsection::paint(juce::Graphics& g)
{
    if (bypassed) {
        // Definir la altura de la sección superior negra
        auto blackSectionHeight = 20; // Puedes ajustar esta altura según sea necesario

        // Pintar la parte superior de negro
        g.setColour(juce::Colours::darkgrey.withAlpha(0.5f));
        g.fillRect(0, 0, getWidth(), blackSectionHeight);

        // Pintar el resto de la región de gris
        g.setColour(juce::Colours::grey.withAlpha(0.5f));
        g.fillRect(0, blackSectionHeight, getWidth(), getHeight() - blackSectionHeight);

        bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    }
    else {
        // Definir la altura de la sección superior negra
        auto blackSectionHeight = 20;

        // Pintar la parte superior de negro
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(0, 0, getWidth(), blackSectionHeight);

        // Pintar el resto de la región de gris
        g.setColour(juce::Colours::grey);
        g.fillRect(0, blackSectionHeight, getWidth(), getHeight() - blackSectionHeight);

        bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
    }
}

void Subsection::sectionResized()
{
    auto area = getLocalBounds();

    removeButton.setBounds(area.getWidth()-25, 0, 20, 20);
    moveDownButton.setBounds(area.getWidth()-50, 0, 20, 20);
    moveUpButton.setBounds(area.getWidth()-75, 0, 20, 20);
    bypassButton.setBounds(0, 0, 20, 20);

    subsectionName.setBounds(bypassButton.getBounds().getX() + 2, 0, 20, 20);
    subsectionName.setText(getSubType()  + getIdAsString());
}

juce::String Subsection::createParameterID(const juce::String& type, int id, const juce::String& parameterName)
{
    return type + juce::String("_") + juce::String(id) + juce::String("_") + parameterName;
}

juce::String Subsection::createParameterID(const juce::String& parameterName)
{
    return getSubType() + juce::String("_") + getIdAsString() + juce::String("_") + parameterName;
}

void Subsection::setBypassed(bool bypassed)
{
    this->bypassed = bypassed;
    repaint();
}
