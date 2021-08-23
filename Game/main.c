 #include <stdio.h>
 #include <SDL.h>
 #include <SDL_timer.h>
 #include <SDL_ttf.h>
 #include <SDL_image.h>
 #include <SDL_mixer.h>
 #include <math.h>
 #include <string.h>

 #define WINDOW_WIDTH 1200
 #define WINDOW_HEIGHT 600

 #define PLAY_AREA_X_MIN 400
 #define PLAY_AREA_X_MAX 1180
 #define PLAY_AREA_Y_MIN 20
 #define PLAY_AREA_Y_MAX 580

 #define SPEED 20
 #define CELL 20

 #define UP 1
 #define DOWN 2
 #define RIGHT 3
 #define LEFT 4

 #define A 1
 #define B 2
 #define C 3
 #define D 4

 #define STOP_SYSTEM 0
 #define RUN_START 1
 #define RUN_DIFFICULTY 2
 #define RUN_HELP 3
 #define RUN_GAME 4
 #define RUN_OVER 5
 #define RUN_WIN 6

 typedef struct Snake {

 int win;
 int direction;
 int score;
 int tailCount;
 int xVelocity;
 int yVelocity;
 float xPosition;
 float yPosition;
 float tailX[100];
 float tailY[100];
 float lastPosX;
 float lastPosY;
 SDL_Texture* texture;

 } Snake;

 typedef struct Question {

    int num1, num2, num3, num4, num5, num6, num7, num8;
    float digitAnswer;
    int letterAnswer;
    char arithmetic[50],
         aChoice[15],
         bChoice[15],
         cChoice[15],
         dChoice[15];

 } Question;

 //Start functions
 void loadStart(int select);

 //Difficulty functions
 void loadDifficulties(int select);

 //Game functions
 void putSnake(SDL_Texture* texture, float x, float y);

 void putA(float x, float y);
 void putB(float x, float y);
 void putC(float x, float y);
 void putD(float x, float y);

 void setDirection(Snake* player, int direction);
 void wallBound(Snake* player);
 void eatSnake(Snake* p1, Snake* p2);
 void trailSnake(Snake* player);
 void deathDetect(Snake* player, float xPos, float yPos);

 int randomX();
 int randomY();

 void setAnswerPosition();
 void setChoices(int answer);

 void setQuestions(int dif);

 void createEasy();
 void setEasy(int op);

 void createAdvanced();
 void setAdvanced(int op);

 void createExpert();
 void setExpert(int op);

 void displayQuestion(char arithmetic[]);
 void displayChoices(char aString[], char bString[], char cString[], char dString[]);
 void displayScore(int multiplayer);
 void displayOverScore();
 void displayLives();
 void displayTime();
 void resetTime();
 void pause();
 void death();
 //Game Functions END

 //Game System Functions
 void setUI(int state);
 void selectEnd(int state);
 void resetGame();
 void loadingScreen();
 void gameCount();
 void opening();
 void credits();

 //Sounds
 void eatSound();
 void chooseSound();
 void selectSound();
 void winSound();
 void wrongSound();
 void overSound();
 void music(int state);

 void cleanUp();

 SDL_Renderer* render;

 SDL_Window* window;

 SDL_Surface* uiSurface;

 SDL_Surface* difficultySurface;

 SDL_Texture* startTexture1;
 SDL_Texture* startTexture2;
 SDL_Texture* startTexture3;

 SDL_Texture* difficultyTexture1;
 SDL_Texture* difficultyTexture2;
 SDL_Texture* difficultyTexture3;

 SDL_Texture* helpTexture;

 SDL_Surface* gameBG;
 SDL_Texture* bgTexture;
 SDL_Texture* arithmeticTexture;

 SDL_Surface* p1;
 SDL_Texture* p1Texture;

 SDL_Surface* p2;
 SDL_Texture* p2Texture;

 SDL_Surface* letterPic;
 SDL_Texture* textureA;
 SDL_Texture* textureB;
 SDL_Texture* textureC;
 SDL_Texture* textureD;

 SDL_Texture* p1ScoreTexture;
 SDL_Texture* p2ScoreTexture;
 SDL_Texture* overScoreTexture;

 SDL_Texture* timeTexture;

 SDL_Surface* livesSurface;
 SDL_Texture* livesTexture;

 SDL_Surface* overSurface;
 SDL_Texture* overTexture1;
 SDL_Texture* overTexture2;
 SDL_Texture* overTexture3;

 SDL_Surface* winSurface;
 SDL_Texture* winTexture1_1;
 SDL_Texture* winTexture1_2;
 SDL_Texture* winTexture1_3;
 SDL_Texture* winTexture2_1;
 SDL_Texture* winTexture2_2;
 SDL_Texture* winTexture2_3;

 SDL_Surface* loadSurface;
 SDL_Texture* loadTexture1;
 SDL_Texture* loadTexture2;
 SDL_Texture* loadTexture3;

 SDL_Surface* countSurface;
 SDL_Texture* countTexture1;
 SDL_Texture* countTexture2;
 SDL_Texture* countTexture3;

 SDL_Surface* openSurface;
 SDL_Texture* openTexture1;
 SDL_Texture* openTexture2;

 SDL_Surface* creditsSurface;
 SDL_Texture* creditsTexture;

 SDL_Surface* pauseSurface;
 SDL_Texture* pauseTexture1;
 SDL_Texture* pauseTexture2;
 SDL_Texture* pauseTexture3;

 TTF_Font* font;
 SDL_Color fontColor;

 Mix_Music* bgMusic;

 Mix_Chunk* eat;
 Mix_Chunk* select;
 Mix_Chunk* choose;
 Mix_Chunk* win;
 Mix_Chunk* wrong;
 Mix_Chunk* over;

 //Text
 float AXPosition, AYPosition;
 float BXPosition, BYPosition;
 float CXPosition, CYPosition;
 float DXPosition, DYPosition;

 Question question;

 Snake player1;
 Snake player2;

 int difficulty;
 int p1Direction;
 int p2Direction;
 int lives = 3;
 float mlSecs;
 int secsTime;
 int gameExit = 0;
 int musicState;

 int multiplayer = 0;
 int systemRunning = 1;
 int startRunning = 1;
 int difficultyRunning = 0;
 int helpRunning = 0;
 int gameRunning = 0;
 int overRunning = 0;
 int winRunning = 0;
 int openingRunning = 1;

 int main(int argc, char* argv[]) {

    srand(time(NULL));

    //Initialize graphics
    SDL_Init(SDL_INIT_EVERYTHING);

    //Initialize text
    TTF_Init();

    fontColor.r = 255;
    fontColor.g = 255;
    fontColor.b = 255;
    fontColor.a = 255;

    font = TTF_OpenFont("runescape_uf.ttf", 27);

    //Initialize window
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    //Initialize renderer
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Initialize image loader
    IMG_Init(IMG_INIT_PNG);

    //Initialize sound loader
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    //Load background music
    bgMusic = Mix_LoadMUS("sfx/bgMusic.mp3");

    //Load sound effect
    eat = Mix_LoadWAV("sfx/eat.mp3");
    select = Mix_LoadWAV("sfx/select.mp3");
    choose = Mix_LoadWAV("sfx/choose.mp3");
    win = Mix_LoadWAV("sfx/win.mp3");
    over = Mix_LoadWAV("sfx/over.mp3");
    wrong = Mix_LoadWAV("sfx/wrong.mp3");

    //plays music
    music(1);

    //Start graphics
    uiSurface = SDL_LoadBMP("ui/uiStart1.bmp");
    startTexture1 = SDL_CreateTextureFromSurface(render, uiSurface);
    SDL_FreeSurface(uiSurface);

    uiSurface = SDL_LoadBMP("ui/uiStart2.bmp");
    startTexture2 = SDL_CreateTextureFromSurface(render, uiSurface);
    SDL_FreeSurface(uiSurface);

    uiSurface = SDL_LoadBMP("ui/uiStart3.bmp");
    startTexture3 = SDL_CreateTextureFromSurface(render, uiSurface);
    SDL_FreeSurface(uiSurface);

    uiSurface = SDL_LoadBMP("ui/uiHelp1.bmp");
    helpTexture = SDL_CreateTextureFromSurface(render, uiSurface);
    SDL_FreeSurface(uiSurface);

    difficultySurface = SDL_LoadBMP("ui/uiDifficulties1.bmp");
    difficultyTexture1 = SDL_CreateTextureFromSurface(render, difficultySurface);
    SDL_FreeSurface(difficultySurface);

    difficultySurface = SDL_LoadBMP("ui/uiDifficulties2.bmp");
    difficultyTexture2 = SDL_CreateTextureFromSurface(render, difficultySurface);
    SDL_FreeSurface(difficultySurface);

    difficultySurface = SDL_LoadBMP("ui/uiDifficulties3.bmp");
    difficultyTexture3 = SDL_CreateTextureFromSurface(render, difficultySurface);
    SDL_FreeSurface(difficultySurface);

    //Game graphics
    gameBG = SDL_LoadBMP("game/background.bmp");
    bgTexture = SDL_CreateTextureFromSurface(render, gameBG);
    SDL_FreeSurface(gameBG);

    p1 = SDL_LoadBMP("game/p1.bmp");
    player1.texture = SDL_CreateTextureFromSurface(render, p1);
    SDL_FreeSurface(p1);

    p2 = SDL_LoadBMP("game/p2.bmp");
    player2.texture = SDL_CreateTextureFromSurface(render, p2);
    SDL_FreeSurface(p2);

    letterPic= IMG_Load("game/A.png");
    textureA = SDL_CreateTextureFromSurface(render, letterPic);
    SDL_FreeSurface(letterPic);

    letterPic = IMG_Load("game/B.png");
    textureB = SDL_CreateTextureFromSurface(render, letterPic);
    SDL_FreeSurface(letterPic);

    letterPic = IMG_Load("game/C.png");
    textureC = SDL_CreateTextureFromSurface(render, letterPic);
    SDL_FreeSurface(letterPic);

    letterPic = IMG_Load("game/D.png");
    textureD = SDL_CreateTextureFromSurface(render, letterPic);
    SDL_FreeSurface(letterPic);

    livesSurface = IMG_Load("ui/uiLives.png");
    livesTexture = SDL_CreateTextureFromSurface(render, livesSurface);
    SDL_FreeSurface(livesSurface);

    overSurface = SDL_LoadBMP("ui/uiOver1.bmp");
    overTexture1 = SDL_CreateTextureFromSurface(render, overSurface);
    SDL_FreeSurface(overSurface);

    overSurface = SDL_LoadBMP("ui/uiOver2.bmp");
    overTexture2 = SDL_CreateTextureFromSurface(render, overSurface);
    SDL_FreeSurface(overSurface);

    overSurface = SDL_LoadBMP("ui/uiOver3.bmp");
    overTexture3 = SDL_CreateTextureFromSurface(render, overSurface);
    SDL_FreeSurface(overSurface);

    winSurface = SDL_LoadBMP("ui/uiWin1.1.bmp");
    winTexture1_1 = SDL_CreateTextureFromSurface(render, winSurface);
    SDL_FreeSurface(winSurface);

    winSurface = SDL_LoadBMP("ui/uiWin1.2.bmp");
    winTexture1_2 = SDL_CreateTextureFromSurface(render, winSurface);
    SDL_FreeSurface(winSurface);

    winSurface = SDL_LoadBMP("ui/uiWin1.3.bmp");
    winTexture1_3 = SDL_CreateTextureFromSurface(render, winSurface);
    SDL_FreeSurface(winSurface);

    winSurface = SDL_LoadBMP("ui/uiWin2.1.bmp");
    winTexture2_1 = SDL_CreateTextureFromSurface(render, winSurface);
    SDL_FreeSurface(winSurface);

    winSurface = SDL_LoadBMP("ui/uiWin2.2.bmp");
    winTexture2_2 = SDL_CreateTextureFromSurface(render, winSurface);
    SDL_FreeSurface(winSurface);

    winSurface = SDL_LoadBMP("ui/uiWin2.3.bmp");
    winTexture2_3 = SDL_CreateTextureFromSurface(render, winSurface);
    SDL_FreeSurface(winSurface);

    loadSurface = SDL_LoadBMP("ui/uiLoading1.bmp");
    loadTexture1 = SDL_CreateTextureFromSurface(render, loadSurface);
    SDL_FreeSurface(loadSurface);

    loadSurface = SDL_LoadBMP("ui/uiLoading2.bmp");
    loadTexture2 = SDL_CreateTextureFromSurface(render, loadSurface);
    SDL_FreeSurface(loadSurface);

    loadSurface = SDL_LoadBMP("ui/uiLoading3.bmp");
    loadTexture3 = SDL_CreateTextureFromSurface(render, loadSurface);
    SDL_FreeSurface(loadSurface);

    countSurface = SDL_LoadBMP("game/count1.bmp");
    countTexture1 = SDL_CreateTextureFromSurface(render, countSurface);
    SDL_FreeSurface(countSurface);

    countSurface = SDL_LoadBMP("game/count2.bmp");
    countTexture2 = SDL_CreateTextureFromSurface(render, countSurface);
    SDL_FreeSurface(countSurface);

    countSurface = SDL_LoadBMP("game/count3.bmp");
    countTexture3 = SDL_CreateTextureFromSurface(render, countSurface);
    SDL_FreeSurface(countSurface);

    openSurface = SDL_LoadBMP("ui/uiOpen1.bmp");
    openTexture1 = SDL_CreateTextureFromSurface(render, openSurface);
    SDL_FreeSurface(openSurface);

    openSurface = SDL_LoadBMP("ui/uiOpen2.bmp");
    openTexture2 = SDL_CreateTextureFromSurface(render, openSurface);
    SDL_FreeSurface(openSurface);

    creditsSurface = SDL_LoadBMP("ui/uiCredits.bmp");
    creditsTexture = SDL_CreateTextureFromSurface(render, creditsSurface);
    SDL_FreeSurface(pauseSurface);

    pauseSurface = SDL_LoadBMP("ui/uiPause1.bmp");
    pauseTexture1 = SDL_CreateTextureFromSurface(render, pauseSurface);
    SDL_FreeSurface(pauseSurface);

    pauseSurface = SDL_LoadBMP("ui/uiPause2.bmp");
    pauseTexture2 = SDL_CreateTextureFromSurface(render, pauseSurface);
    SDL_FreeSurface(pauseSurface);

    pauseSurface = SDL_LoadBMP("ui/uiPause3.bmp");
    pauseTexture3 = SDL_CreateTextureFromSurface(render, pauseSurface);
    SDL_FreeSurface(pauseSurface);

    SDL_Event event;

    credits();
    opening();

    p1Direction = 0;
    p2Direction = 0;
    setAnswerPosition();

    int uiSelect = 1;
    int difficultySelect = 1;
    int overScore;

    setQuestions(difficulty);
    musicState = 1;
    while(systemRunning) {

        while(startRunning) {

            while(SDL_PollEvent(&event)) {

                switch(event.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        break;

                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {

                            case SDLK_UP:
                                chooseSound();
                                (uiSelect > 1)? uiSelect--: 0;
                                break;

                            case SDLK_DOWN:
                                chooseSound();
                                (uiSelect < 3)? uiSelect++: 0;
                                break;

                            case SDLK_f:
                                selectSound();
                                setUI(STOP_SYSTEM);
                                break;

                            case SDLK_RETURN:
                                selectSound();
                                switch(uiSelect) {

                                    case 1:
                                        multiplayer = 0;
                                        setUI(RUN_DIFFICULTY);
                                        break;

                                    case 2:
                                        multiplayer = 1;
                                        setUI(RUN_DIFFICULTY);
                                        break;

                                    case 3:
                                        setUI(RUN_HELP);
                                        break;
                                }
                                break;

                            case SDLK_m:
                                    musicState = (musicState == 0)? 1: 0;
                                    music(musicState);
                                    break;

                        }
                        break;
                }

            }

            SDL_RenderClear(render);
            loadStart(uiSelect);
            SDL_RenderPresent(render);

        }

        while(difficultyRunning) {

            while(SDL_PollEvent(&event)) {

                switch(event.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        break;

                    case SDL_KEYDOWN:

                        switch(event.key.keysym.sym) {

                            case SDLK_m:
                                    musicState = (musicState == 0)? 1: 0;
                                    music(musicState);
                                    break;

                            case SDLK_UP:
                                chooseSound();
                                (difficultySelect > 1)? difficultySelect--: 0;
                                break;

                            case SDLK_DOWN:
                                chooseSound();
                                (difficultySelect < 3)? difficultySelect++: 0;
                                break;
                            case SDLK_b:
                                selectSound();
                                setUI(RUN_START);
                                break;

                            case SDLK_RETURN:
                                selectSound();
                                difficulty = difficultySelect;
                                setUI(RUN_GAME);
                                break;
                        }
                    break;
                }
            }
            SDL_RenderClear(render);
            loadDifficulties(difficultySelect);
            SDL_RenderPresent(render);
        }

        while(helpRunning) {

            while(SDL_PollEvent(&event)) {

                switch(event.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        break;

                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {

                            case SDLK_m:
                                musicState = (musicState == 0)? 1: 0;
                                music(musicState);
                                break;

                            case SDLK_b:
                                selectSound();
                                setUI(RUN_START);
                                break;
                        }
                    break;
                }
            }
            SDL_RenderClear(render);
            SDL_RenderCopy(render, helpTexture, NULL, NULL);
            SDL_RenderPresent(render);
        }


        setQuestions(difficulty);

        switch(difficulty) {

            case 1:
                mlSecs = 10;
                break;
            case 2: case 3:
                mlSecs = 15;
                break;

        }

        int speed = 0;
        int initialized = 0;
        while(gameRunning) {
            while(initialized == 0) {
                initialized++;
                gameExit = 0;
                p1Direction = 0;
                p2Direction = 0;
                resetGame();
                resetTime();
                loadingScreen();
                gameCount();
            }
            while(SDL_PollEvent(&event)) {

                switch(event.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        break;

                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {

                            case SDLK_m:
                                musicState = (musicState == 0)? 1: 0;
                                music(musicState);
                                break;

                            case SDLK_x:
                                player2.score = 15;
                                break;

                            case SDLK_z:
                                setAnswerPosition();
                                break;

                            case SDLK_p:
                                pause();
                                break;

                            case SDLK_b:
                                setUI(RUN_START);

                            case SDLK_UP:
                                (p1Direction != DOWN)? p1Direction = UP: 0;
                                break;

                            case SDLK_DOWN:
                                (p1Direction != UP)? p1Direction = DOWN: 0;
                                break;

                            case SDLK_RIGHT:
                                (p1Direction != LEFT)? p1Direction = RIGHT: 0;
                                break;

                            case SDLK_LEFT:
                                (p1Direction != RIGHT)? p1Direction = LEFT: 0;
                                break;

                            case SDLK_w:
                                (p2Direction != DOWN)? p2Direction = UP: 0;
                                break;

                            case SDLK_s:
                                (p2Direction != UP)? p2Direction = DOWN: 0;
                                break;

                            case SDLK_d:
                                (p2Direction != LEFT)? p2Direction = RIGHT: 0;
                                break;

                            case SDLK_a:
                                (p2Direction != RIGHT)? p2Direction = LEFT: 0;
                                break;

                        }
                        break;
                }
            }

                SDL_RenderClear(render);

                speed = (speed >= 0)? (player1.score + player2.score): 0;
                if(multiplayer) {
                    if(player1.score == 15) {
                        player1.win = 1;
                        setUI(RUN_WIN);
                    }
                    else if(player2.score == 15) {
                        player2.win = 1;
                        setUI(RUN_WIN);
                    }

                }
                SDL_RenderCopy(render, bgTexture, NULL, NULL);

                setDirection(&player1, p1Direction);
                eatSnake(&player1, &player2);
                wallBound(&player1);
                putSnake(player1.texture, player1.xPosition, player1.yPosition);
                trailSnake(&player1);

                if(multiplayer == 1) {
                setDirection(&player2, p2Direction);
                eatSnake(&player2, &player1);
                wallBound(&player2);
                putSnake(player2.texture, player2.xPosition, player2.yPosition);
                trailSnake(&player2);
                }

                putA(AXPosition, AYPosition);
                putB(BXPosition, BYPosition);
                putC(CXPosition, CYPosition);
                putD(DXPosition, DYPosition);

                displayQuestion(question.arithmetic);
                displayChoices(question.aChoice, question.bChoice, question.cChoice, question.dChoice);
                displayScore(multiplayer);

                if(multiplayer == 0 && lives == 0) {
                    overScore = player1.score;
                    death();
                }
                (multiplayer == 0)? displayTime(): 0;
                (multiplayer == 0)? displayLives(): 0;

                (gameExit == 1)? setUI(RUN_START): SDL_RenderPresent(render);

                SDL_Delay(1000/ (10 + speed));


        }

        int overSelect = 1;
        while(overRunning) {

            while(SDL_PollEvent(&event)) {

                switch(event.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        break;

                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {

                            case SDLK_m:
                                musicState = (musicState == 0)? 1: 0;
                                music(musicState);
                                break;

                            case SDLK_UP: case SDLK_LEFT:
                                chooseSound();
                                (overSelect > 1)? overSelect--: 0;
                                break;

                            case SDLK_DOWN: case SDLK_RIGHT:
                                chooseSound();
                                (overSelect < 3)? overSelect++: 0;
                                break;

                            case SDLK_RETURN:
                                selectSound();
                                selectEnd(overSelect);
                                break;

                        }

                }

            }

            SDL_RenderClear(render);
            switch(overSelect) {

                case 1:
                    SDL_RenderCopy(render, overTexture1, NULL, NULL);
                    break;

                case 2:
                    SDL_RenderCopy(render, overTexture2, NULL, NULL);
                    break;

                case 3:
                    SDL_RenderCopy(render, overTexture3, NULL, NULL);
                    break;

            }
            displayOverScore();
            SDL_RenderPresent(render);

        }

        int winSelect = 1;
        while(winRunning) {
            while(SDL_PollEvent(&event)) {

                switch(event.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        break;

                    case SDL_KEYDOWN:

                        switch(event.key.keysym.sym) {

                            case SDLK_m:
                                musicState = (musicState == 0)? 1: 0;
                                music(musicState);
                                break;

                            case SDLK_UP: case SDLK_LEFT:
                                chooseSound();
                                (winSelect > 1)? winSelect--: 0;
                                break;

                            case SDLK_DOWN: case SDLK_RIGHT:
                                chooseSound();
                                (winSelect < 3)? winSelect++: 0;
                                break;

                            case SDLK_RETURN:
                                selectSound();
                                selectEnd(winSelect);
                                break;

                        }
                        break;
                }

            }
            SDL_RenderClear(render);
            if(player1.win) {

                switch(winSelect) {
                    case 1:
                        SDL_RenderCopy(render, winTexture1_1, NULL, NULL);
                        break;

                    case 2:
                        SDL_RenderCopy(render, winTexture1_2, NULL, NULL);
                        break;

                    case 3:
                        SDL_RenderCopy(render, winTexture1_3, NULL, NULL);
                        break;
                }

            }
            else if(player2.win) {
                switch(winSelect) {

                    case 1:
                        SDL_RenderCopy(render, winTexture2_1, NULL, NULL);
                        break;

                    case 2:
                        SDL_RenderCopy(render, winTexture2_2, NULL, NULL);
                        break;

                    case 3:
                        SDL_RenderCopy(render, winTexture2_3, NULL, NULL);
                        break;
                }

            }
            SDL_RenderPresent(render);

        }


    }


    //System closing
    cleanUp();

    return 0;
    }

    void credits() {
        SDL_Event credit;
        int x = 1;

        while(x <= 90) {

            while(SDL_PollEvent(&credit)) {

                switch(credit.type) {

                    case SDL_QUIT:
                        x = 90;
                        openingRunning = 0;
                        setUI(STOP_SYSTEM);

                        break;

                }
            }

            SDL_RenderClear(render);
            if(x <= 25) {
                SDL_SetTextureBlendMode(creditsTexture, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(creditsTexture, (x*10));
                SDL_RenderCopy(render, creditsTexture, NULL, NULL);
                SDL_Delay(10);
            }
            else if(x <= 65) {
                SDL_RenderCopy(render, creditsTexture, NULL, NULL);
                SDL_Delay(100);
            }
            else {
                SDL_SetTextureAlphaMod(creditsTexture, ((90 - x) * 10));
                SDL_RenderCopy(render, creditsTexture, NULL, NULL);
                SDL_Delay(10);
            }
            SDL_RenderPresent(render);
            x++;

        }
    }

    void opening() {

        int blink = 1;
        SDL_Event open;

        while(openingRunning) {

            while(SDL_PollEvent(&open)) {

                 switch(open.type) {

                    case SDL_QUIT:
                        setUI(STOP_SYSTEM);
                        openingRunning = 0;
                        break;

                    case SDL_KEYDOWN:
                        selectSound();
                        openingRunning = 0;
                        break;

                }

            }
            SDL_RenderClear(render);
            if(blink <= 7) {
                SDL_RenderCopy(render, openTexture1, NULL, NULL);
                blink++;
            }
            else if(blink <= 14){
                SDL_RenderCopy(render, openTexture2, NULL, NULL);
                blink++;
                blink = (blink == 14)? 1: blink;
            }
            SDL_RenderPresent(render);
            SDL_Delay(100);
        }

    }
    //Start functions
    void loadStart(int select) {

        switch(select) {

            case 1:
                SDL_RenderCopy(render, startTexture1, NULL, NULL);
                break;
            case 2:
                SDL_RenderCopy(render, startTexture2, NULL, NULL);
                break;
            case 3:
                SDL_RenderCopy(render, startTexture3, NULL, NULL);
                break;
        }

    }
    //Start functions END

    //Difficulty functions
    void loadDifficulties(int select) {

        switch(select) {

            case 1:
                SDL_RenderCopy(render, difficultyTexture1, NULL, NULL);
                break;
            case 2:
                SDL_RenderCopy(render, difficultyTexture2, NULL, NULL);
                break;
            case 3:
                SDL_RenderCopy(render, difficultyTexture3, NULL, NULL);
                break;

        }

    }
    //Difficulty functions END

    //Game functions
    void putSnake(SDL_Texture* texture, float x, float y) {
        SDL_Rect head;
        head.h = 20;
        head.w = 20;
        head.x = x;
        head.y = y;

        SDL_RenderCopy(render, texture, NULL, &head);
    }

     void trailSnake(Snake* player) {
        for(int x = player->tailCount; x > 0; x--) {
            player->tailX[x] = player->tailX[x - 1];
            player->tailY[x] = player->tailY[x - 1];
        }

        for(int z = 1; z <= player->tailCount; z++) {
                putSnake(player->texture, player->tailX[z], player->tailY[z]);
                deathDetect(player, player->tailX[z + 3], player->tailY[z + 3]);
        }
    }

    void putA(float x, float y) {
        SDL_Rect answer;
        answer.w = 25;
        answer.h = 25;
        answer.x = x;
        answer.y = y;
        SDL_RenderCopy(render, textureA, NULL, &answer);
    }

    void putB(float x, float y) {
        SDL_Rect answer;
        answer.w = 25;
        answer.h = 25;
        answer.x = x;
        answer.y = y;
        SDL_RenderCopy(render, textureB, NULL, &answer);
    }

    void putC(float x, float y) {
        SDL_Rect answer;
        answer.w = 25;
        answer.h = 25;
        answer.x = x;
        answer.y = y;
        SDL_RenderCopy(render, textureC, NULL, &answer);
    }

    void putD(float x, float y) {
        SDL_Rect answer;
        answer.w = 25;
        answer.h = 25;
        answer.x = x;
        answer.y = y;
        SDL_RenderCopy(render, textureD, NULL, &answer);
    }

    void setDirection(Snake* player, int direction) {

        player->xVelocity = 0;
        player->yVelocity = 0;

        if(direction == UP){
            player->yVelocity = -SPEED;
        }
        if(direction == DOWN) {
            player->yVelocity = SPEED;
        }
        if(direction == RIGHT) {
            player->xVelocity = SPEED;
        }
        if(direction == LEFT) {
            player->xVelocity = -SPEED;
        }

        player->lastPosX = player->xPosition;
        player->lastPosY = player->yPosition;

        player->tailX[0] = player->lastPosX;
        player->tailY[0] = player->lastPosY;

        player->xPosition += player->xVelocity;
        player->yPosition += player->yVelocity;

    }

    void wallBound(Snake* player) {

        if(player->xPosition < PLAY_AREA_X_MIN) {
            wrongSound();
            player->xPosition = PLAY_AREA_X_MIN;
            player->xPosition = ((PLAY_AREA_X_MIN + PLAY_AREA_X_MAX) / 2) + 10;
            player->yPosition = (PLAY_AREA_Y_MIN + PLAY_AREA_Y_MAX) / 2;
            (multiplayer == 1)? player->score--: 0;
            (multiplayer == 0)? lives--: 0;
        }
        if(player->xPosition > (PLAY_AREA_X_MAX - CELL)) {
            wrongSound();
            player->xPosition = PLAY_AREA_X_MAX;
            player->xPosition = ((PLAY_AREA_X_MIN + PLAY_AREA_X_MAX) / 2) + 10;
            player->yPosition = (PLAY_AREA_Y_MIN + PLAY_AREA_Y_MAX) / 2;
            (multiplayer == 1)? player->score--: 0;
            (multiplayer == 0)? lives--: 0;
        }
        if(player->yPosition < PLAY_AREA_Y_MIN) {
            wrongSound();
            player->yPosition = PLAY_AREA_Y_MIN;
            player->xPosition = ((PLAY_AREA_X_MIN + PLAY_AREA_X_MAX) / 2) + 10;
            player->yPosition = (PLAY_AREA_Y_MIN + PLAY_AREA_Y_MAX) / 2;
            (multiplayer == 1)? player->score--: 0;
            (multiplayer == 0)? lives--: 0;
        }
        if(player->yPosition > (PLAY_AREA_Y_MAX - CELL)) {
            wrongSound();
            player->yPosition = PLAY_AREA_Y_MAX - CELL;
            player->xPosition = ((PLAY_AREA_X_MIN + PLAY_AREA_X_MAX) / 2) + 10;
            player->yPosition = (PLAY_AREA_Y_MIN + PLAY_AREA_Y_MAX) / 2;
            (multiplayer == 1)? player->score--: 0;
            (multiplayer == 0)? lives--: 0;
        }

    }

    int randomX() {
        return 400 + ((((rand() % 39) + 1) * CELL) - 20);
    }

    int randomY() {
        return 20 + ((((rand() % 28) + 1) * CELL) - 20);
    }

    void setAnswerPosition() {
        AXPosition = randomX();
        AYPosition = randomY();
        BXPosition = randomX();
        BYPosition = randomY();
        CXPosition = randomX();
        CYPosition = randomY();
        DXPosition = randomX();
        DYPosition = randomY();
    }

    void eatSnake(Snake* p1, Snake* p2) {

        if(AXPosition == p1->xPosition && AYPosition == p1->yPosition) {
            eatSound();
            (multiplayer == 0)? resetTime(): 0;
            setAnswerPosition();
            if(question.letterAnswer == A) {
                p1->score++;
                p1->tailCount++;
                setQuestions(difficulty);
            }
            else {
                (multiplayer == 0)? lives--: 0;
                p2->score++;
                p2->tailCount++;
                setQuestions(difficulty);
            }
        }

        if(BXPosition == p1->xPosition && BYPosition == p1->yPosition) {
            eatSound();
            (multiplayer == 0)? resetTime(): 0;
            setAnswerPosition();
            if(question.letterAnswer == B) {
                p1->score++;
                p1->tailCount++;
                setQuestions(difficulty);
            }
            else {
                (multiplayer == 0)? lives--: 0;
                p2->score++;
                p2->tailCount++;
                setQuestions(difficulty);
            }

        }

        if(CXPosition == p1->xPosition && CYPosition == p1->yPosition) {
            eatSound();
            (multiplayer == 0)? resetTime(): 0;
            setAnswerPosition();
            if(question.letterAnswer == C) {
                p1->score++;
                p1->tailCount++;
                setQuestions(difficulty);
            }
            else {
                (multiplayer == 0)? lives--: 0;
                p2->score++;
                p2->tailCount++;
                setQuestions(difficulty);
            }
        }

        if(DXPosition == p1->xPosition && DYPosition == p1->yPosition) {
            eatSound();
            (multiplayer == 0)? resetTime(): 0;
            setAnswerPosition();
            if(question.letterAnswer == D) {
                p1->score++;
                p1->tailCount++;
                setQuestions(difficulty);
            }
            else {
                (multiplayer == 0)? lives--: 0;
                p2->score++;
                p2->tailCount++;
                setQuestions(difficulty);
            }
        }
    }

    void createEasy() {
        int operand;

        question.num1 = (rand() % 10) + 1;
        question.num2 = (rand() % 10) + 1;
        question.num3 = (rand() % 10) + 1;

        if(player1.score < 7 && player2.score < 7) {
            operand = (rand() % 2) + 1;
            switch(operand) {

                case 1:
                    question.digitAnswer = question.num1 + question.num2;
                    break;
                case 2:
                    question.digitAnswer = question.num1 - question.num2;
                    break;

            }
        }
        else {
                question.digitAnswer = (question.num1 + question.num2) - question.num3;
        }

        question.letterAnswer = (rand() % 4) + 1;
        setEasy(operand);
        setChoices(question.letterAnswer);

    }

    void createAdvanced() {
        int operand;

        question.num1 = (rand() % 50) + 10;
        question.num2 = (rand() % 50) + 10;
        question.num3 = (rand() % 10) + 1;

        if(player1.score < 7 && player2.score < 7) {
            operand = (rand() % 3) + 1;
            switch(operand) {

                case 1:
                    question.digitAnswer = question.num1 + question.num2;
                    break;

                case 2:
                    question.digitAnswer = question.num1 - question.num2;
                    break;
                case 3:
                    question.digitAnswer = question.num1 * question.num3;
                    break;
            }
        }

        else {
            operand = (rand() % 2) + 1;
            switch(operand) {

                case 1:
                    question.digitAnswer = (question.num1 + question.num2) * question.num3;
                    break;

                case 2:
                    question.digitAnswer = (question.num1 - question.num2) * question.num3;
                    break;

            }
        }

        question.letterAnswer = (rand() % 4) + 1;
        setAdvanced(operand);
        setChoices(question.letterAnswer);

    }

    void createExpert() {
        int operand;

        question.num1 = (rand() % 100) + 10;
        question.num2 = (rand() % 100) + 10;
        question.num3 = (rand() % 10) + 1;
        question.num4 = (rand() % 5) + 1;
        question.num5 = (rand() % 30) + 1;
        question.num6 = (rand() % 10) + 1;
        question.num7 = (rand() % 15) + 1;
        question.num8 = (rand() % 40) + 40;

        operand = (rand() % 4) + 1;
        if(player1.score < 3 && player2.score < 3) {
            switch(operand) {
                case 1:
                    question.digitAnswer = question.num1 + question.num2;
                    break;
                case 2:
                    question.digitAnswer = question.num1 - question.num2;
                    break;
                case 3:
                    question.digitAnswer = question.num1 * question.num3;
                    break;
                case 4:
                    question.digitAnswer = (float)question.num1 / (float)question.num4;
                    break;
            }
        }

        else {
            switch(operand) {
                case 1:
                    question.digitAnswer = (float)(question.num1 + question.num2) / (float)(question.num5 - question.num3);
                    break;
                case 2:
                    question.digitAnswer = (question.num1 + question.num2) * (question.num7 - question.num6);
                    break;
                case 3:
                    question.digitAnswer = (float)(question.num1 - question.num8) / (float)(question.num6 + question.num7);
                    break;
                case 4:
                    question.digitAnswer = (question.num1 - question.num8) * (question.num4 + question.num6);
                    break;
            }
        }

        question.letterAnswer = (rand() % 4) + 1;
        setExpert(operand);
        setChoices(question.letterAnswer);
    }

    void setQuestions(int dif) {
        switch(dif) {

            case 1:
                createEasy();
                break;
            case 2:
                createAdvanced();
                break;
            case 3:
                createExpert();

        }
    }

    void setEasy(int op) {

        strcpy(question.arithmetic, "");

        char firstNum[5];
        char secondNum[5];
        char thirdNum[5];

        sprintf(firstNum, "%d", question.num1);
        sprintf(secondNum, "%d", question.num2);
        sprintf(thirdNum, "%d", question.num3);

        if(player1.score < 7 && player2.score < 7) {
            strcat(question.arithmetic, firstNum);
            (op == 1)? strcat(question.arithmetic, " + "): strcat(question.arithmetic, " - ");
            strcat(question.arithmetic, secondNum);
            strcat(question.arithmetic, " = ?");
        }

        else {
            strcat(question.arithmetic, "(");
            strcat(question.arithmetic, firstNum);
            strcat(question.arithmetic, " + ");
            strcat(question.arithmetic, secondNum);
            strcat(question.arithmetic, ")");
            strcat(question.arithmetic, " - ");
            strcat(question.arithmetic, thirdNum);
            strcat(question.arithmetic, " = ?");
        }
    }

    void setAdvanced(int op) {

        strcpy(question.arithmetic, "");

        char firstNum[5];
        char secondNum[5];
        char thirdNum[5];

        sprintf(firstNum, "%d", question.num1);
        sprintf(secondNum, "%d", question.num2);
        sprintf(thirdNum, "%d", question.num3);

        if(player1.score < 7 && player2.score < 7) {
            strcat(question.arithmetic, firstNum);
            switch(op) {

                case 1:
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, secondNum);
                    break;

                case 2:
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, secondNum);
                    break;

                case 3:
                    strcat(question.arithmetic, " * ");
                    strcat(question.arithmetic, thirdNum);
                    break;
            }
            strcat(question.arithmetic, " = ?");
        }

        else {
            switch(op) {

                case 1:
                    strcat(question.arithmetic, "(");
                    strcat(question.arithmetic, firstNum);
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, secondNum);
                    strcat(question.arithmetic, ")");
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, thirdNum);
                    strcat(question.arithmetic, " = ?");
                    break;

                case 2:
                     strcat(question.arithmetic, "(");
                    strcat(question.arithmetic, firstNum);
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, secondNum);
                    strcat(question.arithmetic, ")");
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, thirdNum);
                    strcat(question.arithmetic, " = ?");
                    break;
            }
        }
    }

    void setExpert(int op) {

        strcpy(question.arithmetic, "");

        char firstNum[5];
        char secondNum[5];
        char thirdNum[5];
        char fourthNum[5];
        char fifthNum[5];
        char sixthNum[5];
        char seventhNum[5];
        char eightNum[5];

        sprintf(firstNum, "%d", question.num1);
        sprintf(secondNum, "%d", question.num2);
        sprintf(thirdNum, "%d", question.num3);
        sprintf(fourthNum, "%d", question.num4);
        sprintf(fifthNum, "%d", question.num5);
        sprintf(sixthNum, "%d", question.num6);
        sprintf(seventhNum, "%d", question.num7);
        sprintf(eightNum, "%d", question.num8);

        strcat(question.arithmetic, firstNum);
        if(player1.score < 3 && player2.score < 3) {
            switch(op) {

                case 1:
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, secondNum);
                    break;

                case 2:
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, secondNum);
                    break;

                case 3:
                    strcat(question.arithmetic, " * ");
                    strcat(question.arithmetic, thirdNum);
                    break;

                case 4:
                    strcat(question.arithmetic, " / ");
                    strcat(question.arithmetic, fourthNum);
                    break;
            }
            strcat(question.arithmetic, " = ?");
        }
        else {
            strcpy(question.arithmetic, "");
            switch(op) {

                case 1:
                    strcat(question.arithmetic, "(");
                    strcat(question.arithmetic, firstNum);
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, secondNum);
                    strcat(question.arithmetic, ") / (");
                    strcat(question.arithmetic, fifthNum);
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, thirdNum);
                    strcat(question.arithmetic, ")");
                    break;

                case 2:
                    strcat(question.arithmetic, "(");
                    strcat(question.arithmetic, firstNum);
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, secondNum);
                    strcat(question.arithmetic, ") * (");
                    strcat(question.arithmetic, seventhNum);
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, sixthNum);
                    strcat(question.arithmetic, ")");
                    break;

                case 3:
                    strcat(question.arithmetic, "(");
                    strcat(question.arithmetic, firstNum);
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, eightNum);
                    strcat(question.arithmetic, ") / (");
                    strcat(question.arithmetic, sixthNum);
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, seventhNum);
                    strcat(question.arithmetic, ")");
                    break;

                case 4:
                    strcat(question.arithmetic, "(");
                    strcat(question.arithmetic, firstNum);
                    strcat(question.arithmetic, " - ");
                    strcat(question.arithmetic, eightNum);
                    strcat(question.arithmetic, ") * (");
                    strcat(question.arithmetic, fourthNum);
                    strcat(question.arithmetic, " + ");
                    strcat(question.arithmetic, sixthNum);
                    strcat(question.arithmetic, ")");
                    break;
                }
            }
    }

    void setChoices(int answer) {
        strcpy(question.aChoice, "A: ");
        strcpy(question.bChoice, "B: ");
        strcpy(question.cChoice, "C: ");
        strcpy(question.dChoice, "D: ");

        char a[10] = "";
        char b[10] = "";
        char c[10] = "";
        char d[10] = "";

        switch(answer) {

            case A:
                sprintf(a, "%.2f", question.digitAnswer);
                sprintf(b, "%.2f", question.digitAnswer - 3);
                sprintf(c, "%.2f", question.digitAnswer + 4);
                sprintf(d, "%.2f", (question.digitAnswer + 1) * 2);
                break;
            case B:
                sprintf(a, "%.2f", question.digitAnswer - 3);
                sprintf(b, "%.2f", question.digitAnswer);
                sprintf(c, "%.2f", question.digitAnswer + 4);
                sprintf(d, "%.2f", (question.digitAnswer + 1) * 2);
                break;
            case C:
                sprintf(a, "%.2f", question.digitAnswer + 4);
                sprintf(b, "%.2f", question.digitAnswer - 3);
                sprintf(c, "%.2f", question.digitAnswer);
                sprintf(d, "%.2f", (question.digitAnswer + 1) * 2);
                break;
            case D:
                sprintf(a, "%.2f", (question.digitAnswer + 1) * 2);
                sprintf(b, "%.2f", question.digitAnswer - 3);
                sprintf(c, "%.2f", question.digitAnswer + 4);
                sprintf(d, "%.2f", question.digitAnswer);
                break;
        }

        strcat(question.aChoice, a);
        strcat(question.bChoice, b);
        strcat(question.cChoice, c);
        strcat(question.dChoice, d);

    }

    void displayQuestion(char arithmetic[]) {
    SDL_Surface* arithmeticSurface;
    SDL_Rect arithmeticProperties;

    arithmeticSurface = TTF_RenderText_Blended(font, arithmetic, fontColor);
    arithmeticTexture = SDL_CreateTextureFromSurface(render, arithmeticSurface);
    SDL_FreeSurface(arithmeticSurface);

    arithmeticProperties.y = 90;
    arithmeticProperties.x = 70;

    SDL_QueryTexture(arithmeticTexture, NULL, NULL, &arithmeticProperties.w, &arithmeticProperties.h);

    SDL_RenderCopy(render, arithmeticTexture, NULL, &arithmeticProperties);
    }

    void displayChoices(char aString[], char bString[], char cString[], char dString[]) {
    SDL_Rect choiceProperties;
    SDL_Surface* textSurface;
    SDL_Texture* choiceTexture;

    textSurface = TTF_RenderText_Blended(font, aString, fontColor);
    choiceTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_FreeSurface(textSurface);
    choiceProperties.x = 70;
    choiceProperties.y = 250;
    SDL_QueryTexture(choiceTexture, NULL, NULL, &choiceProperties.w, &choiceProperties.h);
    SDL_RenderCopy(render, choiceTexture, NULL, &choiceProperties);

    textSurface = TTF_RenderText_Blended(font, bString, fontColor);
    choiceTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_FreeSurface(textSurface);
    choiceProperties.x = 70;
    choiceProperties.y = 285;
    SDL_QueryTexture(choiceTexture, NULL, NULL, &choiceProperties.w, &choiceProperties.h);
    SDL_RenderCopy(render, choiceTexture, NULL, &choiceProperties);

    textSurface = TTF_RenderText_Blended(font, cString, fontColor);
    choiceTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_FreeSurface(textSurface);
    choiceProperties.x = 70;
    choiceProperties.y = 320;
    SDL_QueryTexture(choiceTexture, NULL, NULL, &choiceProperties.w, &choiceProperties.h);
    SDL_RenderCopy(render, choiceTexture, NULL, &choiceProperties);

    textSurface = TTF_RenderText_Blended(font, dString, fontColor);
    choiceTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_FreeSurface(textSurface);
    choiceProperties.x = 70;
    choiceProperties.y = 355;
    SDL_QueryTexture(choiceTexture, NULL, NULL, &choiceProperties.w, &choiceProperties.h);
    SDL_RenderCopy(render, choiceTexture, NULL, &choiceProperties);

    }

    void displayScore(int multiplayer) {
        char p1Display[20] = "PLAYER 1: ";
        char p2Display[20] = "PLAYER 2: ";

        char p1Score[5];
        char p2Score[5];

        sprintf(p1Score, "%d", player1.score);
        sprintf(p2Score, "%d", player2.score);

        strncat(p1Display, p1Score, 20);
        strncat(p2Display, p2Score, 20);

        SDL_Rect scoreProperties;
        SDL_Surface* scoreSurface;

        scoreSurface = TTF_RenderText_Blended(font, p1Display, fontColor);
        p1ScoreTexture = SDL_CreateTextureFromSurface(render, scoreSurface);
        SDL_FreeSurface(scoreSurface);
        scoreProperties.y = 490;
        scoreProperties.x = 70;
        SDL_QueryTexture(p1ScoreTexture, NULL, NULL, &scoreProperties.w, &scoreProperties.h);
        SDL_RenderCopy(render, p1ScoreTexture, NULL, &scoreProperties);

        if(multiplayer) {
            scoreSurface = TTF_RenderText_Blended(font, p2Display, fontColor);
            p2ScoreTexture = SDL_CreateTextureFromSurface(render, scoreSurface);
            SDL_FreeSurface(scoreSurface);
            scoreProperties.y = 520;
            scoreProperties.x = 70;
            SDL_QueryTexture(p2ScoreTexture, NULL, NULL, &scoreProperties.w, &scoreProperties.h);
            SDL_RenderCopy(render, p2ScoreTexture, NULL, &scoreProperties);
        }

    }

    void displayOverScore() {

        char display[20] = "SCORE: ";
        char score[5];
        sprintf(score, "%d", player1.score);
        strncat(display, score, 20);

        SDL_Rect scoreProperties;
        SDL_Surface* scoreSurface;

        scoreSurface = TTF_RenderText_Blended(font, display, fontColor);
        overScoreTexture = SDL_CreateTextureFromSurface(render, scoreSurface);
        SDL_FreeSurface(scoreSurface);
        scoreProperties.y = 530;
        scoreProperties.x = (WINDOW_WIDTH / 2) - 55;
        SDL_QueryTexture(overScoreTexture, NULL, NULL, &scoreProperties.w, &scoreProperties.h);
        SDL_RenderCopy(render, overScoreTexture, NULL, &scoreProperties);

    }

    void displayLives() {
        SDL_Rect livesProperties;
        SDL_QueryTexture(livesTexture, NULL, NULL, &livesProperties.w, &livesProperties.h);

        for(int x = 0; x < lives; x++) {
            livesProperties.y = 490;
            livesProperties.x = 300 + (x * 20);
            SDL_RenderCopy(render, livesTexture, NULL, &livesProperties);
        }

    }

    void displayTime() {

        mlSecs -= 0.04;
        secsTime = (int)mlSecs;

        if(secsTime < 0) {
            switch(difficulty) {

                case 1:
                    lives -= 1;
                    mlSecs = 10;

                    break;
                case 2: case 3:
                    lives -= 1;
                    mlSecs = 15;
                    break;

            }

        }

        SDL_Rect timeProperties;
        SDL_Surface* timeSurface;

        char timeDisplay[20] = "TIME REMAINING: ";
        char time[5];
        sprintf(time, "%d", secsTime);
        strncat(timeDisplay, time, 20);

        timeSurface = TTF_RenderText_Blended(font, timeDisplay, fontColor);
        timeTexture = SDL_CreateTextureFromSurface(render, timeSurface);
        SDL_FreeSurface(timeSurface);
        timeProperties.y = 520;
        timeProperties.x = 70;
        SDL_QueryTexture(timeTexture, NULL, NULL, &timeProperties.w, &timeProperties.h);
        SDL_RenderCopy(render, timeTexture, NULL, &timeProperties);

    }

    void resetTime() {

        switch(difficulty) {

            case 1:
                mlSecs = 10;
                break;

            case 2: case 3:
                mlSecs = 15;
                break;

        }

    }

    void pause() {

        SDL_Event evPause;
        int choice = 1;
        int paused = 1;

        while(paused) {

            while(SDL_PollEvent(&evPause)) {

                switch(evPause.type) {

                    case SDL_KEYDOWN:

                        switch(evPause.key.keysym.sym) {

                            case SDLK_RIGHT:
                                choice = (choice < 3)? ++choice: choice;
                                chooseSound();
                                break;

                            case SDLK_LEFT:
                                choice = (choice > 1)? --choice: choice;
                                chooseSound();
                                break;

                            case SDLK_RETURN:
                                selectSound();
                                switch(choice) {

                                    case 1:
                                        paused = 0;
                                        gameCount();
                                        break;

                                    case 2:
                                        resetGame();
                                        gameCount();
                                        paused = 0;
                                        break;

                                    case 3:
                                        gameExit = 1;
                                        paused = 0;
                                        break;
                                }


                        }

                }

            }

            SDL_RenderClear(render);
            switch(choice) {
                case 1:
                    SDL_RenderCopy(render, pauseTexture1, NULL, NULL);
                    break;

                case 2:
                    SDL_RenderCopy(render, pauseTexture2, NULL, NULL);
                    break;

                case 3:
                    SDL_RenderCopy(render, pauseTexture3, NULL ,NULL);
                    break;

            }
            SDL_RenderPresent(render);

        }


    }

    void deathDetect(Snake* player, float xPos, float yPos) {

        if(player->xPosition == xPos && player->yPosition == yPos && multiplayer == 0) {
            lives--;
            player->xPosition = ((PLAY_AREA_X_MIN + PLAY_AREA_X_MAX) / 2) + 10;
            player->yPosition = (PLAY_AREA_Y_MIN + PLAY_AREA_Y_MAX) / 2;
            wrongSound();
        }

    }

    void death() {

        setUI(RUN_OVER);

    }

    void eatSound() {

        Mix_PlayChannel(1, eat, 0);

    }

    void chooseSound() {

        Mix_PlayChannel(2, choose, 0);

    }

    void selectSound() {

        Mix_PlayChannel(3, select, 0);
        SDL_Delay(300);

    }

    void winSound() {

        Mix_PlayChannel(4, win, 0);

    }

    void wrongSound() {

        Mix_PlayChannel(5, wrong, 0);

    }

    void overSound() {

        Mix_PlayChannel(6, over, 0);

    }

    void music(int state) {

        (state == 1)? Mix_PlayMusic(bgMusic, -1): Mix_HaltMusic();;

    }

    void setUI(int state) {

        startRunning = 0;
        difficultyRunning = 0;
        helpRunning = 0;
        gameRunning = 0;
        overRunning = 0;
        winRunning = 0;

        switch(state) {

            case 0:
                systemRunning = 0;
                break;

            case 1:
                startRunning = 1;
                break;

            case 2:
                difficultyRunning = 1;
                break;

            case 3:
                helpRunning = 1;
                break;

            case 4:
                gameRunning = 1;
                break;

            case 5:
                overSound();
                overRunning = 1;
                break;

            case 6:
                winSound();
                winRunning = 1;
                break;
        }

    }

    void selectEnd(int state) {

        switch(state) {

            case 1:
                setUI(RUN_START);
                break;

            case 2:
                resetGame();
                setUI(RUN_GAME);
                break;

            case 3:
                setUI(STOP_SYSTEM);
                break;

        }

    }

    void resetGame() {

        lives = 3;
        p1Direction = 0;
        player1.score = 0;
        player1.tailCount = 3;
        player1.xVelocity = 0;
        player1.yVelocity = 0;
        player1.xPosition = PLAY_AREA_X_MIN + 100;
        player1.yPosition = PLAY_AREA_Y_MIN + 100;

        p2Direction = 0;
        player2.score = 0;
        player2.xVelocity = 0;
        player2.yVelocity = 0;
        player2.tailCount = 3;
        player2.xPosition = PLAY_AREA_X_MAX - 100;
        player2.yPosition = PLAY_AREA_Y_MAX - 100;

    }

    void loadingScreen() {
        music(0);
        for(int x = 1; x <= 5; x++) {
            SDL_RenderClear(render);
            switch((x % 3) + 1) {

                case 1:
                    SDL_RenderCopy(render, loadTexture1, NULL, NULL);
                    break;

                case 2:
                     SDL_RenderCopy(render, loadTexture2, NULL, NULL);
                     break;

                case 3:
                     SDL_RenderCopy(render, loadTexture3, NULL, NULL);
                     break;

            }
            SDL_RenderPresent(render);
            SDL_Delay(400);
        }
        (musicState == 1)? music(musicState): 0;
    }

    void gameCount() {

        for(int x = 3; x >= 1; x--) {
            SDL_RenderClear(render);
            switch(x) {

                case 3:
                    SDL_RenderCopy(render, countTexture3, NULL, NULL);
                    break;

                case 2:
                    SDL_RenderCopy(render, countTexture2, NULL, NULL);
                    break;

                case 1:
                    SDL_RenderCopy(render, countTexture1, NULL, NULL);
                    break;
            }
            SDL_RenderPresent(render);
            SDL_Delay(500);

        }

    }

    //Game functions END

    void cleanUp() {
    SDL_DestroyTexture(bgTexture);

    SDL_DestroyTexture(creditsTexture);
    SDL_DestroyTexture(startTexture1);
    SDL_DestroyTexture(startTexture2);
    SDL_DestroyTexture(startTexture3);
    SDL_DestroyTexture(pauseTexture1);
    SDL_DestroyTexture(pauseTexture2);
    SDL_DestroyTexture(pauseTexture3);

    SDL_DestroyTexture(difficultyTexture1);
    SDL_DestroyTexture(difficultyTexture2);
    SDL_DestroyTexture(difficultyTexture3);

    SDL_DestroyTexture(textureA);
    SDL_DestroyTexture(textureB);
    SDL_DestroyTexture(textureC);
    SDL_DestroyTexture(textureD);

    SDL_DestroyTexture(p1ScoreTexture);
    SDL_DestroyTexture(p1ScoreTexture);

    SDL_DestroyTexture(p1Texture);
    SDL_DestroyTexture(p2Texture);

    SDL_DestroyTexture(arithmeticTexture);

    SDL_DestroyTexture(timeTexture);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    }
