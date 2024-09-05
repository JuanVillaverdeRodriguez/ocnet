/*
  ==============================================================================

    Subsection.cpp
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "Subsection.h"

Subsection::Subsection(GUI_EventHandler& eventHandler, int id, const juce::String& type, const juce::String& subType) 
    : eventHandler(eventHandler), defaultKnobSize(45), subType(subType), id(id), type(type), desiredHeight(80)
{
    bypassParameterID = createParameterID("bypass");

    bypassed = false;

    this->addAndMakeVisible(removeButton);
    this->addAndMakeVisible(moveUpButton);
    this->addAndMakeVisible(moveDownButton);
    this->addAndMakeVisible(bypassButton);
    this->addAndMakeVisible(subsectionName);

    subsectionName.setText(subType + " " + getIdAsString(), juce::dontSendNotification);

    subsectionName.setFont(juce::Font("Bahnschrift", 12.0f, juce::Font::bold));
    subsectionName.setColour(subsectionName.textColourId, Palette::Text);

    removeButton.setButtonText("X");
    moveUpButton.setButtonText("<");
    moveDownButton.setButtonText(">");
    bypassButton.setButtonText("");

    removeButton.setLookAndFeel(&lookAndFeel);
    moveUpButton.setLookAndFeel(&lookAndFeel);
    moveDownButton.setLookAndFeel(&lookAndFeel);

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
    else if (clickedButton == &moveUpButton) {
        eventHandler.onMoveSubsection(*this, -1);
    }
    else if (clickedButton == &moveDownButton) {
        eventHandler.onMoveSubsection(*this, 1);
    }
    else if (clickedButton == &bypassButton) {
        bypassButton.setToggleState(!bypassed, juce::NotificationType::dontSendNotification);
        setBypassed(!bypassButton.getToggleState());
        eventHandler.onBypassChanged(*this, bypassButton.getToggleState());
    }
    else {
        buttonClickedCalled(clickedButton);
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

void Subsection::setDesiredHeight(int newHeight)
{
    desiredHeight = newHeight;
}

int Subsection::getDesiredHeight()
{
    return desiredHeight;
}

void Subsection::paint(juce::Graphics& g)
{
    const float cornerRadius = 5.0f; // Radio para los bordes redondeados
    const float blackSectionHeight = 20.0f; // Altura de la sección superior negra

    juce::Path topRoundedRect;
    juce::Path bottomRoundedRect;

    // Crear el rectángulo superior con bordes redondeados solo arriba
    topRoundedRect.addRoundedRectangle(0, 0, getWidth(), blackSectionHeight, cornerRadius, cornerRadius, true, true, false, false);

    // Crear el rectángulo inferior con bordes redondeados solo abajo
    bottomRoundedRect.addRoundedRectangle(0, blackSectionHeight, getWidth(), getHeight() - blackSectionHeight, cornerRadius, cornerRadius, false, false, true, true);

    if (bypassed) {
        // Pintar la parte superior con bordes redondeados solo arriba
        g.setColour(Palette::SubsectionTop);
        g.setOpacity(0.5);
        g.fillPath(topRoundedRect);

        // Pintar el resto de la región de gris con bordes redondeados solo abajo
        g.setColour(Palette::Subsection);
        g.setOpacity(0.5);
        g.fillPath(bottomRoundedRect);

        bypassButton.setColour(juce::TextButton::buttonColourId, Palette::BypasButtonToggled);
    }
    else {
        // Pintar la parte superior con bordes redondeados solo arriba
        g.setColour(Palette::SubsectionTop);
        g.fillPath(topRoundedRect);

        // Pintar el resto de la región de gris con bordes redondeados solo abajo
        g.setColour(Palette::Subsection);
        g.fillPath(bottomRoundedRect);

        bypassButton.setColour(juce::TextButton::buttonColourId, Palette::BypasButtonOff);
    }

    paintCalled(g);
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