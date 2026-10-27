
#include "OcnetGUI.h"

OcnetGUI::OcnetGUI(GUI_EventHandler& eventHandler, juce::MidiKeyboardState& keyboardState) : 
    eventHandler(eventHandler),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard) 
{
    setLookAndFeel(&baseLookAndFeel);

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

    for (auto* viewport : { &modulatorsSectionViewport, &effectsSectionViewport, &oscillatorsSectionViewport }) {
        viewport->setScrollBarsShown(true, false, false, false);
        viewport->setScrollBarThickness(8);
    }

    // Teclado
    keyboardComponent.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colour(0xffdfe2e9));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colour(0xff15181e));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colour(0xff9aa1b0));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, Palette::Accent.withAlpha(0.35f));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, Palette::Accent.withAlpha(0.85f));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::shadowColourId, juce::Colours::black.withAlpha(0.35f));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::textLabelColourId, juce::Colour(0xff6b7180));

    addAndMakeVisible(keyboardComponent);
    separatorsSection.get()->setInterceptsMouseClicks(false, false);
    visualizeOscillatorSection();
}

OcnetGUI::~OcnetGUI()
{
    setLookAndFeel(nullptr);
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
    g.fillAll(Palette::Background);

    // Barra superior
    g.setColour(Palette::Panel.withAlpha(0.6f));
    g.fillRect(topBarBounds);
    g.setColour(Palette::Border);
    g.drawHorizontalLine(topBarBounds.getBottom() - 1, 0.0f, (float)getWidth());

    paintLogo(g, topBarBounds.reduced(margin + 4, 0).toFloat());

    // Paneles
    paintPanel(g, leftPanelBounds);
    paintPanel(g, rightPanelBounds);

    // Titulo del panel de moduladores
    auto modHeader = rightPanelBounds.withHeight(panelHeaderHeight).reduced(16, 0);
    g.setColour(Palette::Text);
    g.setFont(Fonts::caps(13.0f, true, 0.1f));
    g.drawText("MODULATORS", modHeader, juce::Justification::centredLeft);

    g.setColour(Palette::TextFaint);
    g.setFont(Fonts::ui(12.0f));
    g.drawText("Drag the node icon onto any knob to modulate it", modHeader, juce::Justification::centredRight);

    // Marco del teclado
    g.setColour(Palette::Panel);
    g.fillRoundedRectangle(keyboardPanelBounds.toFloat(), 10.0f);
    g.setColour(Palette::Border);
    g.drawRoundedRectangle(keyboardPanelBounds.toFloat().reduced(0.5f), 10.0f, 1.0f);
}

void OcnetGUI::paintPanel(juce::Graphics& g, juce::Rectangle<int> panel)
{
    const auto bounds = panel.toFloat();
    const float radius = 10.0f;

    g.setColour(Palette::Panel);
    g.fillRoundedRectangle(bounds, radius);

    g.setColour(Palette::Border);
    g.drawHorizontalLine(panel.getY() + panelHeaderHeight, bounds.getX() + 1.0f, bounds.getRight() - 1.0f);
    g.drawRoundedRectangle(bounds.reduced(0.5f), radius, 1.0f);
}

void OcnetGUI::paintLogo(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Marca: cuadrado redondeado con degradado y una onda
    auto mark = area.removeFromLeft(30.0f).withSizeKeepingCentre(30.0f, 30.0f);

    g.setGradientFill(juce::ColourGradient(Palette::Primary, mark.getX(), mark.getY(),
                                           Palette::Secondary, mark.getRight(), mark.getBottom(), false));
    g.fillRoundedRectangle(mark, 8.0f);

    juce::Path wave;
    auto waveArea = mark.reduced(7.0f, 10.0f);
    wave.startNewSubPath(waveArea.getX(), waveArea.getCentreY());
    for (float x = 0.0f; x <= 1.0f; x += 0.05f)
        wave.lineTo(waveArea.getX() + x * waveArea.getWidth(),
                    waveArea.getCentreY() - std::sin(x * juce::MathConstants<float>::twoPi) * waveArea.getHeight() * 0.5f);

    g.setColour(Palette::Background);
    g.strokePath(wave, juce::PathStrokeType(2.2f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    area.removeFromLeft(12.0f);

    g.setColour(Palette::White);
    g.setFont(Fonts::caps(20.0f, true, 0.22f));
    g.drawText("OCNET", area.withHeight(24.0f).withCentre({ area.getCentreX(), area.getCentreY() - 6.0f }), juce::Justification::centredLeft);

    g.setColour(Palette::TextFaint);
    g.setFont(Fonts::caps(9.0f, true, 0.2f));
    g.drawText("MODULAR SYNTHESIZER", area.withHeight(12.0f).withCentre({ area.getCentreX(), area.getCentreY() + 11.0f }), juce::Justification::centredLeft);
}

void OcnetGUI::resized()
{
    auto area = getLocalBounds();

    topBarBounds = area.removeFromTop(topBarHeight);
    area.reduce(margin, margin);

    keyboardPanelBounds = area.removeFromBottom(76);
    area.removeFromBottom(margin);

    leftPanelBounds = area.removeFromLeft((area.getWidth() - margin) / 2);
    area.removeFromLeft(margin);
    rightPanelBounds = area;

    // Pestanas en la cabecera del panel izquierdo, controles globales en la barra superior
    headerSection->setBounds(leftPanelBounds.withHeight(panelHeaderHeight));
    footerSection->setBounds(topBarBounds.withTrimmedLeft(topBarBounds.getWidth() / 2).reduced(margin + 4, 0));

    // Los viewports se insertan un poco para respetar las esquinas redondeadas del panel
    auto viewportBounds = [](juce::Rectangle<int> panel) {
        return panel.withTrimmedTop(panelHeaderHeight + 1).withTrimmedBottom(6).reduced(1, 0);
    };

    oscillatorsSectionViewport.setBounds(viewportBounds(leftPanelBounds));
    effectsSectionViewport.setBounds(viewportBounds(leftPanelBounds));
    modulatorsSectionViewport.setBounds(viewportBounds(rightPanelBounds));

    // El contenido ocupa el ancho del viewport menos la barra de scroll
    auto contentBounds = [](juce::Viewport& viewport) {
        return juce::Rectangle<int>(0, 0, viewport.getWidth() - viewport.getScrollBarThickness(), viewport.getHeight());
    };

    oscillatorsSection->setBounds(contentBounds(oscillatorsSectionViewport));
    effectsSection->setBounds(contentBounds(effectsSectionViewport));
    modulatorsSection->setBounds(contentBounds(modulatorsSectionViewport));

    // Teclado MIDI: las 75 teclas blancas del rango completo caben en el ancho disponible
    auto keyboardBounds = keyboardPanelBounds.reduced(8);
    keyboardComponent.setAvailableRange(0, 127);
    keyboardComponent.setKeyWidth(keyboardBounds.getWidth() / 75.0f);
    keyboardComponent.setBounds(keyboardBounds);
    keyboardComponent.setScrollButtonsVisible(false);

    separatorsSection->setBounds(getLocalBounds());
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
