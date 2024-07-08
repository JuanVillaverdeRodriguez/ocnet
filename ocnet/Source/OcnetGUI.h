#pragma once

#include <JuceHeader.h>
#include "Knobs/Knob1.h"
#include "GUI_Sections/HeaderSection.h"
#include "GUI_Sections/FooterSection.h"
#include "GUI_Sections/OscillatorsSection.h"
#include "GUI_Sections/ModulatorsSection.h"




class OcnetGUI : public juce::Component, juce::Button::Listener, 
    HeaderSection::Listener, OscillatorsSection::Listener, ModulatorsSection::Listener 
{
public:
    OcnetGUI();
    ~OcnetGUI() override;

    //Interfaz de comunicación con OcnetGUI_interface
    class Listener { 
    public:
        virtual ~Listener() { }
        virtual void addOscillator(int option) = 0;
        virtual void addEffect(int option) = 0;
        virtual void addEffectsChain(int option) = 0;
        virtual void addModulator(int option) = 0;
        virtual void attachParams() = 0;
    };

    //HeaderSectionListener
    void visualizeOscillatorSection() override;
    void visualizeEffectsSection() override;

    //OscillatorsSectionListener
    void addOscillator(int option) override;

    //ModulatorsSectionListener
    void addModulator(int option) override;

    void assignController(Listener* listener);

    void clear();

    void buttonClicked(juce::Button* clickedButton) override;

    void initializeGUI();

    void attachParams(juce::AudioProcessorValueTreeState& apvts);

    void resized() override;


protected:
    //Listener* listener;
    std::vector<Listener*> listeners_;
    bool added = false;

    int currentView = 0;

    /*std::unique_ptr<ModulatorsSection>();
    std::unique_ptr<EffectsSection>();
    std::unique_ptr<OscillatorsSection>();
    std::unique_ptr<SynthSection>();
    std::unique_ptr<EffectsSection>();
    std::unique_ptr<FooterSection>();*/
    std::unique_ptr<HeaderSection> headerSection;
    std::unique_ptr<FooterSection> footerSection;
    std::unique_ptr<OscillatorsSection> oscillatorsSection;
    std::unique_ptr<ModulatorsSection> modulatorsSection;


    std::vector<juce::Component*> getComps();
    Knob1 midiVolume;

    Knob1 attackKnob;
    Knob1 decayKnob;
    Knob1 sustainKnob;
    Knob1 releaseKnob;
    juce::ComboBox oscSelector;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;


    std::unique_ptr<SliderAttachment> attackAttachment; //A
    std::unique_ptr<SliderAttachment> decayAttachment; //D
    std::unique_ptr<SliderAttachment> sustainAttachment; //S
    std::unique_ptr<SliderAttachment> releaseAttachment; //R


    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<ButtonAttachment> buttonAttachment; //R
    std::unique_ptr<OptionAttachment> optionAttachment; //R

    juce::TextButton effectsButton{ "effectsButton" };


    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;
    //Unique ponteirs para que al cerrar la UI (el plugin se destruye) se libere la memoria


    juce::TextButton addOscillatorButton{ "Mostrar Menu" };

};
