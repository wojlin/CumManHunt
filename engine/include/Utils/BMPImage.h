#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

class BMPImage {
public:
    BMPImage(int width, int height) : width_(width), height_(height), pixelData_(width * height * 4, 0) {}

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        int index = (y * width_ + x) * 4;
        pixelData_[index] = b;
        pixelData_[index + 1] = g;
        pixelData_[index + 2] = r;
        pixelData_[index + 3] = a;
    }

    void createImage(const std::string& filename) {
        BMPHeader header;
        std::ofstream file(filename, std::ios::binary);

        // Set up the BMP header
        header.bfType = 0x4D42; // BM (Bitmap signature)
        header.bfSize = sizeof(BMPHeader) + pixelData_.size();
        header.bfReserved1 = 0;
        header.bfReserved2 = 0;
        header.bfOffBits = sizeof(BMPHeader);
        header.biSize = 40; // Size of the info header
        header.biWidth = width_;
        header.biHeight = -height_; // Negative height to indicate top-down image
        header.biPlanes = 1;
        header.biBitCount = 32; // 32-bit (RGBA) image
        header.biCompression = 0; // No compression
        header.biSizeImage = pixelData_.size(); // Image data size in bytes
        header.biXPelsPerMeter = 0;
        header.biYPelsPerMeter = 0;
        header.biClrUsed = 0;
        header.biClrImportant = 0;

        // Write the BMP header to the file
        file.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));

        // Write the pixel data (RGBA) to the file
        file.write(reinterpret_cast<const char*>(pixelData_.data()), pixelData_.size());

        file.close();
    }


private:
    int width_;
    int height_;
    std::vector<uint8_t> pixelData_;

    // Struct for BMP header
    #pragma pack(push, 1) // Ensure there's no padding between struct members
    struct BMPHeader {
        // Bitmap File Header
        uint16_t bfType;      // File type signature ('BM' for Bitmap)
        uint32_t bfSize;      // Size of the BMP file
        uint16_t bfReserved1; // Reserved (set to 0)
        uint16_t bfReserved2; // Reserved (set to 0)
        uint32_t bfOffBits;   // Offset from the beginning of the file to the pixel data
        // Bitmap Info Header
        uint32_t biSize;        // Size of the Bitmap Info Header
        int32_t biWidth;        // Width of the image in pixels
        int32_t biHeight;       // Height of the image in pixels
        uint16_t biPlanes;      // Number of color planes (set to 1)
        uint16_t biBitCount;    // Number of bits per pixel (32 for RGBA)
        uint32_t biCompression; // Compression method (set to 0 for uncompressed)
        uint32_t biSizeImage;   // Size of the image data in bytes
        int32_t biXPelsPerMeter; // Horizontal resolution in pixels per meter (set to 0)
        int32_t biYPelsPerMeter; // Vertical resolution in pixels per meter (set to 0)
        uint32_t biClrUsed;      // Number of colors in the palette (set to 0 for full color)
        uint32_t biClrImportant; // Number of important colors (set to 0)
    };
    #pragma pack(pop)
};