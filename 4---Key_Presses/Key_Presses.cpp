#include <SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurface
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

bool Init();
bool LoadMedia();
void Close();

SDL_Surface* LoadSurface(std::string path);
SDL_Window* mWindow = NULL;
SDL_Surface* mScreen = NULL;
SDL_Surface* mKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* mCurrentSurface = NULL;

bool Init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Unable to initialize SDL: %s", SDL_GetError());
		success = false;
	}
	else 
	{
		mWindow = SDL_CreateWindow("WY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			printf("Unable to create window: %s", SDL_GetError());
			success = false;
		}
		else
		{
			mScreen = SDL_GetWindowSurface(mWindow);
		}
	}
	return success;
}

SDL_Surface* LoadSurface(std::string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if (loadedSurface == NULL)
	{
		printf("Failed to load BMP: %s from %s", SDL_GetError(), path.c_str());
	}
	return loadedSurface;
}

bool LoadMedia()
{
	bool success = true;
	mKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("D:/Documents/visual stdio documents/Exercises/Key_Presses/press.bmp");
	if (mKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}

	mKeyPressSurface[KEY_PRESS_SURFACE_UP] = LoadSurface("D:/Documents/visual stdio documents/Exercises/Key_Presses/up.bmp");
	if (mKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	mKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = LoadSurface("D:/Documents/visual stdio documents/Exercises/Key_Presses/down.bmp");
	if (mKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	mKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = LoadSurface("D:/Documents/visual stdio documents/Exercises/Key_Presses/left.bmp");
	if (mKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	mKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("D:/Documents/visual stdio documents/Exercises/Key_Presses/right.bmp");
	if (mKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}
	return success;
}

void Close()
{
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
	{
		SDL_FreeSurface(mKeyPressSurface[i]);
		mKeyPressSurface[i] = NULL;
	}
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to initialize!");
	}
	else
	{
		if(!LoadMedia())
		{
		printf("Failed to load BMP!");
		}
		else
		{
			bool quit = false;
			SDL_Event event;
			mCurrentSurface = mKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
	
			while (!quit)
			{
				while (SDL_PollEvent(&event) != 0)
				{
					if (event.type == SDL_QUIT)
					{	
						quit = true;
					}
					else if (event.type == SDL_KEYDOWN)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_UP:
							mCurrentSurface = mKeyPressSurface[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_DOWN:
							mCurrentSurface = mKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							mCurrentSurface = mKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							mCurrentSurface = mKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
							break;
						default:
							mCurrentSurface = mKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}
				SDL_BlitSurface(mCurrentSurface, NULL, mScreen, NULL);
				SDL_UpdateWindowSurface(mWindow);
			}
		}
	}
	Close();
	return 0;
}