#include <map>
#include <math.h>

#ifndef ENDOOMDATA_H
#define ENDOOMDATA_H
#include "Interfaces.h"
#endif

struct color_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};


class PlayPalData: public baseResourceWAD
{
    public:

        PlayPalData()
        {
        }

        PlayPalData(WADStructure *wad)
        {
            filePath = wad->filePath;
            playPalLump = wad->findLump("PLAYPAL");
            readPlayPal();
        }

        color_t getColor(int palleteNumber, uint8_t index)
        {
            return pallete[palleteNumber][unsigned(index)];
        }

        void printColor(int palleteNumber, uint8_t index)
        {
            color_t color = pallete[palleteNumber][index];
            cout << "r:" << unsigned(color.red) << ",   g:"<< unsigned(color.green) << ",   b:" << unsigned(color.blue) << endl;
        }

        int getPalletesAmount()
        {
            return PALLETES_AMOUNT;
        }

        void printInfo()
        {
            for(int i = 0; i < PALLETES_AMOUNT; i++)
            {
                cout << "|PALLETE " << i << "|" << endl;
                for(int x = 0; x < PALLETE_COLORS; x++)
                {   
                    cout << x << "   ";
                    printColor(i, x);
                }
            }
        }

    private:
        string filePath;
        WADStructure::lumpInfo_t playPalLump;

        int PALLETE_COLORS = 255;
        int PALLETES_AMOUNT;

        vector<vector<color_t>> pallete;


        void readPlayPal()
        {
            float amount = playPalLump.size / (PALLETE_COLORS * 3 * sizeof(uint8_t));
            
            if (roundf(amount) != amount) {
                cout << "Error reading PLAYPAL data" << std::endl;
                exit(0);
                return;
            } 

            PALLETES_AMOUNT = roundf(amount);

            

            std::ifstream file(filePath, std::ios::binary);
            file.seekg(playPalLump.filepos);

            for(int i = 0; i < PALLETES_AMOUNT; i++)
            {
                pallete.push_back(vector<color_t>());
                for(int x = 0; x < PALLETE_COLORS; x++)
                {
                    color_t color;
                    file.read(reinterpret_cast<char*>(&color), sizeof(color));  
                    pallete[i].push_back(color);
                }
            }
            file.close();
        }
};