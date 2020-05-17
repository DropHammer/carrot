#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>

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
		printf("Warning: Linear texture filtering not enabled!");
	}

	mWindow = SDL_CreateWindow("wy", 0, 0, 640, 480, 0);
	if (!mWindow)
	{
		printf("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
	{
		printf("Failed to create render: %s", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Failed to init img: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool LoadMedia()
{
	mTex = LoadTexture("D:/Documents/visual stdio documents/Exercises/ViewPort/viewport.png");
	if (mTex == NULL)
	{
		printf("Failed to load texture!");
		return false;
	}
	return true;
}

void Close()
{
	SDL_DestroyTexture(mTex);
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* LoadTexture(std::string path)
{
	SDL_Texture* newTex = NULL;
	SDL_Surface* loadedSurface = NULL;
	loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Failed to load img!");
	}

	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Failed to create texture!");
	}
	SDL_FreeSurface(loadedSurface);
	return newTex;
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to INIT£¡");
		return 0;
	}
	if (!LoadMedia())
	{
		printf("Failed to Load!");
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

		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		//TOP left
		SDL_Rect topleft{ 0,0,640 / 2,480 / 2 };
		SDL_RenderSetViewport(mRenderer, &topleft);
		SDL_RenderCopy(mRenderer, mTex,NULL,NULL);

		//Top right
		SDL_Rect topright{ 640 / 2,0,640 / 2,480 / 2 };
		SDL_RenderSetViewport(mRenderer, &topright);
		SDL_RenderCopy(mRenderer, mTex, NULL, NULL);

		//Bottom
		SDL_Rect bottom{ 0,640 / 2,640,480 / 2 };
		SDL_RenderSetViewport(mRenderer, &bottom);
		SDL_RenderCopy(mRenderer, mTex, NULL, NULL);

		SDL_RenderPresent(mRenderer);
	}

	Close();
	return 0;
}