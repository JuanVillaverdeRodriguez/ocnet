
#include "OcnetGUI.h"
#include "OcnetGUI_interface.h"

OcnetGUI::OcnetGUI()
{
    headerSection = std::make_unique<HeaderSection>();
    footerSection = std::make_unique<FooterSection>();
    oscillatorsSection = std::make_unique<OscillatorsSection>();
    modulatorsSection = std::make_unique<ModulatorsSection>();
    effectsSection = std::make_unique<EffectsSection>();

    headerSection->addListener(this);

    initializeGUI();

}

OcnetGUI::~OcnetGUI()
{
}

void OcnetGUI::attachParams(juce::AudioProcessorValueTreeState& apvts) {
    /*using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButstonAttachment;

    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackKnob);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decayKnob);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainKnob);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseKnob);
    buttonAttachment = std::make_unique<ButtonAttachment>(apvts, "BUTTON", addOscillatorButton);*/
}


void OcnetGUI::initializeGUI()
{
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
