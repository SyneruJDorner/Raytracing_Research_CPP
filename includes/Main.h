#ifndef MAIN_HPP
# define MAIN_HPP

#include <iostream>
#include <tuple>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "rtweekend.h"

#include "App.h"
#include "Trace.h"
#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

App* app = &App::instance();

#endif