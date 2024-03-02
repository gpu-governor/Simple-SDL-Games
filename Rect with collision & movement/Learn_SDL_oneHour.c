#include<stdio.h>
#include<SDL2/SDL.h>

int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Move Rectangle", 0, 0, 640, 480, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set render draw color
    SDL_SetRenderDrawColor(renderer, 245, 0, 0, 111);

    // Define initial rectangle position and dimensions
    SDL_Rect shape = {150, 168, 85, 80};
    SDL_Rect staticRect = {150, 250, 400, 20}; // Static rectangle (platform)

    // Event handler
    SDL_Event event;

    // Flag to control the main loop
    int quit = 0;

    // Main loop
    while (!quit) {
        // Event loop
        while (SDL_PollEvent(&event)) {
            // Check for quit event
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                // Check for keydown event
                switch (event.key.keysym.sym) {
                    // Move the rectangle up
                    case SDLK_UP:
                        if (shape.y > 0) {
                            shape.y -= 10;
                        }
                        break;
                    // Move the rectangle down
                    case SDLK_DOWN:
                    // Downward movement & collision detection 
                        if (shape.y + shape.h + 10 <= staticRect.y) {
                            shape.y += 10;
                         }
                        break;
                    // Move the rectangle left
                    case SDLK_LEFT:
                            shape.x -= 10;
                        break;
                    // Move the rectangle right
                    case SDLK_RIGHT:
                            shape.x += 10;
                        break;
                    default:
                        break;
                }
            }
        }

        // Clear the renderer with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the static rectangle (platform)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 111);
        SDL_RenderFillRect(renderer, &staticRect);

        // Draw the moving rectangle
        SDL_SetRenderDrawColor(renderer, 245, 0, 0, 111);
        SDL_RenderFillRect(renderer, &shape);

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(10); // Adjust this delay to control the speed of the rectangle's movement
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
