/*
  ==============================================================================

    GUI.cpp
    Created: 1 Jul 2024 6:36:37pm
    Author:  TecNo

  ==============================================================================
*/

#include "GUI.h"

GUI::GUI(Controller controller) 
    : controller(controller)
{
    initializeGUI();
}

void GUI::initializeGUI()
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    button.setClickingTogglesState(true);

    for (auto* comp : getComps()) {
        addAndMakeVisible(comp);
        controller.addAttachment(comp);

        //comp->addListener(this);
    }

    //button.setButtonText("Mostrar Menú");
    //button.addListener(this);


    for each(auto parameter in parameterVector) {
    }

    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackKnob);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decayKnob);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainKnob);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseKnob);
    buttonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "BUTTON", button);

    //oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);
}



void GUI::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &button) {
        juce::PopupMenu menu;
        menu.addItem(1, "Wavetable oscillator");
        menu.addItem(2, "Sampler");
        menu.addItem(3, "Opcion 3");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(button),
            [this](int result)
            {
                if (result > 0) {
                    juce::Logger::writeToLog("Seleccionaste la opcion: " + juce::String(result));
                    //audioProcessor.addWavetableOscillator();

                    if (result == 1) {
                        controller.addOscillatorController();
                    }
                    else if (result == 2) {
                        //controller.addSampler();
                    }
                    else if (result == 3) {
                        //controller.addGranular();
                    }
                }
            });
    }
}

void GUI::resized()
{
    auto bounds = getLocalBounds();

    attackKnob.setBounds(60, 30, 100, getHeight() - 60);
    decayKnob.setBounds(160, 30, 100, getHeight() - 60);
    sustainKnob.setBounds(260, 30, 100, getHeight() - 60);
    releaseKnob.setBounds(360, 30, 100, getHeight() - 60);
    button.setBounds(460, 30, 100, getHeight() - 60);
}

// En vez de meter knobs individuales, aqui irian componentes generales 
// (Oscilador, efecto, lfo, interfaz general)
// De hecho, solo habria que cargar un unico componente => InterfazGeneral
std::vector<juce::Component*> GUI::getComps() {
    return {
        &attackKnob,
        &decayKnob,
        &sustainKnob,
        &releaseKnob,
        &button
    };
}

std::vector<Parameter> GUI::getParams()
{
    return {
        new Parameter("ATTACK", attackKnob);
    };
}


//&oscilator