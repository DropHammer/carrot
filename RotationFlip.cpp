#include <string>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool LoadFromFile(std::string path);
	void free();
	//void SetColor(Uint8 r, Uint8 g, Uint8 b);
	//void SetBlendMode(SDL_BlendMode blending);
	//void SetAlpha(Uint8 alphad);
	void render(int x, int y, SDL_Rect* Clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
private:
	int mWidth;
	int mHeight;
	SDL_Texture* mTexture;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;
LTexture mRF;

LTexture::LTexture()
	:mTexture(NULL)
	,mWidth(0)
	,mHeight(0)
{}

LTexture::~LTexture()
{
	free();
}

bool LTexture::LoadFromFile(std::string path)
{
	free();
	SDL_Texture* newTex = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("a");
		return false;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("b");
		return false;
	}

	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
	mTexture = newTex;

	return mTexture != NULL;
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* Clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad{ x,y,mWidth,mHeight };
	if (Clip != NULL)
	{
		renderQuad.w = Clip->w;
		renderQuad.h = Clip->h;
	}
	SDL_RenderCopyEx(mRenderer, mTexture, Clip, &renderQuad, angle, center, flip);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("c");
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning!");
	}

	mWindow = SDL_CreateWindow("wy", 100, 100, 640, 480, 0);
	if (!mWindow)
	{
		printf("d");
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
	{
		printf("e");
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("f");
		return false;
	}
	return true;
}

bool LoadMedia()
{
	if (!mRF.LoadFromFile("D:/Documents/visual stdio documents/Exercises/Rotation_and_Flipping/arrow.png"))
	{
		printf("g");
		return false;
	}
	return true;
}

void Close()
{
	mRF.free();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("h");
		return 0;
	}
	if (!LoadMedia())
	{
		printf("i");
		return 0;
	}
	bool quit = false;
	SDL_Event event;
	double degrees = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (!quit)
	{
		while(SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_a:
					degrees -= 60; break;
				case SDLK_d:
					degrees += 60; break;
				case SDLK_q:
					flip = SDL_FLIP_HORIZONTAL; break;
				case SDLK_w:
					flip = SDL_FLIP_NONE; break;
				case SDLK_e:
					flip = SDL_FLIP_VERTICAL; break;
				}
			}
		}
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		mRF.render((640 - mRF.GetWidth())/ 2, (480 - mRF.GetHeight()) / 2, NULL, degrees,NULL, flip);
		
		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}