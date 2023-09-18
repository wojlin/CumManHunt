
#include "../../include/GameData/ColorMapData.h"

namespace ColorMapData
{

    ColorMapData::ColorMapData()
    {
    }

    ColorMapData::ColorMapData(WADStructure::WADStructure *wad)
    {
        filePath = wad->filePath;
        colormapLump = wad->findLump("COLORMAP");
        readColormap();
    }


    uint8_t ColorMapData::getColor(int mapNumber, uint8_t index)
    {
        return colormap[mapNumber][index];
    }

    int ColorMapData::getColorMapsAmount()
    {
        return COLORMAPS_AMOUNT;
    }

    void ColorMapData::printInfo()
    {
        for(int i = 0; i < COLORMAPS_AMOUNT; i++)
        {
            cout << "|COLORMAP " << i << "|" << endl;
            for(int x = 0; x < COLORMAP_SIZE; x++)
            {   
                cout << x << ":   " << unsigned(colormap[i][x]) <<endl;
                
            }
        }
    }

    void ColorMapData::readColormap()
    {

        float amount = colormapLump.size / (COLORMAP_SIZE * sizeof(uint8_t));
        
        if (roundf(amount) != amount) {
            cout << "Error reading PLAYPAL data" << std::endl;
            exit(0);
            return;
        } 

        COLORMAPS_AMOUNT = roundf(amount);

        std::ifstream file(filePath, std::ios::binary);
        file.seekg(colormapLump.filepos);

        for(int i = 0; i < COLORMAPS_AMOUNT; i++)
        {
            colormap.push_back(vector<uint8_t>());
            for(int x = 0; x < COLORMAP_SIZE; x++)
            {
                uint8_t value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));  
                colormap[i].push_back(value);
            }
            
        }

        file.close();

    }

}