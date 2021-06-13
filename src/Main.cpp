#include "Main.h"
#include <iostream>

Scene scene;

void start()
{
    // World
    scene = app->createScene("Raytracing Research", 1280, 800, 500, 50);

    scene.camera.FOV(30);
    scene.camera.position(Vector3(13, 2, 3));
    scene.camera.lookAt(Vector3(0, 0, 0));
    scene.camera.focalDistance(10.0);
    scene.camera.aperture(0.1);

    // auto material_ground = make_shared<Lambertian>(Color(0.0, 0.8, 0.0));
    // auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.5));
    // auto material_left   = make_shared<Dielectric>(1.5);
    // auto material_right  = make_shared<Metal>(Color(0.0, 0.3, 0.8), 0.1);

    // scene.world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // scene.world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // scene.world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // scene.world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   -0.45, material_left));
    // scene.world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    scene.world = random_scene();
}

void raytrace()
{
    std::cout << "P3\n" << app->width << ' ' << app->height << "\n255\n";

    for (int y = app->height - 1; y >= 0; --y)
    {
        std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
        for (int x = 0; x < app->width; ++x)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < app->samplesPerPixel; ++s)
            {
                auto u = (x + random_double()) / (app->width - 1);
                auto v = (y + random_double()) / (app->height - 1);
                Ray ray = scene.camera.get_ray(u, v);
                pixel_color += trace(ray, scene.world, app->max_depth);
            }

            write_color(app, x, y, pixel_color);
        }
    }
    std::cout << "\nDone.\n";
}

void render()
{
    bool isRunning = true;
	SDL_Event event;

    while (isRunning)
	{
        SDL_UpdateTexture(app->texture, NULL, app->pixels, app->width * sizeof(uint32_t));
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
        }

        SDL_RenderClear(app->renderer);
        SDL_RenderCopy(app->renderer, app->texture, NULL, NULL);
        SDL_RenderPresent(app->renderer);
    }
}

void quit()
{
    delete[] app->pixels;
    SDL_DestroyTexture(app->texture);
    SDL_DestroyRenderer(app->renderer);

    SDL_DestroyWindow(app->window);
    SDL_Quit();
}

int main(int argv, char** args)
{
    (void)(argv);
    (void)(args);

    start();
    raytrace();
    render();
    quit();

    return 0;
}