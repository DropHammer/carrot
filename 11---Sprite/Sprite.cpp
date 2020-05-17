#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <stdio.h>

class RTexture
{
public:
	RTexture();
	~RTexture();

	bool LoadFromFile(std::string path);
	void Free();
	void Render(int x, int y, SDL_Rect* clip = NULL);

	int GetWidth() { return mWidth;}
	int GetHeight() { return mHeight; }

private:
	int mWidth;
	int mHeight;
	SDL_Texture* mTexture;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow;
SDL_Renderer* mRenderer;
SDL_Rect mSpriteClips[4];
RTexture mSpriteSheetTexture;

RTexture::RTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

RTexture::~RTexture()
{
	Free();
}

void RTexture::Free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool RTexture::LoadFromFile(std::string path)
{
	Free();
	SDL_Texture* newTex = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image: %s", IMG_GetError());
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Unable to create texture: %s", SDL_GetError());
	}
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);

	mTexture = newTex;
	return mTexture != NULL;
}

void RTexture::Render(int x, int y, SDL_Rect* Clip)
{
	SDL_Rect renderQuad{ x,y,mWidth,mHeight };
	if (Clip != NULL)
	{
		renderQuad.w = Clip->w;
		renderQuad.h = Clip->h;
	}
	SDL_RenderCopy(mRenderer, mTexture, Clip, &renderQuad);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: texture filtering not enable!");
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
		printf("Failed to create render: %s", SDL_GetError());
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
	if (!mSpriteSheetTexture.LoadFromFile("D:/Documents/visual stdio documents/Exercises/Sprite/dots.png"))
	{
		printf("Failed to load sprite texture!");
		return false;
	}

	mSpriteClips[0].x = 0;
	mSpriteClips[0].y = 0;
	mSpriteClips[0].w = 100;
	mSpriteClips[0].h = 100;

	mSpriteClips[1].x = 100;
	mSpriteClips[1].y = 0;
	mSpriteClips[1].w = 100;
	mSpriteClips[1].h = 100;

	mSpriteClips[2].x = 0;
	mSpriteClips[2].y = 100;
	mSpriteClips[2].w = 100;
	mSpriteClips[2].h = 100;

	mSpriteClips[3].x = 100;
	mSpriteClips[3].y = 100;
	mSpriteClips[3].w = 100;
	mSpriteClips[3].h = 100;

	return true;
}

void Close()
{
	mSpriteSheetTexture.Free();

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to init!");
		return 0;
	}
	if (!LoadMedia())
	{
		printf("Failed to load!");
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

		mSpriteSheetTexture.Render(0, 0, &mSpriteClips[0]);
		mSpriteSheetTexture.Render(640 - mSpriteClips[1].w, 0, &mSpriteClips[1]);
		mSpriteSheetTexture.Render(0, 480 - mSpriteClips[2].h, &mSpriteClips[2]);
		mSpriteSheetTexture.Render(640 - mSpriteClips[3].w, 480 - mSpriteClips[3].h, &mSpriteClips[3]);

		SDL_RenderPresent(mRenderer);
	}

	Close();
	return 0;
}