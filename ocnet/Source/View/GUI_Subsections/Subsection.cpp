/*
  ==============================================================================

    Subsection.cpp
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "Subsection.h"

Subsection::Subsection(GUI_EventHandler& eventHandler, int id, const juce::String& type, const juce::String& subType) 
    : eventHandler(eventHandler), defaultKnobSize(45), subType(subType), id(id), type(type), desiredHeight(125)
{
    bypassParameterID = createParameterID("bypass");

    bypassed = false;

    this->addAndMakeVisible(removeButton);
    this->addAndMakeVisible(moveUpButton);
    this->addAndMakeVisible(moveDownButton);
    this->addAndMakeVisible(bypassButton);
    // El titulo se dibuja en paint() (nombre + numero en dos tonos)
    this->addChildComponent(subsectionName);
    subsectionName.setText(subType + " " + getIdAsString(), juce::dontSendNotification);

    removeButton.setButtonText("X");
    moveUpButton.setButtonText("^");
    moveDownButton.setButtonText("v");
    bypassButton.setButtonText("");

    removeButton.setTooltip("Remove");
    moveUpButton.setTooltip("Move up");
    moveDownButton.setTooltip("Move down");
    bypassButton.setTooltip("Enable / bypass");

    removeButton.setLookAndFeel(&lookAndFeel);
    moveUpButton.setLookAndFeel(&lookAndFeel);
    moveDownButton.setLookAndFeel(&lookAndFeel);
    bypassButton.setLookAndFeel(&lookAndFeel);

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

juce::Array<Knob1*> Subsection::getAllKnobs()
{
    juce::Array<Knob1*> knobs;

    for (auto& component : getChildren()) {
        if (auto knob = dynamic_cast<Knob1*>(component))
            knobs.add(knob);
    }
    return knobs;
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
    const float cornerRadius = 8.0f;
    const auto bounds = getLocalBounds().toFloat();
    const auto typeColour = getTypeColour();

    // Tarjeta
    g.setColour(Palette::Subsection);
    g.fillRoundedRectangle(bounds, cornerRadius);

    // Cabecera
    juce::Path header;
    header.addRoundedRectangle(0.0f, 0.0f, bounds.getWidth(), (float)headerHeight, cornerRadius, cornerRadius, true, true, false, false);
    g.setColour(Palette::SubsectionTop);
    g.fillPath(header);

    g.setColour(Palette::Border);
    g.drawHorizontalLine(headerHeight, 0.0f, bounds.getWidth());
    g.drawRoundedRectangle(bounds.reduced(0.5f), cornerRadius, 1.0f);

    // LED de bypass (o un punto fijo si la subseccion no se puede desactivar)
    auto led = bypassButton.getBounds().toFloat().withSizeKeepingCentre(9.0f, 9.0f);
    if (!bypassButton.isVisible())
        led = juce::Rectangle<float>(9.0f, 9.0f).withCentre({ 16.0f, headerHeight * 0.5f });

    if (!bypassed) {
        g.setColour(typeColour.withAlpha(0.25f));
        g.fillEllipse(led.expanded(3.0f));
        g.setColour(typeColour);
        g.fillEllipse(led);
    }
    else {
        g.setColour(Palette::TextFaint);
        g.drawEllipse(led.reduced(0.5f), 1.4f);
    }

    // Titulo: "WAVETABLE  1"
    auto titleArea = juce::Rectangle<int>(30, 0, moveUpButton.getX() - 30, headerHeight);
    const auto nameFont = Fonts::caps(12.0f, true, 0.12f);
    const auto name = getDisplayName().toUpperCase();

    g.setFont(nameFont);
    g.setColour(bypassed ? Palette::TextDim : Palette::Text);
    g.drawText(name, titleArea, juce::Justification::centredLeft, true);

    juce::GlyphArrangement glyphs;
    glyphs.addLineOfText(nameFont, name, 0.0f, 0.0f);
    const int nameWidth = (int)std::ceil(glyphs.getBoundingBox(0, -1, true).getWidth());

    g.setColour(Palette::TextFaint);
    g.drawText(getIdAsString(), titleArea.withTrimmedLeft(nameWidth + 8), juce::Justification::centredLeft, true);

    paintCalled(g);
}

void Subsection::paintOverChildren(juce::Graphics& g)
{
    if (!bypassed)
        return;

    // Atenua el contenido de la subseccion cuando esta desactivada
    juce::Path body;
    body.addRoundedRectangle(1.0f, (float)headerHeight + 1.0f, (float)getWidth() - 2.0f, (float)getHeight() - headerHeight - 2.0f,
                             7.0f, 7.0f, false, false, true, true);
    g.setColour(Palette::Subsection.withAlpha(0.55f));
    g.fillPath(body);
}

void Subsection::layoutHeader()
{
    auto area = getLocalBounds();
    const int buttonSize = 24;
    const int buttonY = (headerHeight - buttonSize) / 2;

    removeButton.setBounds(area.getWidth() - buttonSize - 4, buttonY, buttonSize, buttonSize);
    moveDownButton.setBounds(removeButton.getX() - buttonSize, buttonY, buttonSize, buttonSize);
    moveUpButton.setBounds(moveDownButton.getX() - buttonSize, buttonY, buttonSize, buttonSize);
    bypassButton.setBounds(4, buttonY, buttonSize, buttonSize);

    // Los controles toman el color del tipo de subseccion
    const auto typeColour = getTypeColour();
    for (auto* child : getChildren()) {
        if (auto* slider = dynamic_cast<juce::Slider*>(child))
            slider->setColour(juce::Slider::rotarySliderFillColourId, typeColour);
        else if (auto* button = dynamic_cast<juce::TextButton*>(child))
            button->setColour(juce::TextButton::buttonOnColourId, typeColour);
    }
}

juce::Rectangle<int> Subsection::getContentArea()
{
    return getLocalBounds().withTrimmedTop(headerHeight).reduced(padding);
}

void Subsection::placeKnob(Knob1& knob, int x, int y)
{
    knob.setBounds(x, y, knobWidth, knobHeight);
    knob.showLabel(*this, knob);
}

juce::Colour Subsection::getTypeColour()
{
    return Palette::forType(type);
}

juce::String Subsection::getDisplayName()
{
    if (subType == "WavetableOscillator")
        return "Wavetable";
    if (subType == "Equalizer")
        return "EQ";
    return subType;
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