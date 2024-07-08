/*
  ==============================================================================

    gui_interface.cpp
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetGUI_interface.h"

OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor) : processor(*processor)
{
    numberOfEnvelopes = 0;
    gui_ = std::make_unique<OcnetGUI>();
    //gui_->attachParams(processor->apvts);

    //gui_->getHeaderSection()->addListener(this);
    //gui_->getFooterSection()->addListener(this);
    gui_->getOscillatorsSection()->addListener(this);
    gui_->getModulatorsSection()->addListener(this);
    gui_->getEffectsSection()->addListener(this);

}

void OcnetGUI_interface::addOscillator(int option)
{
    DBG("OcnetGUI_interface::addOscillator(int option)");

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

void OcnetGUI_interface::addEffectsChain()
{
    DBG("OcnetGUI_interface::addEffectsChain()");
}

void OcnetGUI_interface::addModulator(int option)
{
    if (option > 0) {
        if (option == 1) {
            gui_->getModulatorsSection()->addEnvelope(numberOfEnvelopes, processor.apvts);
            processor.addEnvelope(numberOfEnvelopes);

        }
    }
    DBG("OcnetGUI_interface::addModulator(int option)");

}

