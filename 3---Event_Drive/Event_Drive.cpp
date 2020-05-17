#include <SDL.h>
#include <stdio.h>

SDL_Window* mWindow = nullptr;
SDL_Surface* mScreenSurface = nullptr;
SDL_Surface* mX = nullptr;

bool Init();
bool LoadMedia();
void Close();

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("EVENT DRIVE", 100, 100, 640, 480, 0);
	if(!mWindow)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	mScreenSurface = SDL_GetWindowSurface(mWindow);
	return true;
}

bool LoadMedia()
{
	mX = SDL_LoadBMP("D:/Documents/visual stdio documents/Exercises/Event_Drive/X.bmp");
	if (!mX)
	{
		SDL_Log("Unable to load bmp: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Close()
{
	SDL_FreeSurface(mX);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		SDL_Log("Unable to init: %s", SDL_GetError());
		return false;
	}
	if (!LoadMedia())
	{
		SDL_Log("Unable to load: %s", SDL_GetError());
		return false;
	}
	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		
		SDL_BlitSurface(mX, NULL, mScreenSurface, NULL);
		SDL_UpdateWindowSurface(mWindow);
	}

	Close();
	return 0;
}