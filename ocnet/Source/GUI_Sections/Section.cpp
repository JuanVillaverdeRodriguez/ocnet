/*
  ==============================================================================

    Section.cpp
    Created: 5 Aug 2024 1:06:18am
    Author:  TecNo

  ==============================================================================
*/

#include "Section.h"

std::vector<std::unique_ptr<Subsection>>* Section::getListOfSubsections()
{
    static std::vector<std::unique_ptr<Subsection>> emptyList;
    return &emptyList; // Devuelve un puntero a un vector estático vacío
}

void Section::moveSubsection(int id, int positions)
{
    auto& subsectionsVector = *getListOfSubsections();

    if (subsectionsVector.empty())
        return;

    int initIndex = 0;

    // Encuentra el elemento a mover
    for (auto& modulator : subsectionsVector) {
        if (modulator->getId() == id) {
            break;
        }
        initIndex++;
    }

    // Si positions es negativo, mover arriba (Izquierda en la lista)

    if (positions < 0) {
        if (initIndex == 0) //El primer elemento no se puede mover mas para atras
            return;

        positions = -positions;
        // Mientras no se haya movido posiciones veces, o no haya llegado al inicio de la lista...
        for (int i = initIndex; i < positions + initIndex || (i - 1) == -1; i++) {
            std::iter_swap(subsectionsVector.begin() + (i), subsectionsVector.begin() + (i - 1));
        }
    }
    else {
        if (initIndex == subsectionsVector.size()-1) //El ultimo elemento no se puede mover mas para adelante
            return;
        std::rotate(subsectionsVector.begin() + (initIndex), subsectionsVector.begin() + (initIndex + 1), subsectionsVector.begin() + ((initIndex + 1) + positions));
    }

    resized();
}

void Section::deleteSubsection(int id)
{
    auto& subsectionsVector = *getListOfSubsections();

    if (subsectionsVector.empty())
        return;

    subsectionsVector.erase(
        std::remove_if(subsectionsVector.begin(), subsectionsVector.end(),
            [&id](const std::unique_ptr<Subsection>& subsection) {
                return subsection->getId() == id;
            }
        ),
        subsectionsVector.end()
    );

    resized();
}

int Section::sectionResized()
{
    auto& subsectionsVector = *getListOfSubsections();

    auto area = getLocalBounds();

    int lastSubsectionPosition = 0;

    for (auto& wavetableOscillator : subsectionsVector) {
        wavetableOscillator->setBounds(0, lastSubsectionPosition + 5, area.getWidth(), 50);
        lastSubsectionPosition += 50;
    }

    return lastSubsectionPosition;
}
