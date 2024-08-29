/*
  ==============================================================================

    Utils.h
    Created: 26 Jul 2024 12:59:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "OcnetTypes.h"

using namespace Ocnet;

namespace Ocnet {
    ProcessorType fromString(const juce::String& type);
    juce::String createSynthParameter(const juce::String& paramName);

}

class Utils {
public:
    static float linearToDecibels(float linearGain) {
        if (linearGain <= 0.0f)
            return -std::numeric_limits<float>::infinity();
        else
            return 20.0f * std::log10(linearGain);
    }

    /*
    Mueve en el vector el elemento en la posicion initIndex a la posicion (initIndex + positions)
        positions < 0: Desplazamiento hacia la izquierda.
        positions > 0: Desplazamiento hacia la derecha.
    */
    template<typename T>
    static void moveElement(std::vector<T>& elementsVector, int initIndex, int positions) {
        int listSize = elementsVector.size();

        if (initIndex < 0 || initIndex >= listSize || elementsVector.empty()) {
            DBG("Indice fuera de rango.");
            return;
        }

        if (positions < 0) {
            if (initIndex == 0) //El primer elemento no se puede mover mas para atras
                return;

            positions = -positions;
            // Mientras no se haya movido posiciones veces, o no haya llegado al inicio de la lista...
            for (int i = initIndex; i < positions + initIndex || (i - 1) == -1; i++) {
                std::iter_swap(elementsVector.begin() + (i), elementsVector.begin() + (i - 1));
            }
        }
        else {
            if (initIndex == listSize - 1) // El último elemento no se puede mover más para adelante
                return;

            std::rotate(elementsVector.begin() + (initIndex), elementsVector.begin() + (initIndex + 1), elementsVector.begin() + ((initIndex + 1) + positions));
        }
    }

    /*
    * Busca el elemento (Processor o Subsection) con dicho ID en la lista.
    * Devuelve el indice del elemento encontrado.
    */
    template<typename T>
    static int findElementPositionByID(const std::vector<T>& elementsVector, int id) {
        int initIndex = 0;
        for (auto& element : elementsVector) {
            if (element->getId() == id) {
                return initIndex;
            }
            initIndex++;
        }

        return -1;
    }

    /*
    * Busca el elemento (Processor o Subsection) con dicho ID en la lista.
    * Devuelve el elemento encontrado.
    */
    template<typename T>
    static T* findElementByID(std::vector<T>& elementsVector, int id) {
        for (auto& element : elementsVector) {
            if (element->getId() == id) {
                return &element;
            }
        }
        return nullptr;
    }

    static std::tuple<juce::String, juce::String, juce::String> splitParameterID(const juce::String& input);
    static std::tuple<juce::String, juce::String> splitParameterModulationID(const juce::String& input);


    static float average(float* values, int numValues, bool inAbsoluteValue = false, int loopIncrement = 1);

private:
    Utils();


    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Utils)


};