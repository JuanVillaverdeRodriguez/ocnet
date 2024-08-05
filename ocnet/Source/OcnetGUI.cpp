
#include "OcnetGUI.h"

OcnetGUI::OcnetGUI(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    headerSection = std::make_unique<HeaderSection>();
    footerSection = std::make_unique<FooterSection>();
    oscillatorsSection = std::make_unique<OscillatorsSection>(eventHandler);
    modulatorsSection = std::make_unique<ModulatorsSection>(eventHandler);
    effectsSection = std::make_unique<EffectsSection>(eventHandler);

    headerSection->addListener(this);

    initializeGUI();

}

OcnetGUI::~OcnetGUI()
{
}

void OcnetGUI::attachParams(juce::AudioProcessorValueTreeState& apvts) {

    //getModulatorsSection()->attachParams(apvts);
}


void OcnetGUI::initializeGUI()
{
    DBG("OcnetGUI::initializeGUI()");

    this->addAndMakeVisible(headerSection.get());
    this->addAndMakeVisible(footerSection.get());
    this->addAndMakeVisible(oscillatorsSection.get());
    this->addAndMakeVisible(modulatorsSection.get());
    this->addAndMakeVisible(effectsSection.get());
}

void OcnetGUI::clear() {
    this->removeAllChildren();
}

void OcnetGUI::visualizeOscillatorSection()
{
    DBG("OcnetGUI::visualizeOscillatorSection()");

    this->removeChildComponent(this->getIndexOfChildComponent(getEffectsSection()));
    this->addAndMakeVisible(getOscillatorsSection());
}


void OcnetGUI::visualizeEffectsSection()
{
    DBG("OcnetGUI::visualizeEffectsSection()");

    this->removeChildComponent(this->getIndexOfChildComponent(getOscillatorsSection()));
    this->addAndMakeVisible(getEffectsSection());
}


void OcnetGUI::resized()
{
    DBG("OcnetGUI::resized()");

    auto area = getLocalBounds();

    DBG("TotalWidth: " + juce::String(area.getWidth()));
    DBG("TotalHeight: " + juce::String(area.getHeight()));

    auto headerSectionBounds = area.withTrimmedRight(area.getWidth() / 2.2).withTrimmedBottom(area.getHeight() - 50);

    area = getLocalBounds();
    auto footerSectionBounds = getLocalBounds().removeFromBottom(50);

    area = getLocalBounds();
    auto oscillatorsSectionBounds = area.removeFromLeft(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto modulatorsSectionBounds = area.withTrimmedLeft(headerSectionBounds.getWidth()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto effectsSectionBounds = area.removeFromLeft(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    footerSection.get()->setBounds(footerSectionBounds);
    modulatorsSection.get()->setBounds(modulatorsSectionBounds);
    headerSection.get()->setBounds(headerSectionBounds);
    oscillatorsSection.get()->setBounds(oscillatorsSectionBounds);
    effectsSection.get()->setBounds(effectsSectionBounds);

}

Section* OcnetGUI::getSection(const juce::String& type)
{
    if (type == "Modulators")
        return getModulatorsSection();
    else if (type == "Effects")
        return getEffectsSection();
    else if (type == "Oscillators")
        return getOscillatorsSection();
}