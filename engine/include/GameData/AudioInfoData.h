#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#ifndef AUDIODATA_H
#define AUDIODATA_H
#include "Interfaces.h"
#endif

struct voiceData_t
{
    uint8_t modulatorMulti;
    uint8_t modulatorAttackRate;
    uint8_t modulatorSustainLevel;
    uint8_t modulatorWaveformSelect;
    uint8_t modulatorKeyScaleLevel;
    uint8_t modulatorOutputLevel;
    uint8_t feedback;
    uint8_t carrierMulti;
    uint8_t carrierAttackRate;
    uint8_t carrierSustainLevel;
    uint8_t carrierWaveformSelect;
    uint8_t carrierKeyScaleLevel;
    uint8_t carrierOutputLevel;
    uint8_t unused;
    uint16_t noteOffset;
};

struct midiData_t
{
    uint16_t flags;
    uint8_t tuning;
    uint8_t note;
    voiceData_t voice1;
    voiceData_t voice2;
};


struct DMXData_t
{
    uint16_t patch;
    uint8_t column256K;
    uint8_t column512K;
    uint8_t column768K;
    uint8_t column1024K;
    string patchName;
};

class AudioInfoData: public baseResourceWAD
{
    public:

        AudioInfoData()
        {
        }

        AudioInfoData(WADStructure *wad)
        {
            filePath = wad->filePath;
            GENMIDILump = wad->findLump("GENMIDI");
            DMXGUSLump = wad->findLump("DMXGUSC");
            readGENMIDIData();
            readDMXGUSData();
        }

        void printInfo()
        {
            cout << endl << "|GENMIDI|" << endl;
            for(int i = 0; i < midiData.size(); i++)
            {
                cout << i << ":  flags:" << midiData[i].flags << ",   tuning:" << unsigned(midiData[i].tuning) << ",   note:" << unsigned(midiData[i].note) << endl;
            }

            //cout << endl << "|GENMIDI NAMES|" << endl;
            //for(int i = 0; i < instrumentsData.size(); i++)
            //{
            //    cout << i << ": '" << instrumentsData[i] << "'" << endl;
            //}

            cout << endl << "|DMXGUSC|" << endl;
            for (const auto &data : dmxData)
            {
                std::cout << "" << data.patch
                        << ", " << static_cast<int>(data.column256K)
                        << ", " << static_cast<int>(data.column512K)
                        << ", " << static_cast<int>(data.column768K)
                        << ", " << static_cast<int>(data.column1024K)
                        << ", " << data.patchName << std::endl;
            }
        }

        vector<midiData_t> getMIDIData()
        {
            return midiData;
        }

        midiData_t getMIDIDataByIndex(int index)
        {
            return midiData[index];
        }

        vector<DMXData_t> getDMXData()
        {
            return dmxData;
        }

    private:
        string filePath;
        WADStructure::lumpInfo_t GENMIDILump;
        WADStructure::lumpInfo_t DMXGUSLump;

        int INSTRUMENTS_AMOUNT = 175;
        

        vector<midiData_t> midiData;
        vector<char*> instrumentsData;


        vector<DMXData_t> dmxData;

        void readGENMIDIData()
        {
            std::ifstream file(filePath, std::ios::binary);
            file.seekg(GENMIDILump.filepos);
            char header[8];

            file.read(reinterpret_cast<char*>(&header), sizeof(header));  

            for(int i =0; i < INSTRUMENTS_AMOUNT; i++)
            {
                midiData_t data;

                file.read(reinterpret_cast<char*>(&data), sizeof(data)); 

                midiData.push_back(data);

            }

            for(int i =0; i < INSTRUMENTS_AMOUNT; i++)
            {
                char name[32];
                file.read(reinterpret_cast<char*>(&name), sizeof(name)); 
                instrumentsData.push_back(name);
            }

            file.close();

        }

        int safe_stoi(const std::string& str)
        {
            try {
                return std::stoi(str);
            } catch (const std::invalid_argument&) {
                // Return 0 for empty strings, you can choose another default value if needed
                return 0;
            }
        }

        void trim(std::string& str)
        {
            const std::string whitespace = " \t";
            const auto start = str.find_first_not_of(whitespace);
            if (start != std::string::npos)
            {
                const auto end = str.find_last_not_of(whitespace);
                str = str.substr(start, end - start + 1);
            }
            else
            {
                str.clear();
            }
        }

        void readDMXGUSData()
        {
            int size = DMXGUSLump.size;
            char data[DMXGUSLump.size];

            std::ifstream file(filePath, std::ios::binary);
            file.seekg(DMXGUSLump.filepos);
            file.read(reinterpret_cast<char*>(&data), sizeof(data));
            data[size] = '\0';
            file.close();

            std::istringstream iss(data);
            std::string line;

            // Skip the comment line
            std::getline(iss, line);

            // Skip the header line and extract column names
            std::getline(iss, line);
            std::istringstream iss_header(line);
            std::string columnHeader;
            for (int i = 0; i < 5; ++i) {
                std::getline(iss_header, columnHeader, ' ');
            }

            while (std::getline(iss, line))
            {
                if (line.empty() || line[0] == '#')
                {
                    continue; // Skip comment lines
                }

                std::istringstream iss_line(line);
                DMXData_t data;

                std::string patch, column256K, column512K, column768K, column1024K, patchName;
                std::getline(iss_line, patch, ',');
                std::getline(iss_line, column256K, ',');
                std::getline(iss_line, column512K, ',');
                std::getline(iss_line, column768K, ',');
                std::getline(iss_line, column1024K, ',');
                std::getline(iss_line, patchName);

                trim(patchName);

                data.patch = safe_stoi(patch);
                data.column256K = safe_stoi(column256K);
                data.column512K = safe_stoi(column512K);
                data.column768K = safe_stoi(column768K);
                data.column1024K = safe_stoi(column1024K);
                data.patchName = patchName;
                

                dmxData.push_back(data);
            } 
        }
};