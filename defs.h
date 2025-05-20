

#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "đuổi bắt";
using namespace std;

enum GameState{
    MENU,
    PLAY,
    GG,
    QUIT
};

void handleMusicToggle(SDL_Event& e, SDL_Rect btnRect, bool& isPlaying) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mx = e.button.x, my = e.button.y;
        if (mx>=btnRect.x && mx<=btnRect.x+btnRect.w &&
            my>=btnRect.y && my<=btnRect.y+btnRect.h)
        {
            if (isPlaying) Mix_PauseMusic();
            else Mix_ResumeMusic();
            isPlaying = !isPlaying;
        }
    }
}

void drawMusicButton(SDL_Renderer* renderer, SDL_Texture* On, SDL_Texture* Off,
                     SDL_Rect btnRect, bool isPlaying)
{
    SDL_Texture* cur = isPlaying ? On : Off;
    SDL_RenderCopy(renderer, cur, nullptr, &btnRect);
}

void drawButton(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const SDL_Color& bgColor, const string& text, bool hover) {
    SDL_Color fillColor = bgColor;
    if (hover) {
        fillColor.r = min(255, bgColor.r + 40);
        fillColor.g = min(255, bgColor.g + 40);
        fillColor.b = min(255, bgColor.b + 40);
    }

    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textW, textH;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRect = {
        rect.x + (rect.w - textW) / 2,
        rect.y + (rect.h - textH) / 2,
        textW, textH
    };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

GameState showStartMenu(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Rect btnRect,
                        bool& isPlaying, SDL_Texture* On , SDL_Texture* Off)
{
    SDL_Event e;
    SDL_Rect startButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 60, 200, 50 };
    SDL_Rect exitButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 10, 200, 50 };

    while (true) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        SDL_Point mousePoint = { mx, my };
        bool hoverStart = SDL_PointInRect(&mousePoint, &startButton);
        bool hoverExit  = SDL_PointInRect(&mousePoint, &exitButton);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return QUIT;
            handleMusicToggle(e, btnRect, isPlaying);
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (hoverStart) { return PLAY; }
                if (hoverExit) { return QUIT;}
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        drawButton(renderer, font, startButton, {50, 50, 50, 0}, "START GAME", hoverStart);
        drawButton(renderer, font, exitButton, {50, 50, 50, 0}, "QUIT", hoverExit);
        drawMusicButton(renderer, On, Off, btnRect, isPlaying);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    return QUIT;
}
GameState showGameOverScreen(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Rect btnRect,
                             bool& isPlaying, SDL_Texture* On , SDL_Texture* Off)
{
    SDL_Rect menuBtn = { SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 20, 200, 50 };
    SDL_Rect retryBtn= { SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 90, 200, 50 };
    SDL_Event e;
    while (true) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mp = { mx, my };
        bool hMenu  = SDL_PointInRect(&mp, &menuBtn);
        bool hRetry = SDL_PointInRect(&mp, &retryBtn);

        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT) { return MENU; }
            handleMusicToggle(e, btnRect, isPlaying);
            if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                if (hMenu)  { return MENU; }
                if (hRetry) { return PLAY; }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30,30,30,255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255,255,255,255};
        SDL_Surface* sTitle = TTF_RenderUTF8_Blended(font, "YOU LOSE", white);
        SDL_Texture* tTitle = SDL_CreateTextureFromSurface(renderer, sTitle);
        int tw, th; SDL_QueryTexture(tTitle,nullptr,nullptr,&tw,&th);
        SDL_Rect rTitle = { (SCREEN_WIDTH-tw)/2, SCREEN_HEIGHT/2 - 120, tw, th };
        SDL_RenderCopy(renderer, tTitle, nullptr, &rTitle);
        SDL_FreeSurface(sTitle);
        SDL_DestroyTexture(tTitle);

        drawButton(renderer, font, menuBtn,  {50,50,50,0}, "MAIN MENU", hMenu);
        drawButton(renderer, font, retryBtn, {50,50,50,0}, "PLAY AGAIN",  hRetry);
        drawMusicButton(renderer, On, Off, btnRect, isPlaying);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

#endif
