
#ifndef TGA_IMAGE_H
#define TGA_IMAGE_H
#include <vector>
#include <fstream>
#include <array>
using namespace std;
//initialize Image header class and stores header date
struct Header {
    char _idLength;
    char _colorMapType;
    char _dataTypeCode;
    short _colorMapOrigin;
    short _colorMapLength;
    char _colorMapDepth;
    short _xOrigin;
    short _yOrigin;
    short _width;
    short _height;
    char _bitsPerPixel;
    char _imageDescriptor;
    Header(char idLength=0,char colorMapType=0,char dataTypeCode=0,short colorMapOrigin=0,short colorMapLength=0,
    char colorMapDepth=0, short xOrigin=0,short yOrigin=0, short width=0, short height = 0,
    char bitsPerPixel=0,char imageDescriptor=0);
    void Deserialize (ifstream &filename);
};
// creates struct for pixels holding rgb values
struct Pixel {
   unsigned char Red;
   unsigned char Green;
   unsigned char Blue;
    Pixel(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
};

//creates image class holding a header and pixels
class Image {
public:
    Header image_header;
    vector<Pixel> pixels;
    Image(const Header &header  = Header(), const vector<Pixel> &pix = vector<Pixel>() );

    //reads in image data from file
    void Deserialize (ifstream &filename);

    //writes out image data to a file
    void Serialize (ofstream &filename);
};
//multiply blending method
vector <Pixel> multiply (const vector<Pixel> &a, const vector<Pixel> &b);

//subtract blending method
vector <Pixel> subtract (const vector<Pixel> &a, const vector<Pixel> &b);

//screen blending method
vector <Pixel> Screen (const vector<Pixel> &a, const vector<Pixel> &b);

//overlay blending method
vector <Pixel> Overlay (const vector<Pixel> &a, const vector<Pixel> &b);

//allows for you to add numeric value to color channel of image
vector <Pixel> AddToChanel(const vector<Pixel> &a, string colorChannel, int increase);

//allows for you to scale a color channel
vector <Pixel> ScaleChannel(const vector<Pixel> &a, string colorChannel, int scaleSize);

//splits image into three components
void Split (const vector<Pixel> &a,vector<Pixel> &red,vector<Pixel> &green, vector<Pixel> &blue);

//combines three separate component files into one file
vector <Pixel> Combine (const vector<Pixel> &a, const vector<Pixel> &b, const vector<Pixel> &c);

//rotates the image
vector <Pixel> Rotate180 (const vector<Pixel> &a);

//reads in the data
void GetImageData(const string& filename);

//accomplish tasks 1-10 respectively
void taskOne();
void taskTwo();
void taskThree();
void taskFour();
void taskFive();
void taskSix();
void taskSeven();
void taskEight();
void taskNine();
void taskTen();

#endif //TGA_IMAGE_H
