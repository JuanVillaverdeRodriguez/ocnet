#pragma once

#include <JuceHeader.h>
#include "Knobs/Knob1.h"
#include "GUI_Sections/HeaderSection.h"
#include "GUI_Sections/FooterSection.h"
#include "GUI_Sections/OscillatorsSection.h"
#include "GUI_Sections/ModulatorsSection.h"
#include "GUI_Sections/EffectsSection.h"





class OcnetGUI : public juce::Component, HeaderSection::Listener
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


    /*Knob1 attackKnob;
    Knob1 decayKnob;
    Knob1 sustainKnob;
    Knob1 releaseKnob;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment; //A
    std::unique_ptr<SliderAttachment> decayAttachment; //D
    std::unique_ptr<SliderAttachment> sustainAttachment; //S
    std::unique_ptr<SliderAttachment> releaseAttachment; //R


    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;*/
    //Unique ponteirs para que al cerrar la UI (el plugin se destruye) se libere la memoria



};
