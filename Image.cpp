#include <SDL.h>
#include <stdio.h>

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Surface* mScreenSurface = NULL;
SDL_Surface* mHello = NULL;

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to init SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("WANGYUN",100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	else
	{
		mScreenSurface = SDL_GetWindowSurface(mWindow);
	}
	return true;
}

bool LoadMedia()
{
	mHello = SDL_LoadBMP("D:/Documents/visual stdio documents/Exercises/Image/wh.bmp");
	if (!mHello)
	{
		SDL_Log("Failed to load SDL: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Close()
{
	SDL_FreeSurface(mHello);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to initialize!\n");
		return false;
	}
	if (!LoadMedia())
	{
		printf("Failed to load media!\n");
		return false;
	}
	else
	{
		SDL_BlitSurface(mHello, NULL, mScreenSurface, NULL);
		SDL_UpdateWindowSurface(mWindow);
		SDL_Delay(2000);
	}

	Close();
	return 0;
}