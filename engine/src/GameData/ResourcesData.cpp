#include "../../include/GameData/ResourcesData.h"
namespace ResourcesData
{

    Image::Image(uint16_t w,  uint16_t h,  int16_t l,  int16_t t, vector<imageColumn_t> c, int lSize)
    {
        width = w;
        height = h;
        leftOffset = l;
        topOffset = t;
        columns = c;
        size = lSize;
    }

    uint16_t Image::getWidth()
    {
        return width;
    }

    int Image::getSize()
    {
        return size;
    }

    uint16_t Image::getHeight()
    {
        return width;
    }

    int16_t Image::getLeftOffset()
    {
        return width;
    }

    int16_t Image::getRightOffset()
    {
        return width;
    }

    int16_t Image::getTopOffset()
    {
        return width;
    }

    vector<imageColumn_t> Image::getColumns()
    {
        return columns;
    }

    void Image::printInfo()
    {
        printInfoHeader("H1", "IMAGE");
        cout << "width: " << width << endl;
        cout << "height: " << height << endl;
        cout << "left offset: " << leftOffset << endl;
        cout << "top offset: " << topOffset << endl;
        cout << "size: " << size << " bytes" << endl;
        printInfoHeader("H1");
    }

    void Image::saveAsFile(string filePath)
    {
        cout << "Saving image to \"" << filePath << " \"..." << endl;
        BMPImage image(width, height);

        for(int x = 0; x < columns.size(); x++)
        {   
            for(int b = 0; b < columns[x].pixelCount; b++)
            {
                PlayPalData::color_t pixel = columns[x].pixels[b];
                image.setPixel(columns[x].column, columns[x].rowStart + b, pixel.red, pixel.green, pixel.blue, 255);
            }

        }
        image.createImage(filePath);
        cout << "Image Saved!" << endl;
    }



    ResourcesData::ResourcesData()
    {

    }

    ResourcesData::ResourcesData(WADStructure::WADStructure *wad, PlayPalData::PlayPalData*  playpalPointer): filePath(wad->filePath), playpal(playpalPointer)
    {
        wadStructure = wad;
        filePath = wad->filePath;
        std::ifstream file(filePath, std::ios::binary);
        readSprites(wad);
        readFlats(wad);
        readPatches(wad);
        readPNames(wad);

        vector<WADStructure::lumpInfo_t> lumps = wad->findLumps("TEXTURE");

        for(int i = 0; i < lumps.size(); i++)
        {
            cout << "reading \"" << lumps[i].name << "\" lump..." << endl;
            readTextureX(lumps[i]);
        }
        

        file.close();
    }


    Image ResourcesData::readFlat(string name)
    {
        
        int width = 64;
        int height = 64;

        WADStructure::lumpInfo_t* spriteInfo;

        spriteInfo = flatsMap[name];

        if(spriteInfo == 0)
        {
            std::cout << "The '" << name << "' is not in the game resources." << std::endl;
            exit(0);
        }

        ifstream file(spriteInfo->path, std::ios::binary);


        vector<imageColumn_t> columns;
        file.seekg(spriteInfo->filepos);
        for(int x =0; x<width; x++)
        {
            imageColumn_t column;
            column.pixelCount = height;
            column.rowStart = 0;
            column.column = x;
            for(int y =0; y<height; y++)
            {
                file.seekg(spriteInfo->filepos + (width * y) + x);
                uint8_t value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                column.pixels.push_back(playpal->getColor(0, value));
            }
            columns.push_back(column);
        }

        file.close();

        return Image(width, height, 0, 0, columns, spriteInfo->size);

    }

    Image ResourcesData::readSprite(string name)
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

        

        WADStructure::lumpInfo_t* spriteInfo;

        spriteInfo = spritesMap[name];

        if(spriteInfo == 0)
        {
            std::cout << "The '" << name << "' is not in the game resources." << std::endl;
            exit(0);
        }

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
                    PlayPalData::color_t color = playpal->getColor(0, value);
                    column.pixels.push_back(color);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo->size);

    }

    Image ResourcesData::readPatch(string name)
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

        

        WADStructure::lumpInfo_t* spriteInfo;

        spriteInfo = patchesMap[name];

        if(spriteInfo == 0)
        {
            std::cout << "The '" << name << "' is not in the game resources." << std::endl;
            exit(0);
        }

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
                    PlayPalData::color_t color = playpal->getColor(0, value);
                    column.pixels.push_back(color);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo->size);

    }

    Image ResourcesData::readGameFlat(string name)
    {
        int width = 64;
        int height = 64;

        WADStructure::lumpInfo_t spriteInfo;

        spriteInfo = wadStructure->findLump(name);

        ifstream file(spriteInfo.path, std::ios::binary);

        vector<imageColumn_t> columns;
        file.seekg(spriteInfo.filepos);
        for(int x =0; x<width; x++)
        {
            imageColumn_t column;
            column.pixelCount = height;
            column.rowStart = 0;
            column.column = x;
            for(int y =0; y<height; y++)
            {
                file.seekg(spriteInfo.filepos + (width * y) + x);
                uint8_t value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                column.pixels.push_back(playpal->getColor(0, value));
            }
            columns.push_back(column);
        }

        file.close();

        return Image(width, height, 0, 0, columns, spriteInfo.size);

    }

    Image ResourcesData::readGameSprite(string name)
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

        

        WADStructure::lumpInfo_t spriteInfo;

        spriteInfo = wadStructure->findLump(name);

        ifstream file(spriteInfo.path, std::ios::binary);

        file.seekg(spriteInfo.filepos);
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
            int startingPos = spriteInfo.filepos + spriteDataColumnsPos[i];
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
                    PlayPalData::color_t color = playpal->getColor(0, value);
                    column.pixels.push_back(color);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo.size);

    }
    
    Image ResourcesData::readGamePatch(string name)
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
      

        WADStructure::lumpInfo_t spriteInfo;

        spriteInfo = wadStructure->findLump(name);

        ifstream file(spriteInfo.path, std::ios::binary);

        file.seekg(spriteInfo.filepos);
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
            int startingPos = spriteInfo.filepos + spriteDataColumnsPos[i];
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
                    PlayPalData::color_t color = playpal->getColor(0, value);
                    column.pixels.push_back(color);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo.size);

    }
    
    

    int ResourcesData::getSpritesAmount()
    {
        return spritesAmount;
    }

    int ResourcesData::getFlatsAmount()
    {
        return flatsAmount;
    }

    int ResourcesData::getPatchesAmount()
    {
        return patchesAmount;
    }

    char* ResourcesData::getPNameByIndex(int index)
    {
        for(int i = 0; i < pnames.size(); i++)
        {
            if(i == index)
            {
                return pnames[i];
            }
        }
        cout << "pname of index '"<< index << "' does not exist" << endl;
        exit(0);
    }

    int ResourcesData::getPNameIndexByName(char* name)
    {
        for(int i = 0; i < pnames.size(); i++)
        {
            if(strcmp(pnames[i], name) == 0)
            {
                return i;
            }
        }
        cout << "pname of name '"<< name << "' does not exist" << endl;
        exit(0);
    }

    int ResourcesData::getPNamesAmount()
    {
        return numMapPatches;
    }

    vector<textureX_t> ResourcesData::getTextureX()
    {
        return textureX;
    }

    void ResourcesData::printInfo()
    {
        for(int a =0; a < textureX.size(); a++)
        {
            cout << "|TEXTURE" << a << "|" << endl;
            cout << "numTextures: " << textureX[a].numTextures << endl;

            for(int b =0; b < textureX[a].numTextures; b++)
            {
                cout << b << ": " << endl;
                cout << "   name: " << textureX[a].mapTextures[b].name << endl;
                cout << "   masked: " << textureX[a].mapTextures[b].masked << endl; 
                cout << "   width: " << textureX[a].mapTextures[b].width << endl;
                cout << "   height: " << textureX[a].mapTextures[b].height << endl;
                cout << "   columnDirectory: " << textureX[a].mapTextures[b].columnDirectory << endl;
                cout << "   patchCount: " << textureX[a].mapTextures[b].patchCount << endl;
                cout << endl;

                for(int c = 0; c < textureX[a].mapTextures[b].mapPatches.size(); c++)
                {   
                    cout << "       |PATCH" << c << "|" << endl;
                    cout << "       originX: " << textureX[a].mapTextures[b].mapPatches[c].originX << endl;
                    cout << "       originY: " << textureX[a].mapTextures[b].mapPatches[c].originY << endl; 
                    cout << "       patch: " << textureX[a].mapTextures[b].mapPatches[c].patch << endl;
                    cout << "       stepDir: " << textureX[a].mapTextures[b].mapPatches[c].stepDir << endl;
                    cout << "       colorMap: " << textureX[a].mapTextures[b].mapPatches[c].colorMap << endl;
                    cout << endl;
                }
            }
        }
        
    }

        
    void ResourcesData::readTextureX(WADStructure::lumpInfo_t lump)
    {

        textureX_t texture;

        std::ifstream file(filePath, std::ios::binary);
        file.seekg(lump.filepos);

        uint32_t numTextures;
        vector<uint32_t> offset;

        file.read(reinterpret_cast<char*>(&numTextures), sizeof(numTextures)); 

        texture.numTextures = numTextures;

        uint32_t value;
        for(int i = 0; i < numTextures; i++)
        {
            file.read(reinterpret_cast<char*>(&value), sizeof(value)); 
            offset.push_back(value);
        }

        for(int i = 0; i <  offset.size(); i++)
        {

            file.seekg(lump.filepos + offset[i]);

            char name[8]; 
            bool maskedBool;
            uint32_t masked;
            short width;
            short height;
            uint32_t columnDirectory;
            uint16_t patchCount;
            vector<mappatch_t> mapPatches;

            file.read(reinterpret_cast<char*>(&name), sizeof(name));
            file.read(reinterpret_cast<char*>(&masked), sizeof(masked));
            file.read(reinterpret_cast<char*>(&width), sizeof(width));
            file.read(reinterpret_cast<char*>(&height), sizeof(height));
            file.read(reinterpret_cast<char*>(&columnDirectory), sizeof(columnDirectory));
            file.read(reinterpret_cast<char*>(&patchCount), sizeof(patchCount));

            if(masked == 1)
            {
                maskedBool = true;
            }else
            {
                maskedBool = false;
            }

            mappatch_t mapPatch;
            for(int x = 0; x <  patchCount; x++)
            {
                file.read(reinterpret_cast<char*>(&mapPatch), sizeof(mapPatch));
                mapPatches.push_back(mapPatch);
            }

            maptexture_t mapTexture;

            mapTexture.name = string(name);
            mapTexture.masked = maskedBool;
            mapTexture.width = width;
            mapTexture.height = height;
            mapTexture.columnDirectory = columnDirectory;
            mapTexture.patchCount = patchCount;
            mapTexture.mapPatches = mapPatches;

            texture.mapTextures.push_back(mapTexture);
        }

        textureX.push_back(texture);
        file.close();
    }

            
    void ResourcesData::readPNames(WADStructure::WADStructure *wad)
    {
        WADStructure::lumpInfo_t colormapLump = wad->findLump("PNAMES");

        std::ifstream file(filePath, std::ios::binary);
        file.seekg(colormapLump.filepos);

        file.read(reinterpret_cast<char*>(&numMapPatches), sizeof(numMapPatches)); 

        for(int i = 0; i < numMapPatches; i++)
        {
            char name[8];
            file.read(reinterpret_cast<char*>(&name), sizeof(char[8])); 
            pnames.push_back(name);
        } 

        file.close();

    }

    void ResourcesData::readSprites(WADStructure::WADStructure *wad)
    {
        bool markerFound = false;
        for(int i = 0; i < wad->getDirectoryCount();i++)
        {
            if(wad->directory[i].name == "S_START")
            {
                markerFound = true;
            }
            else if(wad->directory[i].name == "S_END")
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

    void ResourcesData::readFlats(WADStructure::WADStructure *wad)
    {
        bool markerFound = false;
        for(int i = 0; i < wad->getDirectoryCount();i++)
        {
            if(wad->directory[i].name == "F_START")
            {
                markerFound = true;
            }
            else if(wad->directory[i].name == "F_END")
            {
                markerFound = false;
            }
            else if(markerFound == true)
            {
                flatsMap[wad->directory[i].name] = &(wad->directory[i]);
                flatsAmount++;
            }
        }
        cout << "reading flats..." << endl;
    }

    void ResourcesData::readPatches(WADStructure::WADStructure *wad)
    {
        bool markerFound = false;
        for(int i = 0; i < wad->getDirectoryCount();i++)
        {
            if(wad->directory[i].name == "P_START")
            {
                markerFound = true;
            }
            else if(wad->directory[i].name == "P_END")
            {
                markerFound = false;
            }
            else if(markerFound == true)
            {
                patchesMap[wad->directory[i].name] = &(wad->directory[i]);
                patchesAmount++;
            }
        }
        cout << "reading patches..." << endl;
    }
}