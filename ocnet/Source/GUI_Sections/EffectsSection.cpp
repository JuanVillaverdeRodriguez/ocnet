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

    int lastEffectPosition = 0;

    for (auto& distortion : effectsSubsectionList) {
        distortion->setBounds(5, lastEffectPosition + 5, area.getWidth(), 50);
        lastEffectPosition += 50;
    }


    addEffectButton.setBounds(area.getWidth() / 2 - 25, lastEffectPosition + 5, 50, 50);
}


void EffectsSection::addDistortion(int id, ParameterHandler& parameterHandler)
{
    std::unique_ptr<DistortionSubsection> distortion = std::make_unique<DistortionSubsection>(id, eventHandler);

    effectsSubsectionList.push_back(std::move(distortion));
    this->addAndMakeVisible(*effectsSubsectionList.back());
    resized();
    effectsSubsectionList.back()->attachParams(parameterHandler);
}

void EffectsSection::deleteEffect(int id)
{
    effectsSubsectionList.remove_if([&id](const std::unique_ptr<EffectsSubsection>& effect) {
        return effect->getId() == id;
        });

    resized();
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