#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>

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
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

bool Init();
bool LoadMedia();
void Close();

SDL_Window* mWindow = NULL;
SDL_Renderer* mRenderer = NULL;
LTexture mPromptTexture;

Mix_Music* mMusic = NULL;
Mix_Chunk* mScratch = NULL;
Mix_Chunk* mHigh = NULL;
Mix_Chunk* mMedium = NULL;
Mix_Chunk* mLow = NULL;

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

	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image.");
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture.");
	}
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);

	mTexture = newTexture;
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
	SDL_Rect renderQuad = { x,y,mWidth,mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("1");
		return false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning:Linear texture filtering not enabled!");
		}

		mWindow = SDL_CreateWindow("WY", 100, 100, 640, 480, 0);
		if (mWindow == NULL)
		{
			printf("5");
			return false;
		}
		else
		{
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mRenderer == NULL)
			{
				printf("6");
				return false;
			}
			else
			{
				if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
				{
					printf("7");
					return false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("8");
					return false;
				}
			}
		}
	}
	return true;
}

bool LoadMedia()
{
	if (!mPromptTexture.LoadFromFile("D:/Documents/visual stdio documents/Exercises/18---Sound Effects and music/prompt.png"))
	{
		printf("Failed to load prompt texture!\n");
		return false;
	}

	mMusic = Mix_LoadMUS("D:/Documents/visual stdio documents/Exercises/18---Sound Effects and music/beat.wav");
	if (mMusic == NULL)
	{
		printf("Failed to load beat music!");
		return false;
	}

	mScratch = Mix_LoadWAV("D:/Documents/visual stdio documents/Exercises/18---Sound Effects and music/scratch.wav");
	if (mScratch == NULL)
	{
		printf("Failed to load scratch sound effect!");
		return false;
	}

	mHigh = Mix_LoadWAV("D:/Documents/visual stdio documents/Exercises/18---Sound Effects and music/high.wav");
	if (mHigh == NULL)
	{
		printf("Failed to load high sound effect!");
		return false;
	}

	mMedium = Mix_LoadWAV("D:/Documents/visual stdio documents/Exercises/18---Sound Effects and music/medium.wav");
	if (mMedium == NULL)
	{
		printf("Failed to load medium effect!");
		return false;
	}

	mLow = Mix_LoadWAV("D:/Documents/visual stdio documents/Exercises/18---Sound Effects and music/low.wav");
	if (mLow == NULL)
	{
		printf("Failed to load low sound effect!");
		return false;
	}

	return true;
}

void Close()
{
	mPromptTexture.free();

	Mix_FreeChunk(mScratch);
	Mix_FreeChunk(mHigh);
	Mix_FreeChunk(mMedium);
	Mix_FreeChunk(mLow);

	Mix_FreeMusic(mMusic);

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc,char* args[])
{
	if (!Init())
	{
		printf("2");
		return 0;
	}
	if (!LoadMedia())
	{
		printf("3");
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
			else if(event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					Mix_PlayChannel(-1, mHigh, 0);break;
				case SDLK_2:
					Mix_PlayChannel(-1, mMedium, 0); break;
				case SDLK_3:
					Mix_PlayChannel(-1, mLow, 0); break;
				case SDLK_4:
					Mix_PlayChannel(-1, mScratch, 0); break;
				
				case SDLK_9:
					if (Mix_PlayingMusic() == 0)
					{
						Mix_PlayMusic(mMusic, -1);
					}
					else
					{
						if (Mix_PausedMusic() == 1)
						{
							Mix_ResumeMusic();
						}
						else
						{
							Mix_PauseMusic();
						}
					}
					break;
					
				case SDLK_0:
					Mix_HaltMusic(); break;
				}
			}
		}
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderPresent(mRenderer);

		mPromptTexture.render(0, 0);
		SDL_RenderPresent(mRenderer);
	}
	Close();
	return 0;
}