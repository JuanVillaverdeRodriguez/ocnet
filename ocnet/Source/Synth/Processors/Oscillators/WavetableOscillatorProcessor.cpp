/*
  ==============================================================================

    WavetableOscillatorProcessor.cpp
    Created: 15 Jul 2024 5:40:22pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorProcessor.h"
#include "../Source/Utils/Utils.h"

WavetableOscillatorProcessor::WavetableOscillatorProcessor(int id)
{
    setId(id);
    currentFrequency = 0.0f;

    waveTypeIndexChoice = 0;

    sawWaveTables = createWaveTables(2048, juce::String("Saw"));
    squareWaveTables = createWaveTables(2048, juce::String("Square"));
    sineWaveTables = createWaveTables(2048, juce::String("Sine"));

    tables = &squareWaveTables;

    wavetable = &(*tables)[9];
    tableSize = (*tables)[0].waveTable.getNumSamples() - 1;

    //tables = createWaveTables(2048, juce::String("Square"));
    //tables = createWaveTables(2048, juce::String("Sine"));

    cnt = 0;
    //jassert(wavetable->waveTable.getNumChannels() == 1); // Asegúrate de que la wavetable sea mono
    oscGain = 1.0f;

    isPrepared = false;
    sampleRate = 0.0f;
    currentIndex = 0.0f;
    tableDelta = 0.0f;
    numWavetables = (*tables).size();
    DBG("TAMANO: " + juce::String(numWavetables));

}

void WavetableOscillatorProcessor::setFrequency(float frequency, float sampleRate)
{
    currentFrequency = frequency;  // Almacenar la frecuencia actual

    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;

    // Coger la wavetable adecuada
    int waveTableIdx = 0;
    float frequencyOverSamplerate = frequency / sampleRate;

    while ((frequencyOverSamplerate >= (*tables)[waveTableIdx].topFreq) && (waveTableIdx < (numWavetables - 1))) {
        ++waveTableIdx;
    }

    wavetable = &(*tables)[waveTableIdx];
}

void WavetableOscillatorProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    gainParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_volume"));
    waveTypeParameter = parameterHandler.syncWithComboBoxParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_waveType"));
}

float WavetableOscillatorProcessor::getNextSample(int sample)
{
    float newGainValue = oscGain + oscGainModulationBuffer[sample];

    if (newGainValue < 0.0f)
        newGainValue = 0.0f;

    gain.setGainLinear(newGainValue);

    auto index0 = (unsigned int)currentIndex;
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float)index0;

    auto* table = wavetable->waveTable.getReadPointer(0); // Obtener puntero de lectura a la wavetable
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0);

    if ((currentIndex += tableDelta) >= (float)tableSize)
        currentIndex -= (float)tableSize;

    currentSample *= gain.getGainLinear();

    return currentSample;
}

void WavetableOscillatorProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    setFrequency(static_cast<float>(frequency), sampleRate);
}

void WavetableOscillatorProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void WavetableOscillatorProcessor::updateParameterValues()
{
    oscGain = gainParameter->getValue();
    oscGainModulationBuffer = gainParameter->getModulationBuffer(getVoiceNumberId());
    waveTypeIndexChoice = waveTypeParameter->getCurrentIndex();

    if (waveTypeIndexChoice == 0)
        tables = &sawWaveTables;
    else if (waveTypeIndexChoice == 1)
        tables = &squareWaveTables;
    else if (waveTypeIndexChoice == 2)
        tables = &sineWaveTables;

    setFrequency(currentFrequency, sampleRate);

    //DBG(juce::String("TIPO DE ONDA SELECIONADA: ") + juce::String(waveTypeIndexChoice));
}

void WavetableOscillatorProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    gain.prepare(spec);
    DBG(juce::String(spec.sampleRate));
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

WavetableStruct WavetableOscillatorProcessor::makeWaveTable(int tableSize, std::unique_ptr<double[]>& ar, std::unique_ptr<double[]>& ai, double topFreq) {
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

std::vector<WavetableStruct> WavetableOscillatorProcessor::fillWavetables(std::unique_ptr<double[]>& freqWaveRe, std::unique_ptr<double[]>& freqWaveIm, int tableSize) {
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
    DBG("MAX HARMONIC: " + juce::String(maxHarmonic));
    double topFreq = 2.0 / 3.0 / maxHarmonic;
    DBG("MAX HARMONIC: " + juce::String(topFreq));


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
        topFreq *= 2;
        maxHarmonic >>= 1;
        // 1024, 512, 256...

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

std::vector<WavetableStruct> WavetableOscillatorProcessor::createWaveTables(int tableSize, const juce::String& waveType) {
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
    else if(waveType == "Square")
        square(freqWaveRe, tableSize);

    // Sine
    else if (waveType == "Sine")
        square(freqWaveRe, tableSize);

    wavetablesStructs = fillWavetables(freqWaveRe, freqWaveIm, tableSize);

    return wavetablesStructs;

}

void WavetableOscillatorProcessor::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    int numSamples = outputBuffer.getNumSamples();
    int numChannels = outputBuffer.getNumChannels();

    auto* firstChannelBuffer = outputBuffer.getWritePointer(0);

    for (int sample = 0; sample < numSamples; ++sample) {
        firstChannelBuffer[sample] += getNextSample(sample);
    }

    // Copiar el contenido del primer canal a los otros canales
    for (int channel = 1; channel < numChannels; ++channel) {
        auto* buffer = outputBuffer.getWritePointer(channel);

        // Copiar los valores del primer canal a este canal
        for (int sample = 0; sample < numSamples; ++sample) {
            buffer[sample] = firstChannelBuffer[sample];
        }
    }

}
