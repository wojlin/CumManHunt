#include <map>
#include "../Utils/BMPImage.h"

struct imageColumn_t
{
    int column;
    int rowStart;
    int pixelCount;
    vector<color_t> pixels;
};


class Image
{
    public:
        
        Image(uint16_t w,  uint16_t h,  int16_t l,  int16_t t, vector<imageColumn_t> c)
        {
            width = w;
            height = h;
            leftOffset = l;
            topOffset = t;
            columns = c;
        }

        uint16_t getWidth()
        {
            return width;
        }

        uint16_t getHeight()
        {
            return width;
        }

        int16_t getLeftOffset()
        {
            return width;
        }

        int16_t getRightOffset()
        {
            return width;
        }

        int16_t getTopOffset()
        {
            return width;
        }

        vector<imageColumn_t> getColumns()
        {
            return columns;
        }

        void printInfo()
        {
            cout << "width: " << width << endl;
            cout << "height: " << height << endl;
            cout << "left offset: " << leftOffset << endl;
            cout << "top offset: " << topOffset << endl;
        }

        void saveAsFile(string filePath)
        {
            cout << "Saving image to \"" << filePath << " \"..." << endl;
            BMPImage image(width, height);

            for(int x = 0; x < columns.size(); x++)
            {   
                for(int b = 0; b < columns[x].pixelCount; b++)
                {
                    color_t pixel = columns[x].pixels[b];
                    image.setPixel(columns[x].column, columns[x].rowStart + b, pixel.red, pixel.green, pixel.blue, 255);
                }

            }
            image.createImage(filePath);
            cout << "Image Saved!" << endl;
        }

    private:
        uint16_t width;
        uint16_t height;
        int16_t leftOffset;
        int16_t topOffset;
        vector<imageColumn_t> columns;
};




class ResourcesData
{
    public:
        

        ResourcesData(WADStructure *wad, PlayPalData*  playpalPointer): filePath(wad->filePath), playpal(playpalPointer)
        {
            filePath = wad->filePath;
            std::ifstream file(filePath, std::ios::binary);
            readSprites(wad);

            file.close();
        }

        Image readSprite(string name)
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

            vector<imageColumn_t> columns;

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
                    imageColumn_t column;
                    column.column = i;
                    column.rowStart = unsigned(rowstart);
                    column.pixelCount = unsigned(pixelcount);
                    
                    file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));

                    for(int y = 0; y < pixelcount; y++)
                    {
                        uint8_t value = 0;
                        file.read(reinterpret_cast<char*>(&value), sizeof(value));                      
                        color_t color = playpal->getColor(0, value);
                        column.pixels.push_back(color);
                    }
                    columns.push_back(column);
                    file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
                }
            }
            return Image(spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns);

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

        PlayPalData* playpal;

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