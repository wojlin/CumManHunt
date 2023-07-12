#include <map>

class ResourcesData
{
    public:
        

        ResourcesData(WADStructure *wad)
        {
            filePath = wad->filePath;
            std::ifstream file(filePath, std::ios::binary);
            readSprites(wad);

            file.close();
        }

        void readSprite(string name)
        {

            struct spriteData_t
            {
                uint16_t width;  // Width of graphic 
                uint16_t height;  // Height of graphic 
                int16_t leftOffset;  // Offset in pixels to the left of the origin 
                int16_t topOffset;  // Offset in pixels below the origin 
            };

            struct postData_t
            {
                uint8_t topDelta; // Starting offset of the span (Y position) 
                uint8_t length; // Number of pixels in this span 
            };

            vector<uint32_t> spriteDataColumnsPos; //Array of column offsets relative to the beginning of the patch header
            spriteData_t spriteData;

            ifstream file(filePath, std::ios::binary);
            WADStructure::lumpInfo_t* spriteInfo = spritesMap[name];
            file.seekg(spriteInfo->filepos);
            file.read(reinterpret_cast<char*>(&spriteData), sizeof(spriteData));
            for(int i = 0; i < spriteData.width; i++)
            {
                uint32_t value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                spriteDataColumnsPos.push_back(value);
            }

            cout << "width: " << spriteData.width << endl;
            cout << "height: " << spriteData.height << endl;
            cout << "left offset: " << spriteData.leftOffset << endl;
            cout << "top offset: " << spriteData.topOffset << endl;

            vector<vector<uint8_t>> matrix(spriteData.height, std::vector<uint8_t>(spriteData.width, 0));

            for(int i =0; i<spriteDataColumnsPos.size(); i++)
            {   
                postData_t postData;
                int startingPos = spriteInfo->filepos + spriteDataColumnsPos[i];
                file.seekg(startingPos, std::ios::beg);

                uint8_t dummy = 0;
                uint8_t rowstart = 0;
                while(rowstart != 255)
                {
                    file.read(reinterpret_cast<char*>(&rowstart), sizeof(rowstart));
                    if(rowstart == 255)
                    {
                        break;
                    }
                    uint8_t pixelcount = 0;
                    file.read(reinterpret_cast<char*>(&pixelcount), sizeof(pixelcount));
                    cout << "row: " << i << "   rowstart: " << unsigned(rowstart) << "   pixelcount: " << unsigned(pixelcount) << endl;
                    file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
                    for(int y = 0; y < pixelcount; y++)
                    {
                        uint8_t value = 0;
                        file.read(reinterpret_cast<char*>(&value), sizeof(value));
                        cout << unsigned(value) << ", ";
                        matrix[y + rowstart][i] = value;
                    }
                    cout << endl;
                    file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
                }
                
            }

            cout << "matrix:" << endl;
            for(int y = 0; y < spriteData.height; y++)
            {
                for(int x = 0; x < spriteData.width; x++)
                {
                    cout << unsigned(matrix[y][x]) << ", ";
                }
                cout << endl;
            }

        }

        int getSpritesAmount()
        {
            return spritesAmount;
        }

    private:

        string filePath;

        map<std::string, WADStructure::lumpInfo_t*> spritesMap;
        int spritesAmount = 0;
        map<std::string, WADStructure::lumpInfo_t> flatsMap;
 

        void readSprites(WADStructure *wad)
        {
            bool markerFound = false;
            for(int i = 0; i < wad->directoryCount;i++)
            {
                if(strcmp("S_START", wad->directory[i].name) == 0)
                {
                    markerFound = true;
                }
                else if(strcmp("S_END", wad->directory[i].name) == 0)
                {
                    markerFound = false;
                }
                else if(markerFound == true)
                {
                    spritesMap[wad->directory[i].name] = &(wad->directory[i]);
                    spritesAmount++;
                }
            }
            cout << "reading sprites..." << endl;
        }


};