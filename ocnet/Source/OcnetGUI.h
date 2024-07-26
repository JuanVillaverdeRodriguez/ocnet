#pragma once

#include <JuceHeader.h>
#include "Knobs/Knob1.h"
#include "GUI_Sections/HeaderSection.h"
#include "GUI_Sections/FooterSection.h"
#include "GUI_Sections/OscillatorsSection.h"
#include "GUI_Sections/ModulatorsSection.h"
#include "GUI_Sections/EffectsSection.h"

class OcnetGUI : public juce::Component, HeaderSection::Listener, public juce::DragAndDropContainer
{
public:
    OcnetGUI();
    ~OcnetGUI() override;

    //HeaderSectionListener
    void visualizeOscillatorSection() override;
    void visualizeEffectsSection() override;

    void clear();

    void initializeGUI();

    void attachParams(juce::AudioProcessorValueTreeState& apvts);

    void resized() override;

    HeaderSection* getHeaderSection() { return headerSection.get(); }
    FooterSection* getFooterSection() { return footerSection.get(); }
    OscillatorsSection* getOscillatorsSection() { return oscillatorsSection.get(); }
    ModulatorsSection* getModulatorsSection() { return modulatorsSection.get(); }
    EffectsSection* getEffectsSection() { return effectsSection.get(); }

protected:
    int currentView = 0;

    std::unique_ptr<HeaderSection> headerSection;
    std::unique_ptr<FooterSection> footerSection;
    std::unique_ptr<OscillatorsSection> oscillatorsSection;
    std::unique_ptr<ModulatorsSection> modulatorsSection;
    std::unique_ptr<EffectsSection> effectsSection;


    



};
