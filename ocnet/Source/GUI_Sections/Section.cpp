/*
  ==============================================================================

    Section.cpp
    Created: 5 Aug 2024 1:06:18am
    Author:  TecNo

  ==============================================================================
*/

#include "Section.h"
#include "../Utils/Utils.h"

std::vector<std::unique_ptr<Subsection>>* Section::getListOfSubsections()
{
    static std::vector<std::unique_ptr<Subsection>> emptyList;
    return &emptyList; // Devuelve un puntero a un vector estático vacío
}

void Section::moveSubsection(int id, int positions)
{
    // Obten la lista de subsecciones
    auto& subsectionsVector = *getListOfSubsections();

    // Encontrar la posicion en la lista
    int initIndex = Utils::findElementPositionByID(subsectionsVector, id);

    // Mueve el elemento
    Utils::moveElement(subsectionsVector, initIndex, positions);

    // Actualiza la section
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

    int lastSubsectionPosition = 5;

    for (auto& wavetableOscillator : subsectionsVector) {
        wavetableOscillator->setBounds(5, lastSubsectionPosition, area.getWidth()-10, wavetableOscillator->getDesiredHeight());
        lastSubsectionPosition += wavetableOscillator->getHeight()+5;
    }

    return lastSubsectionPosition;
}

Subsection* Section::getSubsectionWithID(int id)
{
    auto& subsectionsVector = *getListOfSubsections();
    return Utils::findElementByID(subsectionsVector, id)->get();
}