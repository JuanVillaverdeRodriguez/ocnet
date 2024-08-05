/*
  ==============================================================================

    Section.cpp
    Created: 5 Aug 2024 1:06:18am
    Author:  TecNo

  ==============================================================================
*/

#include "Section.h"

void Section::moveSubsection(int id, int positions)
{
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

    if (positions < 0) {
        positions = -positions;
        // Mientras no se haya movido posiciones veces, o no haya llegado al inicio de la lista...
        for (int i = 0; i < positions || (positions - i) == 1; i++) {
            std::rotate(subsectionsVector.begin() + (initIndex - i), subsectionsVector.begin() + ((initIndex - i) + 1), subsectionsVector.begin() + (positions - i));
        }
    }
    else {
        std::rotate(subsectionsVector.begin() + (initIndex), subsectionsVector.begin() + (initIndex + 1), subsectionsVector.begin() + ((initIndex + 1) + positions));
    }

    resized();
}

void Section::deleteSubsection(int id)
{
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
    auto area = getLocalBounds();

    int lastSubsectionPosition = 0;

    for (auto& wavetableOscillator : subsectionsVector) {
        wavetableOscillator->setBounds(0, lastSubsectionPosition + 5, area.getWidth(), 50);
        lastSubsectionPosition += 50;
    }

    return lastSubsectionPosition;
}
