#pragma once

#include <JuceHeader.h>
#include "GUI_Elements/Knobs/Knob1.h"
#include "GUI_Sections/Footer/FooterSection.h"
#include "GUI_Sections/Oscillators/OscillatorsSection.h"
#include "GUI_Sections/Modulators/ModulatorsSection.h"
#include "GUI_Sections/Effects/EffectsSection.h"
#include "GUI_Sections/Header/HeaderSection.h"
#include "GUI_Sections/Separators/Separators.h"
#include "../Controller/GUI_EventHandler.h"
#include "GUI_Sections/Section.h"
#include "../Model/ParameterHandler/ParameterHandler.h"

class OcnetGUI : public juce::Component, HeaderSection::Listener, public juce::DragAndDropContainer
{
public:
    OcnetGUI(GUI_EventHandler& eventHandler, juce::MidiKeyboardState& keyboardState);
    ~OcnetGUI() override;

    //HeaderSectionListener
    void visualizeOscillatorSection() override;
    void visualizeEffectsSection() override;

    void addSynthParams(ParameterHandler& parameterHandler);
    void attachSynthParams(ParameterHandler& parameterHandler);

    void paint(juce::Graphics& g) override;


    void clear();

    void resized() override;

    Section* getSection(const juce::String& type);




    HeaderSection* getHeaderSection() { return headerSection.get(); }
    FooterSection* getFooterSection() { return footerSection.get(); }
    OscillatorsSection* getOscillatorsSection() { return oscillatorsSection.get(); }
    ModulatorsSection* getModulatorsSection() { return modulatorsSection.get(); }
    EffectsSection* getEffectsSection() { return effectsSection.get(); }

    void editorIsShowing();


protected:
    void paintLogo(juce::Graphics& g, juce::Rectangle<float> area);
    void paintPanel(juce::Graphics& g, juce::Rectangle<int> panel);

    // Debe declararse antes que los componentes para destruirse despues de ellos
    OcnetBaseLookAndFeel baseLookAndFeel;

    int currentView = 0;

    juce::Rectangle<int> topBarBounds;
    juce::Rectangle<int> leftPanelBounds;
    juce::Rectangle<int> rightPanelBounds;
    juce::Rectangle<int> keyboardPanelBounds;

    static constexpr int topBarHeight = 56;
    static constexpr int panelHeaderHeight = 46;
    static constexpr int margin = 12;

    GUI_EventHandler& eventHandler;

    std::unique_ptr<HeaderSection> headerSection;
    std::unique_ptr<FooterSection> footerSection;
    std::unique_ptr<OscillatorsSection> oscillatorsSection;
    std::unique_ptr<ModulatorsSection> modulatorsSection;
    std::unique_ptr<EffectsSection> effectsSection;
    std::unique_ptr<Separators> separatorsSection;

    juce::Viewport modulatorsSectionViewport;
    juce::Viewport oscillatorsSectionViewport;
    juce::Viewport effectsSectionViewport;

    juce::MidiKeyboardComponent keyboardComponent;
    juce::TooltipWindow tooltipWindow { this, 700 };


    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetGUI)



};
