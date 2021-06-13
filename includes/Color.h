#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"
#include <iostream>

void write_color(App* app, int x, int y, Color pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    double scale = 1.0 / app->samplesPerPixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    int ir = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    int ig = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    int ib = static_cast<int>(256 * clamp(b, 0.0, 0.999));

    int pixelCoord2D = (app->width * app->height) - ((app->width - x) + (app->width * y));
    app->pixels[pixelCoord2D] = ((ir & 0xff) << 16) + ((ig & 0xff) << 8) + (ib & 0xff);
}

#endif