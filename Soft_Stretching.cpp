#include <SDL.h>
#include <stdio.h>
#include <string>

bool Init();
bool LoadMedia();
void Close();

SDL_Surface* LoadSurface(std::string path);
SDL_Window* mWindow = NULL;
SDL_Surface* mScreen = NULL;
SDL_Surface* mStretchingSurface = NULL;

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("WY", 0, 0, 640, 480, 0);
	if (!mWindow)
	{
		printf("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mScreen = SDL_GetWindowSurface(mWindow);
	return true;
}

bool LoadMedia()
{
	mStretchingSurface = LoadSurface("D:/Documents/visual stdio documents/Exercises/Soft_Stretching/stretch.bmp");
	if (mStretchingSurface == NULL)
	{
		printf("Failed to stretching BMP: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Close()
{
	SDL_FreeSurface(mScreen);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

SDL_Surface* LoadSurface(std::string path)
{
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load BMP: %s", SDL_GetError());
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, mScreen->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unabe to optimize bmp: %s", SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to initialize!");
		return 0;
	}
	if (!LoadMedia())
	{
		printf("Failed to load bmp!");
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

		SDL_Rect stretching{ 0,0,640,480 };
		SDL_BlitScaled(mStretchingSurface, NULL, mScreen, NULL);
		SDL_UpdateWindowSurface(mWindow);
	}
	Close();
	return 0;
}