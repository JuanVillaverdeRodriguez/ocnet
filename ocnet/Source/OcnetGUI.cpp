
#include "OcnetGUI.h"
#include "OcnetGUI_interface.h"

OcnetGUI::OcnetGUI()
{
    headerSection = std::make_unique<HeaderSection>();
    footerSection = std::make_unique<FooterSection>();
    oscillatorsSection = std::make_unique<OscillatorsSection>();
    modulatorsSection = std::make_unique<ModulatorsSection>();
    effectsSection = std::make_unique<EffectsSection>();

    initializeGUI();

}

OcnetGUI::~OcnetGUI()
{
}

void OcnetGUI::attachParams(juce::AudioProcessorValueTreeState& apvts) {
    /*using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

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
    /*for (auto* comp : getComps()) {
        comp->setVisible(false);
        //comp->addListener(this);
    }*/
}

void OcnetGUI::visualizeOscillatorSection()
{
}


void OcnetGUI::visualizeEffectsSection()
{
    //clear();
    oscillatorsSection->setVisible(false);
    currentView = 1;
}


void OcnetGUI::resized()
{
    auto area = getLocalBounds();

    DBG("RESIZED GUI");
    DBG(juce::String(area.getWidth()));
    DBG(juce::String(area.getHeight()));


    auto headerSectionBounds = area.withTrimmedRight(area.getWidth() / 2).withTrimmedBottom(area.getHeight() - 50);

    area = getLocalBounds();
    auto footerSectionBounds = getLocalBounds().removeFromBottom(50);

    area = getLocalBounds();
    auto oscillatorsSectionBounds = area.withTrimmedRight(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto modulatorsSectionBounds = area.withTrimmedLeft(headerSectionBounds.getWidth()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto effectsSectionBounds = area.withTrimmedRight(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    footerSection.get()->setBounds(footerSectionBounds);
    modulatorsSection.get()->setBounds(modulatorsSectionBounds);
    headerSection.get()->setBounds(headerSectionBounds);

    if (currentView == 0) { // Si la vista actual es la de synth...
        //effectsSection.get()->setBounds(effectsSectionBounds);
        oscillatorsSection.get()->setBounds(oscillatorsSectionBounds);
    }
    else { // Si es la de efectos...
        effectsSection.get()->setBounds(effectsSectionBounds);
    }

}
