/*
  ==============================================================================

    EffectsSection.cpp
    Created: 7 Jul 2024 9:44:54pm
    Author:  TecNo

  ==============================================================================
*/

#include "EffectsSection.h"

EffectsSection::EffectsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addEffectButton);
    addEffectButton.setButtonText("+");

    addEffectButton.addListener(this);
}

void EffectsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkorange);
}

void EffectsSection::resized()
{
    auto area = getLocalBounds();

    int lastSubsection = sectionResized();

    addEffectButton.setBounds(area.getWidth() / 2 - 25, lastSubsection + 5, 50, 50);
}

std::vector<std::unique_ptr<Subsection>>* EffectsSection::getListOfSubsections()
{
    return &subsectionsVector;
}

void EffectsSection::addDistortion(int id, ParameterHandler& parameterHandler)
{
    std::unique_ptr<DistortionSubsection> distortion = std::make_unique<DistortionSubsection>(id, eventHandler);

    subsectionsVector.push_back(std::move(distortion));
    this->addAndMakeVisible(*subsectionsVector.back());
    resized();
    subsectionsVector.back()->attachParams(parameterHandler);
}

void EffectsSection::addFilter(int id, ParameterHandler& parameterHandler)
{
    std::unique_ptr<FilterSubsection> subsection = std::make_unique<FilterSubsection>(id, eventHandler);

    subsectionsVector.push_back(std::move(subsection));
    this->addAndMakeVisible(*subsectionsVector.back());
    resized();
    subsectionsVector.back()->attachParams(parameterHandler);
}


void EffectsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addEffectButton) {
        juce::PopupMenu menu;
        menu.addItem(1, "Distortion");
        menu.addItem(2, "Reverb");
        menu.addItem(3, "Filter");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addEffectButton),
            [this](int result)
            {
                eventHandler.onAddEffect(result);
            });
    }

}