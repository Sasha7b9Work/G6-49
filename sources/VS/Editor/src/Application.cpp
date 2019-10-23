#pragma warning(push)
#pragma warning(disable:4018 4189 4355 4365 4459 4548 4571 4625 4668 4774 5026 5027)
#include "Application.h"
#include <SDL.h>
#pragma warning(pop)

#undef main


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (SDL_Init(SDL_INIT_EVENTS) != 0U) //-V2517
    {
        std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
    }

    return wxEntry(argc, argv);
}