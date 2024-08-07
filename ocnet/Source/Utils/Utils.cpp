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

std::tuple<juce::String, juce::String, juce::String> Utils::splitParameterID(const juce::String& input)
{
    // Encontrar la primera parte de la cadena
    juce::String part1 = input.upToFirstOccurrenceOf("_", false, false);

    // Eliminar la primera parte y el guion bajo del input
    juce::String remainder = input.fromFirstOccurrenceOf("_", false, false);

    // Encontrar la segunda parte
    juce::String part2 = remainder.upToFirstOccurrenceOf("_", false, false);

    // Eliminar la segunda parte y el guion bajo del resto
    juce::String part3 = remainder.fromFirstOccurrenceOf("_", false, false);

    return std::make_tuple(part1, part2, part3);
}