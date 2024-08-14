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
    : unisonVoices(1), unisonDetune(0.20f), unisonSpread(0.15f), gen(rd()),
    maxUnisonDetuning(10), maxUnisonSpread(10), maxUnisonVoices(8),
    currentFrequency2NotesDown(0.0f), currentFrequency2NotesUp(0.0f)
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
    phaseRandomnes = std::uniform_real_distribution<>(0, tableSize);

    oscGain = 1.0f;
    isPrepared = false;
    sampleRate = 0.0f;
    tableDelta = 0.0f;
    numWavetables = (*tables).size();


    // Inicializar vectores de memoria para almacenar los índices
    /*for (int i = 0; i < 8; ++i)
    {
        float* initIndexValue = new float(phaseRandomnes(gen));  // Asignar memoria para un float y asignar un valor

        std::vector<float*> indexBlock(4);  // Crear un vector de 4 punteros a float
        for (int j = 0; j < 4; ++j) {
            indexBlock[j] = initIndexValue;  // Almacenar el puntero en el vector
        }
        unisonVoiceCurrentIndexArray.add(std::move(indexBlock));  // Añadir el vector al juce::Array
    }*/
    
    for (int i = 0; i < 8; ++i)
    {
        float newPhase = 0.0f + phaseRandomnes(gen);
        // Asegurarse de que newPhase no se pasa del tamaño de la wavetable
        while (newPhase >= tableSize) {
            newPhase = 0.0f + phaseRandomnes(gen);
        }
        float* value = new float(newPhase);

        unisonVoiceCurrentIndexArray2.add(value);
    }


    unisonDetuneArray.set(0, -2);
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


    for (float value : unisonSpreadArrayL) {
        __m128 newValue = _mm_set_ps(
            value,
            value,
            value,
            value
        );
        unisonSpreadArrayL128.add(newValue);
    }

    for (float value : unisonSpreadArrayR) {
        __m128 newValue = _mm_set_ps(
            value,
            value,
            value,
            value
        );
        unisonSpreadArrayR128.add(newValue);
    }

    // Inicialización de unisonDetuneArray, unisonSpreadArrayL y R...
}

WavetableOscillatorProcessor::~WavetableOscillatorProcessor()
{
    /*for (auto& indexBlock : unisonVoiceCurrentIndexArray)
    {
        for (auto* ptr : indexBlock)
        {
            delete ptr;  // Liberar la memoria asignada para cada float
        }
    }*/

    for (float* ptr : unisonVoiceCurrentIndexArray2)
    {
        delete ptr;
    }
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
    panningParameter = parameterHandler.syncWithSliderParam(juce::String("WavetableOscillator_") + juce::String(getId()) + juce::String("_panning"));
}

float WavetableOscillatorProcessor::getNextSampleSSE(int sample, float newTableDelta, float* newCurrentIndex)
{
    // Cargar los índices actuales
    __m128 idx = _mm_load_ps(newCurrentIndex);

    // Calcular la fracción
    __m128 frac = _mm_sub_ps(idx, _mm_floor_ps(idx));

    // Convertir los índices a enteros
    __m128i idx0 = _mm_cvttps_epi32(idx);
    __m128i idx1 = _mm_add_epi32(idx0, _mm_set1_epi32(1));

    // Cargar las muestras adyacentes
    const float* table = wavetable->waveTable.getReadPointer(0);
    __m128 value0 = _mm_set_ps(
        table[_mm_extract_epi32(idx0, 3)],
        table[_mm_extract_epi32(idx0, 2)],
        table[_mm_extract_epi32(idx0, 1)],
        table[_mm_extract_epi32(idx0, 0)]
    );
    __m128 value1 = _mm_set_ps(
        table[_mm_extract_epi32(idx1, 3)],
        table[_mm_extract_epi32(idx1, 2)],
        table[_mm_extract_epi32(idx1, 1)],
        table[_mm_extract_epi32(idx1, 0)]
    );

    // Interpolación lineal
    __m128 result = _mm_add_ps(value0, _mm_mul_ps(frac, _mm_sub_ps(value1, value0)));

    // Actualizar los índices
    __m128 tableDelta = _mm_set1_ps(newTableDelta);
    idx = _mm_add_ps(idx, tableDelta);

    // Envolver los índices si se exceden del tamaño de la tabla
    __m128 mask = _mm_cmpge_ps(idx, _mm_set1_ps(static_cast<float>(tableSize)));
    idx = _mm_sub_ps(idx, _mm_and_ps(mask, _mm_set1_ps(static_cast<float>(tableSize))));

    // Guardar los índices actualizados
    _mm_store_ps(newCurrentIndex, idx);

    // Devolver la media de los resultados
    float output[4];
    _mm_store_ps(output, result);
    return (output[0] + output[1] + output[2] + output[3]) * 0.25f;
}

void WavetableOscillatorProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    currentFrequency2NotesDown = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber - 2);
    currentFrequency2NotesUp = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber + 2);

    setFrequency(static_cast<float>(frequency), sampleRate);


}

void WavetableOscillatorProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void WavetableOscillatorProcessor::updateParameterValues()
{
    oscGain = gainParameter->getValue();
    oscGainModulationBuffer = gainParameter->getModulationBuffer(getVoiceNumberId());

    panning = panningParameter->getValue();
    panningModulationBuffer = panningParameter->getModulationBuffer(getVoiceNumberId());

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
    //DBG(juce::String(spec.sampleRate));
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
    else if (waveType == "Square")
        square(freqWaveRe, tableSize);

    // Sine
    else if (waveType == "Sine")
        square(freqWaveRe, tableSize);

    wavetablesStructs = fillWavetables(freqWaveRe, freqWaveIm, tableSize);

    return wavetablesStructs;

}

void WavetableOscillatorProcessor::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int numSamples = outputBuffer.getNumSamples();
    auto* leftChannelBuffer = outputBuffer.getWritePointer(0);
    auto* rightChannelBuffer = outputBuffer.getWritePointer(1);

    __m128 globalPanAngle = _mm_set1_ps((panning + panningModulationBuffer[0]) * juce::MathConstants<float>::halfPi);
    __m128 globalPanningLeft = _mm_cos_ps(globalPanAngle);
    __m128 globalPanningRight = _mm_sin_ps(globalPanAngle);
    __m128 gainValue = _mm_set1_ps(oscGain + oscGainModulationBuffer[0]);


    //DBG("Before: " + juce::String(*unisonVoiceCurrentIndexArray[0][0]));  // Verificación antes



    for (int unisonVoice = 0; unisonVoice < unisonVoices; unisonVoice++) {
        const float newVoiceDelta = unisonVoices == 1 ? tableDelta : getUnisonDeltaFromFrequency(freqRelativeTo(currentFrequency, unisonDetuneArray[unisonVoice] * unisonDetune), sampleRate);

        // Almacenar el puntero en una variable local antes del bucle
        //std::vector<float>& currentIndexVector = unisonVoiceCurrentIndexArray[unisonVoice];
        //float* newCurrentIndex0 = unisonVoiceCurrentIndexArray[unisonVoice][0];
        //float* newCurrentIndex1 = unisonVoiceCurrentIndexArray[unisonVoice][1];
        //float* newCurrentIndex2 = unisonVoiceCurrentIndexArray[unisonVoice][2];
        //float* newCurrentIndex3 = unisonVoiceCurrentIndexArray[unisonVoice][3];
        __m128 gainLeft;
        __m128 gainRight;

        if (unisonVoices > 1) {
            gainLeft = _mm_mul_ps(globalPanningLeft, _mm_mul_ps(unisonSpreadArrayL128[unisonVoice], gainValue));
            gainRight = _mm_mul_ps(globalPanningRight, _mm_mul_ps(unisonSpreadArrayR128[unisonVoice], gainValue));
        }
        else {
            gainLeft = _mm_mul_ps(globalPanningLeft, gainValue);
            gainRight = _mm_mul_ps(globalPanningRight, gainValue);
        }

        float* newCurrentIndex = unisonVoiceCurrentIndexArray2[unisonVoice];

        for (int sample = 0; sample < numSamples; sample += 4) {
            const float nextSample1 = getNextSample(newVoiceDelta, newCurrentIndex);
            const float nextSample2 = getNextSample(newVoiceDelta, newCurrentIndex);
            const float nextSample3 = getNextSample(newVoiceDelta, newCurrentIndex);
            const float nextSample4 = getNextSample(newVoiceDelta, newCurrentIndex);

            __m128 nextSample = _mm_set_ps(
                nextSample4,
                nextSample3,
                nextSample2,
                nextSample1
            );

            // Aplicar ganancia y panning
            __m128 leftSample = _mm_mul_ps(nextSample, gainLeft);
            __m128 rightSample = _mm_mul_ps(nextSample, gainRight);

            // Sumar al buffer de salida
            _mm_store_ps(leftChannelBuffer + sample, _mm_add_ps(_mm_load_ps(leftChannelBuffer + sample), leftSample));
            _mm_store_ps(rightChannelBuffer + sample, _mm_add_ps(_mm_load_ps(rightChannelBuffer + sample), rightSample));

        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    //DBG("After: " + juce::String(*unisonVoiceCurrentIndexArray[0][0]));  // Verificación antes
    auto durationInMicroseconds = std::chrono::duration<double, std::micro>(end - begin).count();

    DBG("LE LLEVO: " + juce::String(durationInMicroseconds, 5) + "Microsegundos");

}

float WavetableOscillatorProcessor::getNextSample(const float newTableDelta, float* newCurrentIndex)
{
    auto index0 = (unsigned int)*newCurrentIndex;
    auto index1 = index0 + 1;

    auto frac = *newCurrentIndex - (float)index0;

    auto* table = wavetable->waveTable.getReadPointer(0); // Obtener puntero de lectura a la wavetable
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0);

    if ((*newCurrentIndex += newTableDelta) >= (float)tableSize)
        *newCurrentIndex -= (float)tableSize;

    return currentSample;
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

float WavetableOscillatorProcessor::freqRelativeTo(float relativeFreq, float notes)
{
    return relativeFreq * std::pow(2, (notes / 12));
}
