#include <SDL2/SDL.h>
#include <stdbool.h>

// Window dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Player dimensions
const int PLAYER_WIDTH = 50;
const int PLAYER_HEIGHT = 50;

// Platform dimensions
const int PLATFORM_WIDTH = 200;
const int PLATFORM_HEIGHT = 20;

// Player properties
const int GRAVITY = 1;
const int JUMP_FORCE = -15;
const float MOVE_SPEED = 1.0f; // Decreased walking speed

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Function to initialize SDL
bool initSDL() {
    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    window = SDL_CreateWindow("Simple Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Function to close SDL
void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Function to handle user input
void handleInput(bool* quit, bool* jump, bool* moveLeft, bool* moveRight) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            *quit = true;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                *jump = true;
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                *moveLeft = true;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                *moveRight = true;
            }
        } else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LEFT) {
                *moveLeft = false;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                *moveRight = false;
            }
        }
    }
}

// Main function
int main(int argc, char* args[]) {
    // Initialize SDL
    if (!initSDL()) {
        return 1;
    }

    // Initialize player rectangle
    SDL_Rect playerRect = {180, 380,
                           PLAYER_WIDTH, PLAYER_HEIGHT};

    // Initialize platform rectangles
    SDL_Rect platformRects[] = {
        {SCREEN_WIDTH / 4, SCREEN_HEIGHT - PLATFORM_HEIGHT, PLATFORM_WIDTH, PLATFORM_HEIGHT},
        {SCREEN_WIDTH / 2, SCREEN_HEIGHT - PLATFORM_HEIGHT * 3, PLATFORM_WIDTH, PLATFORM_HEIGHT}, // Adjusted position of the second platform
        {SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT - PLATFORM_HEIGHT * 5, PLATFORM_WIDTH, PLATFORM_HEIGHT}, // Adjusted position of the third platform
        {SCREEN_WIDTH / 4, SCREEN_HEIGHT - PLATFORM_HEIGHT * 7, PLATFORM_WIDTH, PLATFORM_HEIGHT} // Adjusted position of the closed platform
    };

    // Initialize flags and variables
    bool quit = false;
    bool jumping = false;
    bool moveLeft = false;
    bool moveRight = false;
    float xVelocity = 0.0f; // Changed to float for smoother movement
    int yVelocity = 0; // Added yVelocity for gravity

    // Main loop
    while (!quit) {
        // Handle user input
        handleInput(&quit, &jumping, &moveLeft, &moveRight);

        // Apply jump if requested
        if (jumping) {
            yVelocity = JUMP_FORCE;
            jumping = false;
        }

        // Apply horizontal movement
        if (moveLeft) {
            xVelocity = -MOVE_SPEED;
        } else if (moveRight) {
            xVelocity = MOVE_SPEED;
        } else {
            xVelocity = 0.0f;
        }

        // Update player position
        playerRect.x += (int)xVelocity; // Convert float to int for position update

        // Apply gravity
        yVelocity += GRAVITY;
        playerRect.y += yVelocity;

        // Collision detection with platforms
        for (int i = 0; i < sizeof(platformRects) / sizeof(platformRects[0]); i++) {
            if (playerRect.y + PLAYER_HEIGHT > platformRects[i].y &&
                playerRect.y < platformRects[i].y + PLATFORM_HEIGHT &&
                playerRect.x + PLAYER_WIDTH > platformRects[i].x &&
                playerRect.x < platformRects[i].x + PLATFORM_WIDTH) {
                // Player is on the platform
                yVelocity = 0;
                playerRect.y = platformRects[i].y - PLAYER_HEIGHT;
                break; // Exit loop since player can only be on one platform at a time
            }
        }

        // Render background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render platforms
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        for (int i = 0; i < sizeof(platformRects) / sizeof(platformRects[0]); i++) {
            SDL_RenderFillRect(renderer, &platformRects[i]);
        }

        // Render player
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        // Show rendered content
        SDL_RenderPresent(renderer);
    }

    // Close SDL
    closeSDL();

    return 0;
}
