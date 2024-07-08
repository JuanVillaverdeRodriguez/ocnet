/*
  ==============================================================================

    gui_interface.cpp
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetGUI_interface.h"

OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor) : processor(*processor), OcnetGUI::Listener()
{
    gui_ = std::make_unique<OcnetGUI>();
    gui_->assignController(this);
    gui_->attachParams(processor->apvts);
}

void OcnetGUI_interface::addOscillator(int option)
{
    if (option > 0) {
        if (option == 1) {
            processor.addWavetableOscillator();
        }
        else if (option == 2) {
            //processor.addSampler();
        }
        else if (option == 3) {
            //processor.addGranular();
        }
    }
}

void OcnetGUI_interface::addEffect(int option)
{
}

void OcnetGUI_interface::addEffectsChain(int option)
{
}

void OcnetGUI_interface::addModulator(int option)
{
}

void OcnetGUI_interface::attachParams()
{
}

