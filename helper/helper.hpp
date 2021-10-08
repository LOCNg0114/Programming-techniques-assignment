//
// Created by Vu_Van_Tien on 30/08/2021.
//

#pragma once

#include <string>

#include "libs/QrCode.hpp"
#include "libs/TinyPngOut.hpp"

/* Function to create a QR Code from a message */
/*** Parameters
 * inMessage: message to be encoded
 * outSize  : pass as reference param to output the size of QR Code
 */
/*** Return
 * An 2D array in which,
 *      a value of `true` represents for a black pixel,
 *      a value of `false` represents for a white pixel
 */
bool** encodeQR(std::string inMessage, int & outSize);

/* Function to write a QR Code to a PNG image */
/*** Parameters
 * qrPtr   : pointer points to an 2D array, represents a QR Code
 * size    : size of QR Code
 * filename: name of image
 */
/*** Return: The writing process is successful (`true`) or not (`false`)
 */
bool writeQRToPNG(bool** qrPtr, int size, std::string filename);