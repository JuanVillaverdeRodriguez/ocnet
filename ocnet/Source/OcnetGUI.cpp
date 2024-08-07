
#include "OcnetGUI.h"

OcnetGUI::OcnetGUI(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    headerSection = std::make_unique<HeaderSection>();
    footerSection = std::make_unique<FooterSection>();
    oscillatorsSection = std::make_unique<OscillatorsSection>(eventHandler);
    modulatorsSection = std::make_unique<ModulatorsSection>(eventHandler);
    effectsSection = std::make_unique<EffectsSection>(eventHandler);

    headerSection->addListener(this);


    this->addAndMakeVisible(headerSection.get());
    this->addAndMakeVisible(footerSection.get());
    this->addAndMakeVisible(effectsSectionViewport);
    this->addAndMakeVisible(oscillatorsSectionViewport);
    this->addAndMakeVisible(modulatorsSectionViewport);

    //this->addAndMakeVisible(oscillatorsSection.get());
    //this->addAndMakeVisible(modulatorsSection.get());
    //this->addAndMakeVisible(effectsSection.get());

    modulatorsSectionViewport.setViewedComponent(modulatorsSection.get(), false);
    effectsSectionViewport.setViewedComponent(effectsSection.get(), false);
    oscillatorsSectionViewport.setViewedComponent(oscillatorsSection.get(), false);

    modulatorsSectionViewport.setScrollBarsShown(true, false, false, false);
    effectsSectionViewport.setScrollBarsShown(true, false, false, false);
    oscillatorsSectionViewport.setScrollBarsShown(true, false, false, false);
    
}

OcnetGUI::~OcnetGUI()
{
}

void OcnetGUI::attachParams(juce::AudioProcessorValueTreeState& apvts) {

    //getModulatorsSection()->attachParams(apvts);
}

void OcnetGUI::clear() {
    this->removeAllChildren();
}

void OcnetGUI::visualizeOscillatorSection()
{
    DBG("OcnetGUI::visualizeOscillatorSection()");

    //this->removeChildComponent(this->getIndexOfChildComponent(getEffectsSection()));
    //this->addAndMakeVisible(getOscillatorsSection());
    oscillatorsSectionViewport.setVisible(true);
    effectsSectionViewport.setVisible(false);
}


void OcnetGUI::visualizeEffectsSection()
{
    oscillatorsSectionViewport.setVisible(false);
    effectsSectionViewport.setVisible(true);
    //DBG("OcnetGUI::visualizeEffectsSection()");

    //this->removeChildComponent(this->getIndexOfChildComponent(getOscillatorsSection()));
    //this->addAndMakeVisible(getEffectsSection());
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
    auto oscillatorsSectionViewportBounds = area.removeFromLeft(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto modulatorsSectionViewportBounds = area.withTrimmedLeft(headerSectionBounds.getWidth()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto effectsSectionViewportBounds = area.removeFromLeft(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    modulatorsSectionViewport.setBounds(modulatorsSectionViewportBounds);
    effectsSectionViewport.setBounds(effectsSectionViewportBounds);
    oscillatorsSectionViewport.setBounds(oscillatorsSectionViewportBounds);

    footerSection.get()->setBounds(footerSectionBounds);
    modulatorsSection.get()->setBounds(modulatorsSectionViewportBounds);
    headerSection.get()->setBounds(headerSectionBounds);
    oscillatorsSection.get()->setBounds(oscillatorsSectionViewportBounds);
    effectsSection.get()->setBounds(effectsSectionViewportBounds);

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

