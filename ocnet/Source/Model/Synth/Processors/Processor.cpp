/*
  ==============================================================================

    Processor.cpp
    Created: 12 Jul 2024 2:36:16pm
    Author:  TecNo

  ==============================================================================
*/

#include "Processor.h"

Processor::Processor()
{
    id = 0;
    voiceId = 0;
    bypassed = false;
}

void Processor::setBypassed(bool bypassed)
{
    this->bypassed = bypassed;
}

bool Processor::isBypassed()
{
    return bypassed;
}
