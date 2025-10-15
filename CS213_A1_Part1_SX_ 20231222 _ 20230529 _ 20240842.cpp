/*
CS213 - Object Oriented Programming
Assignment 1 - Part 1

Section: x

Team Members:
1. [20231222] - [ارسيل صفوت عطيه] - Filter 1,4
2. [20230529] - [احمد محمد سعيد] - Filter 2,5
3. [20240842] - [عبدالله عبدالكريم عبدالله] - Filter 3,6

This file contains:
- Menu system for image processing

*/
#include "Image_Class.h"
#include <iostream>
using namespace std;

//filter 3 invert
void invert(Image &img) {
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            img(i, j, 0) = 255 - img(i, j, 0);
            img(i, j, 1) = 255 - img(i, j, 1);
            img(i, j, 2) = 255 - img(i, j, 2);
        }
    }
}

//filter 6 rotate
Image rotate90(const Image &img) {
    Image rotated(img.height, img.width);
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            for (int c = 0; c < img.channels; c++) {
                rotated(j, img.height - 1 - i, c) = img(i, j, c);
            }
        }
    }
    return rotated;
}

Image rotate(const Image &img, int angle) {
    if (angle == 90) return rotate90(img);
    if (angle == 180) return rotate90(rotate90(img));
    if (angle == 270) return rotate90(rotate90(rotate90(img)));
    return img;
}

//filter 1 grayscale
void grayscale(Image &img) {
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            unsigned char gray = (img(i, j, 0) + img(i, j, 1) + img(i, j, 2)) / 3;
            img(i, j, 0) = gray;
            img(i, j, 1) = gray;
            img(i, j, 2) = gray;
        }
    }
}

//filter 4 merge two photos
Image mergeImages(const Image &img1, const Image &img2) {
    int newW = min(img1.width, img2.width);
    int newH = min(img1.height, img2.height);
    Image merged(newW, newH);

    for (int i = 0; i < newH; i++) {
        for (int j = 0; j < newW; j++) {
            for (int c = 0; c < 3; c++) {
                merged(i, j, c) = (img1(i, j, c) + img2(i, j, c)) / 2;
            }
        }
    }
    return merged;
}

//filter 2 black and white
void blackAndWhite(Image &img) {
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            int gray = (img(y, x, 0) + img(y, x, 1) + img(y, x, 2)) / 3;
            if (gray > 127) {
                img(y, x, 0) = img(y, x, 1) = img(y, x, 2) = 255;
            } else {
                img(y, x, 0) = img(y, x, 1) = img(y, x, 2) = 0;
            }
        }
    }
}

//filter 8 flip photo
void flipImage(Image &img, bool horizontal) {
    int w = img.width;
    int h = img.height;
    Image newImg(w, h);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int c = 0; c < img.channels; c++) {
                if (horizontal) {
                    newImg(y, x, c) = img(y, w - 1 - x, c);
                } else {
                    newImg(y, x, c) = img(h - 1 - y, x, c);
                }
            }
        }
    }
    img = newImg;
    cout << "Flip filter applied.\n";
}

int main() {
    string photoname;
    cout << "Enter photo name: ";
    cin >> photoname;
    Image img(photoname);

    int choice;
    do {
        cout << "1= invert\n";
        cout << "2= rotate\n";
        cout << "3= grayscale\n";
        cout << "4= merge with another image\n";
        cout << "5= black and white\n";
        cout << "6= flip image\n";
        cout << "7= save and exit\n";
        cin >> choice;

        if (choice == 1) {
            invert(img);
        } else if (choice == 2) {
            int angle;
            cout << "Enter angle (90/180/270): ";
            cin >> angle;
            img = rotate(img, angle);
        } else if (choice == 3) {
            grayscale(img);
        } else if (choice == 4) {
            string photoname2;
            cout << "Enter photoname 2 : ";
            cin >> photoname2;
            Image img2(photoname2);
            img = mergeImages(img, img2);
        } else if (choice == 5) {
            blackAndWhite(img);
        } else if (choice == 6) {
            int type;
            cout << "1= horizontal ,2= vertical  ";
            cin >> type;
            flipImage(img, type == 1);
        } else if (choice == 7) {
            string outName;
            cout << "Enter new photo name: ";
            cin >> outName;
            img.saveImage(outName);
        }

    } while (choice != 7);

    return 0;
}
