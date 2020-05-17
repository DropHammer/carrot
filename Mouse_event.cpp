#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
	BUTTON_SPRITE_MOUSE_DOWN,
	BUTTON_SPRITE_MOUSE_UP,
	BUTTON_SPRITE_TOTAL
};

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool LoadFromFile(std::string path);
	void free();
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

private:
	int mWidth;
	int mHeight;
	SDL_Texture* mTexture;
};

class LButton
{
public:
	LButton();
	void SetPosition(int x, int y);
	void handleEvent(SDL_Event* event);
	void render();
private:
	SDL_Point mPosition;
	LButtonSprite mCurrentSprite;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;

SDL_Rect mSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture mButtonSheet;

LButton mButton[4];

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
		printf("Unable to load img.");
		return false;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0,255,255));
	
	newTex = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTex == NULL)
	{
		printf("Unable to create texture.");
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

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::SetPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;

		if (x < mPosition.x)
		{
			inside = false;
		}
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		else if (y < mPosition.y)
		{
			inside = false;
		}
		else if (y > mPosition.y + BUTTON_WIDTH)
		{
			inside = false;
		}

		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		else
		{
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::render()
{
	mButtonSheet.render(mPosition.x, mPosition.y, &mSpriteClips[mCurrentSprite]);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL failed to initialize : %s", SDL_GetError());
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning!");
	}

	mWindow = SDL_CreateWindow("wy", 100, 100, 640, 480, 0);
	if (!mWindow)
	{
		printf("Failed to create window.\n");
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		printf("Failed to create renderer.\n");
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Failed to init img.\n");
		return false;
	}
	return true;
}

bool LoadMedia()
{
	if (!mButtonSheet.LoadFromFile("D:/Documents/visual stdio documents/Exercises/17---Mouse_event/button.png"))
	{
		printf("Failed to load texture.\n");
		return false;
	}

	for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
	{
		mSpriteClips[i].x = 0;
		mSpriteClips[i].y = i * 200;
		mSpriteClips[i].w = BUTTON_WIDTH;
		mSpriteClips[i].h = BUTTON_HEIGHT;
	}
	mButton[0].SetPosition(0, 0);
	mButton[1].SetPosition(640 - BUTTON_WIDTH, 0);
	mButton[2].SetPosition(0, 480 - BUTTON_HEIGHT);
	mButton[3].SetPosition(640 - BUTTON_WIDTH, 480 - BUTTON_HEIGHT);

	return true;
}

void Close()
{
	mButtonSheet.free();

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to initialize.\n");
	}
	if (!LoadMedia())
	{
		printf("Failed to load media.\n");
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
			for (int i = 0; i < TOTAL_BUTTONS; i++)
			{
				mButton[i].handleEvent(&event);
			}
		}
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		for (int i = 0; i < TOTAL_BUTTONS; i++)
		{
			mButton[i].render();
		}
		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}