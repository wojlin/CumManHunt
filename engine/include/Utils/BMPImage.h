#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <stdexcept>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t signature;   // BM
    uint32_t fileSize;   // File size in bytes
    uint16_t reserved1;  // Reserved, set to 0
    uint16_t reserved2;  // Reserved, set to 0
    uint32_t dataOffset; // Offset to the start of image data in bytes
};

struct BMPInfoHeader {
    uint32_t headerSize;      // Size of the info header
    int32_t width;            // Image width
    int32_t height;           // Image height (positive for bottom-up, negative for top-down)
    uint16_t planes;          // Number of color planes (must be 1)
    uint16_t bitsPerPixel;    // Number of bits per pixel (32 for RGBA)
    uint32_t compression;     // Compression type (0 for none)
    uint32_t imageSize;       // Size of image data in bytes (can be 0 for no compression)
    int32_t xPixelsPerMeter;  // Horizontal resolution (pixels per meter)
    int32_t yPixelsPerMeter;  // Vertical resolution (pixels per meter)
    uint32_t colorsUsed;      // Number of colors in the palette (0 for full color)
    uint32_t colorsImportant; // Number of important colors (0 for all)
};

#pragma pack(pop)

class BMPImage {
public:
    BMPImage(int width, int height) : width_(width), height_(height) {
        if (width_ <= 0 || height_ <= 0) {
            throw std::invalid_argument("Width and height must be positive.");
        }

        image_.resize(width_ * height_ * 4, 0);

    }

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) {
            return; // Ignore out-of-bounds pixels
        }

        size_t index = (y * width_ + x) * 4;
        image_[index] = b;
        image_[index + 1] = g;
        image_[index + 2] = r;
        image_[index + 3] = a;
    }

    void createImage(const std::string& filename) {
        BMPFileHeader fileHeader;
        BMPInfoHeader infoHeader;

        fileHeader.signature = 0x4D42; // 'BM'
        fileHeader.reserved1 = 0;
        fileHeader.reserved2 = 0;
        fileHeader.dataOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

        infoHeader.headerSize = sizeof(BMPInfoHeader);
        infoHeader.width = width_;
        infoHeader.height = -height_; // Negative height for top-down image
        infoHeader.planes = 1;
        infoHeader.bitsPerPixel = 32;
        infoHeader.compression = 0;
        infoHeader.imageSize = 0; // Set to 0 for no compression
        infoHeader.xPixelsPerMeter = 0;
        infoHeader.yPixelsPerMeter = 0;
        infoHeader.colorsUsed = 0;
        infoHeader.colorsImportant = 0;

        fileHeader.fileSize = fileHeader.dataOffset + (width_ * height_ * 4); // 4 bytes per pixel for RGBA

        std::ofstream bmpFile(filename, std::ios::out | std::ios::binary);

        if (!bmpFile.is_open()) {
            throw std::runtime_error("Failed to open BMP file for writing.");
        }

        bmpFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
        bmpFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));
        bmpFile.write(reinterpret_cast<char*>(&image_[0]), image_.size());

        bmpFile.close();

        std::cout << "BMP image saved successfully as '" << filename << "'." << std::endl;
    }

private:
    int width_;
    int height_;
    std::vector<uint8_t> image_;
};
