#include "../../include/GameData/ResourcesData.h"
namespace ResourcesData
{
    Image::Image()
    {

    }

    Image::Image(ColorMapData::ColorMapData*  colormapPointer, PlayPalData::PlayPalData*  playpalPointer, uint16_t w,  uint16_t h,  int16_t l,  int16_t t, vector<imageColumn_t> c, int lSize, string lName)
    {
        width = w;
        height = h;
        leftOffset = l;
        topOffset = t;
        columns = c;
        size = lSize;
        name = lName;
        pallete = 0;
        playpal = playpalPointer;
        colormap = colormapPointer;

        for(int x = 0; x < width; x++)
        {
            pixels.push_back(vector<PlayPalData::colorRGB_t>());
            pixelsPlayPal.push_back(vector<uint8_t>());

            for(int y = 0; y < height; y++)
            {
                PlayPalData::colorRGB_t color;
                color.red = 255;
                color.green = 255;
                color.blue = 255;
                color.transparent = true;
                pixels[x].push_back(color);
                pixelsPlayPal[x].push_back(255);
            }
        }

        for(int c = 0; c < columns.size(); c++)
        {
            for(int i = 0; i < columns[c].pixelCount; i++)
            {
                PlayPalData::colorRGB_t color = playpal->getColor(pallete, columns[c].pixels[i]);
                color.transparent = false;
                pixels[columns[c].column][columns[c].rowStart + i] = color;
                pixelsPlayPal[columns[c].column][columns[c].rowStart + i] = columns[c].pixels[i];
            }              
        }
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

    string Image::getName()
    {
        return name;
    }

    void Image::setPallete(int value)
    {
        pallete = value;
    }

    int Image::getPallete()
    {
        return pallete;
    }

    PlayPalData::colorRGB_t Image::getPixel(int x, int y, int lightLevel)
    {
        int colormapIndex = 31 - static_cast<int>((static_cast<double>(lightLevel) / 255) * 31);
        uint8_t color = pixelsPlayPal[y][x];
        uint8_t remappedColor = colormap->getColor(colormapIndex, color);
        PlayPalData::colorRGB_t newColor = playpal->getColor(pallete, remappedColor);
        newColor.transparent = false;
        return newColor;
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
                uint8_t color = columns[x].pixels[b];
                PlayPalData::colorRGB_t pixel = playpal->getColor(pallete, color);
                image.setPixel(columns[x].column, columns[x].rowStart + b, pixel.red, pixel.green, pixel.blue, 255);
            }

        }
        image.createImage(filePath);
        cout << "Image Saved!" << endl;
    }



    ResourcesData::ResourcesData()
    {

    }

    ResourcesData::ResourcesData(WADStructure::WADStructure *wad, PlayPalData::PlayPalData*  playpalPointer, ColorMapData::ColorMapData*  colormapPointer): filePath(wad->filePath), playpal(playpalPointer), colormap(colormapPointer)
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
            string error = "The '" + name + "' is not in the game resources.";
            throw ResourceReadoutException(error);
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
                column.pixels.push_back(value);
            }
            columns.push_back(column);
        }

        file.close();

        return Image(colormap, playpal, width, height, 0, 0, columns, spriteInfo->size, name);

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
            string error = "The '" + name + "' is not in the game resources.";
            throw ResourceReadoutException(error);
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
                    //PlayPalData::colorRGB_t color = playpal->getColor(0, value);
                    column.pixels.push_back(value);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(colormap, playpal, spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo->size, name);

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
            string error = "The '" + name + "' is not in the game resources.";
            throw ResourceReadoutException(error);
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
                    //PlayPalData::colorRGB_t color = playpal->getColor(0, value);
                    column.pixels.push_back(value);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(colormap, playpal, spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo->size, name);

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
                column.pixels.push_back(value);
            }
            columns.push_back(column);
        }

        file.close();

        return Image(colormap, playpal, width, height, 0, 0, columns, spriteInfo.size, name);

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
                    //PlayPalData::colorRGB_t color = playpal->getColor(0, value);
                    column.pixels.push_back(value);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(colormap, playpal, spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo.size, name);

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
                    //PlayPalData::colorRGB_t color = playpal->getColor(0, value);
                    column.pixels.push_back(value);
                }
                columns.push_back(column);
                file.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
            }
        }
        file.close();
        return Image(colormap, playpal, spriteData.width, spriteData.height, spriteData.leftOffset, spriteData.topOffset, columns, spriteInfo.size, name);

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

    string ResourcesData::getPNameByIndex(int index)
    {
        for(int i = 0; i < pnames.size(); i++)
        {
            if(i == index)
            {
                return pnames[i];
            }
        }
        string error =  "pname of index '" + to_string(index) + "' does not exist";
        throw ResourceReadoutException(error);
    }

    int ResourcesData::getPNameIndexByName(string name)
    {
        for(int i = 0; i < pnames.size(); i++)
        {
            if(pnames[i] == name)
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
            string convertedString = toUpper(charsToString(name, 8));
            pnames.push_back(convertedString);
        } 

        file.close();

    }

     string ResourcesData::charsToString(char* chars, int size)
    {   
        string str(chars, size);
        return str.c_str();
    }

    string ResourcesData::toUpper(string name)
    {
        for (char &c : name) {
        c = std::toupper(c);
        }
        return name;
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
                if(wad->directory[i].size != 0)
                {   
                    patchesMap[wad->directory[i].name] = &(wad->directory[i]);
                    patchesAmount++;
                }   
            }
        }
        cout << "reading patches..." << endl;
    }
}