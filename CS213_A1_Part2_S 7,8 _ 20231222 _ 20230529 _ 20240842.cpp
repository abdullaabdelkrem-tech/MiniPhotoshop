/*
CS213 - Object Oriented Programming
Assignment 1 - Part 2

Section: 7,8

Team Members:
1. [20231222] - [ارسيل صفوت عطيه] - Filter 7,10
2. [20240842] - [عبدالله عبدالكريم عبدالله] - Filter 9,12


This file contains:

- Menu system for image processing

- link document :https://drive.google.com/file/d/14iTNuJUzpObqf3H6XoinVZXnp7sfAG9k/view?usp=sharing

- link video : https://drive.google.com/file/d/1aeCn9yl7Si-YvT-cCV0j2rRLyK8vVIlC/view?usp=sharing

*/
#include "Image_Class.h"
#include <iostream>
#include <cmath>
using namespace std;

bool isValid(const Image &img) {
    if (img.width <= 0 || img.height <= 0 || img.channels < 3) {
        cout << "Error: Image not loaded properly or unsupported format." << endl;
        return false;
    }
    return true;
}
// filter 9
void addFrame(Image &img) {
    if (!isValid(img)) return;
    int thickness;
    cout << "Enter frame thickness: ";
    cin >> thickness;
    if (thickness <= 0) {
        cout << "Invalid thickness." << endl;
        return;
    }
    if (thickness * 2 > img.width || thickness * 2 > img.height) {
        cout << "Thickness too large for image dimensions." << endl;
        return;
    }
    int r, g, b;
    cout << "Enter frame color (R G B) each color 0-255: ";
    cin >> r >> g >> b;
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            if (i < thickness || j < thickness || i >= img.height - thickness || j >= img.width - thickness) {
                img(i, j, 0) = (unsigned char)r;
                img(i, j, 1) = (unsigned char)g;
                img(i, j, 2) = (unsigned char)b;
                if (img.channels == 4) img(i, j, 3) = 255;
            }
        }
    }
    cout << "Frame added." << endl;
}
// filter 12
Image blurImage(const Image &img) {
    if (!isValid(img)) return img;
    Image out(img.width, img.height);

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            for (int c = 0; c < img.channels; ++c) out(i, j, c) = img(i, j, c);
        }
    }

    for (int i = 1; i < img.height - 1; ++i) {
        for (int j = 1; j < img.width - 1; ++j) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    sumR += (int)img(i + di, j + dj, 0);
                    sumG += (int)img(i + di, j + dj, 1);
                    sumB += (int)img(i + di, j + dj, 2);
                }
            }
            out(i, j, 0) = (unsigned char)(sumR / 9);
            out(i, j, 1) = (unsigned char)(sumG / 9);
            out(i, j, 2) = (unsigned char)(sumB / 9);
            if (img.channels == 4) out(i, j, 3) = img(i, j, 3);
        }
    }
    cout << "Blur applied." << endl;
    return out;
}
// filter 7
void darkenLighten(Image &img) {
    if (!isValid(img)) return;
    cout << "1 = Lighten, 2 = Darken: ";
    int choice; cin >> choice;
    if (choice != 1 && choice != 2) {
        cout << "Invalid choice." << endl;
        return;
    }
    double percent;
    cout << "Enter percentage (1-100): ";
    cin >> percent;
    if (percent < 1.0) percent = 1.0;
    if (percent > 100.0) percent = 100.0;
    double factor = (choice == 1) ? (1.0 + percent / 100.0) : (1.0 - percent / 100.0);

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            for (int c = 0; c < 3; ++c) {
                int v = (int)round((double)img(i, j, c) * factor);
                if (v < 0) v = 0;
                if (v > 255) v = 255;
                img(i, j, c) = (unsigned char)v;
            }
        }
    }
    cout << (choice == 1 ? "Lighten" : "Darken") << " applied by " << percent << "%." << endl;
}
// filter 10
Image detectEdges(const Image &img) {
    if (!isValid(img)) return img;
    Image gray(img.width, img.height);
    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            int g = ((int)img(i, j, 0) + (int)img(i, j, 1) + (int)img(i, j, 2)) / 3;
            gray(i, j, 0) = gray(i, j, 1) = gray(i, j, 2) = (unsigned char)g;
            if (img.channels == 4) gray(i, j, 3) = img(i, j, 3);
        }
    }

    Image out(img.width, img.height);

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            out(i, j, 0) = out(i, j, 1) = out(i, j, 2) = 255;
            if (img.channels == 4) out(i, j, 3) = gray(i, j, 3);
        }
    }

    const int threshold = 50;
    for (int i = 1; i < img.height - 1; ++i) {
        for (int j = 1; j < img.width - 1; ++j) {
            int sumDiff = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    sumDiff += abs((int)gray(i, j, 0) - (int)gray(i + di, j + dj, 0));
                }
            }
            int avgDiff = sumDiff / 8;
            if (avgDiff > threshold) {
                out(i, j, 0) = out(i, j, 1) = out(i, j, 2) = 0;
            } else {
                out(i, j, 0) = out(i, j, 1) = out(i, j, 2) = 255;
            }
        }
    }

    cout << "Edge detection completed." << endl;
    return out;
}

int main() {
    string filename;
    cout << "Enter photo name : ";
    cin >> filename;
    Image img(filename);
    if (!isValid(img)) return 1;

    cout << "Image loaded. Width: " << img.width << ", Height: " << img.height << ", Channels: " << img.channels << endl;

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1 = Add Frame\n";
        cout << "2 = Blur\n";
        cout << "3 = Darken/Lighten\n";
        cout << "4 = Detect Edges\n";
        cout << "5 = Save & Exit\n";
        cout << "Choose filter: ";
        cin >> choice;

        if (choice == 1) {
            addFrame(img);
        } else if (choice == 2) {
            img = blurImage(img);
        } else if (choice == 3) {
            darkenLighten(img);
        } else if (choice == 4) {
            img = detectEdges(img);
        } else if (choice == 5) {
            string outname;
            cout << "Enter new photo name: ";
            cin >> outname;
            img.saveImage(outname);
            cout << "Saved as " << outname << endl;
        } else {
            cout << "Invalid choice." << endl;
        }
    } while (choice != 5);

    return 0;
}


