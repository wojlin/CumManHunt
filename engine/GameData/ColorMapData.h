#include <math.h>


class ColorMapData
{
    public:
        ColorMapData(WADStructure *wad)
        {
            filePath = wad->filePath;
            colormapLump = wad->findLump("COLORMAP");
            readColormap();
        }


        uint8_t getColor(int mapNumber, uint8_t index)
        {
            return colormap[mapNumber][index];
        }

        int getColorMapsAmount()
        {
            return COLORMAPS_AMOUNT;
        }

        void printInfo()
        {
            for(int i = 0; i < COLORMAPS_AMOUNT; i++)
            {
                cout << "|COLORMAP " << i << "|" << endl;
                for(int x = 0; x < COLORMAPS_AMOUNT; x++)
                {   
                    cout << x << ":   " << unsigned(colormap[i][x]) <<endl;
                    
                }
            }
        }



    private:
        string filePath;
        WADStructure::lumpInfo_t colormapLump;
        vector<vector<uint8_t>> colormap;

        int COLORMAPS_AMOUNT;
        int COLORMAP_SIZE = 256;

        void readColormap()
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
};