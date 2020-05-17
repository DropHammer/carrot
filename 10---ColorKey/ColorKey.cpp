#include <SDL.h>
#include <string>
#include <stdio.h>
#include <SDL_image.h>

class mTexture
{
public:
	mTexture();
	~mTexture();
	bool LoadFromFile(std::string path);
	void free();
	void render(int x, int y);

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }
private:
	int mWidth;
	int mHeight;

	SDL_Texture* mTex;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow =NULL;
SDL_Renderer* mRenderer = NULL;
mTexture mPeo;
mTexture mBack;

mTexture::mTexture()
{
	mTex = NULL;
	mWidth = 0;
	mHeight = 0;
}

mTexture::~mTexture()
{
	free();
}

void mTexture::free()
{
	if (mTex != NULL)
	{
		SDL_DestroyTexture(mTex);
		mTex = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool mTexture::LoadFromFile(std::string path)
{
	free();
	SDL_Texture* newTex = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Failed to load IMG: %s",IMG_GetError());
		return false;
	}
	
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Unable to create texture: %s", SDL_GetError());
		return false;
	}

	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);

	mTex = newTex;
	return mTex != NULL;
}

void mTexture::render(int x,int y)
{
	SDL_Rect quad { x,y,mWidth,mHeight };
	SDL_RenderCopy(mRenderer, mTex, NULL, &quad);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}
	mWindow = SDL_CreateWindow("WY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (!mWindow)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	return true;
}

bool LoadMedia()
{
	if (!mPeo.LoadFromFile("D:/Documents/visual stdio documents/Exercises/ColorKey/foo.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		return false;
	}
	if (!mBack.LoadFromFile("D:/Documents/visual stdio documents/Exercises/ColorKey/background.png"))
	{
		printf("Failed to load background texture image!\n");
		return false;
	}
	return true;
}

void Close()
{
	mPeo.free();
	mBack.free();

	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to initialize!\n");
	}
	if (!LoadMedia())
	{
		printf("Failed to load media!\n");
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

		SDL_SetRenderDrawColor(mRenderer,255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		mBack.render(0, 0);
		mPeo.render(240, 190);
		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}