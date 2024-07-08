
#include "OcnetGUI.h"
#include "OcnetGUI_interface.h"

OcnetGUI::OcnetGUI()
{
    headerSection = std::make_unique<HeaderSection>();
    footerSection = std::make_unique<FooterSection>();
    oscillatorsSection = std::make_unique<OscillatorsSection>();
    modulatorsSection = std::make_unique<ModulatorsSection>();

    oscillatorsSection->addListener(this);
    modulatorsSection->addListener(this);

    initializeGUI();

}

OcnetGUI::~OcnetGUI()
{
}

void OcnetGUI::attachParams(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackKnob);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decayKnob);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainKnob);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseKnob);
    buttonAttachment = std::make_unique<ButtonAttachment>(apvts, "BUTTON", addOscillatorButton);
}


void OcnetGUI::initializeGUI()
{
    addOscillatorButton.setClickingTogglesState(true);

    /*for (auto* comp : getComps()) {
        this->addAndMakeVisible(comp);
        //comp->addListener(this);
    }*/

    this->addAndMakeVisible(headerSection.get());
    this->addAndMakeVisible(footerSection.get());
    this->addAndMakeVisible(oscillatorsSection.get());
    this->addAndMakeVisible(modulatorsSection.get());


    addOscillatorButton.addListener(this);
    effectsButton.addListener(this);

}

void OcnetGUI::addModulator(int option)
{
    listeners_[0]->addModulator(option);
}

void OcnetGUI::assignController(Listener* listener)
{
    listeners_.push_back(listener);
}

void OcnetGUI::clear() {
    for (auto* comp : getComps()) {
        comp->setVisible(false);
        //comp->addListener(this);
    }
    effectsButton.setVisible(true);
}

void OcnetGUI::visualizeOscillatorSection()
{
}


void OcnetGUI::visualizeEffectsSection()
{
    clear();
}

void OcnetGUI::addOscillator(int option)
{
    listeners_[0]->addOscillator(option);
}

void OcnetGUI::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &effectsButton) {
        visualizeEffectsSection();
    }
    // Lo mismo con efectos, cadena de efectos y moduladores
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

    if (currentView == 0) { // Si la vista actual es la de synth...
        headerSection.get()->setBounds(headerSectionBounds);
        oscillatorsSection.get()->setBounds(oscillatorsSectionBounds);
        footerSection.get()->setBounds(footerSectionBounds);
        modulatorsSection.get()->setBounds(modulatorsSectionBounds);


    }
    else { // Si es la de efectos...

    }
    /*attackKnob.setBounds(60, 30, 100, getHeight() - 60);
    decayKnob.setBounds(160, 30, 100, getHeight() - 60);
    sustainKnob.setBounds(260, 30, 100, getHeight() - 60);
    releaseKnob.setBounds(360, 30, 100, getHeight() - 60);
    addOscillatorButton.setBounds(460, 30, 100, getHeight() - 60);
    effectsButton.setBounds(560, 30, 100, getHeight() - 60);*/
}










// En vez de meter knobs individuales, aqui irian componentes generales 
// (Oscilador, efecto, lfo, interfaz general)
// De hecho, solo habria que cargar un unico componente => InterfazGeneral
std::vector<juce::Component*> OcnetGUI::getComps() {
    return {
        &attackKnob,
        &decayKnob,
        &sustainKnob,
        &releaseKnob,
        &addOscillatorButton,
        &effectsButton
    };

}