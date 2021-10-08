//
// Created by Vu_Van_Tien on 30/08/2021.
//

#include <iostream>
#include <experimental/filesystem>
#include <fstream>

#include "helper.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

const bool OVERWRITE_EXISTING_FILE = true; // hard code - quick setup
int BORDER = 2; // hard code - quick setup
int IMAGE_SIZE = 450; // hard code - quick setup
const int MIN_MODULE_PIXEL_SIZE = 3; // hard code - quick setup

/* BEGIN Forward declaration */
template<typename T>
T** allocateDynamic2D(int H, int W);

bool _writeToPNG(bool** qrPtr, int size, string filename);
int _imgSizeWithBorder(int qrSize);
/* END Forward declaration */

bool** encodeQR(std::string inMessage, int & outSize) {
    auto ecc = qrcodegen::QrCode::Ecc::LOW; // just for declaration
    if (inMessage.length() <= 36) {
        ecc = qrcodegen::QrCode::Ecc::MEDIUM;
    }
    else {
        ecc = qrcodegen::QrCode::Ecc::LOW;
    }
    auto qr = qrcodegen::QrCode::encodeText(inMessage.c_str(), ecc);

    outSize = qr.getSize();
    bool ** modulePtr = allocateDynamic2D<bool>(outSize, outSize);

    for (int r = 0; r < outSize; ++r) { // row
        for (int c = 0; c < outSize; ++c) { // colums
            modulePtr[r][c] = qr.getModule(c, r);
        }
    }

    return modulePtr;
}

bool writeQRToPNG(bool** qrPtr, int size, std::string filename) {
    if (qrPtr == NULL) {
        cerr << "The QR Code pointer is NULL" << endl;
        exit(1);
    }

    if (size <= 25) {
        IMAGE_SIZE = 500;
        BORDER = 2;
    }
    else {
        IMAGE_SIZE = 600;
        BORDER = 2;
    }

    if (!OVERWRITE_EXISTING_FILE && fs::exists(filename)) {
        return false;
    }

    if (OVERWRITE_EXISTING_FILE && fs::exists(filename)) {
        if (!fs::copy_file(filename, filename+".tmp", fs::copy_options::overwrite_existing)) {
            return false;
        }
    }

    auto result = _writeToPNG(qrPtr, size, filename);

    if (result) {
        fs::remove(filename+".tmp");
    }

    return result;
}

bool _writeToPNG(bool** qrPtr, int qrSize, string filename) {
    ofstream out(filename.c_str(), std::ios::binary);
    auto t = out.fail();
    int pngWH = _imgSizeWithBorder(qrSize);
    TinyPngOut pngout(pngWH, pngWH, out);

    auto qrSizeWithBorder = qrSize + BORDER;

    if (qrSizeWithBorder > IMAGE_SIZE) {
        return false; // qrcode doesn't fit
    }

    int qrSizeFitsInMaxImgSizeTimes = IMAGE_SIZE / qrSizeWithBorder;
    int pixelsWHPerModule = qrSizeFitsInMaxImgSizeTimes;

    if (qrSizeFitsInMaxImgSizeTimes < MIN_MODULE_PIXEL_SIZE)
        return false; // image would be to small to scan

    std::vector<uint8_t> tmpData;
    const uint8_t blackPixel = 0x00;
    const uint8_t whitePixel = 0xFF;

    /* The below loop converts the qrData to RGB8.8.8 pixels and writes it with
     * the tinyPNGoutput library. since we probably have requested a larger
     * qr module pixel size we must transform the qrData modules to be larger
     * pixels (than just 1x1). */

    // border above
    for (int i = 0; i < pngWH; i++) // row
        for (int j = 0; j < pixelsWHPerModule; j++) // module pixel (height)
            tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

    pngout.write(tmpData.data(), static_cast<size_t>(tmpData.size() / 3));
    tmpData.clear();

    for (int qrModuleAtY = 0; qrModuleAtY < qrSize; qrModuleAtY++) {
        for (int col = 0; col < pixelsWHPerModule; col++) {
            // border left
            for (int i = 0; i < qrSizeFitsInMaxImgSizeTimes; ++i)
                tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

            // qr module to pixel
            for (int qrModuleAtX = 0; qrModuleAtX < (qrSize); qrModuleAtX++) {
                for (int row = 0; row < qrSizeFitsInMaxImgSizeTimes; ++row) {
                    // if (qrData.getModule(qrModuleAtX, qrModuleAtY)) {
                    if (qrPtr[qrModuleAtY][qrModuleAtX]) {
                        // insert saves us a for loop or 3 times the same line.
                        tmpData.insert(tmpData.end(), {blackPixel, blackPixel, blackPixel});
                    } else {
                        tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});
                    }
                }
            }
            // border right
            for (int i = 0; i < qrSizeFitsInMaxImgSizeTimes; ++i)
                tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

            // write the entire  row
            pngout.write(tmpData.data(), static_cast<size_t>(tmpData.size() / 3));
            tmpData.clear();
        }
    }

    // border below
    for (int i = 0; i < pngWH; i++) // row
        for (int j = 0; j < pixelsWHPerModule; j++) // module pixel (height)
            tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

    pngout.write(tmpData.data(), static_cast<size_t>(tmpData.size() / 3));
    tmpData.clear();
    out.close();

    return fs::exists(filename);
}

template <typename T>
T** allocateDynamic2D(int H, int W) {
    T** p2D = new T*[H];
    for (int h = 0; h < H; ++h) {
        p2D[h] = new T[W];
    }
    return p2D;
}

int _imgSizeWithBorder(int qrSize) {
    return (IMAGE_SIZE / (qrSize + BORDER)) * (qrSize + BORDER);
}
