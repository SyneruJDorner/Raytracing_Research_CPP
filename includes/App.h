#ifndef APP_HPP
# define APP_HPP

#include <string>
#include <tuple>
#include <SDL2/SDL.h>
#include "Scene.h"

class App
{
public:
    static App &instance() {static App app; return app;}
    
    double aspectRatio;
	int width;
    int height;
    int samplesPerPixel;
    int max_depth;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* texture;
    uint32_t* pixels;

    Scene scene = Scene();
    
    //void setUp(std::string title, int width, int height, int samplesPerPixel, int max_depth);
    Scene createScene(std::string title, int width, int height, int samplesPerPixel, int max_depth);


private:
    App() {}
};

Scene App::createScene(std::string title, int width, int height, int samplesPerPixel, int max_depth)
{
    this->width = width;
    this->height = height;
    this->aspectRatio = width / height;
    this->samplesPerPixel = samplesPerPixel;
    this->max_depth = max_depth;

    SDL_Init(SDL_INIT_EVERYTHING);
    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_ALLOW_HIGHDPI);
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, this->width, this->height);
    this->pixels = new uint32_t[this->width * this->height];
    memset(this->pixels, 255, this->width * this->height * sizeof(uint32_t));

    this->scene = Scene(title);

    this->scene.camera = Camera();
    this->scene.camera.aspectRatio(this->aspectRatio);
    //Scene scene = this->scene;
    //Camera camera = this->scene.camera;

    return this->scene;
}

#endif