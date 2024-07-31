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

// Función genérica para copiar elementos de un array a otro
/*template<typename SourceType, typename DestinationType>
void Utils::copyArray(const SourceType& source, DestinationType& destination, int size) {
    for (int i = 0; i < size; ++i) {
        destination[i] = source[i];
    }
}*/

float* Utils::getArrayCopy(const juce::var& propertyModulations, int voice, int& size)
{
    // Inicializa el puntero a nullptr y el tamaño a 0.
    float* arrayCopy = nullptr;
    size = 0;

    // Asegúrate de que `propertyModulations` es un array de `juce::var`.
    if (auto* modulationArray = propertyModulations.getArray())
    {
        // Verifica que el índice `voice` esté dentro de los límites y que el elemento sea un array.
        if (voice < modulationArray->size() && modulationArray->getReference(voice).isArray())
        {
            // Obtén el `juce::Array<float>` correspondiente.
            const auto& subArray = *modulationArray->getReference(voice).getArray();
            size = subArray.size();

            // Reserva espacio para el array nativo y copia los elementos.
            arrayCopy = new float[size];
            for (int i = 0; i < size; ++i)
            {
                arrayCopy[i] = subArray[i];
            }
        }
    }

    // Retorna el puntero al array nativo. El tamaño del array se devuelve por referencia a `size`.
    return arrayCopy;
}