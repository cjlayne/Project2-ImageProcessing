#include "TGA_Image.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <array>
using namespace std;
//constructor for Header class
Header::Header(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin, short colorMapLength,
    char colorMapDepth, short xOrigin, short yOrigin, short width, short height, char bitsPerPixel, char imageDescriptor) {
     _idLength = idLength;
     _colorMapType = colorMapType;
     _dataTypeCode = dataTypeCode;
     _colorMapOrigin = colorMapOrigin;
     _colorMapLength=colorMapLength;
     _colorMapDepth=colorMapDepth;
     _xOrigin=xOrigin;
    _yOrigin=yOrigin;
     _width=width;
     _height=height;
     _bitsPerPixel=bitsPerPixel;
     _imageDescriptor=imageDescriptor;
}

//reads in header data from file and adds to header class
void Header::Deserialize(ifstream &filename) {
    filename.read(reinterpret_cast<char*>(&_idLength), sizeof(_idLength));
    filename.read(reinterpret_cast<char*>(&_colorMapType), sizeof(_colorMapType));
    filename.read(reinterpret_cast<char*>(&_dataTypeCode), sizeof(_dataTypeCode));
    filename.read(reinterpret_cast<char*>(&_colorMapOrigin), sizeof(_colorMapOrigin));
    filename.read(reinterpret_cast<char*>(&_colorMapLength), sizeof(_colorMapLength));
    filename.read(reinterpret_cast<char*>(&_colorMapDepth), sizeof(_colorMapDepth));
    filename.read(reinterpret_cast<char*>(&_xOrigin), sizeof(_xOrigin));
    filename.read(reinterpret_cast<char*>(&_yOrigin), sizeof(_yOrigin));
    filename.read(reinterpret_cast<char*>(&_width), sizeof(_width));
    filename.read(reinterpret_cast<char*>(&_height), sizeof(_height));
    filename.read(reinterpret_cast<char*>(&_bitsPerPixel), sizeof(_bitsPerPixel));
    filename.read(reinterpret_cast<char*>(&_imageDescriptor), sizeof(_imageDescriptor));
}
//default constructor for pixel
Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b) {
    Red = r;
    Green = g;
    Blue = b;
}
//default constructor for image
Image::Image(const Header& header, const vector<Pixel>& pix)
    : image_header(header), pixels(pix) {}

//reads in image data from file and adds to image class
void Image::Deserialize(ifstream &filename) {
    if (!filename.is_open()) {
        cout << "Error: File stream is not open." << endl;
        return;
    }
    image_header.Deserialize(filename);

        int pixelCount = image_header._width * image_header._height;
        pixels.resize(pixelCount);
    //loops through pixel data and stores as a vector of pixel objects
        for (int i = 0; i < pixelCount; ++i) {
            filename.read(reinterpret_cast<char*>(&pixels[i].Blue), 1);
            filename.read(reinterpret_cast<char*>(&pixels[i].Green), 1);
            filename.read(reinterpret_cast<char*>(&pixels[i].Red), 1);
        }
}

//writes data from image class to a file
void Image::Serialize(ofstream &filename) {
    if (!filename.is_open()) {
        cout << "Error: File stream is not open." << endl;
        return;
    }
    filename.write(reinterpret_cast<char*>(&image_header._idLength), sizeof(image_header._idLength));
    filename.write(reinterpret_cast<char*>(&image_header._colorMapType), sizeof(image_header._colorMapType));
    filename.write(reinterpret_cast<char*>(&image_header._dataTypeCode), sizeof(image_header._dataTypeCode));
    filename.write(reinterpret_cast<char*>(&image_header._colorMapOrigin), sizeof(image_header._colorMapOrigin));
    filename.write(reinterpret_cast<char*>(&image_header._colorMapLength), sizeof(image_header._colorMapLength));
    filename.write(reinterpret_cast<char*>(&image_header._colorMapDepth), sizeof(image_header._colorMapDepth));
    filename.write(reinterpret_cast<char*>(&image_header._xOrigin), sizeof(image_header._xOrigin));
    filename.write(reinterpret_cast<char*>(&image_header._yOrigin), sizeof(image_header._yOrigin));
    filename.write(reinterpret_cast<char*>(&image_header._width), sizeof(image_header._width));
    filename.write(reinterpret_cast<char*>(&image_header._height), sizeof(image_header._height));
    filename.write(reinterpret_cast<char*>(&image_header._bitsPerPixel), sizeof(image_header._bitsPerPixel));
    filename.write(reinterpret_cast<char*>(&image_header._imageDescriptor), sizeof(image_header._imageDescriptor));
    int pixelCount = image_header._width * image_header._height;
    for (int i = 0; i <pixelCount; ++i) {
        filename.write(reinterpret_cast<const char*>(&pixels[i].Blue), 1);
        filename.write(reinterpret_cast<const char*>(&pixels[i].Green), 1);
        filename.write(reinterpret_cast<const char*>(&pixels[i].Red), 1);
    }

}

//multiply blending method
vector <Pixel> multiply (const vector<Pixel> &a, const vector<Pixel> &b) {
    vector <Pixel> result;
    unsigned char red;
    unsigned char blue;
    unsigned char green;
    for (unsigned int i=0; i < a.size(); ++i) {

        red = static_cast<unsigned char>((a[i].Red * b[i].Red)/255) ;

        green = static_cast<unsigned char>((a[i].Green * b[i].Green) / 255);

        blue = static_cast<unsigned char>((a[i].Blue * b[i].Blue) / 255);

        result.push_back(Pixel(red,green,blue));
    }
    return result;
}

//subtract blending method
vector <Pixel> subtract (const vector<Pixel> &a, const vector<Pixel> &b) {
    vector <Pixel> result;
   unsigned char red;
   unsigned char blue;
   unsigned char green;
    for (unsigned int i=0; i < a.size(); ++i) {
            //converts data to int to prevent overflow
            int redTemp = static_cast<int>(a[i].Red) - static_cast<int>(b[i].Red);
            int greenTemp = static_cast<int>(a[i].Green) - static_cast<int>(b[i].Green);
            int blueTemp = static_cast<int>(a[i].Blue) - static_cast<int>(b[i].Blue);

        //checks for negative values and sets correct value
            if (redTemp < 0) {
                red = 0;
            } else {
                red = static_cast<unsigned char>(redTemp);
            }
            if (greenTemp <0) {
                green = 0;
            } else {
                green = static_cast<unsigned char>(greenTemp);
            }
            if (blueTemp <0) {
                blue = 0;
            } else {
                blue = static_cast<unsigned char>(blueTemp);
            }
            result.push_back(Pixel(red,green,blue));
        }
        return result;
    }

//screen blending method
vector <Pixel> Screen (const vector<Pixel> &a, const vector<Pixel> &b) {
    vector <Pixel> result;
    unsigned red;
    unsigned char blue;
    unsigned char green;

    for (unsigned int i = 0; i < a.size(); ++i) {

            red = static_cast<unsigned char>(255 -  ((255 - a[i].Red) * (255 - b[i].Red)/255));

            green =static_cast<unsigned char> (255 -  ((255 - a[i].Green) * (255 - b[i].Green)/255));

            blue = static_cast<unsigned char>(255 - ((255 - a[i].Blue) * (255 - b[i].Blue)/255));
            result.push_back(Pixel(red,green,blue));
        }
    return result;
}

//overlay blending methos
vector <Pixel> Overlay (const vector<Pixel> &a, const vector<Pixel> &b) {
    vector <Pixel> result;
    unsigned char red;
    unsigned char blue;
    unsigned char green;

    for (unsigned int i = 0; i < a.size(); ++i) {
        if (a[i].Red < 128 ) {
            red = static_cast<unsigned char> (2 * a[i].Red * b[i].Red / 255);
        }
        else {
            red = static_cast<unsigned char>(255 - 2 * (255 - a[i].Red) * (255 - b[i].Red) / 255);
        }
        if (a[i].Green < 128) {
            green =static_cast<unsigned char>(2 * a[i].Green * b[i].Green / 255);
        }
        else {
            green =static_cast<unsigned char>(255 - 2 * (255 - a[i].Green) * (255 - b[i].Green) / 255);
        }
        if (a[i].Blue < 128) {
            blue = static_cast<unsigned char>(2 * a[i].Blue * b[i].Blue / 255);
        }
        else {
            blue = static_cast<unsigned char>(255 - 2 * (255 - a[i].Blue) * (255 - b[i].Blue) / 255);
        }
        result.push_back(Pixel(red,green,blue));
    }

    return result;
    }

//alows user to add numeric value to a color channel
vector <Pixel> AddToChanel(string colorChannel, int increase,const vector<Pixel> &a) {
    vector <Pixel> result;
    unsigned char red;
    unsigned char blue;
    unsigned char green;
    if (colorChannel == "r") {
        for (unsigned int i = 0; i < a.size(); ++i) {
           unsigned int redTemp = static_cast<unsigned int> (a[i].Red + increase);
            if (redTemp > 255){red = 255;}
            else {red = static_cast<unsigned char> (redTemp);}
            result.push_back(Pixel(red,a[i].Green,a[i].Blue));
        }
    }
    else if (colorChannel == "g") {
        for (unsigned int i = 0; i < a.size(); ++i) {
            unsigned int greenTemp = static_cast<unsigned int> (a[i].Green + increase);
            if (greenTemp > 255){green = 255;}
            else {green = static_cast<unsigned char> (greenTemp);}
            result.push_back(Pixel(a[i].Red,green,a[i].Blue));
        }
    }
    else if (colorChannel == "b") {
        for (unsigned int i = 0; i < a.size(); ++i) {
            unsigned int blueTemp = static_cast<unsigned int> (a[i].Blue + increase);
            if (blueTemp > 255){blue = 255;}
            else {blue= static_cast<unsigned char> (blueTemp);}
            result.push_back(Pixel(a[i].Red,a[i].Green,blue));
        }
    }
    else {
        cout << "Invalid color option"<< endl;
    }

    return result;
}

//allows user to scale a specific color channel
vector <Pixel> ScaleChannel(string colorChannel, int scaleSize, const vector<Pixel> &a) {
    vector <Pixel> result;
    unsigned char red;
    unsigned char blue;
    unsigned char green;
    if (colorChannel == "r") {
        for (unsigned int i = 0; i < a.size(); ++i) {
            unsigned int redTemp = static_cast<unsigned int> (a[i].Red  * scaleSize);
            if (redTemp > 255){red = 255;}
            else {red = static_cast<unsigned char> (redTemp);}
            result.push_back(Pixel(red,a[i].Green,a[i].Blue));
        }
    }
    else if (colorChannel == "g") {
        for (unsigned int i = 0; i < a.size(); ++i) {
            unsigned int greenTemp = static_cast<unsigned int> (a[i].Green * scaleSize);
            if (greenTemp > 255){green = 255;}
            else {green = static_cast<unsigned char> (greenTemp);}

            result.push_back(Pixel(a[i].Red,green,a[i].Blue));
        }
    }
    else if (colorChannel == "b") {
        for (unsigned int i = 0; i < a.size(); ++i) {
            unsigned int blueTemp = static_cast<unsigned char> (a[i].Blue *scaleSize);
            if (blueTemp > 255){blue = 255;}
            else {blue= static_cast<unsigned char> (blueTemp);}
            result.push_back(Pixel(a[i].Red,a[i].Green,blue));
        }
    }
    else {
        cout << "Invalid color option"<< endl;
    }

    return result;




}

//splits image into 3 different files red green and blue components
void Split (const vector<Pixel> &a,vector<Pixel> &redComponent,vector<Pixel> &greenComponent, vector<Pixel> &blueComponent) {


    for (unsigned int i = 0; i < a.size(); ++i) {
        redComponent.push_back(Pixel(a[i].Red,a[i].Red,a[i].Red));
        greenComponent.push_back(Pixel(a[i].Green,a[i].Green,a[i].Green));
        blueComponent.push_back(Pixel(a[i].Blue,a[i].Blue,a[i].Blue));
    }}

//combined 3 component files into one imgae file
vector <Pixel> Combine (const vector<Pixel> &red, const vector<Pixel> &green, const vector<Pixel> &blue) {
    vector<Pixel> result;
    for (unsigned int i = 0; i < red.size(); ++i) {
        result.push_back(Pixel(red[i].Red,green[i].Green,blue[i].Blue));
    }


return result;

}

//rotates the image 180 degrees
vector <Pixel> Rotate180 (const vector<Pixel> &a) {
vector<Pixel> result;
    for (auto iter = a.rbegin(); iter != a.rend(); ++iter) {
        result.push_back(*iter);
    }
    return  result;
}

//reads in data from a file
void GetImageData(const string &filename,vector<Image> &Images) {
    ifstream inFile(filename, ios_base::binary);
    if (!inFile) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }
    Image someImage;
    someImage.Deserialize(inFile);
    Images.push_back(someImage);
}


void taskOne() {
    vector<Image> Images;
    GetImageData("input/layer1.tga",Images);
    GetImageData("input/pattern1.tga", Images);
    Image result;
    cout << Images.size();
    result.image_header = Images[0].image_header;
    result.pixels = multiply(Images[0].pixels,Images[1].pixels);
    ofstream part1("output/part1.tga", ios::binary);
    result.Serialize(part1);
    part1.close();
    cout << "Task 1 Complete" << endl;
}
void taskTwo() {
    vector<Image> Images;
    GetImageData("input/layer2.tga",Images);
    GetImageData("input/car.tga", Images);
    Image result;
    result.image_header = Images[0].image_header;
    result.pixels = subtract(Images[1].pixels, Images[0].pixels);
    ofstream part2("output/part2.tga", ios::binary);
    result.Serialize(part2);
    part2.close();
    cout << "Task 2 Complete" << endl;
}
void taskThree() {
    vector<Image> Images;
    GetImageData("input/layer1.tga",Images);
    GetImageData("input/pattern2.tga", Images);
    vector<Pixel> multiplyResult;
    multiplyResult = multiply(Images[0].pixels,Images[1].pixels);
    GetImageData("input/text.tga",Images);
    Image screenResult;
    screenResult.image_header = Images[2].image_header;
    screenResult.pixels = Screen(Images[2].pixels,multiplyResult);
    ofstream part3("output/part3.tga", ios::binary);
    screenResult.Serialize(part3);
    part3.close();
    cout << "Task 3 Complete" << endl;

}
void taskFour() {
    vector<Image> Images;
    GetImageData("input/layer2.tga",Images);
    GetImageData("input/circles.tga", Images);
    vector<Pixel> multiplyResult;
    multiplyResult = multiply(Images[0].pixels,Images[1].pixels);
    GetImageData("input/pattern2.tga",Images);
    Image subtractResult;
    subtractResult.image_header = Images[2].image_header;
    subtractResult.pixels = subtract(multiplyResult,Images[2].pixels);
    ofstream part4("output/part4.tga", ios::binary);
    subtractResult.Serialize(part4);
    part4.close();
    cout << "Task 4 Complete" << endl;
}
void taskFive() {
    vector<Image> Images;
    GetImageData("input/layer1.tga",Images);
    GetImageData("input/pattern1.tga", Images);
    Image result;
    result.image_header = Images[0].image_header;
    result.pixels = Overlay(Images[1].pixels,Images[0].pixels);
    ofstream part5("output/part5.tga", ios::binary);
    result.Serialize(part5);
    part5.close();
    cout << "Task 5 Complete" << endl;
}
void taskSix() {
    vector<Image> Images;
    GetImageData("input/car.tga",Images);
    Image result;
    result.image_header = Images[0].image_header;
    result.pixels = AddToChanel("g",200, Images[0].pixels);
    ofstream part6("output/part6.tga", ios::binary);
    result.Serialize(part6);
    part6.close();
    cout << "Task 6 Complete" << endl;
}
void taskSeven() {
    vector<Image> Images;
    GetImageData("input/car.tga",Images);
    Image result;
    result.image_header = Images[0].image_header;
    result.pixels = ScaleChannel("r",4, Images[0].pixels);
    result.pixels = ScaleChannel("b",0,result.pixels);
    ofstream part7("output/part7.tga", ios::binary);
    result.Serialize(part7);
    part7.close();
    cout << "Task 7 Complete" << endl;
}
void taskEight() {
    vector<Image> Images;
    GetImageData("input/car.tga",Images);
    Image redImage,greenImage,blueImage;
    redImage.image_header = Images[0].image_header;
    greenImage.image_header = Images[0].image_header;
    blueImage.image_header = Images[0].image_header;
    Split(Images[0].pixels,redImage.pixels,greenImage.pixels,blueImage.pixels);
    ofstream redFile("output/part8_r.tga", ios::binary);
    redImage.Serialize(redFile);
    redFile.close();

    ofstream greenFile("output/part8_g.tga", ios::binary);
    greenImage.Serialize(greenFile);
    greenFile.close();
    ofstream blueFile("output/part8_b.tga", ios::binary);
    blueImage.Serialize(blueFile);
    blueFile.close();
    cout << "Task 8 Complete" << endl;
}
void taskNine() {
    vector<Image> Images;
    GetImageData("input/layer_red.tga",Images);
    GetImageData("input/layer_green.tga",Images);
    GetImageData("input/layer_blue.tga",Images);
    Image result;
    result.image_header = Images[0].image_header;
    result.pixels = Combine(Images[0].pixels,Images[1].pixels,Images[2].pixels);
    ofstream part9("output/part9.tga", ios::binary);
    result.Serialize(part9);
    part9.close();
    cout << "Task 9 Complete" << endl;
}
void taskTen() {
    vector<Image> Images;
    GetImageData("input/text2.tga",Images);
    Image result;
    result.image_header = Images[0].image_header;
    result.pixels = Rotate180(Images[0].pixels);
    ofstream part10("output/part10.tga", ios::binary);
    result.Serialize(part10);
    part10.close();
    cout << "Task 10 Complete" << endl;

}

