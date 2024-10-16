#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//GRAPHICS
#include <SDL2/SDL.h>

//MY SCREEN RESOLUTION (you may need to tweak it according to your own screen)
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_X SDL_WINDOWPOS_CENTERED
#define WINDOW_Y SDL_WINDOWPOS_CENTERED

#define GRID_SIZE 60
#define GRID_DIM 600

typedef struct {
    int x;
    int y;
    int count;

} apple;
apple Apple;

struct snake {
    
    int x;
    int y;
    int dir;

    struct snake *next;

};
typedef struct snake Snake;

Snake *head;
Snake *tail;

enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT,
};

void init_snake()
{
    Snake *new = malloc(sizeof(Snake));
    new->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4); 
    new->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4); 
    new->dir = rand() % 4;
    new->next = NULL;
    
    head = new;
    tail = new;

    return;
}

void increase_snake()
{
    Snake *new = malloc(sizeof(Snake));
    //TEMP


    switch(tail->dir) {
        case SNAKE_UP:
            new->y = tail->y + 1;
            new->x = tail->x;
            break;
        case SNAKE_DOWN:
            new->y = tail->y - 1;
            new->x = tail->x;
            break;
        case SNAKE_LEFT:
            new->y = tail->y;
            new->x = tail->x + 1;
            break;
        case SNAKE_RIGHT:
            new->y = tail->y;
            new->x = tail->x - 1;
            break;
    }



    new->next = NULL;
    tail->next = new;

    tail = new;


    return;
}

void reset_snake()
{
    Snake *track = head;
    Snake *temp;

    while(track != NULL) {

        temp = track;
        track = track->next;
        free(temp);
    }   

    head = NULL;
    tail = NULL;

    init_snake();
    increase_snake();
    increase_snake();
    increase_snake();

    Apple.count = 0;

    return;
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void render_snake(SDL_Renderer *renderer, int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);

    int seg_size = GRID_DIM / GRID_SIZE;
    SDL_Rect seg;
    seg.w = seg_size;
    seg.h = seg_size;

    Snake *track = head;
    while(track != NULL) {
        seg.x = x + (seg_size * track->x);
        seg.y = y + (seg_size * track->y);
        SDL_RenderFillRect(renderer, &seg);

        track = track->next;
    }

    return;
}

void move_snake()
{
    int prev_x = head->x;
    int prev_y = head->y;
    int prev_dir = head->dir;

    switch(head->dir) {
        case SNAKE_UP:
            head->y--;
            break;
        case SNAKE_DOWN:
            head->y++;
            break;
        case SNAKE_LEFT:
            head->x--;
            break;
        case SNAKE_RIGHT:
            head->x++;
            break;
    }

    Snake *track = head;

    if(track->next != NULL) {
        track = track->next;
    }

    while(track != NULL) {
        
        int new_x = track->x;
        int new_y = track->y;
        int new_dir = track->dir;

        track->x = prev_x;
        track->y = prev_y;
        track->dir = prev_dir;

        track = track->next;

        prev_x = new_x;
        prev_y = new_y;
        prev_dir = new_dir;

    }


    return;
}


void render_grid(SDL_Renderer *renderer, int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 255);

#if 0
    int cell_size = GRID_DIM / GRID_SIZE;

    SDL_Rect cell;
    cell.w = cell_size;
    cell.h = cell_size;

    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
#else

    SDL_Rect outline;
    outline.x = x;
    outline.y = y;
    outline.w = GRID_DIM;
    outline.h = GRID_DIM;
    SDL_RenderDrawRect(renderer, &outline);
#endif

    return;


}


void gen_apple()
{
    bool in_snake;

    do {
        in_snake = false;
        Apple.x = rand() % GRID_SIZE;
        Apple.y = rand() % GRID_SIZE;

        Snake *track = head;
        while(track != NULL) {
            
            if(Apple.x == track->x && Apple.y == track->y) {
                in_snake = true;
            }

            track = track->next;
        }
    }
    while(in_snake);

    Apple.count++;
}

void render_apple(SDL_Renderer *renderer, int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);


    int apple_size = GRID_DIM / GRID_SIZE;
#if 0

    SDL_Rect app;
    app.x = x + (apple_size * Apple.x);
    app.y = y + (apple_size * Apple.y);

    SDL_RenderFillRect(renderer, &app);
#else
    int circle_x = x + (apple_size * Apple.x) + ( apple_size / 2 );
    int circle_y = y + (apple_size * Apple.y) + ( apple_size / 2 );

    SDL_Color c = {0xff, 0x00, 0x00, 255}; 

    SDL_RenderFillCircle(renderer, circle_x, circle_y, apple_size / 2, c);

#endif
    return;
}

void detect_apple()
{
    if(head->x == Apple.x && head->y == Apple.y) {
        gen_apple();
        increase_snake();
    }

}

void detect_crash()
{
    if(head->x < 0 || head->x >= GRID_SIZE || head->y < 0 || head->y >= GRID_SIZE) {
        reset_snake();
        gen_apple();
    }
    
    Snake *track = head;
    
    if(track->next != NULL) {
        track = track->next;
    }

    while(track != NULL) {
        if(head->x == track->x && head->y == track->y) {
            reset_snake();
            gen_apple();
        }
        track = track->next;
    }


    return;
}

void render_score(SDL_Renderer *renderer, int x, int y)
{
    int score_x = x + GRID_DIM / 2 - 100;
    int score_y = y - 150;

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);
#if 0    

    SDL_Rect score;
    score.w = 200;
    score.h = 100;
    score.x = score_x;
    score.y = score_y;
    
    SDL_RenderDrawRect(renderer, &score);
#endif

    int cell_width = 10;

    SDL_Rect cell;
    cell.w = cell_width;
    cell.h = cell_width;

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%3d", Apple.count);
    
    int x_step = 100;

    for(int k = 0; k < 3; k++) {

        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 10; j++) {
                cell.x = score_x + i * cell_width + (k * x_step); 
                cell.y = score_y + j * cell_width;
            }
        }
    }


    return;
}

int main()
{
    //INIT
    srand(time(0));
    init_snake();
    increase_snake();
    increase_snake();
    increase_snake();
    increase_snake();

    gen_apple();
    Apple.count = 0;

    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_INIT_VIDEO < 0) {
        fprintf(stderr, "ERROR: SDL_INIT_VIDEO");
    }

    window = SDL_CreateWindow(
        "",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if(!window) {
        fprintf(stderr, "ERROR !window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer) {
        fprintf(stderr, "ERROR: !renderer");
    }

    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    bool quit = false;
    bool paused = false;
    SDL_Event event;

    int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIM / 2);
    int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIM / 2);
    bool moved = false;

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        case SDLK_UP:
                            if(head->dir != SNAKE_DOWN && moved != true) {
                                head->dir = SNAKE_UP;
                                moved = true;
                            }
                            break;
                        case SDLK_DOWN:
                            if(head->dir != SNAKE_UP && moved != true) {
                                head->dir = SNAKE_DOWN;
                                moved = true;
                            }
                            break;
                        case SDLK_LEFT:
                            if(head->dir != SNAKE_RIGHT && moved != true) {
                                head->dir = SNAKE_LEFT;
                                moved = true;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(head->dir != SNAKE_LEFT && moved != true) {
                                head->dir = SNAKE_RIGHT;
                                moved = true;
                            }
                            break;
                        case SDLK_p:
                            if (paused == true) {paused = false;}
                            if (paused == false) {paused = true;}
                            break;
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);
        //RENDER LOOP

        detect_apple();

        render_grid(renderer, grid_x, grid_y);
        render_snake(renderer, grid_x, grid_y);
        render_apple(renderer, grid_x, grid_y);
        render_score(renderer, grid_x, grid_y);

        if(paused == false) {
            move_snake();
            moved = false;
            detect_crash();
        }
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);
        SDL_Delay(80);
    }



    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
