#include "../../include/GameData/SoundData.h"
#include "../../include/Utils/playSound.h"

namespace SoundData
{

    Soundtrack::Soundtrack()
    {

    }

    void Soundtrack::play()
    {

    }

    void Soundtrack::printInfo()
    {

    }


    SoundEffect::SoundEffect(string formatValue, string nameString, int sampleRateValue, int numberOfSamplesValue, vector<uint8_t> samplesVector)
    {
        format = formatValue;
        name = nameString;
        sampleRate = sampleRateValue;
        numberOfSamples = numberOfSamplesValue;
        samples = samplesVector;
    }

    int  SoundEffect::getSampleRate()
    {
        return sampleRate;
    }

    int  SoundEffect::getNumberOfSamples()
    {
        return numberOfSamples;
    }

    vector<uint8_t>  SoundEffect::getSamples()
    {
        return samples;
    }

    void  SoundEffect::printInfo()
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

    void  SoundEffect::play()
    {
        if(format == "pc_speaker")
        {
            playBuzzer(samples, sampleRate);    
        }
        else if(format == "doom_format")
        {
            playSfx(samples, sampleRate);    
        }
        else
        {
            cout << "unsupported audio format!" << endl;
        }
    }







    SoundData::SoundData()
    {
    }

    SoundData::SoundData(WADStructure::WADStructure *wad)
    {
        filePath = wad->filePath;
        wadStructure = wad;
    }

    baseSound* SoundData::readSound(string name)
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
            return new SoundEffect("doom_format", name, static_cast<int>(sampleRate), static_cast<int>(numberOfSamples), samples);            
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
            return new SoundEffect("pc_speaker", name, static_cast<int>(sampleRate), static_cast<int>(numberOfSamples), samples);  
        }
        else
        {
            
            file.seekg(lump.filepos);

            char format[3];
            file.read(reinterpret_cast<char*>(&format), sizeof(format)); 
            if(strcmp(format, "MUS") == 0)
            {
                file.seekg(lump.filepos + 4); // skipping signature

                uint16_t songLength;
                uint16_t songOffset;
                uint16_t primaryChannels;
                uint16_t secodnaryChannels;
                uint16_t numInstruments;
                vector<uint16_t> instruments;


                file.read(reinterpret_cast<char*>(&songLength), sizeof(songLength));
                file.read(reinterpret_cast<char*>(&songOffset), sizeof(songOffset)); 
                file.read(reinterpret_cast<char*>(&primaryChannels), sizeof(primaryChannels)); 
                file.read(reinterpret_cast<char*>(&secodnaryChannels), sizeof(secodnaryChannels)); 
                file.read(reinterpret_cast<char*>(&numInstruments), sizeof(numInstruments)); 

                file.seekg(lump.filepos + 16); // skipping header

                uint16_t value;
                for(int i = 0; i < numInstruments; i++)
                {
                    file.read(reinterpret_cast<char*>(&value), sizeof(value)); 
                    instruments.push_back(value);
                }

                cout << "songLength: " << songLength << endl;
                cout << "songOffset: " << songOffset << endl;
                cout << "primaryChannels: " << primaryChannels << endl;
                cout << "secodnaryChannels: " << secodnaryChannels << endl;
                cout << "numInstruments: " << numInstruments << endl;
                for(int i = 0; i < numInstruments; i++)
                {
                    cout << i << ": " << instruments[i] << endl;
                }

                file.seekg(lump.filepos + songOffset);

                for(int i = 0; i < songLength; i++)
                {
                    //cout << i << ": " << instruments[i] << endl;
                }
                

                cout << "reading MIDI" << endl;

                return new Soundtrack();
            }

            cout << "Error reading sound data" << endl;
            exit(0);
        }   
    }

}