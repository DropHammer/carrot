#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>
#include <cmath>

bool Init();
void Close();

SDL_Texture* LoadTexture(std::string path);
SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"));
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	mWindow = SDL_CreateWindow("WY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
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

void Close()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* LoadTexture(std::string path)
{
	SDL_Texture* newTex = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load SDL_image: %s", SDL_GetError());
	}
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Unable to create texture: %s", SDL_GetError());
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
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		//Render red filled quad
		SDL_Rect redfill{640 / 4,480 / 4,640 / 2,480 / 2};
		SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
		SDL_RenderFillRect(mRenderer, &redfill);

		//Render green outlined quad
		SDL_Rect outline{640 / 6,480 / 6,640 * 2/3,480 * 2/3};
		SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(mRenderer, &outline);

		//Render blue line
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(mRenderer, 0, 480 / 2, 640, 480 / 2);

		SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);
		for (int i = 0; i < 480; i += 4)
		{
			SDL_RenderDrawPoint(mRenderer, 640 / 2, i);
		}

		SDL_RenderPresent(mRenderer);
	}

	Close();
	return 0;
}