#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool LoadFromFile(std::string path);
	void free();
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);
	void render(int x, int y, SDL_Rect* clip = NULL);

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
LTexture mModulation;

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
		printf("unable to load img: %s", IMG_GetError());
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("unable to create texture: %s", SDL_GetError());
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

void LTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
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

	if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	mWindow = SDL_CreateWindow("wy", 100,100, 640, 480, 0);
	if (!mWindow)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
	{
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		return false;
	}
	return true;
}

bool LoadMedia()
{
	if (!mModulation.LoadFromFile("D:/Documents/visual stdio documents/Exercises/ColorModulation/colors.png"))
	{return false; }
	return true;
}

void Close()
{
	mModulation.free();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		return 0;
	}
	if(!LoadMedia())
	{
		return 0;
	}

	bool quit = false;
	SDL_Event event;
	Uint8 red = 255;
	Uint8 green = 255;
	Uint8 blue = 255;

	while (!quit)
	{
		while (SDL_PollEvent(&event)!=0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_q:
					red += 32;
					break;
				case SDLK_w:
					green += 32;
					break;
				case SDLK_e:
					blue += 32;
					break;
				case SDLK_a:
					red -= 32;
					break;
				case SDLK_s:
					green -= 32;
					break;
				case SDLK_d:
					blue -= 32;
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		mModulation.SetColor(red, green, blue);
		mModulation.render(0, 0);

		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}