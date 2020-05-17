#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool LoadFromFile(std::string path);
	void free();
	void SetColor(Uint8 r, Uint8 g, Uint8 b);
	void SetBlendMode(SDL_BlendMode blending);
	void SetAlpha(Uint8 alpha);
	void render(int x, int y, SDL_Rect* clip = NULL);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
private:
	int mHeight;
	int mWidth;
	SDL_Texture* mTexture;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;

LTexture mModulation;
LTexture mBackground;

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

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
		return false;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		return false;
	}

	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
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

void LTexture::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(mTexture, r, g, b);
}

void LTexture::SetBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	SDL_Rect renderQuad{ x,y,mWidth,mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning!");
	}

	mWindow = SDL_CreateWindow("wy", 100, 100, 640, 480, 0);
	if (!mWindow)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
	{
		return false;
	}

	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)
	{
		return false;
	}
	return true;
}

bool LoadMedia()
{
	if (!mModulation.LoadFromFile("D:/Documents/visual stdio documents/Exercises/AlphaBlending/fadeout.png"))
	{
		return false;
	}
	mModulation.SetBlendMode(SDL_BLENDMODE_BLEND);
	if (!mBackground.LoadFromFile("D:/Documents/visual stdio documents/Exercises/AlphaBlending/fadein.png"))
	{
		return false;
	}
	return true;
}

void Close()
{
	mModulation.free();
	mBackground.free();

	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		return 0;
	}
	if (!LoadMedia())
	{
		return 0;
	}
	bool quit = false;
	SDL_Event event;
	Uint8 a = 255;
	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_w)
				{
					if (a + 32 > 255)
						a = 255;
					else
						a += 32;
				}

				if (event.key.keysym.sym == SDLK_s)
				{
					if (a - 32 < 0)
						a = 0;
					else
						a -= 32;
				}
			}
		}
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		mBackground.render(0, 0);

		mModulation.SetAlpha(a);
		mModulation.render(0, 0);

		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}