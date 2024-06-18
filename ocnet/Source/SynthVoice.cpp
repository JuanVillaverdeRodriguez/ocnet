/*
  ==============================================================================

    SynthVoice.cpp
    Created: 2 May 2024 1:45:23am
    Author:  TecNo

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    //oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber), true);
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    wavetableOsc.setFrequency(static_cast<float>(frequency), getSampleRate());

    adsr.noteOn();

}
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared); // No deberia ser necesario, JUCE ya lo deberia comprobar, pero por si acaso

    if (!isVoiceActive()) // Si no hay voces, no devuelve nada
        return;

    synthBuffer.setSize(1, numSamples, false, false, true);
    synthBuffer.clear();


    for (int channel = 0; channel < 1; ++channel) {
        auto* buffer = synthBuffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {

            float currentSample = wavetableOsc.getNextSample();

            // Aplicar la envolvente ADSR a cada muestra
            float envValue = adsr.getNextSample();
            currentSample *= envValue;

            buffer[sample] = currentSample;
        }
    }

    //Habria que cambiar todo esto a esto, que cada processor procese
    /*for each (Processor processor in ProcessorList) {
        juce::dsp::AudioBlock<float> audioBLock{ synthBuffer };
        juce::dsp::ProcessContextReplacing<float> processContext{ audioBLock };

        processor.process(processContext);
    }*/

    juce::dsp::AudioBlock<float> audioBLock { synthBuffer };
    juce::dsp::ProcessContextReplacing<float> processContext { audioBLock };

    gain.process(processContext);

    // Añadir synthBuffer al outputBuffer
    for (int channel = 0; channel < 1; ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
    }

    // Si la envolvente ADSR ha terminado, limpiar la nota actual
    if (!adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    DBG("PREPARE TO PLAY");

    juce::dsp::ProcessSpec spec;


    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = outputChannels;

    spec.sampleRate = sampleRate;

    adsr.setSampleRate(sampleRate);



    gain.prepare(spec);

    gain.setGainDecibels(-48.0f);

    isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    adsr.setParameters(adsrParams);
}

/*juce::AudioSampleBuffer SynthVoice::createWaveTable(int tableSize)
{
    juce::AudioSampleBuffer sineTable(1, tableSize);

    auto* samples = sineTable.getWritePointer(0);                                   // [3]

    auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1); // [4]
    auto currentAngle = 0.0;

    for (unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin(currentAngle);                                       // [5]
        samples[i] = (float)sample;
        currentAngle += angleDelta;
    }

    return sineTable;
}*/

juce::AudioSampleBuffer SynthVoice::createWaveTable(int tableSize)
{
    juce::AudioSampleBuffer sineTable;
    sineTable.setSize(1, (int)tableSize + 1);
    sineTable.clear();

    auto* samples = sineTable.getWritePointer(0);



    int harmonics[] = { 1, 3, 5, 6, 7, 9, 11, 15 };
    float harmonicWeights[] = { 0.5f, 0.1f, 0.05f, 0.125f, 0.09f, 0.005f, 0.002f, 0.001f }; // [1]

    jassert(juce::numElementsInArray(harmonics) == juce::numElementsInArray(harmonicWeights));

    for (auto harmonic = 0; harmonic < juce::numElementsInArray(harmonics); ++harmonic)
    {
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * harmonics[harmonic]; // [2]
        auto currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] += (float)sample * harmonicWeights[harmonic]; // [3]
            currentAngle += angleDelta;
        }
    }

    samples[tableSize] = samples[0];
    DBG("NUMERO DE CHANELS: " + sineTable.getNumChannels());

    return sineTable;
}

juce::AudioSampleBuffer SynthVoice::createSawWaveTable(int tableSize)
{
    juce::AudioSampleBuffer sawTable;
    sawTable.setSize(1, (int)tableSize + 1);
    sawTable.clear();

    auto* samples = sawTable.getWritePointer(0);

    // Generar onda de sierra
    for (int i = 0; i < tableSize; ++i)
    {
        // Normalizando la posición en el rango [-1, 1)
        float phase = static_cast<float>(i) / static_cast<float>(tableSize);
        samples[i] = 2.0f * phase - 1.0f; // Convierte la fase de [0, 1) a [-1, 1)
    }

    // Hacer la tabla cíclica copiando la primera muestra al final
    samples[tableSize] = samples[0];

    DBG("NUMERO DE CHANELS: " + sawTable.getNumChannels());

    return sawTable;
}

juce::AudioSampleBuffer SynthVoice::createSineWaveTable(int tableSize)
{
    juce::AudioSampleBuffer sineTable;
    sineTable.setSize(1, (int)tableSize + 1);
    sineTable.clear();

    auto* samples = sineTable.getWritePointer(0);

    int harmonics[] = { 1 };
    float harmonicWeights[] = { 0.5f }; // [1]

    jassert(juce::numElementsInArray(harmonics) == juce::numElementsInArray(harmonicWeights));

    for (auto harmonic = 0; harmonic < juce::numElementsInArray(harmonics); ++harmonic)
    {
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * harmonics[harmonic]; // [2]
        auto currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] += (float)sample * harmonicWeights[harmonic]; // [3]
            currentAngle += angleDelta;
        }
    }

    samples[tableSize] = samples[0];
    DBG("NUMERO DE CHANELS: " + sineTable.getNumChannels());

    return sineTable;
}

void fft(int N, double* ar, double* ai)
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
WavetableStruct SynthVoice::makeWaveTable(int tableSize, double* ar, double* ai, double topFreq) {
    fft(tableSize, ar, ai);

    juce::AudioSampleBuffer table;
    table.setSize(1, (int)tableSize + 1);
    table.clear();

    auto* samples = table.getWritePointer(0);

    for (int idx = 0; idx < tableSize; idx++) {
        samples[idx] = ai[idx];
    }

    //El ultimo indice (tableSize+1) se usa para hacer mas eficiente el calculo del wraping al final de cada ciclo de onda (Se ahorran comparaciones)
    samples[tableSize] = samples[0]; 

    WavetableStruct wavetableStruct;
    wavetableStruct.topFreq = topFreq;
    wavetableStruct.waveTable = table;

    return wavetableStruct;
}

std::vector<WavetableStruct> SynthVoice::fillWavetables(double *freqWaveRe, double *freqWaveIm, int tableSize) {
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
    double* ar = new double[tableSize];
    double* ai = new double[tableSize];
    //double scale = 0.0; Se podria optimizar el espacio que usa si cada wavetable tuviera una numero de samples variable
    //Tablas para freuencias altas no necesitan tantos samples
    //Frecuencias bajas necesitan mas

    // Ensure arrays are initialized
    std::fill(ar, ar + tableSize, 0.0);
    std::fill(ai, ai + tableSize, 0.0);


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

inline void saw(double *freqWaveRe, int tableSize) {
    freqWaveRe[0] = freqWaveRe[tableSize >> 1] = 0.0;
    for (int idx = 1; idx < (tableSize >> 1); idx++) {
        freqWaveRe[idx] = 1.0 / idx;                    // sawtooth spectrum
        freqWaveRe[tableSize - idx] = -freqWaveRe[idx];  // mirror
    }
}

inline void square(double* freqWaveRe, int tableSize) {
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

std::vector<WavetableStruct> SynthVoice::createSawWaveTables(int tableSize) {
    std::vector<WavetableStruct> wavetablesStructs;

    double* freqWaveRe = new double[tableSize];
    double* freqWaveIm = new double[tableSize];

    // Crear parte imaginaria
    for (int idx = 0; idx < tableSize; idx++) {
        freqWaveIm[idx] = 0.0;
    }

    // Saw
    //saw(freqWaveRe, tableSize);

    // Square
    square(freqWaveRe, tableSize);

    
    wavetablesStructs = fillWavetables(freqWaveRe, freqWaveIm, tableSize);

    return wavetablesStructs;

}
