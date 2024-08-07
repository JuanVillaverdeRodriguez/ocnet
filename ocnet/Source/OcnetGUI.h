#pragma once

#include <JuceHeader.h>
#include "Knobs/Knob1.h"
#include "GUI_Sections/HeaderSection.h"
#include "GUI_Sections/FooterSection.h"
#include "GUI_Sections/OscillatorsSection.h"
#include "GUI_Sections/ModulatorsSection.h"
#include "GUI_Sections/EffectsSection.h"
#include "GUI_EventHandler.h"

class OcnetGUI : public juce::Component, HeaderSection::Listener, public juce::DragAndDropContainer
{
public:
    OcnetGUI(GUI_EventHandler& eventHandler);
    ~OcnetGUI() override;

    //HeaderSectionListener
    void visualizeOscillatorSection() override;
    void visualizeEffectsSection() override;

    void clear();

    void attachParams(juce::AudioProcessorValueTreeState& apvts);

    void resized() override;

    Section* getSection(const juce::String& type);

    HeaderSection* getHeaderSection() { return headerSection.get(); }
    FooterSection* getFooterSection() { return footerSection.get(); }
    OscillatorsSection* getOscillatorsSection() { return oscillatorsSection.get(); }
    ModulatorsSection* getModulatorsSection() { return modulatorsSection.get(); }
    EffectsSection* getEffectsSection() { return effectsSection.get(); }

protected:
    int currentView = 0;

    GUI_EventHandler& eventHandler;

    std::unique_ptr<HeaderSection> headerSection;
    std::unique_ptr<FooterSection> footerSection;
    std::unique_ptr<OscillatorsSection> oscillatorsSection;
    std::unique_ptr<ModulatorsSection> modulatorsSection;
    std::unique_ptr<EffectsSection> effectsSection;

    juce::Viewport modulatorsSectionViewport;
    juce::Viewport oscillatorsSectionViewport;
    juce::Viewport effectsSectionViewport;


    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetGUI)



};
