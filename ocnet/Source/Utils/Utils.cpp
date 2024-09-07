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

std::tuple<juce::String, juce::String> Utils::splitParameterModulationID(const juce::String& input)
{
    // Encontrar la primera parte de la cadena
    juce::String part1 = input.upToFirstOccurrenceOf("_", false, false);

    // Eliminar la primera parte y el guion bajo del input
    juce::String remainder = input.fromFirstOccurrenceOf("_", false, false);

    return std::make_tuple(part1, remainder);
}

float Utils::average(float* values, int numValues, bool inAbsoluteValue, int loopIncrement)
{
    if (numValues <= 0) {
        return 0.0f; // Evitar división por cero o procesamiento innecesario
    }

    float sum = 0.0f;

    // Recorrer el array y sumar todos los valores
    for (int i = 0; i < numValues; i += loopIncrement) {
        if (inAbsoluteValue) {
            sum += std::abs(values[i]);
        }
        else {
            sum += values[i];
        }
    }

    // Calcular la media
    float average = sum / numValues;

    return average;
}

namespace Ocnet {
    ProcessorType fromString(const juce::String& type)
    {
        if (type == "Filter") return Filter;
        if (type == "Distortion") return Distortion;
        if (type == "WavetableOscillator") return WavetableOscillator;
        if (type == "Envelope") return Envelope;
        if (type == "LFO") return LFO;
        if (type == "Sampler") return Sampler;
        if (type == "Reverb") return Reverb;
        if (type == "Macro") return Macro;
        if (type == "Randomizer") return Randomizer;
        if (type == "Equalizer") return Equalizer;
        if (type == "Delay") return Delay;
        return Unknown;
    }
    juce::String createSynthParameter(const juce::String& paramName)
    {
        return "Synth_-1_" + paramName;
    }
}