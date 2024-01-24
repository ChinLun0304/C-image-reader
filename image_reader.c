#include<stdio.h>
#include<stdlib.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

#pragma pack(1)
typedef struct{
    char CharB;
    char CharM;
    UINT32 Size;
    UINT32 Reserved;
    UINT32 ImageOffset;

    UINT32 HeaderSize;
    UINT32 PixelWidth;
    UINT32 PixelHeight;
    UINT16 Planes;
    UINT16 BitPerPixel;
    UINT32 CompressionType;
    UINT32 ImageSize;
    UINT32 XPixelPerMeter;
    UINT32 YPixelPerMeter;
    UINT32 NumberOfColors;
    UINT32 ImportantColors;

}Header;

#pragma pack()

typedef struct{

    Header *ImageHeader;
    UINT8 header[14];
    UINT8 headerInfo[40];
    UINT8 *data;

}IplImage;

typedef struct{

    UINT8 B,G,R;

}Pixel;



IplImage *LoadImage(char *filename){

    IplImage *src=(IplImage *)malloc(sizeof(IplImage));
    FILE *file = fopen(filename, "rb");
    UINT8 *ImageHeaderBuffer = (UINT8 *)malloc(sizeof(UINT8)*54);

    fread(ImageHeaderBuffer, sizeof(UINT8), sizeof(UINT8)*54, file);
    src->ImageHeader = (Header *)ImageHeaderBuffer;

    memcpy(src->header, ImageHeaderBuffer, sizeof(src->header));
    memcpy(src->headerInfo, ImageHeaderBuffer+14, sizeof(src->headerInfo));

    src->data = malloc(sizeof(UINT8)*src->ImageHeader->PixelWidth*src->ImageHeader->PixelHeight*3);
    fseek(file, src->ImageHeader->ImageOffset, SEEK_SET);
    fread(src->data, sizeof(UINT8),sizeof(UINT8)*src->ImageHeader->PixelWidth*src->ImageHeader->PixelHeight*3, file );
    fclose(file);

    return src;
}

void ShowImage(IplImage* src){

    printf("===Header===\n");
    printf("Signature=%c\n", src->ImageHeader->CharB);
    printf("Signature=%c\n", src->ImageHeader->CharM);

    printf("File Size = %d\n", src->ImageHeader ->Size);
    printf("Offset = %d\n", src->ImageHeader ->ImageOffset);

    printf("===Info===\n");
    printf("Size = %d\n", src->ImageHeader ->HeaderSize);
    printf("Width = %d\n", src->ImageHeader ->PixelWidth);
    printf("Height = %d\n", src->ImageHeader ->PixelHeight);
    printf("Planes = %d\n", src->ImageHeader ->Planes);
    printf("BitsPerPixel = %d\n", src->ImageHeader ->BitPerPixel);
    printf("Compression = %ld\n", src->ImageHeader ->CompressionType);
    printf("ImageSize = %ld\n", src->ImageHeader ->ImageSize);
    printf("XpixelsPerM = %ld\n", src->ImageHeader ->XPixelPerMeter);
    printf("YpixelsPerM = %ld\n", src->ImageHeader ->YPixelPerMeter);
    printf("ColorsUsed = %ld\n", src->ImageHeader ->XPixelPerMeter);

    printf("ColorsImportant=%ld\n", src->ImageHeader->ImportantColors);



}

void SaveImage(IplImage *src, char *filename){

    FILE *file = fopen(filename, "wb");
    fwrite(src->header, sizeof(UINT8), sizeof(src->header), file);
    fwrite(src->headerInfo, sizeof(UINT8), sizeof(src->headerInfo), file);
    fseek(file, src->ImageHeader->ImageOffset, SEEK_SET);
    fwrite(src->data, sizeof(UINT8),sizeof(UINT8)*src->ImageHeader->PixelWidth*src->ImageHeader->PixelHeight*3, file );
    fclose(file);
}

void SetPixel(IplImage *src, int x, int y){

    Pixel *pixel;
    int index = y+src->ImageHeader->PixelWidth*3*x;

    pixel = (Pixel*)&src->data[index];
    UINT8 Gray = pixel->R*0.3+pixel->G*0.6+pixel->B*0.1;
    pixel->R = Gray;
    pixel->G = Gray;
    pixel->B = Gray;

}
void SetScalar(IplImage *src){

    for(int i =0; i<src->ImageHeader->PixelHeight; i++){
        for(int j=0; j< src->ImageHeader->PixelWidth*3; j++){

            SetPixel(src, i, j);

        }


    }



}

void ReleaseImage(IplImage **src){

    if(src !=NULL && *src!=NULL){
        free((*src)->ImageHeader);
        free((*src)->data);
        free(*src);

    }
    *src=NULL;

}


int main(){

    IplImage *src = LoadImage("Logo.bmp");
    ShowImage(src);
    SetScalar(src);
    SaveImage(src, "ddd.bmp");

    ReleaseImage(&src);


}
