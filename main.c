
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char u8;
/**
 * @description: 将图片旋转90°
 */
int rotateImage();
/**
 * @description: 显示图像参数
 */
void showData(BITMAPFILEHEADER file,BITMAPINFOHEADER info);
int main(){
    rotateImage();
    return 1;
}
void showData(BITMAPFILEHEADER file,BITMAPINFOHEADER info){
    printf("文件大小[bfsize] = %ld\n",file.bfSize);
    printf("偏移[bfOffBits] = %ld\n",file.bfOffBits);
    printf("图像宽度[biWidth] = %ld\n",info.biWidth);
    printf("图像高度[biHeight] = %ld\n",info.biHeight);
    printf("位深[biBitCount] = %d\n",info.biBitCount);
    printf("调色板[biPlanes] = %d\n",info.biPlanes);
}
int rotateImage(){
    FILE *fp = fopen("D:\\Cminiproject\\demo2\\view_256.bmp", "rb");
    if (fp == 0){
        printf("文件打开失败\n");
        return 0;
    }
    BITMAPFILEHEADER input_fileHead;
    BITMAPINFOHEADER input_infoHead;
    fread(&input_fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&input_infoHead, sizeof(BITMAPINFOHEADER), 1, fp);
    printf("源图片\n");
    showData(input_fileHead,input_infoHead);
    int width = input_infoHead.biWidth;
    int height = input_infoHead.biHeight;
    int biCount = input_infoHead.biBitCount;
    int lineByte = (width*biCount / 8 + 3) / 4 * 4;

    RGBQUAD *input_ColorTable;
    // lineByte*height
    input_ColorTable = (RGBQUAD*) malloc(256*sizeof(RGBQUAD));
    fread(input_ColorTable, sizeof(RGBQUAD), 256, fp);
    u8 *input_BmpBuf;
    input_BmpBuf = (u8*) malloc(lineByte*height*sizeof(u8));
    fread(input_BmpBuf, lineByte*height, 1, fp);
    fclose(fp);
    u8 *output_BmpBuf;
    int output_lineByte = (height*biCount / 8 + 3) / 4 * 4;
    // output_lineByte*width
    output_BmpBuf = (u8*) malloc(output_lineByte*width*sizeof(u8));
    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){
            u8 *p;
            p = (u8 *)(output_BmpBuf + output_lineByte*i + j);
            (*p) = 255;
        }
    }
    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){
            u8 *input_pic,*output_pic;
            input_pic = (u8 *)(input_BmpBuf + lineByte*j + i);  // 原图像
            // 逆时针旋转90°
            output_pic = (u8 *)(output_BmpBuf + output_lineByte*i + height-j-1);// 新图像
            (*output_pic) = (*input_pic);
        }
    }
    FILE *fpo = fopen("D:\\Cminiproject\\demo2\\output.bmp", "wb");
    if (fpo == 0)
        return 0;
    fwrite(&input_fileHead, sizeof(BITMAPFILEHEADER), 1, fpo);
    BITMAPINFOHEADER output_infoHead;
    output_infoHead.biBitCount = biCount;
    output_infoHead.biClrImportant = 0;
    output_infoHead.biClrUsed = 0;
    output_infoHead.biCompression = 0;
    output_infoHead.biHeight = width;
    output_infoHead.biPlanes = 1;
    output_infoHead.biSize = 40;
    output_infoHead.biSizeImage = output_lineByte*width;
    output_infoHead.biWidth = height;
    output_infoHead.biXPelsPerMeter = 0;
    output_infoHead.biYPelsPerMeter = 0;
    printf("--------------------------------------\n");
    printf("旋转后图片\n");
    showData(input_fileHead,output_infoHead);
    fwrite(&output_infoHead, sizeof(BITMAPINFOHEADER), 1, fpo);
    fwrite(input_ColorTable, sizeof(RGBQUAD), 256, fpo);
    fwrite(output_BmpBuf, output_lineByte*width, 1, fpo);
    fclose(fpo);
    printf("success");
}
