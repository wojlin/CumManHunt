#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>

#include <SDL2/SDL.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <alsa/asoundlib.h>
#endif


void playSfx(vector<uint8_t> samples, uint16_t sampleRate)
{

    const double volumeFactor = 0.5; // Adjust this value as needed

    for (int i = 0; i < samples.size(); ++i) {
        Uint8* sample = &samples[i];
        *sample = static_cast<Uint8>(*sample * volumeFactor);
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        // Handle initialization error
        cout << "sdl init error!" << endl;
    }

    SDL_AudioSpec want, have;

    SDL_memset(&want, 0, sizeof(want));
    want.freq = sampleRate;
    want.format = AUDIO_S8;
    want.channels = 1; // Stereo
    want.samples = samples.size(); // Audio buffer size
    want.callback = nullptr; // You can provide a callback function for audio data

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (audioDevice == 0) 
    {

        SDL_Quit();
        cout << "audio device error" << endl;
    }

    SDL_QueueAudio(audioDevice, samples.data(), samples.size());
    SDL_PauseAudioDevice(audioDevice, 0); // Unpause audio playback

     // Wait for sound playback to finish
    SDL_Delay(static_cast<Uint32>(samples.size() * 1000.0 / (sampleRate * 2))); // Assuming 16-bit stereo

    // Close audio device
    SDL_CloseAudioDevice(audioDevice);

    // Quit SDL
    SDL_Quit();
    
}

void playBuzzer(vector<uint8_t> samples, uint16_t sampleRate) 
{
#ifdef _WIN32
    cout << "buzzer on windows is not supported yet!" << endl;

#elif __linux__
    std::map<int, double> valueToFrequency = {
    {0, 0.0},
    {1, 175.00},
    {2, 180.02},
    {3, 185.01},
    {4, 190.02},
    {5, 196.02},
    {6, 202.02},
    {7, 208.01},
    {8, 214.02},
    {9, 220.02},
    {10, 226.02},
    {11, 233.04},
    {12, 240.02},
    {13, 247.03},
    {14, 254.03},
    {15, 262.00},
    {16, 269.03},
    {17, 277.03},
    {18, 285.04},
    {19, 294.03},
    {20, 302.07},
    {21, 311.04},
    {22, 320.05},
    {23, 330.06},
    {24, 339.06},
    {25, 349.08},
    {26, 359.06},
    {27, 370.09},
    {28, 381.08},
    {29, 392.10},
    {30, 403.10},
    {31, 415.01},
    {32, 427.05},
    {33, 440.12},
    {34, 453.16},
    {35, 466.08},
    {36, 480.15},
    {37, 494.07},
    {38, 508.16},
    {39, 523.09},
    {40, 539.16},
    {41, 554.19},
    {42, 571.17},
    {43, 587.19},
    {44, 604.14},
    {45, 622.09},
    {46, 640.11},
    {47, 659.21},
    {48, 679.10},
    {49, 698.17},
    {50, 719.21},
    {51, 740.18},
    {52, 762.41},
    {53, 784.47},
    {54, 807.29},
    {55, 831.48},
    {56, 855.32},
    {57, 880.57},
    {58, 906.67},
    {59, 932.17},
    {60, 960.69},
    {61, 988.55},
    {62, 1017.20},
    {63, 1046.64},
    {64, 1077.85},
    {65, 1109.93},
    {66, 1141.79},
    {67, 1175.54},
    {68, 1210.12},
    {69, 1244.19},
    {70, 1281.61},
    {71, 1318.43},
    {72, 1357.42},
    {73, 1397.16},
    {74, 1439.30},
    {75, 1480.37},
    {76, 1523.85},
    {77, 1569.97},
    {78, 1614.58},
    {79, 1661.81},
    {80, 1711.87},
    {81, 1762.45},
    {82, 1813.34},
    {83, 1864.34},
    {84, 1921.38},
    {85, 1975.46},
    {86, 2036.14},
    {87, 2093.29},
    {88, 2157.64},
    {89, 2217.80},
    {90, 2285.78},
    {91, 2353.41},
    {92, 2420.24},
    {93, 2490.98},
    {94, 2565.97},
    {95, 2639.77}
    };

    string command = "beep -f ";

    double freq = valueToFrequency[samples[0]];
    command += to_string(freq);
    command += " -l ";
    command += to_string((1.0/(float)sampleRate)*1000.0);

    for(int i = 1; i < samples.size(); i++)
    {
        freq = valueToFrequency[samples[i]];
        if(freq == 0)
        {
            freq = 1;
        }
        command += " -n -f ";
        command += to_string(freq);
        command += " -l ";
        command += to_string((1.0/(float)sampleRate)*1000.0);
        
        
    }
    //cout << command << endl;
    system(command.c_str());

#endif
}
