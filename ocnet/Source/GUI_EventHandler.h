/*
  ==============================================================================

    GUI_EventHandler.h
    Created: 4 Aug 2024 8:59:38pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "ParameterHandler/Parameter.h"

// Declaración adelantada de Subsection
class Subsection;

class GUI_EventHandler {
public:
    virtual void onAddEffect(int effectID) {};
    virtual void onAddOscillator(int option) {};
    virtual void onAddModulator(int option) {};

    virtual void onDeleteSubsection(Subsection& subsection) {};

    virtual void onConnectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) {};

    virtual void onMoveSubsection(Subsection& subsection, int positions) {};

    virtual void onBypassChanged(Subsection& subsection, bool toggled) {};

private:

};