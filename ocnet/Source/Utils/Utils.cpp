/*
  ==============================================================================

    Utils.cpp
    Created: 26 Jul 2024 12:59:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "Utils.h"
#include <limits>
#include <cmath>

Utils::Utils()
{
}

float Utils::linearToDecibels(float linearGain)
{
    if (linearGain <= 0.0f)
        return -std::numeric_limits<float>::infinity();
    else
        return 20.0f * std::log10(linearGain);
}
