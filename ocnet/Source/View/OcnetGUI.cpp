
#include "OcnetGUI.h"

OcnetGUI::OcnetGUI(GUI_EventHandler& eventHandler, juce::MidiKeyboardState& keyboardState) : 
    eventHandler(eventHandler),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard) 
{
    headerSection = std::make_unique<HeaderSection>();
    separatorsSection = std::make_unique<Separators>();
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
    this->addAndMakeVisible(separatorsSection.get());

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
    separatorsSection.get()->setInterceptsMouseClicks(false, false);
    visualizeOscillatorSection();
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

void OcnetGUI::paint(juce::Graphics& g)
{
    g.setColour(Palette::BackgroundThird);
    g.fillAll();

}

void OcnetGUI::resized()
{
    auto area = getLocalBounds();
    const int margin = 0;

    auto headerSectionBounds = area.withTrimmedRight(area.getWidth() / 2).withTrimmedBottom(area.getHeight() - 50);

    area = getLocalBounds();
    auto keyboardSectionBounds = area.removeFromBottom(50);

    area = getLocalBounds();
    auto footerSectionBounds = area.removeFromBottom(105);

    area = getLocalBounds();
    auto oscillatorsSectionViewportBounds = area.removeFromLeft(headerSectionBounds.getWidth())
        .withTrimmedTop(headerSectionBounds.getHeight())
        .withTrimmedBottom(footerSectionBounds.getHeight() + margin);

    area = getLocalBounds();
    auto modulatorsSectionViewportBounds = area.withTrimmedLeft(headerSectionBounds.getWidth() - (margin * 2))
        //.withTrimmedTop(headerSectionBounds.getHeight() + margin)
        .withTrimmedBottom(footerSectionBounds.getHeight() + margin);
    //modulatorsSectionViewportBounds.setX(headerSectionBounds.getWidth() + margin);

    area = getLocalBounds();
    auto effectsSectionViewportBounds = area.removeFromLeft(headerSectionBounds.getWidth())
        .withTrimmedTop(headerSectionBounds.getHeight())
        .withTrimmedBottom(footerSectionBounds.getHeight() + margin);

    modulatorsSectionViewport.setBounds(modulatorsSectionViewportBounds);
    effectsSectionViewport.setBounds(effectsSectionViewportBounds);
    oscillatorsSectionViewport.setBounds(oscillatorsSectionViewportBounds);

    // Ajustar el teclado MIDI
    int keyboardWidth = keyboardSectionBounds.getWidth();
    int numKeys = 128; // Número total de teclas MIDI
    float keyWidth = 8 + static_cast<float>(keyboardWidth) / numKeys; // Ancho de cada tecla

    keyboardComponent.setKeyWidth(keyWidth); // Ajustar el ancho de las teclas
    keyboardComponent.setAvailableRange(0, 127); // Rango completo de notas MIDI
    keyboardComponent.setBounds(keyboardSectionBounds);

    footerSection.get()->setBounds(footerSectionBounds);
    separatorsSection.get()->setBounds(getLocalBounds());
    modulatorsSection.get()->setBounds(modulatorsSectionViewportBounds);
    headerSection.get()->setBounds(headerSectionBounds);
    oscillatorsSection.get()->setBounds(oscillatorsSectionViewportBounds);
    effectsSection.get()->setBounds(effectsSectionViewportBounds);

    keyboardComponent.setScrollButtonsVisible(false);
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
