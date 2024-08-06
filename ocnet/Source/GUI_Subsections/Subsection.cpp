/*
  ==============================================================================

    Subsection.cpp
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "Subsection.h"

Subsection::Subsection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler), defaultKnobSize(35)
{
    id = 0;

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
        auto blackSectionHeight = 20; // Puedes ajustar esta altura según sea necesario

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

    subsectionName.setBounds(20, 0, 20, 20);
}

void Subsection::setBypassed(bool bypassed)
{
    this->bypassed = bypassed;
    repaint();
}
