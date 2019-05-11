

#include "AssetManager.h"
#include "ECS\Components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);

}

void AssetManager::CreateEnemy(Vector2D pos, int sp, int health,std::string id, int w, int h, int sc, int Type)
{
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, w, h, sc);
	enemy.addComponent<SpriteComponent>(id, false);
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addComponent<HealthComponent>(health);
	
	auto& heart(manager->addEntity());
	heart.addComponent<TransformComponent>(enemy.getComponent<TransformComponent>().position.x, enemy.getComponent<TransformComponent>().position.y, 10, 10, 1);
	heart.addComponent<SpriteComponent>("heart", false);
    
	enemy.addComponent<EnemyComponent>(sp, health, heart, Type);
	enemy.getComponent<EnemyComponent>().type = Type;
	
	enemy.addGroup(Game::groupEnemies);
}



void AssetManager::CreateMed(Vector2D pos, std::string id, int w, int h, int sc)
{
	auto& med(manager->addEntity());
	med.addComponent<TransformComponent>(pos.x, pos.y, w, h, sc);
    med.addComponent<SpriteComponent>(id, false);
	med.addComponent<ColliderComponent>("meds");
	med.addGroup(Game::groupMeds);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}