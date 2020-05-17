#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

bool Init();
bool LoadMedia();
void Close();

SDL_Surface* LoadSurface(std::string path);
SDL_Window* mWindow = NULL;
SDL_Surface* mScreen = NULL;
SDL_Surface* mPNGSurface = NULL;

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("WY", 100, 100, 640, 480, 0);
	if (!mWindow)
	{
		printf("Failed to create window: %s", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Failed to init PNG: %s", SDL_GetError());
		return false;
	}

	mScreen = SDL_GetWindowSurface(mWindow);
	return true;
}

bool LoadMedia()
{
	mPNGSurface = LoadSurface("D:/Documents/visual stdio documents/Exercises/IMG_LOAD/loaded.png");
	if (mPNGSurface == NULL)
	{
		printf("Failed to load PNG!\n");
		return false;
	}
	return true;
}

void Close()
{
	SDL_FreeSurface(mPNGSurface);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* LoadSurface(std::string path)
{
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image: %s", SDL_GetError());
	}
	optimizedSurface = SDL_ConvertSurface(loadedSurface, mScreen->format, 0);
	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image: %s", SDL_GetError());
	}
	SDL_FreeSurface(loadedSurface);

	return optimizedSurface;
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Unable to init: %s", SDL_GetError());
		return 0;
	}

	if (!LoadMedia())
	{
		printf("Unable to load: %s", SDL_GetError());
		return 0;
	}

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		SDL_BlitSurface(mPNGSurface, NULL, mScreen, NULL);
		SDL_UpdateWindowSurface(mWindow);
	}

	Close();
	return 0;
}