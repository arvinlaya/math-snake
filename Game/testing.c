 #include <stdio.h>
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_timer.h>
 #include <SDL2/SDL_ttf.h>

 #define WINDOW_WIDTH 900
 #define WINDOW_HEIGHT 500
 #define SPEED 300

 int main(int argc, char* argv[]) {

    //Initialize graphics
    SDL_Init(SDL_INIT_EVERYTHING);

    //Initialize window
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    //Initialize renderer
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, SDL_GetWindowSurface(window));

    SDL_RenderCopy(render, texture, NULL, NULL);

    TTF_Init();

    TTF_Font* font = TTF_OpenFont("baby blocks.ttf", 100);
    if(font == NULL) {
        printf("Error");
    }

    SDL_Color fontColor;
    fontColor.r = 0;
    fontColor.g = 0;
    fontColor.b = 0;
    fontColor.a = 255;

    SDL_Rect fontProperties;
    fontProperties.y = 0;
    fontProperties.x = 0;

    SDL_Surface* bgSurface = SDL_LoadBMP("p1.bmp");
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(render, bgSurface);
    SDL_RenderCopy(render, bgTexture, NULL, NULL);

    SDL_Surface* fontSurface = TTF_RenderText_Solid(font, "samplenewline", fontColor);
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(render, fontSurface);
    SDL_QueryTexture(texture, NULL, NULL, &fontProperties.w, &fontProperties.h);
    fontProperties.h = 50;
    fontProperties.w = 100;

    SDL_RenderCopy(render, fontTexture, NULL, &fontProperties);



    SDL_RenderPresent(render);

    SDL_Delay(2000);
    //System closing
    TTF_CloseFont(font);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
 }
