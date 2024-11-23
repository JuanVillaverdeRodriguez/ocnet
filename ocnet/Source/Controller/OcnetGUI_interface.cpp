/*
  ==============================================================================

    gui_interface.cpp
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetGUI_interface.h"

using namespace Ocnet;


OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor) : processor(*processor), keyboardState()
{
    maxCurrentID = 1;
    keyboardState.addListener(this);

    gui_ = std::make_unique<OcnetGUI>(*this, keyboardState);
    gui_->addSynthParams(processor->parameterHandler);
    gui_->attachSynthParams(processor->parameterHandler);



}

OcnetGUI_interface::~OcnetGUI_interface()
{
    keyboardState.removeListener(this);
}

void OcnetGUI_interface::onAddOscillator(int processorType)
{
    std::unique_ptr<Subsection>* subsection = gui_->getOscillatorsSection()->addOscillator(processorType, maxCurrentID, processor.parameterHandler);
    gui_->getOscillatorsSection()->updateOscillatorsFMCombo();
    gui_->getOscillatorsSection()->addAndMakeVisible(**subsection);
    subsection->get()->addParamsToParameterHandler(processor.parameterHandler);
    subsection->get()->attachParams(processor.parameterHandler);
    processor.addOscillator(processorType, maxCurrentID);
    maxCurrentID++;
}

void OcnetGUI_interface::onAddEffect(int processorType)
{
    std::unique_ptr<Subsection>* subsection = gui_->getEffectsSection()->addEffect(processorType, maxCurrentID, processor.parameterHandler);
    gui_->getEffectsSection()->addAndMakeVisible(**subsection);
    subsection->get()->addParamsToParameterHandler(processor.parameterHandler);
    subsection->get()->attachParams(processor.parameterHandler);
    processor.addEffect(processorType, maxCurrentID);
    maxCurrentID++;
}

void OcnetGUI_interface::onAddModulator(int processorType, int subMenuID)
{
    std::unique_ptr<Subsection>* subsection = gui_->getModulatorsSection()->addModulator(processorType, maxCurrentID, processor.parameterHandler, subMenuID);
    if (subsection != nullptr) {
        gui_->getModulatorsSection()->addAndMakeVisible(**subsection);
        subsection->get()->addParamsToParameterHandler(processor.parameterHandler);
        subsection->get()->attachParams(processor.parameterHandler);
        processor.addModulator(processorType, maxCurrentID, subMenuID);
        maxCurrentID++;
    }
}


void OcnetGUI_interface::onConnectModulation(Subsection& modulator, juce::String& parameterID, bool recreateParameters)
{

    auto modulatorCasted = dynamic_cast<ModulatorsSubsection*>(&modulator);
    juce::String modulationParameterID = modulatorCasted->getSubType() + juce::String("_") + juce::String(modulatorCasted->getId()) + juce::String("_modulationAmount_") + parameterID;

    // Comprobar no se este itentando modular el mismo parametro mas de una vez con el mismo modulador
    if (!modulatorCasted->isModulating(modulationParameterID)) {
        // 1º Crear la burbuja en la GUI
        std::unique_ptr<ModulationBubble>* modulationBubble = modulatorCasted->createModulationBubble(processor.parameterHandler, parameterID, *this);

        juce::String modulationID = modulationBubble->get()->getModulationID();

        // 2º Recrear los parametros si no estan en el arbol
        if (recreateParameters) {
            modulatorCasted->addModulationParameter(processor.parameterHandler, modulationID);
        }

        // 3º Attachear los parametros a los sliders
        modulatorCasted->attachModulationParameter(processor.parameterHandler, modulationID);

        // Se obtiene la referencia al parametro que se quiere modular
        std::shared_ptr<SliderParameter>* parameterToModulate = processor.parameterHandler.getSliderParameter(parameterID);

        // Se conecta con el processor
        processor.connectModulation(modulator.getId(), *parameterToModulate, modulationParameterID);
    }

}

void OcnetGUI_interface::onDeleteSubsection(Subsection& subsection)
{
    int id = subsection.getId();

    // No se puede eliminar el envelope principal.
    // Nunca deberia ocurrir esto, ya que el boton de eliminar esta desactivado para el mismo
    if (id == 0) {
        return;
    }

    juce::String type = subsection.getType();
    juce::String subType = subsection.getSubType();



    juce::Array<juce::String> subsectionProperties = processor.parameterHandler.getParameterIDs(subType, juce::String(id));

    // Primero eliminar las modulaciones de cada parametro
    for (auto property : subsectionProperties) {
        juce::Array<juce::String> modulationsID = processor.parameterHandler.getParameterModulationIDs(property);

        for (auto modulationID : modulationsID) {
            onRemoveModulation(modulationID);
        }
    }

    gui_->getSection(subsection.getType())->deleteSubsection(id);

    if (type == "Oscillator") {
        gui_->getOscillatorsSection()->updateOscillatorsFMCombo();
    }

    processor.deleteProcessor(id);


    processor.parameterHandler.deleteAttachedParameters(type, juce::String(id));
}

void OcnetGUI_interface::onMoveSubsection(Subsection& subsection, int positions)
{
    gui_->getSection(subsection.getType())->moveSubsection(subsection.getId(), positions);
    processor.moveProcessor(subsection.getId(), positions);
}

void OcnetGUI_interface::onBypassChanged(Subsection& subsection, bool toggled)
{
    subsection.setBypassed(toggled);
    processor.setBypassed(subsection.getId(), toggled);
}

void OcnetGUI_interface::onRemoveModulation(const juce::String& modulationID)
{
    DBG("ELIMINANDO MODULACION" + modulationID);
    processor.parameterHandler.removeModulationParameterWithID(modulationID);
    processor.removeModulation(modulationID);
    gui_->getModulatorsSection()->removeModulation(modulationID);
}

void OcnetGUI_interface::onFmFromChanged(const juce::String& carrierID, const juce::String& modulatorID)
{
    processor.setFmFrom(carrierID.getIntValue(), modulatorID.getIntValue());
}

void OcnetGUI_interface::onSamplerSampleChanged(int samplerID, const juce::String& parameterID, const juce::String& sampleName)
{
    processor.samplerSampleChanged(samplerID, sampleName);
    processor.parameterHandler.updateParameter(parameterID, sampleName);
    Subsection* subsection = gui_->getOscillatorsSection()->getSubsectionWithID(samplerID);
    if (subsection != nullptr) {
        juce::Array<float> samples = processor.getSamplerSampleSamples(samplerID);
        if (auto samplerSubsection = dynamic_cast<SamplerSubsection*>(subsection)) {
            samplerSubsection->updateWaveformView(samples);
        }
    }
        
}

void OcnetGUI_interface::initialiseGUIFromTree(juce::ValueTree tree)
{
    // Obtener arboles de moduladores, osciladores y efectos
    auto modulatorsTree = tree.getChildWithName("Modulators");
    auto oscillatorsTree = tree.getChildWithName("Oscillators");
    auto effectsTree = tree.getChildWithName("Effects");

    processor.parameterHandler.printValueTree(tree, 1);

    // Inicializar moduladores
    initialiseModulators(modulatorsTree);

    // Inicializar osciladores
    initialiseOscillators(oscillatorsTree);

    // Inicializar efectos
    initialiseEffects(effectsTree);

    // Actualizar el ID máximo actual
    maxCurrentID = processor.parameterHandler.getMaxCurrentID();
}

void OcnetGUI_interface::initialiseModulators(juce::ValueTree& modulatorsTree)
{
    for (int i = 0; modulatorsTree.getChild(i).isValid(); ++i)
    {
        auto type = modulatorsTree.getChild(i).getType().toString();
        auto subTree = modulatorsTree.getChild(i);

        for (int j = 0; subTree.getChild(j).isValid(); ++j)
        {
            auto id = subTree.getChild(j).getType().toString();
            std::unique_ptr<Subsection>* subsection;
            subsection = gui_->getModulatorsSection()->addModulator(fromString(type), id.getIntValue(), processor.parameterHandler);

            /*if (fromString(type) == Macro) {
                juce::Value macroID = subTree.getChild(j).getPropertyAsValue("macroID", nullptr);
                subsection = gui_->getModulatorsSection()->addModulator(fromString(type), id.getIntValue(), processor.parameterHandler, macroID.getValue().toString().getIntValue());
            }
            else 
                subsection = gui_->getModulatorsSection()->addModulator(fromString(type), id.getIntValue(), processor.parameterHandler);*/

            gui_->getModulatorsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);

            attachModulationParameters(subsection, subTree.getChild(j));
        }
    }
}

juce::Array<Knob1*> OcnetGUI_interface::getAllKnobs()
{
    juce::Array<Knob1*> knobs;

    knobs.addArray(gui_->getOscillatorsSection()->getAllKnobs());
    knobs.addArray(gui_->getModulatorsSection()->getAllKnobs());
    knobs.addArray(gui_->getEffectsSection()->getAllKnobs());

    return knobs;
}

void OcnetGUI_interface::initialiseOscillators(juce::ValueTree& oscillatorsTree)
{
    for (int i = 0; oscillatorsTree.getChild(i).isValid(); ++i)
    {
        auto type = oscillatorsTree.getChild(i).getType().toString();
        auto subTree = oscillatorsTree.getChild(i);

        for (int j = 0; subTree.getChild(j).isValid(); ++j)
        {
            auto id = subTree.getChild(j).getType().toString();
            auto subsection = gui_->getOscillatorsSection()->addOscillator(fromString(type), id.getIntValue(), processor.parameterHandler);

            gui_->getOscillatorsSection()->updateOscillatorsFMCombo();
            gui_->getOscillatorsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);
        }
    }
}

void OcnetGUI_interface::initialiseEffects(juce::ValueTree& effectsTree)
{
    for (int i = 0; effectsTree.getChild(i).isValid(); ++i)
    {
        auto type = effectsTree.getChild(i).getType().toString();
        auto subTree = effectsTree.getChild(i);

        for (int j = 0; subTree.getChild(j).isValid(); ++j)
        {
            auto id = subTree.getChild(j).getType().toString();
            auto subsection = gui_->getEffectsSection()->addEffect(fromString(type), id.getIntValue(), processor.parameterHandler);

            gui_->getEffectsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);
        }
    }
}

void OcnetGUI_interface::attachModulationParameters(std::unique_ptr<Subsection>* subsection, juce::ValueTree& subTree)
{
    for (int k = 0; k < subTree.getNumProperties(); ++k)
    {
        auto propertyIdentifier = subTree.getPropertyName(k);
        if (isModulationParameter(propertyIdentifier.toString()))
        {
            auto [modulationTag, parameterModulatingID] = Utils::splitParameterModulationID(propertyIdentifier.toString());
            auto modulatorCasted = dynamic_cast<ModulatorsSubsection*>(subsection->get());
            auto modulationBubble = modulatorCasted->createModulationBubble(processor.parameterHandler, parameterModulatingID, *this);

            modulatorCasted->attachModulationParameter(processor.parameterHandler, modulationBubble->get()->getModulationID());
        }
    }
}

void OcnetGUI_interface::editorIsShowing()
{
#if RUN_TESTS
    juce::UnitTestRunner runner;
    runner.runAllTests();
#endif

    gui_->editorIsShowing();
    initialiseGUIFromTree(processor.parameterHandler.getRootTree());
    processor.syncSynthParameters();

    // Si no esta añadido, añadir un envelope principal
    if (!processor.getHasMainEnvelope()) {
        maxCurrentID = 0;
        onAddModulator(Envelope, -1);
    }

    //gui_->addSynthParams(processor.parameterHandler);
    //gui_->attachSynthParams(processor.parameterHandler);
}

bool OcnetGUI_interface::synthHasMainEnvelope()
{
    return processor.getHasMainEnvelope();
}

bool OcnetGUI_interface::isModulationParameter(const juce::String& parameterTag)
{
    return parameterTag.startsWith("modulationAmount_");
}

OcnetGUI* OcnetGUI_interface::getGui() {
    return gui_.get(); 
}

// Implementar el callback para el teclado MIDI
void OcnetGUI_interface::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // Lógica cuando se presiona una tecla

    processor.noteOn(midiChannel, midiNoteNumber, velocity);
}

void OcnetGUI_interface::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // Lógica cuando se libera una tecla
    processor.noteOff(midiChannel, midiNoteNumber, velocity, true);
}

void OcnetGUI_interface::onDraggingModulationStarted(ModulatorsSubsection& modulator)
{
    juce::Array<Knob1*> knobs = getAllKnobs();
    for (auto& knob : knobs) {
        juce::String modulationParameterID = modulator.getSubType() + juce::String("_") + juce::String(modulator.getId()) + juce::String("_modulationAmount_") + knob->getParameterID();
        if (!modulator.isModulating(modulationParameterID))
            knob->showModulationTarget(true);
    }
}

void OcnetGUI_interface::onDraggingModulationEnded()
{
    juce::Array<Knob1*> knobs = getAllKnobs();
    for (auto& knob : knobs) {
        knob->showModulationTarget(false);
    }
}
