#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <SDL_image.h>
#include <SDL_ttf.h>

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool LoadFromFile(std::string path);
	bool LoadFromRenderedText(std::string trueText, SDL_Color TextColor);
	void free();
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
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
TTF_Font* mFont = NULL;
LTexture mTextTexture;

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
		printf("Unable to load img: %s", IMG_GetError());
		return false;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Can't create texture from surface: %s", SDL_GetError());
		return false;
	}
	
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	mTexture = newTex;
	SDL_FreeSurface(loadedSurface);

	return mTexture != NULL;
}

bool LTexture::LoadFromRenderedText(std::string tureText, SDL_Color TextColor)
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, tureText.c_str(), TextColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface: %s", TTF_GetError());
	}

	mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	if (mTexture == NULL)
	{
		printf("Unable to create texture from rendered text: %s", SDL_GetError());
	}
	mWidth = textSurface->w;
	mHeight = textSurface->h;
	SDL_FreeSurface(textSurface);

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

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad{ x,y,mWidth,mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Failed to init.");
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning!");
		return false;
	}

	mWindow = SDL_CreateWindow("wy", 100, 100, 640, 480, 0);
	if (!mWindow)
	{
		printf("Failed to create window.");
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		printf("Failed to create render.");
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Failed to init png.");
		return false;
	}

	if (TTF_Init() == -1)
	{
		printf("Failed to init ttf.");
		return false;
	}
	return true;
}

bool LoadMedia()
{
	mFont = TTF_OpenFont("D:/Documents/visual stdio documents/Exercises/True_type/lazy.ttf", 28);
	if (mFont == NULL)
	{
		printf("Failed to open file.");
		return false;
	}
	SDL_Color textColor{ 0,0,0 };
	if (!mTextTexture.LoadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
	{
		printf("Failed to load text.");
		return false;
	}
	return true;
}

void Close()
{
	mTextTexture.free();
	
	TTF_CloseFont(mFont);
	mFont = NULL;

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	TTF_Quit();
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

		mTextTexture.render((640 - mTextTexture.GetWidth()) / 2, (480 - mTextTexture.GetHeight()) / 2);

		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}