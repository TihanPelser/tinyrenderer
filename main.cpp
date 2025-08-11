#include <cmath>
#include "tgaimage.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

// Simple implementation of a lien drawing algorithm
void simple_line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    for (float t=0.; t<1.; t+=0.002) {
        int x = std::round((1 - t) * ax + t * bx);
        int y = std::round((1 - t) * ay + t * by);
        framebuffer.set(x, y, color);
    }
}

// A repeat of the above, but with more accurate sampling
void multidirectional_line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    // Account for lines that are steep, i.e. where the dy >> dx by transposing the image
    bool steep = std::abs(ax-bx) < std::abs(ay - by);

    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    // Account for lines where x(t) is decreasing
    if (ax > bx){
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    for (int x=ax; x<=bx; x++) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round((1 - t)*ay + t*by);
        if (steep) {
            // de-transpose the image
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
    }
}

// A simple, error progagating, implementation of Bresenham's Line Drawing Algorithm
void multidirectional_line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    // Account for lines that are steep, i.e. where the dy >> dx by transposing the image
    bool steep = std::abs(ax-bx) < std::abs(ay - by);

    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    // Account for lines where x(t) is decreasing
    if (ax > bx){
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    for (int x=ax; x<=bx; x++) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round((1 - t)*ay + t*by);
        if (steep) {
            // de-transpose the image
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
        
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 512;
    constexpr int height = 512;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  70, ay =  30;
    int bx = 120, by = 370;
    int cx = 480, cy = 430;

    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);

    // Draw the lines between vertices a, b and c
    // Blue from a to b
    multidirectional_line(ax, ay, bx, by, framebuffer, blue);

    // Green from c to b
    multidirectional_line(cx, cy, bx, by, framebuffer, green);

    // Yellow from c to a
    multidirectional_line(cx, cy, ax, ay, framebuffer, yellow);

    // Red from a to c
    multidirectional_line(ax, ay, cx, cy, framebuffer, red);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
