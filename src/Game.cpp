#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./EntityManager.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager;
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera;
Map* map;

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }
    this->window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    this->assetManager = new AssetManager(&manager);

    this->camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    this->renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    this->LoadLevel(0);

    this->isRunning = true;
}

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());

    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 200, true, false);
    player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");

    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");

    Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);
}

void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT: {
            this->isRunning = false;
            break;            
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                this->isRunning = false;
            }
        }
        default: {
            break;
        }
    }
}

void Game::Update() {
    int ticksCurrentFrame = SDL_GetTicks();

    // Wait until reach target frame rate
    // int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    // if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
    //     SDL_Delay(timeToWait);
    //     ticksCurrentFrame = SDL_GetTicks();
    // }

    float deltaTime = (ticksCurrentFrame - this->ticksLastFrame) / 1000.0f;
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    this->ticksLastFrame = ticksCurrentFrame;

    manager.Update(deltaTime);
    
    HandleCameraMovement();
}

void Game::Render() {
    SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
    SDL_RenderClear(this->renderer); 

    if (manager.HasNoEntities()) return;

    manager.Render();

    SDL_RenderPresent(this->renderer);
}

void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    // Clamp camera
    camera.x = camera.x < 0 ? 0 : (camera.x > camera.w ? camera.w : camera.x);
    camera.y = camera.y < 0 ? 0 : (camera.y > camera.h ? camera.h : camera.y);
    // printf("Camera x: %d\n", camera.x);
    // printf("Camera y: %d\n", camera.y);
}

void Game::Destroy() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}