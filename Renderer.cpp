#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>

bool Init();
bool LoadMedia();
void Close();

SDL_Texture* LoadTexture(std::string path);
SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;
SDL_Texture* mTex = NULL;

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to init SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning:Linear texture filternig not enabled!");
	}

	mWindow = SDL_CreateWindow("WY", 100, 100, 640, 480, 0);
	if (!mWindow)
	{
		printf("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
	{
		printf("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Failed to init IMG: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool LoadMedia()
{
	mTex = LoadTexture("D:/Documents/visual stdio documents/Exercises/Renderer/texture.png");
	if (mTex == NULL)
	{
		printf("Failed to load image: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Close()
{
	SDL_DestroyTexture(mTex);
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* LoadTexture(std::string path)
{
	SDL_Texture* newTex = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Failed to load PNG: %s", SDL_GetError());
	}
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Failed to create texture: %s", SDL_GetError());
	}
	SDL_FreeSurface(loadedSurface);
	return newTex;
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to init!");
	}
	if (!LoadMedia())
	{
		printf("Failed to load!");
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
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
		SDL_RenderClear(mRenderer);
		SDL_RenderCopy(mRenderer, mTex, NULL, NULL);
		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}