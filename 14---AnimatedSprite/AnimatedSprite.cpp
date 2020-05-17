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
	int mWidth;
	int mHeight;
	SDL_Texture* mTexture;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;

const int WALKING_FRAMES = 4;
SDL_Rect mSpriteClips[WALKING_FRAMES];
LTexture mSpriteSheetTexture;

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

void LTexture::SetColor(Uint8 r,Uint8 g,Uint8 b)
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
	if (!mSpriteSheetTexture.LoadFromFile("D:/Documents/visual stdio documents/Exercises/AnimatedSprite/foo.png"))
	{
		return false;
	}

	mSpriteClips[0].x = 0;
	mSpriteClips[0].y = 0;
	mSpriteClips[0].w = 64;
	mSpriteClips[0].h = 205;

	mSpriteClips[1].x = 64;
	mSpriteClips[1].y = 0;
	mSpriteClips[1].w = 64;
	mSpriteClips[1].h = 205;

	mSpriteClips[2].x = 128;
	mSpriteClips[2].y = 0;
	mSpriteClips[2].w = 64;
	mSpriteClips[2].h = 205;

	mSpriteClips[3].x = 196;
	mSpriteClips[3].y = 0;
	mSpriteClips[3].w = 64;
	mSpriteClips[3].h = 205;

	return true;
}

void Close()
{
	mSpriteSheetTexture.free();

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
	if (!LoadMedia())
	{
		return 0;
	}

	bool quit = false;
	SDL_Event event;
	int frame = 0;
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

		SDL_Rect* Lip = &mSpriteClips[frame / 318];
		mSpriteSheetTexture.render((640 - Lip->w) / 2, (480 - Lip->h) / 2, Lip);

		SDL_RenderPresent(mRenderer);
		++frame;

		if (frame / 318 >= WALKING_FRAMES)
		{
			frame = 0;
		}
	}
	Close();
	return 0;
}