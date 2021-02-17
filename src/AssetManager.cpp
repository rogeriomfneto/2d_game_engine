#include "./AssetManager.h"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {

}

void AssetManager::ClearData() {
    textures.clear();
    fonts.clear();
}

void AssetManager::AddTexture(std::string textureId, const char* filePath) {
    SDL_Texture* texture = TextureManager::LoadTexture(filePath);
    textures[textureId] = texture;
}

void AssetManager::AddFont(std::string fontId, const char* filePath, int fontSize) {
    TTF_Font* font = FontManager::LoadFont(filePath, fontSize);
    fonts[fontId] = font;
}

SDL_Texture* AssetManager::GetTexture(std::string textureId) {
    return textures[textureId];
}

TTF_Font* AssetManager::GetFont(std::string fontId) {
    return fonts[fontId];
}