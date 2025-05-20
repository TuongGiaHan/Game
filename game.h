

#ifndef GAME_H
#define GAME_H

#define CHEESE_SIZE 8
#define MOUSE_SIZE 20
const int boxLeft = 25;
const int boxRight = 775;
const int boxTop = 100;
const int boxBottom = 575;
float MOUSE_SPEED = 4.0f;


bool inside(int x, int y, SDL_Rect r) {
    return x > r.x && x < r.x+r.w && y > r.y && y < r.y+r.h;
}

bool overlap(const SDL_Rect& r1, const SDL_Rect& r2) {
    return SDL_HasIntersection(&r1 ,&r2);
}

struct Dot
{
    SDL_Rect rect;
   float x,y;
   Dot(int x, int y) {
        rect.x = x;
        rect.y = y;
        rect.h = 10;
        rect.w = 10;
    }
};

struct Cheese
{
    SDL_Rect rect;
    Cheese(int x, int y) {
        rect.x = x;
        rect.y = y;
        rect.h = CHEESE_SIZE;
        rect.w = CHEESE_SIZE;
    }
};

struct Leaf
{
    SDL_Rect rect;
    Leaf (int x,int y)
    {
        rect.x = x;
        rect.y = y;
        rect.h = CHEESE_SIZE;
        rect.w = CHEESE_SIZE;
    }
};

struct Mouse
{
    SDL_Rect rect;
    int dx = 0, dy = 0;
    Mouse(int x, int y) {
        rect.x = x;
        rect.y = y;
        rect.h = MOUSE_SIZE;
        rect.w = MOUSE_SIZE;
    }

    bool canEat(const Cheese& cheese)
    {
        return overlap(rect, cheese.rect);
    }

    bool cansmall (const Leaf& leaf)
    {
        return overlap(rect, leaf.rect);
    }

    void grow() {
        rect.h += 2;
        rect.w += 2;
    }
    void small(){
        MOUSE_SPEED += 0.1f;
    }

    void move()
    {
        int magnitude = sqrt(dx * dx + dy * dy);
        if (magnitude != 0) {
            float dirX = static_cast<float>(dx) / magnitude;
            float dirY = static_cast<float>(dy) / magnitude;

            rect.x += static_cast<int>(dirX * MOUSE_SPEED);
            rect.y += static_cast<int>(dirY * MOUSE_SPEED);
        }

        if (rect.x < boxLeft) rect.x = boxLeft;
        if (rect.x + rect.w > boxRight) rect.x = boxRight - rect.w;
        if (rect.y < boxTop) rect.y = boxTop;
        if (rect.y + rect.h > boxBottom) rect.y = boxBottom - rect.h;
    }

};

void randomizeCheese(Cheese& cheese)
{
    cheese.rect.x = boxLeft + rand() % (boxRight - boxLeft - cheese.rect.w);
    cheese.rect.y = boxTop + rand() % (boxBottom - boxTop - cheese.rect.h);
}

void randomizeLeaf(Leaf& leaf)
{
    leaf.rect.x = boxLeft + rand() % (boxRight - boxLeft - leaf.rect.w);
    leaf.rect.y = boxTop + rand() % (boxBottom - boxTop - leaf.rect.h);
}

void render(const Dot& dot, const Graphics& graphics) {
    SDL_SetRenderDrawColor(graphics.renderer, 178, 34, 34, 0);
    SDL_RenderFillRect(graphics.renderer, &dot.rect);
}

void render(const Cheese& cheese, const Graphics& graphics) {
    SDL_SetRenderDrawColor(graphics.renderer, 255, 160, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &cheese.rect);
}

void render(const Mouse& mouse, const Graphics& graphics) {
    SDL_SetRenderDrawColor(graphics.renderer, 32, 178, 170, 0);
    SDL_RenderFillRect(graphics.renderer, &mouse.rect);
}

void render(const Leaf& leaf, const Graphics& graphics) {
    SDL_SetRenderDrawColor(graphics.renderer, 34, 139, 34, 255);
    SDL_RenderFillRect(graphics.renderer, &leaf.rect);
}

#endif
