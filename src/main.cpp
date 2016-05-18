
#include <Hamjet/Engine.hpp>
#include <Hamjet/Memory.hpp>
#include <Hamjet/ImageLoader.hpp>
#include <Hamjet/Application.hpp>

#include <memory>

using namespace Hamjet;

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256

#define NEED_NONE 0
#define NEED_HAPPINESS 1
#define NEED_SLEEPINESS 2

class DungeonApp : public Application {
private:
	const float MAN_MOVE_DELAY = 5;
	const SDL_Rect manPositions[4] = {
		{ 0, 0, 64, 64 },
		{ -3, 0, 64, 64 },
		{ 0, 0, 64, 64 },
		{ 3, 0, 64, 64 }
	};
	const SDL_Rect hooksArea = { 5, 3, 20, 32 };
	const SDL_Rect waterArea = { 40, 40, 20, 12 };

	SDL_Texture_Ptr renderTexture;

	SDL_Surface_Ptr backgroundSurf;
	SDL_Texture_Ptr backgroundTex;

	SDL_Surface_Ptr dungeonSurf;
	SDL_Texture_Ptr dungeonTex;

	SDL_Surface_Ptr manSurfs[2];
	SDL_Texture_Ptr manTexs[2];

	SDL_Surface_Ptr titleSurfs[2];
	SDL_Texture_Ptr titleTexs[2];

	SDL_Surface_Ptr bubbleSurf;
	SDL_Texture_Ptr bubbleTex;

	SDL_Surface_Ptr happinessSurf;
	SDL_Texture_Ptr happinessTex;
	SDL_Surface_Ptr sleepinessSurf;
	SDL_Texture_Ptr sleepinessTex;

	SDL_Surface_Ptr hookSurfs[2];
	SDL_Texture_Ptr hookTexs[2];
	SDL_Surface_Ptr hookOutlineSurfs[2];
	SDL_Texture_Ptr hookOutlineTexs[2];

	SDL_Surface_Ptr waterSurf;
	SDL_Texture_Ptr waterTex;
	SDL_Surface_Ptr waterOutlineSurf;
	SDL_Texture_Ptr waterOutlineTex;

	int manPosition = 0;
	float timeTillManMove = MAN_MOVE_DELAY;

	float happinessRate = 0.003f;
	float happiness = 0;

	float sleepRate = 0.007f;
	float sleep = 0;

	int currentNeed = NEED_NONE;

public:

	virtual void init(EngineConfig& config) {
		config.windowWidth = WINDOW_WIDTH;
		config.windowHeight = WINDOW_HEIGHT;
		config.windowTitle = "Neatris";
	}

	virtual void postInit() {
		backgroundSurf = Memory::wrap(ImageLoader::loadPng("assets/background.png"));
		backgroundTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, backgroundSurf.get()));

		dungeonSurf = Memory::wrap(ImageLoader::loadPng("assets/dungeon.png"));
		dungeonTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, dungeonSurf.get()));

		manSurfs[0] = Memory::wrap(ImageLoader::loadPng("assets/man1.png"));
		manTexs[0] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, manSurfs[0].get()));
		manSurfs[1] = Memory::wrap(ImageLoader::loadPng("assets/man2.png"));
		manTexs[1] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, manSurfs[1].get()));

		titleSurfs[0] = Memory::wrap(ImageLoader::loadPng("assets/title1.png"));
		titleTexs[0] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, titleSurfs[0].get()));
		titleSurfs[1] = Memory::wrap(ImageLoader::loadPng("assets/title2.png"));
		titleTexs[1] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, titleSurfs[1].get()));

		bubbleSurf = Memory::wrap(ImageLoader::loadPng("assets/bubble.png"));
		bubbleTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, bubbleSurf.get()));

		happinessSurf = Memory::wrap(ImageLoader::loadPng("assets/happiness.png"));
		happinessTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, happinessSurf.get()));
		sleepinessSurf = Memory::wrap(ImageLoader::loadPng("assets/sleepiness.png"));
		sleepinessTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, sleepinessSurf.get()));

		hookSurfs[0] = Memory::wrap(ImageLoader::loadPng("assets/hooks1.png"));
		hookTexs[0] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, hookSurfs[0].get()));
		hookSurfs[1] = Memory::wrap(ImageLoader::loadPng("assets/hooks2.png"));
		hookTexs[1] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, hookSurfs[1].get()));
		hookOutlineSurfs[0] = Memory::wrap(ImageLoader::loadPng("assets/hooks1outline.png"));
		hookOutlineTexs[0] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, hookOutlineSurfs[0].get()));
		hookOutlineSurfs[1] = Memory::wrap(ImageLoader::loadPng("assets/hooks2outline.png"));
		hookOutlineTexs[1] = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, hookOutlineSurfs[1].get()));

		waterSurf = Memory::wrap(ImageLoader::loadPng("assets/water.png"));
		waterTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, waterSurf.get()));
		waterOutlineSurf = Memory::wrap(ImageLoader::loadPng("assets/wateroutline.png"));
		waterOutlineTex = Memory::wrap(SDL_CreateTextureFromSurface(engine->windowRenderer, waterOutlineSurf.get()));


		renderTexture = Memory::wrap(SDL_CreateTexture(engine->windowRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64));
	}

	virtual bool update(float dt) {
		if (timeTillManMove < 0) {
			manPosition++;
			timeTillManMove = MAN_MOVE_DELAY;
		}
		else {
			timeTillManMove -= dt;
		}

		if (currentNeed == NEED_NONE) {
			if (happiness < 1) {
				happiness += happinessRate * dt;
			}
			else if (currentNeed == NEED_NONE) {
				currentNeed = NEED_HAPPINESS;
			}

			if (sleep < 1) {
				sleep += sleepRate * dt;
			}
			else if (currentNeed == NEED_NONE) {
				currentNeed = NEED_SLEEPINESS;
			}
		}

		return true;
	}

	virtual void draw() {
		SDL_Renderer* r = engine->windowRenderer;
		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_SetRenderTarget(r, renderTexture.get());
		SDL_RenderCopy(r, backgroundTex.get(), NULL, NULL);
		SDL_RenderCopy(r, dungeonTex.get(), NULL, NULL);

		if (overHooks(x, y)) {
			SDL_RenderCopy(r, hookOutlineTexs[manPosition % 2].get(), NULL, NULL);
		}
		SDL_RenderCopy(r, hookTexs[manPosition % 2].get(), NULL, NULL);

		if (overWater(x, y)) {
			SDL_RenderCopy(r, waterOutlineTex.get(), NULL, NULL);
		}
		SDL_RenderCopy(r, waterTex.get(), NULL, NULL);

		SDL_RenderCopy(r, manTexs[manPosition % 2].get(), NULL, &manPositions[manPosition % 4]);

		SDL_Rect titlePos = { 0, 1, 64, 64 };
		SDL_RenderCopy(r, titleTexs[manPosition % 2].get(), NULL, &titlePos);

		if (currentNeed != NEED_NONE) {
			SDL_Rect bubbleRect = { 0, (manPosition % 2) * 2, 64, 64 };
			SDL_RenderCopy(r, bubbleTex.get(), NULL, &bubbleRect);

			if (currentNeed == NEED_HAPPINESS) {
				SDL_RenderCopy(r, happinessTex.get(), NULL, &bubbleRect);
			}
			else if (currentNeed == NEED_SLEEPINESS) {
				SDL_RenderCopy(r, sleepinessTex.get(), NULL, &bubbleRect);
			}
		}

		SDL_SetRenderTarget(r, NULL);
		SDL_RenderCopy(r, renderTexture.get(), NULL, NULL);

		SDL_RenderPresent(r);
	}

	virtual void onClick(int x, int y) {
		if (overHooks(x, y)) {
			happiness = 0;
			if (currentNeed == NEED_HAPPINESS) {
				currentNeed = NEED_NONE;
			}
		}
		else if (overWater(x, y)) {
			sleep = 0;
			if (currentNeed = NEED_SLEEPINESS) {
				currentNeed = NEED_NONE;
			}
		}
	}

	virtual void onKeyDown() {

	}

	bool overHooks(int x, int y) {
		return overRect(x, y, hooksArea);
	}

	bool overWater(int x, int y) {
		return overRect(x, y, waterArea);
	}

	bool overRect(int x, int y, const SDL_Rect& r) {
		x = x / 4;
		y = y / 4;
		return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
	}

};

APP_MAIN(DungeonApp);