
#include "OcnetGUI.h"

OcnetGUI::OcnetGUI(GUI_EventHandler& eventHandler, juce::MidiKeyboardState& keyboardState) : 
    eventHandler(eventHandler),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard) 
{


    headerSection = std::make_unique<HeaderSection>();
    footerSection = std::make_unique<FooterSection>(eventHandler);
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

    addAndMakeVisible(keyboardComponent);


    
}

OcnetGUI::~OcnetGUI()
{
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

void OcnetGUI::addSynthParams(ParameterHandler& parameterHandler)
{
    footerSection->addSynthParams(parameterHandler);
}

void OcnetGUI::attachSynthParams(ParameterHandler& parameterHandler)
{
    footerSection->attachSynthParams(parameterHandler);
}

void OcnetGUI::resized()
{

    auto area = getLocalBounds();

    auto headerSectionBounds = area.withTrimmedRight(area.getWidth() / 2.2).withTrimmedBottom(area.getHeight() - 50);

    area = getLocalBounds();
    auto keyboardSectionBounds = area.removeFromBottom(50);

    area = getLocalBounds();
    auto footerSectionBounds = area.removeFromBottom(105);

    area = getLocalBounds();
    auto oscillatorsSectionViewportBounds = area.removeFromLeft(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto modulatorsSectionViewportBounds = area.withTrimmedLeft(headerSectionBounds.getWidth()).withTrimmedBottom(footerSectionBounds.getHeight());

    area = getLocalBounds();
    auto effectsSectionViewportBounds = area.removeFromLeft(headerSectionBounds.getWidth()).withTrimmedTop(headerSectionBounds.getHeight()).withTrimmedBottom(footerSectionBounds.getHeight());

    modulatorsSectionViewport.setBounds(modulatorsSectionViewportBounds);
    effectsSectionViewport.setBounds(effectsSectionViewportBounds);
    oscillatorsSectionViewport.setBounds(oscillatorsSectionViewportBounds);

    keyboardComponent.setBounds(keyboardSectionBounds);
    footerSection.get()->setBounds(footerSectionBounds);
    modulatorsSection.get()->setBounds(modulatorsSectionViewportBounds);
    headerSection.get()->setBounds(headerSectionBounds);
    oscillatorsSection.get()->setBounds(oscillatorsSectionViewportBounds);
    effectsSection.get()->setBounds(effectsSectionViewportBounds);

    // Oculta las barras de desplazamiento (debería ser innecesario si el tamaño es correcto)
    keyboardComponent.setScrollButtonsVisible(false);
    keyboardComponent.setAvailableRange(24, 127);


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


void OcnetGUI::editorIsShowing()
{
    // Permitir que el componente del teclado reciba eventos de teclado
    keyboardComponent.setWantsKeyboardFocus(true);
    keyboardComponent.grabKeyboardFocus();
}
