#include <iostream>
#include <string>
//#include "../Utils/playSound.h"

#include <cstdlib>

class Sound
{
    public:
        Sound(string formatValue, string nameString, int sampleRateValue, int numberOfSamplesValue, vector<uint8_t> samplesVector)
        {
            format = formatValue;
            name = nameString;
            sampleRate = sampleRateValue;
            numberOfSamples = numberOfSamplesValue;
            samples = samplesVector;
        }

        int getSampleRate()
        {
            return sampleRate;
        }

        int getNumberOfSamples()
        {
            return numberOfSamples;
        }

        vector<uint8_t> getSamples()
        {
            return samples;
        }

        void printInfo()
        {
            cout << "|SOUND|" << endl;
            cout << "format: " << format << endl;
            cout << "name: " <<  name << endl;
            cout << "sample rate: " << sampleRate << endl;
            cout << "number of samples: " << numberOfSamples << endl;
            cout << "samples:" << endl;
            for(int i = 0; i < numberOfSamples; i++)
            {
                cout << i << ": " << unsigned(samples[i]) << endl;
            }
            cout << endl;

        }

        void play()
        {
            if(format == "pc_speaker")
            {

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

                for(int i = 1; i < numberOfSamples; i++)
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
                cout << command << endl;
                system(command.c_str());
            }
        }

    private:
        string format;
        string name;
        vector<uint8_t> samples;
        uint16_t sampleRate;
        uint32_t numberOfSamples;

};



class SoundData
{
    public:
        SoundData(WADStructure *wad)
        {
            filePath = wad->filePath;
            wadStructure = wad;
        }

        Sound readSound(string name)
        {
            
            WADStructure::lumpInfo_t lump =  wadStructure->findLump(name);

            std::ifstream file(filePath, std::ios::binary);
            file.seekg(lump.filepos);

            uint16_t format;
            file.read(reinterpret_cast<char*>(&format), sizeof(format)); 
            if(unsigned(format) == 3) // doom sound format
            {
                uint16_t sampleRate = 0;
                uint32_t numberOfSamples = 0;
                vector<uint8_t> samples;
                file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
                file.read(reinterpret_cast<char*>(&numberOfSamples), sizeof(numberOfSamples)); 

                file.seekg(lump.filepos + 18); // skipping padding bytes

                uint8_t value;
                for(int i = 0; i < numberOfSamples; i++)
                {   
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    samples.push_back(value);
                }
                return Sound("doom_format", name, static_cast<int>(sampleRate), static_cast<int>(numberOfSamples), samples);            
            }
            else if(unsigned(format) == 0) // pc speaker
            {
                uint16_t sampleRate = 140; // fixed sample rate
                uint16_t numberOfSamples = 0;
                vector<uint8_t> samples;
                file.read(reinterpret_cast<char*>(&numberOfSamples), sizeof(numberOfSamples)); 

                uint8_t value;
                for(int i = 0; i < numberOfSamples; i++)
                {   
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    samples.push_back(value);
                }
                return Sound("pc_speaker", name, static_cast<int>(sampleRate), static_cast<int>(numberOfSamples), samples);  
            }
            else
            {
                cout << "Error reading sound data" << endl;
                exit(0);
            }   
        }

    private:
        string filePath;
        WADStructure *wadStructure;
};