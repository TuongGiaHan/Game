#include <iostream>
#include <cmath>
#include <string>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "graphics.h"
#include "defs.h"
#include "game.h"

const float SPEED = 3.0f;
const float LOSE_DISTANCE = 15.0f;
const float MIN_DISTANCE = 15.0f;

using namespace std;

void handleCollision(vector<Dot>& dots) {
    for (size_t i = 0; i < dots.size(); ++i) {
        for (size_t j = i + 1; j < dots.size(); ++j) {
            float dx = dots[j].rect.x - dots[i].rect.x;
            float dy = dots[j].rect.y - dots[i].rect.y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < MIN_DISTANCE && distance > 0) {
                float overlap = MIN_DISTANCE - distance;
                float moveX = (dx / distance) * (overlap / 2);
                float moveY = (dy / distance) * (overlap / 2);

                dots[i].rect.x -= moveX;
                dots[i].rect.y -= moveY;
                dots[j].rect.x += moveX;
                dots[j].rect.y += moveY;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont ("TimeNR.ttf",24);
    Graphics graphics;
    graphics.init();
    //
    SDL_Texture* On  = IMG_LoadTexture(graphics.renderer, "V_on.png");
    SDL_Texture* Off = IMG_LoadTexture(graphics.renderer, "V_off.png");
    SDL_Rect btnRect = { 720, 20, 64, 64 };
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* music = Mix_LoadMUS("music.mp3");
    Mix_Chunk* collect = Mix_LoadWAV("collect.mp3");
    Mix_VolumeChunk(collect, 100);
    Mix_PlayMusic(music, -1);
    bool isPlaying = true;
    //
    GameState state = MENU;
    vector <Dot> dots;

    while (state != QUIT)
        {

        //
        if (state == MENU)
        {
            extern GameState showStartMenu(SDL_Renderer*, SDL_Window*, TTF_Font*, SDL_Rect , bool&, SDL_Texture*, SDL_Texture*);
            state = showStartMenu(graphics.renderer, graphics.window, font, btnRect, isPlaying, On, Off);
            if (state == QUIT) break;
            dots.clear();
        }
        //
        if (state == PLAY)
        {
        Mouse mouse(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        Cheese cheese(200,200);
        Leaf leaf(600,400);

        Uint32 lDtime = SDL_GetTicks();
        int point = 0;
        bool quit = false, lost = false;
        SDL_Event event;

        while (!quit && !lost)
        {
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
            handleMusicToggle(event, btnRect, isPlaying);
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        mouse.dx = 0;
        mouse.dy = 0;
        if (currentKeyStates[SDL_SCANCODE_UP]) mouse.dy = -1;
        if (currentKeyStates[SDL_SCANCODE_DOWN]) mouse.dy = 1;
        if (currentKeyStates[SDL_SCANCODE_LEFT]) mouse.dx = -1;
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) mouse.dx = 1;
        mouse.move();

        if (mouse.canEat(cheese))
        {
            mouse.grow();
            randomizeCheese(cheese);
            Mix_PlayChannel( -1, collect, 0 );
            point +=2;
        }
        if (mouse.cansmall(leaf))
        {
            mouse.small();
            randomizeLeaf(leaf);
            Mix_PlayChannel( -1, collect, 0 );
            point +=5;
        }

        Uint32 curentTime = SDL_GetTicks();
        if (curentTime - lDtime >= 7000){
            if (dots.size() >= 20){
                dots.erase(dots.begin());
            }
            dots.push_back({400,100});
            lDtime = curentTime;
        }
        for (auto& dot : dots)
        {
            float dx = mouse.rect.x - dot.rect.x;
            float dy = mouse.rect.y - dot.rect.y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance > 0) {
                dot.rect.x += (dx / distance) * SPEED;
                dot.rect.y += (dy / distance) * SPEED;
            }
            if (distance <= LOSE_DISTANCE){ lost = true; break; }
            render (dot, graphics);
        }
        handleCollision(dots);

        render(mouse, graphics);
        render(cheese, graphics);
        render(leaf, graphics);

        SDL_Texture* curTex = isPlaying ? On : Off;
        SDL_RenderCopy(graphics.renderer, curTex, nullptr, &btnRect);

        renderScore(graphics.renderer, font, point);
        graphics.presentScene();
        SDL_Delay(10);
        if (lost) state = GG;
        }
        }
        //
        if (state == GG){
            state = showGameOverScreen(graphics.renderer, graphics.window, font, btnRect, isPlaying, On, Off);
            dots.clear();
        }
    }
//
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_FreeChunk(collect);
    SDL_DestroyTexture(On);
    SDL_DestroyTexture(Off);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    graphics.quit();
    return 0;
}
