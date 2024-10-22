/*
  ==============================================================================

    WavetableOscillatorProcessor.cpp
    Created: 15 Jul 2024 5:40:22pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorProcessor.h"
#include "../Source/Utils/Utils.h"

WavetableOscillatorProcessor::WavetableOscillatorProcessor(int id, LegatoInfo& legatoInfo)
    : unisonVoices(8), detuneValue(0.20f), unisonSpread(0.15f), gen(rd()),
    maxUnisonDetuning(10), maxUnisonSpread(10), maxUnisonVoices(8), fmMod(1), fmModulatorOsc(nullptr), 
    currentFMIndex(0.0f), transpose(0), lastTableChoice(0), legato(true), legatoInfo(legatoInfo), deltaPreviousNoteCurrentNoteInHertz(0.0f), prevNotePressedInHertz(0.0f), legatoDeltaCents(1), targetFrequency(0.0f), currentMidiNoteNumber(0)

{
    setId(id);
    currentFrequency = 0.0f;

    waveTypeIndexChoice = 0;

    sawWaveTables = createWaveTables(4096, juce::String("Saw"));
    squareWaveTables = createWaveTables(4096, juce::String("Square"));
    sineWaveTables = createWaveTables(4096, juce::String("Sine"));

    tables = &sawWaveTables;

    wavetable = &(*tables)[9];
    tableSize = (*tables)[0].waveTable.getNumSamples() - 1;
    phaseRandomnes = std::uniform_real_distribution<>(0, tableSize);

    isPrepared = false;
    sampleRate = 0.0f;
    tableDelta = 0.0f;
    numWavetables = (*tables).size();

    for (int i = 0; i < 8; ++i)
    {
        float newPhase = 0.0f + phaseRandomnes(gen);
        // Asegurarse de que newPhase no se pasa del tamaño de la wavetable
        while (newPhase >= tableSize) {
            newPhase = 0.0f + phaseRandomnes(gen);
        }
        float* value = new float(newPhase);

        unisonVoiceCurrentIndexArray.add(value);
    }

    unisonDetuneArray.set(1, 2);
    unisonDetuneArray.set(2, 0);
    unisonDetuneArray.set(3, -1);
    unisonDetuneArray.set(4, 1);
    unisonDetuneArray.set(5, -1.5);
    unisonDetuneArray.set(6, 1.5);
    unisonDetuneArray.set(7, -0.25);

    unisonSpreadArrayL.set(0, std::cos(0));
    unisonSpreadArrayL.set(1, std::cos(1));
    unisonSpreadArrayL.set(2, std::cos(0.5));
    unisonSpreadArrayL.set(3, std::cos(0.60));
    unisonSpreadArrayL.set(4, std::cos(0.85));
    unisonSpreadArrayL.set(5, std::cos(0.475));
    unisonSpreadArrayL.set(6, std::cos(0.625));
    unisonSpreadArrayL.set(7, std::cos(0.5375));

    unisonSpreadArrayR.set(0, std::sin(0));
    unisonSpreadArrayR.set(1, std::sin(1));
    unisonSpreadArrayR.set(2, std::sin(0.5));
    unisonSpreadArrayR.set(3, std::sin(0.60));
    unisonSpreadArrayR.set(4, std::sin(0.85));
    unisonSpreadArrayR.set(5, std::sin(0.475));
    unisonSpreadArrayR.set(6, std::sin(0.625));
    unisonSpreadArrayR.set(7, std::sin(0.5375));

    // Inicialización de unisonDetuneArray, unisonSpreadArrayL y R...
}


WavetableOscillatorProcessor::~WavetableOscillatorProcessor()
{
    for (float* ptr : unisonVoiceCurrentIndexArray)
    {
        delete ptr;
    }
}

float WavetableOscillatorProcessor::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;

    // Coger la wavetable adecuada
    int waveTableIdx = 0;
    float frequencyOverSamplerate = frequency / sampleRate;

    while ((frequencyOverSamplerate >= (*tables)[waveTableIdx].topFreq) && (waveTableIdx < (numWavetables - 1))) {
        ++waveTableIdx;
    }

    //DBG("TABLE INDEX: " + juce::String(waveTableIdx));
    wavetable = &(*tables)[waveTableIdx];

    return tableDelta;
}

void WavetableOscillatorProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    gainParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_volume"));
    panningParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_panning"));

    waveTypeParameter = parameterHandler.syncWithComboBoxParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_waveType"));
    unisonDetuneParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_unisonDetuneAmount"));
    unisonNumVoicesParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_unisonNumVoices"));
    fmAmountParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_fmAmount"));
    transposeParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_transpose"));
    fmFromParameter = parameterHandler.syncWithComboBoxParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_fmFrom"));
}

void WavetableOscillatorProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    currentMidiNoteNumber = midiNoteNumber;

    double frequency = 0.0f;
    float currentNotePressedInHertz = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber + transpose);

    if (legatoInfo.legatoIsActive) {
        targetFrequency = currentNotePressedInHertz;
        prevNotePressedInHertz = juce::MidiMessage::getMidiNoteInHertz(legatoInfo.previousMidiNotePressed);
        prevNotePressedInHertz = legatoInfo.previousLegatoFreq;
        // Si no hay ninguna nota anterior o si no se esta tocando simultaneamente, no se aplica el legato.
        if (prevNotePressedInHertz == 0.0f || !legatoInfo.previousNoteIsBeingPlayed)
            frequency = currentNotePressedInHertz;
        else
            frequency = prevNotePressedInHertz;
    }
    else {
        frequency = currentNotePressedInHertz;
    }
    currentFrequency = frequency;  // Almacenar la frecuencia actual

    legatoInfo.previousNoteIsBeingPlayed = true;
    legatoInfo.previousMidiNotePressed = midiNoteNumber;
    legatoInfo.previousLegatoFreq = frequency;
    setFrequency(static_cast<float>(frequency), sampleRate);
}

void WavetableOscillatorProcessor::stopNote(float velocity, bool allowTailOff)
{

}

void WavetableOscillatorProcessor::updateParameterValues()
{
    gainValue.setTargetValue(gainParameter->getModulatedValue(getVoiceNumberId()));
    panValue.setTargetValue(panningParameter->getModulatedValue(getVoiceNumberId()));
    detuneValue.setTargetValue(unisonDetuneParameter->getModulatedValue(getVoiceNumberId()));
    fmAmountValue.setTargetValue(fmAmountParameter->getModulatedValue(getVoiceNumberId()));
    unisonVoices = unisonNumVoicesParameter->getModulatedValue(getVoiceNumberId(), 0, 8);
    fmFromIndexChoice = fmFromParameter->getCurrentIndex();
    waveTypeIndexChoice = waveTypeParameter->getCurrentIndex();
    transpose = transposeParameter->getModulatedValue(getVoiceNumberId());

    if (waveTypeIndexChoice == 0) {
        tables = &sawWaveTables;
    }
    else if (waveTypeIndexChoice == 1) {
        tables = &squareWaveTables;
    }
    else if (waveTypeIndexChoice == 2) {
        tables = &sineWaveTables;
    }

    if (lastTableChoice != waveTypeIndexChoice) {
        setFrequency(currentFrequency, sampleRate);
        lastTableChoice = waveTypeIndexChoice;
    }
}

void WavetableOscillatorProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    gain.prepare(spec);
    gainValue.reset(sampleRate, 0.0005);
    panValue.reset(sampleRate, 0.0005);
    fmAmountValue.reset(sampleRate, 0.0005);
    detuneValue.reset(sampleRate, 0.0005);
    DBG(juce::String(spec.sampleRate));
}

float WavetableOscillatorProcessor::getNextSample(int sample)
{
    return 0.0f;
}

void fft(int N, std::unique_ptr<double[]>& ar, std::unique_ptr<double[]>& ai)
/*
 in-place complex fft

 After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)

 program adapted from FORTRAN
 by K. Steiglitz  (ken@princeton.edu)
 Computer Science Dept.
 Princeton University 08544          */
{
    int i, j, k, L;            /* indexes */
    int M, TEMP, LE, LE1, ip;  /* M = log N */
    int NV2, NM1;
    double t;               /* temp */
    double Ur, Ui, Wr, Wi, Tr, Ti;
    double Ur_old;

    // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2

    NV2 = N >> 1;
    NM1 = N - 1;
    TEMP = N; /* get M = log N */
    M = 0;
    while (TEMP >>= 1) ++M;

    /* shuffle */
    j = 1;
    for (i = 1; i <= NM1; i++) {
        if (i < j) {             /* swap a[i] and a[j] */
            t = ar[j - 1];
            ar[j - 1] = ar[i - 1];
            ar[i - 1] = t;
            t = ai[j - 1];
            ai[j - 1] = ai[i - 1];
            ai[i - 1] = t;
        }

        k = NV2;             /* bit-reversed counter */
        while (k < j) {
            j -= k;
            k /= 2;
        }

        j += k;
    }

    LE = 1.;
    for (L = 1; L <= M; L++) {            // stage L
        LE1 = LE;                         // (LE1 = LE/2) 
        LE *= 2;                          // (LE = 2^L)
        Ur = 1.0;
        Ui = 0.;
        Wr = cos(M_PI / (float)LE1);
        Wi = -sin(M_PI / (float)LE1); // Cooley, Lewis, and Welch have "+" here
        for (j = 1; j <= LE1; j++) {
            for (i = j; i <= N; i += LE) { // butterfly
                ip = i + LE1;
                Tr = ar[ip - 1] * Ur - ai[ip - 1] * Ui;
                Ti = ar[ip - 1] * Ui + ai[ip - 1] * Ur;
                ar[ip - 1] = ar[i - 1] - Tr;
                ai[ip - 1] = ai[i - 1] - Ti;
                ar[i - 1] = ar[i - 1] + Tr;
                ai[i - 1] = ai[i - 1] + Ti;
            }
            Ur_old = Ur;
            Ur = Ur_old * Wr - Ui * Wi;
            Ui = Ur_old * Wi + Ui * Wr;
        }
    }
}



//
// if scale is 0, auto-scales
// returns scaling factor (0.0 if failure), and wavetable in ai array
//

WavetableOscillatorProcessor::WavetableStruct WavetableOscillatorProcessor::makeWaveTable(int tableSize, std::unique_ptr<double[]>& ar, std::unique_ptr<double[]>& ai, double topFreq) {
    fft(tableSize, ar, ai);

    juce::AudioSampleBuffer table;
    table.setSize(1, (int)tableSize + 1);
    table.clear();

    auto* samples = table.getWritePointer(0);

    double maxAmplitude = 0.0;

    for (int idx = 0; idx < tableSize; idx++) {
        samples[idx] = ai[idx];

        // Obtener la amplitud maxima (en valor absoluto) para normalizacion
        if (fabs(ai[idx]) > maxAmplitude) {
            maxAmplitude = fabs(ai[idx]);
        }
    }

    // Normalizar los valores para que estén en el rango -1 a 1
    if (maxAmplitude > 0.0) {
        for (int idx = 0; idx < tableSize; idx++) {
            samples[idx] /= maxAmplitude;
        }
    }

    //El ultimo indice (tableSize+1) se usa para hacer mas eficiente el calculo del wraping al final de cada ciclo de onda (Se ahorran comparaciones)
    samples[tableSize] = samples[0];

    WavetableStruct wavetableStruct;
    wavetableStruct.topFreq = topFreq;
    wavetableStruct.waveTable = table;

    return wavetableStruct;
}

std::vector<WavetableOscillatorProcessor::WavetableStruct> WavetableOscillatorProcessor::fillWavetables(std::unique_ptr<double[]>& freqWaveRe, std::unique_ptr<double[]>& freqWaveIm, int tableSize) {
    std::vector<WavetableStruct> wavetablesStructs;

    int idx;

    // zero DC offset and Nyquist
    freqWaveRe[0] = freqWaveIm[0] = 0.0;
    freqWaveRe[tableSize >> 1] = freqWaveIm[tableSize >> 1] = 0.0;

    // El numero de tablas resultante será log2(tablseSize), por lo que maxHarmonic se calculara log2(tablseSize) veces, (mirar whileLoop)
    // Determinar maxHarmonic, el mayor harmonico distinto de 0 en la onda.
    int maxHarmonic = tableSize >> 1; //Divide a la mitad tablesize
    const double minVal = 0.000001; // -120 dB
    while ((fabs(freqWaveRe[maxHarmonic]) + fabs(freqWaveIm[maxHarmonic]) < minVal) && maxHarmonic)
        --maxHarmonic;

    // calculate topFreq for the initial wavetable
    // maximum non-aliasing playback rate is 1 / (2 * maxHarmonic), but we allow
    // aliasing up to the point where the aliased harmonic would meet the next
    // octave table, which is an additional 1/3

    // Calculo de la topFreq para la tabla de ondas inicial (indice 0)
    //DBG("MAX HARMONIC: " + juce::String(maxHarmonic));
    double topFreq = 2.0 / 3.0 / maxHarmonic;
    //DBG("MAX HARMONIC: " + juce::String(topFreq));


    // for subsquent tables, double topFreq and remove upper half of harmonics
    auto ar = std::make_unique<double[]>(tableSize);
    auto ai = std::make_unique<double[]>(tableSize);

    // Ensure arrays are initialized
    std::fill(ar.get(), ar.get() + tableSize, 0.0);
    std::fill(ai.get(), ai.get() + tableSize, 0.0);

    //double scale = 0.0; Se podria optimizar el espacio que usa si cada wavetable tuviera una numero de samples variable
    //Tablas para freuencias altas no necesitan tantos samples
    //Frecuencias bajas necesitan mas

    // Mientras maxHarmonic sea > 0
    while (maxHarmonic) {
        // fill the table in with the needed harmonics
        for (idx = 0; idx < tableSize; idx++)
            ar[idx] = ai[idx] = 0.0;
        for (idx = 1; idx <= maxHarmonic; idx++) {
            ar[idx] = freqWaveRe[idx];
            ai[idx] = freqWaveIm[idx];
            ar[tableSize - idx] = freqWaveRe[tableSize - idx];
            ai[tableSize - idx] = freqWaveIm[tableSize - idx];
        }

        // make the wavetable
        WavetableStruct wavetableStruct = makeWaveTable(tableSize, ar, ai, topFreq);

        wavetablesStructs.push_back(wavetableStruct);

        // prepare for next table
        topFreq *= 1.5;
        //maxHarmonic >>= 1; // 1024, 512, 256...
        maxHarmonic = static_cast<int>(maxHarmonic / 1.5);
    }

    return wavetablesStructs;
}

inline void saw(std::unique_ptr<double[]>& freqWaveRe, int tableSize) {
    freqWaveRe[0] = freqWaveRe[tableSize >> 1] = 0.0;
    for (int idx = 1; idx < (tableSize >> 1); idx++) {
        freqWaveRe[idx] = 1.0 / idx;                    // sawtooth spectrum
        freqWaveRe[tableSize - idx] = -freqWaveRe[idx];  // mirror
    }
}

inline void square(std::unique_ptr<double[]>& freqWaveRe, int tableSize) {
    freqWaveRe[0] = freqWaveRe[tableSize >> 1] = 0.0;
    for (int idx = 1; idx < (tableSize >> 1); idx++) {
        if (idx % 2 != 0) { // Solo los índices impares
            freqWaveRe[idx] = 1.0 / idx;                    // espectro de onda cuadrada
            freqWaveRe[tableSize - idx] = -freqWaveRe[idx]; // espejo
        }
        else {
            freqWaveRe[idx] = 0.0;                          // los índices pares son 0
            freqWaveRe[tableSize - idx] = 0.0;              // los índices pares son 0
        }
    }
}

inline void sine(std::unique_ptr<double[]>& freqWaveRe, int tableSize) {
    // La onda seno solo tiene un único componente no nulo en la frecuencia fundamental (idx = 1).
    freqWaveRe[0] = freqWaveRe[tableSize >> 1] = 0.0;
    for (int idx = 1; idx < (tableSize >> 1); idx++) {
        if (idx == 1) {
            freqWaveRe[idx] = 1.0; // Componente fundamental
            freqWaveRe[tableSize - idx] = -freqWaveRe[idx]; // Espejo para mantener simetría
        } else {
            freqWaveRe[idx] = 0.0; // Otros armónicos son 0 en el seno
            freqWaveRe[tableSize - idx] = 0.0;
        }
    }
}

std::vector<WavetableOscillatorProcessor::WavetableStruct> WavetableOscillatorProcessor::createWaveTables(int tableSize, const juce::String& waveType) {
    std::vector<WavetableStruct> wavetablesStructs;

    auto freqWaveRe = std::make_unique<double[]>(tableSize);
    auto freqWaveIm = std::make_unique<double[]>(tableSize);

    // Crear parte imaginaria
    for (int idx = 0; idx < tableSize; idx++) {
        freqWaveIm[idx] = 0.0;
    }

    // Saw
    if (waveType == "Saw")
        saw(freqWaveRe, tableSize);

    // Square
    else if (waveType == "Square")
        square(freqWaveRe, tableSize);

    // Sine
    else if (waveType == "Sine")
        sine(freqWaveRe, tableSize);

    wavetablesStructs = fillWavetables(freqWaveRe, freqWaveIm, tableSize);

    return wavetablesStructs;

}

void WavetableOscillatorProcessor::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    if (legatoInfo.legatoIsActive)
        updateLegato();

    int numSamples = outputBuffer.getNumSamples();

    auto* leftChannelBuffer = outputBuffer.getWritePointer(0);
    auto* rightChannelBuffer = outputBuffer.getWritePointer(1);

    float finalDelta = 0.0f;
    float gainLeft;
    float gainRight;

    for (int unisonVoice = 0; unisonVoice < unisonVoices; unisonVoice++) {
        for (int sample = 0; sample < numSamples; ++sample) {
            float newVoiceDelta = unisonVoices == 1 ? tableDelta : getUnisonDeltaFromFrequency(freqRelativeTo(currentFrequency, unisonDetuneArray[unisonVoice] * detuneValue.getNextValue(), false), sampleRate);
            float* newCurrentIndex = unisonVoiceCurrentIndexArray[unisonVoice];

            float globalPanAngle = (panValue.getNextValue() * juce::MathConstants<float>::halfPi);
            float globalPanningLeft = std::cos(globalPanAngle);
            float globalPanningRight = std::sin(globalPanAngle);

            float newGainValue = gainValue.getNextValue();

            if (unisonVoices > 1) {
                gainLeft = globalPanningLeft * newGainValue * unisonSpreadArrayL[unisonVoice];
                gainRight = globalPanningRight * newGainValue * unisonSpreadArrayR[unisonVoice];
            }
            else {
                gainLeft = globalPanningLeft * newGainValue;
                gainRight = globalPanningRight * newGainValue;
            }

            if (fmModulatorOsc != nullptr) {
                float fmValue = fmModulatorOsc->get()->getNextFMValue();
                float newFreq = freqRelativeTo(currentFrequency, fmAmountValue.getNextValue() * 1200 * fmValue, true);
                if (newFreq < 0.0f)
                    newFreq = 0.0f;
                finalDelta = unisonVoices == 1 ? setFrequency(newFreq, sampleRate) : newVoiceDelta + setFrequency(newFreq, sampleRate);
            }
            else {
                finalDelta = newVoiceDelta;
            }

            float nextSample = getNextSample(finalDelta, newCurrentIndex);

            leftChannelBuffer[sample] += nextSample * gainLeft;
            rightChannelBuffer[sample] += nextSample * gainRight;
        }
    }
}

void WavetableOscillatorProcessor::processBlockTest(juce::AudioBuffer<float>& outputBuffer)
{

}

void WavetableOscillatorProcessor::setFmFrom(std::unique_ptr<Processor>* modulator)
{
    fmModulatorOsc = modulator;
}

float WavetableOscillatorProcessor::getNextFMValue()
{
    const auto index0 = (unsigned int)currentFMIndex;
    const auto index1 = index0 + 1;

    const auto frac = currentFMIndex - (float)index0;

    const auto* table = wavetable->waveTable.getReadPointer(0); // Obtener puntero de lectura a la wavetable
    const auto value0 = table[index0];
    const auto value1 = table[index1];

    const auto currentSample = value0 + frac * (value1 - value0);

    if ((currentFMIndex += tableDelta) >= (float)tableSize)
        currentFMIndex -= (float)tableSize;

    return currentSample;
}

float WavetableOscillatorProcessor::getCurrentFreq()
{
    return currentFrequency;
}

void WavetableOscillatorProcessor::updateLegato()
{
    if (targetFrequency < currentFrequency) {
        float newFreq = freqRelativeTo(currentFrequency, -legatoDeltaCents * (1/legatoInfo.glideValue), true);

        if (newFreq > targetFrequency) {
            currentFrequency = newFreq;  // Almacenar la frecuencia actual
        }
        else {
            currentFrequency = targetFrequency;  // Almacenar la frecuencia actual
        }
        setFrequency(static_cast<float>(currentFrequency), sampleRate);
    }
    else if (targetFrequency > currentFrequency) {
        float newFreq = freqRelativeTo(currentFrequency, legatoDeltaCents * (1 / legatoInfo.glideValue), true);

        if (newFreq < targetFrequency) {
            currentFrequency = newFreq;  // Almacenar la frecuencia actual
        }
        else {
            currentFrequency = targetFrequency;  // Almacenar la frecuencia actual
        }
        setFrequency(static_cast<float>(currentFrequency), sampleRate);

    }

    legatoInfo.previousLegatoFreq = currentFrequency;
}


float WavetableOscillatorProcessor::getNextSample(const float newTableDelta, float* newCurrentIndex)
{
    const auto index0 = (unsigned int)*newCurrentIndex;
    const auto index1 = index0 + 1;

    const auto frac = *newCurrentIndex - (float)index0;

    const auto* table = wavetable->waveTable.getReadPointer(0); // Obtener puntero de lectura a la wavetable
    const auto value0 = table[index0];
    const auto value1 = table[index1];

    const auto currentSample = value0 + frac * (value1 - value0);

    if ((*newCurrentIndex += newTableDelta) >= (float)tableSize)
        *newCurrentIndex -= (float)tableSize;

    return currentSample;
}

void WavetableOscillatorProcessor::oldWay(juce::AudioBuffer<float>& buffer)
{
}

void WavetableOscillatorProcessor::newWay(juce::AudioBuffer<float>& buffer)
{
}

float WavetableOscillatorProcessor::getUnisonDeltaFromFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    float newTableDelta = frequency * tableSizeOverSampleRate;

    // Coger la wavetable adecuada
    /*int waveTableIdx = 0;
    float frequencyOverSamplerate = frequency / sampleRate;

    while ((frequencyOverSamplerate >= (*tables)[waveTableIdx].topFreq) && (waveTableIdx < (numWavetables - 1))) {
        ++waveTableIdx;
    }

    wavetable = &(*tables)[waveTableIdx];*/
    return newTableDelta;
}

float WavetableOscillatorProcessor::freqRelativeTo(float relativeFreq, float notes, bool cents)
{
    if (cents)
        return relativeFreq * std::pow(2.0, (notes / 1200.0));
    else 
        return relativeFreq * std::pow(2.0, (notes / 12.0));
}

void WavetableOscillatorProcessor::updateTablePointer()
{
}

void WavetableOscillatorProcessor::setFrequencyWithoutAntiAlias(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;
}
