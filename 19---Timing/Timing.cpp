#include <stdio.h>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool LoadFromRenderText(std::string textureText, SDL_Color tetColor);
	bool LoadFromFile(std::string path);
	void free();
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;
TTF_Font* mfont = NULL;

LTexture mTimeTextTexture;
LTexture mPromptTextTexture;

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
		printf("Failed to load img.");
		return false;
	}
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

	newTex = SDL_CreateTextureFromSurface(mRenderer,loadedSurface);
	if (newTex == NULL)
	{
		printf("Failed to create texture.");
	}
	else
	{
		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;
	}

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

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect Quadrect{ x,y,mWidth,mHeight };
	if (clip != NULL)
	{
		Quadrect.x = clip->w;
		Quadrect.y = clip->h;
	}
	SDL_RenderCopyEx(mRenderer, mTexture, clip, &Quadrect, angle, center, flip);
}

bool LTexture::LoadFromRenderText(std::string textureText, SDL_Color textColor)
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(mfont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create text texture.");
		}
		mWidth = textSurface->w;
		mHeight = textSurface->h;

		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning.");
	}

	mWindow = SDL_CreateWindow("WY", 100, 100, 640, 480, 0);
	if (mWindow ==NULL)
	{
		printf("1");
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer== NULL)
	{
		printf("2");
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("3");
		return false;
	}
	if (TTF_Init() == -1)
	{
		printf("4");
		return false;
	}

	return true;
}

bool LoadMedia()
{
	mfont = TTF_OpenFont("D:/Documents/visual stdio documents/Exercises/19---Timing/lazy.ttf",28);
	if (mfont == NULL)
	{
		printf("Failed to load ttf.");
		return false;
	}
	SDL_Color textColor = { 0,0,0,255 };
	if (!mPromptTextTexture.LoadFromRenderText("Press Enter to Reset Start Time.",textColor))
	{
		printf("Unable to render promot texture!");
		return false;
	}
	return true;
}

void Close()
{
	mTimeTextTexture.free();
	mPromptTextTexture.free();

	TTF_CloseFont(mfont);

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

	SDL_Color textColor = { 0,0,0,255 };
	Uint32 startTime = 0;
	std::stringstream timeText;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
			{
				startTime = SDL_GetTicks();
			}
		}
		timeText.str( "" );
		timeText << "Milliseconds since start time " << SDL_GetTicks() - startTime;

		if (!mTimeTextTexture.LoadFromRenderText(timeText.str().c_str(), textColor))
		{
			printf("6");
		}

		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		mPromptTextTexture.render((640 - mPromptTextTexture.GetWidth()) / 2, 0);
		mTimeTextTexture.render((640 - mPromptTextTexture.GetWidth()) / 2, (480 - mPromptTextTexture.GetHeight()) / 2);

		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}