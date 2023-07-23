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



class AudioData
{
    public:
        AudioData(WADStructure *wad)
        {
            filePath = wad->filePath;
            GENMIDILump = wad->findLump("GENMIDI");
            DMXGUSLump = wad->findLump("DMXGUSC");
            cout << GENMIDILump.size << endl;
            readGENMIDIData();
            readDMXGUSData();
        }

        void printInfo()
        {
            cout << endl << "|GENMIDI|" << endl;
            //for(int i = 0; i < midiData.size(); i++)
            //{
            //    cout << i << ":  flags:" << midiData[i].flags << ",   tuning:" << unsigned(midiData[i].tuning) << ",   note:" << unsigned(midiData[i].note) << endl;
            //}

            for(int i = 0; i < instrumentsData.size(); i++)
            {
                cout << i << ": '" << instrumentsData[i] << "'" << endl;
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

    private:
        string filePath;
        WADStructure::lumpInfo_t GENMIDILump;
        WADStructure::lumpInfo_t DMXGUSLump;

        int INSTRUMENTS_AMOUNT = 175;
        

        vector<midiData_t> midiData;
        vector<char*> instrumentsData;


        void readGENMIDIData()
        {
            std::ifstream file(filePath, std::ios::binary);
            file.seekg(GENMIDILump.filepos);
            char header[8];

            file.read(reinterpret_cast<char*>(&header), sizeof(header));  

            cout << header << endl;

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



        }

        void readDMXGUSData()
        {

        }
};