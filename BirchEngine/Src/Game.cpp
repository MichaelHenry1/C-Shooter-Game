#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "windows.h"
#include "time.h"
#include "EnemyComponent.h"
#include <sstream>
//CAMERA 
//HEART
Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,3500,2400 };

AssetManager * Game::assets = new AssetManager(&manager);



bool Game::isRunning = false;




auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& weapon(manager.addEntity());
auto& background(manager.addEntity());
auto& gameOver(manager.addEntity());
auto& playerHeart(manager.addEntity());
auto& heart(manager.addEntity());
auto& shop(manager.addEntity());
auto& enemy1shop(manager.addEntity());
auto& enemy2shop(manager.addEntity());
auto& enemy3shop(manager.addEntity());
auto& heartShop(manager.addEntity());
auto& bulletShop(manager.addEntity());
auto& med(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}




void Game::init(const char* title, int width, int height, bool fullscreen)
{

	

	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}
	
	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/player_anims.png");
	assets->AddTexture("bullet", "assets/Bullet.png");
	assets->AddTexture("bulletShop", "assets/bulletShop.png");
	assets->AddTexture("weapon", "assets/SCAR.png");
	assets->AddFont("arial", "assets/arial.ttf", 16);
	assets->AddTexture("enemy", "assets/enemy.png");
	assets->AddTexture("enemy1shop", "assets/enemy1shop.png");
	assets->AddTexture("enemy2shop", "assets/enemy2shop.png");
	assets->AddTexture("enemy3shop", "assets/enemy3shop.png");
	assets->AddTexture("heart", "assets/heart.png");
	assets->AddTexture("heartshop", "assets/heartShop.png");
	assets->AddTexture("gameover", "assets/GameOver.png");
	assets->AddTexture("enemy2", "assets/enemy2.png");
	assets->AddTexture("shop", "assets/shop.png");
	assets->AddTexture("enemy3", "assets/enemy3.png");
	assets->AddTexture("med", "assets/medkit.png");
	map = new Map("terrain", 3, 32);

	map->LoadMap("assets/map.map", 50, 40);

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("player",false);
    player.addComponent<KeyboardController>();
	player.addComponent<HealthComponent>(5);
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
	



	//enemy.addComponent<TransformComponent>(600,500,30,32,3.8);
	//enemy.addComponent<SpriteComponent>("enemy", false);

	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);

	
	

	/*assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0),200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 500), Vector2D(2, -1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(500, 600), Vector2D(2, 1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(200, 500), Vector2D(2, 0), 200, 2, "projectile");
	*/
	background.addComponent<TransformComponent>(0,0,1138,1020, 4);
	background.addComponent<SpriteComponent>("background", false);

	weapon.addComponent<TransformComponent>(640, 600, 20,60,1);
	weapon.addComponent<SpriteComponent>("weapon", false);
    
	shop.addComponent<TransformComponent>(100000000,1000000, 150, 150, 4.9);
	shop.addComponent<SpriteComponent>("shop", false);

	enemy1shop.addComponent<TransformComponent>(10000000, 1000000, 32, 30, 2);
	enemy1shop.addComponent<SpriteComponent>("enemy1shop", false);

	enemy2shop.addComponent<TransformComponent>(10000000, 1000000, 65, 45, 2);
	enemy2shop.addComponent<SpriteComponent>("enemy2shop", false);

	enemy3shop.addComponent<TransformComponent>(10000000, 1000000, 65, 55, 2);
	enemy3shop.addComponent<SpriteComponent>("enemy3shop", false);

	heartShop.addComponent<TransformComponent>(10000000, 1000000, 30, 32, 2);
	heartShop.addComponent<SpriteComponent>("heartshop", false);

	bulletShop.addComponent<TransformComponent>(10000000, 1000000, 22, 38, 2);
	bulletShop.addComponent<SpriteComponent>("bulletShop", false);
	//enemy.addComponent<ColliderComponent>("enemy");
	
//	med.addComponent<TransformComponent>(10, 10, 20, 22, 2);
	//med.addComponent<SpriteComponent>("med", false);
	//enemy.addComponent<HealthComponent>(5);

	playerHeart.addComponent<TransformComponent>(player.getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y, 10, 10, 1);
	playerHeart.addComponent<SpriteComponent>("heart", false);

	gameOver.addComponent<TransformComponent>(1000000, 10000000, 320, 424, 2);
	gameOver.addComponent<SpriteComponent>("gameover", false);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& meds(manager.getGroup(Game::groupMeds));

float timer;
float enemySpawn;
float medSpawn = 30;

int coins = 0;



void Game::handleEvents()
{
	

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}



void Game::update()
{

	timer -= 0.05;
    enemySpawn -= 0.05;
	
	medSpawn -= 0.025;

	for (auto& e : enemies)
	{

	}

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;

	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	int x, y;

	int mousePos = SDL_GetMouseState(&x, &y);

	std::stringstream ss;
	ss << " Coins: " << coins;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	for (auto& e : enemies) {
		Vector2D enemyFollow = Vector2D((playerPos.x - e->getComponent<TransformComponent>().position.x), (playerPos.y - e->getComponent<TransformComponent>().position.y)).Normalize() * 2;
		// change to update enemyfollow within enemycomponetnt.h
		e->getComponent<EnemyComponent>().EnemyFollow = enemyFollow;
	}

	for (auto& c : colliders)
	{

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}

		for (auto& e : enemies) {

			SDL_Rect enemyCol = e->getComponent<ColliderComponent>().collider;

			if (Collision::AABB(cCol, enemyCol))
			{
				e->getComponent<TransformComponent>().position -= e->getComponent<EnemyComponent>().EnemyFollow * 2;
			}
			else if (Collision::AABB(enemyCol, playerCol))
			{
				if (timer <= 0) {
					player.getComponent<HealthComponent>().takeDamage(1);
					timer = 3;
				}
				if (player.getComponent<HealthComponent>().HP <= 0) {
					//player.getComponent<TransformComponent>().position.x = 999999999099090909;

					gameOver.getComponent<TransformComponent>().position.x = camera.x;
					gameOver.getComponent<TransformComponent>().position.y = camera.y;

				}


				


			}
		}

		if (player.getComponent<KeyboardController>().shop == true) {
			
			shop.getComponent<TransformComponent>().position.x = camera.x;
			shop.getComponent<TransformComponent>().position.y = camera.y;

			enemy1shop.getComponent<TransformComponent>().position.x = camera.x + 100;
			enemy1shop.getComponent<TransformComponent>().position.y = camera.y + 60;
		     
			enemy2shop.getComponent<TransformComponent>().position.x = camera.x + 200;
			enemy2shop.getComponent<TransformComponent>().position.y = camera.y + 45;
			
			enemy3shop.getComponent<TransformComponent>().position.x = camera.x + 400;
			enemy3shop.getComponent<TransformComponent>().position.y = camera.y + 40;

			bulletShop.getComponent<TransformComponent>().position.x = camera.x + 350;
			bulletShop.getComponent<TransformComponent>().position.y = camera.y + 200;

			heartShop.getComponent<TransformComponent>().position.x = camera.x + 150;
			heartShop.getComponent<TransformComponent>().position.y = camera.y + 200;

				player.getComponent<KeyboardController>().canMove = false;

		

			for (auto& e : enemies)
			{
				e->getComponent<EnemyComponent>().EnemyFollow * 0;
			}
		}

		if (player.getComponent<KeyboardController>().shop == false)
		{
			shop.getComponent<TransformComponent>().position.x = 100000;
			shop.getComponent<TransformComponent>().position.y = 10000000;

			enemy1shop.getComponent<TransformComponent>().position.x = 100000;
			enemy1shop.getComponent<TransformComponent>().position.y = 10000000;

			enemy2shop.getComponent<TransformComponent>().position.x = 100000;
			enemy2shop.getComponent<TransformComponent>().position.y = 10000000;

			enemy3shop.getComponent<TransformComponent>().position.x = 100000;
			enemy3shop.getComponent<TransformComponent>().position.y = 10000000;

			bulletShop.getComponent<TransformComponent>().position.x = 100000;
			bulletShop.getComponent<TransformComponent>().position.y = 10000000;

			heartShop.getComponent<TransformComponent>().position.x = 100000;
			heartShop.getComponent<TransformComponent>().position.y = 10000000;
			
			player.getComponent<KeyboardController>().canMove = true;
		
			
		}


		for (auto& m : meds) {
			SDL_Rect medCol = m->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(medCol, playerCol)) {

				player.getComponent<HealthComponent>().HP = player.getComponent<HealthComponent>().MaxHealth;
				m->getComponent<TransformComponent>().position.x = 999999999999999999;

			}
		}

		for (auto& p : projectiles)
		{
			SDL_Rect bulletCol = p->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(bulletCol, cCol))
			{
				p->destroy();
			}	
		}
		

	}



	for (auto& p : projectiles) {
		for (auto& e : enemies)
		{

			if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				std::cout << e->getComponent<HealthComponent>().HP << std::endl;
				e->getComponent<HealthComponent>().takeDamage(player.getComponent<KeyboardController>().bulletStrength);
				
				p->destroy();

			}
		}
	}

	if (player.getComponent<HealthComponent>().HP <= 0) {

		player.getComponent<KeyboardController>().canMove = false;


	}




	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 320);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 250);

	weapon.getComponent<TransformComponent>().position.x = playerPos.x + 50;
	weapon.getComponent<TransformComponent>().position.y = playerPos.y + 55;



	playerHeart.getComponent<TransformComponent>().position.x = player.getComponent<TransformComponent>().position.x + 30 + (5 * (5 - player.getComponent<HealthComponent>().HP));
	playerHeart.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y - 20 + (10 * (5 - player.getComponent<HealthComponent>().HP));

	playerHeart.getComponent<TransformComponent>().scale = player.getComponent<HealthComponent>().HP;

	if (player.getComponent<KeyboardController>().Fire == true)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		std::cout << playerPos.x << "," << playerPos.y << std::endl;
		std::cout << camera.x << "," << camera.y << std::endl;
		std::cout << x << "," << y << std::endl;



		if (camera.x < 0 && camera.y < 0) {
			assets->CreateProjectile(Vector2D(playerPos.x + 100, playerPos.y + 50), Vector2D(x - playerPos.x + 100, y - playerPos.y + 50).Normalize() * 10, 2500, 15, "bullet");
		}
		else if (camera.y < 0) {
			assets->CreateProjectile(Vector2D(playerPos.x + 100, playerPos.y + 50), Vector2D(x - (playerPos.x + 100 - camera.x), y - playerPos.y + 50).Normalize() * 10, 2500, 15, "bullet");
		}
		else if (camera.x < 0) {
			assets->CreateProjectile(Vector2D(playerPos.x + 100, playerPos.y + 50), Vector2D(x - playerPos.x + 100, y - (playerPos.y + 50 - camera.y)).Normalize() * 10, 2500, 15, "bullet");
		}
		else {
			assets->CreateProjectile(Vector2D(playerPos.x + 100, playerPos.y + 50), Vector2D(x - (playerPos.x + 100 - camera.x), y - (playerPos.y + 50 - camera.y)).Normalize() * 10, 2500, 15, "bullet");
		}
		//player.getComponent<KeyboardController>().canPress = false;

	}
	if (player.getComponent<TransformComponent>().velocity.x == -1)
	{
		weapon.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
		weapon.getComponent<TransformComponent>().position.x = playerPos.x + 18;
		weapon.getComponent<TransformComponent>().position.y = playerPos.y + 60;
		player.getComponent<KeyboardController>().Fire = false;
	}
	else if (player.getComponent<TransformComponent>().velocity.x == 1) {
		weapon.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
		weapon.getComponent<TransformComponent>().position.x = playerPos.x + 55;
		weapon.getComponent<TransformComponent>().position.y = playerPos.y + 60;
		player.getComponent<KeyboardController>().Fire = false;
	}
	else if (player.getComponent<TransformComponent>().velocity.x == 0)
	{
		weapon.getComponent<TransformComponent>().position.x = playerPos.x + 56;
		weapon.getComponent<TransformComponent>().position.y = playerPos.y + 58;
		weapon.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
		player.getComponent<KeyboardController>().Fire = false;
	}
	// Upgrades
	

	if (enemySpawn <= 0)
	{

		srand(time(NULL));


		int x = rand() % 3500 + 1;
		int y = rand() % 2800 + 1;

		int randE = rand() % 10 + 1;

		if (randE >= 5) {
			assets->CreateEnemy(Vector2D(x, y), 2, 5, "enemy", 28, 30, 2, 0);
		}
		else if (randE < 5 && randE >= 2) {
			assets->CreateEnemy(Vector2D(x, y), 2.5, 10, "enemy2", 56, 60, 2, 1);
		}
		else if (randE == 1) {
			assets->CreateEnemy(Vector2D(x, y), 2, 20, "enemy3", 84, 96, 2, 2);
		}

		enemySpawn = 5;
	}

	for (auto& e : enemies) {
		if (e->getComponent<HealthComponent>().HP <= 0) {
			e->getComponent<HealthComponent>().HP = -1;
			
			int t = e->getComponent<EnemyComponent>().type;

			if (t == 0) {
				coins += 1;
			}
			else if (t == 1)
			{
				coins += 3;
		    }
			else if (t == 2)
			{
				coins += 5;
			}
			std::cout << "COINS: " << coins << "\n";
		}
	

		if (medSpawn <= 0)
		{
			srand(time(NULL));
			int x = rand() % 3500;
			int y = rand() % 2800;
			assets->CreateMed(Vector2D(x, y), "med", 20, 22, 2);
			std::cout << "Med Dropped!" << x << " , " << y;
			medSpawn = 30;
		}


		if (camera.x < 0)
			camera.x = 0;
		if (camera.y < 0)
			camera.y = 0;
		if (camera.x > camera.w)
			camera.x = camera.w;
		if (camera.y > camera.h)
			camera.y = camera.h;

	}

}


void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	/*for (auto& c : colliders)
	{
		c->draw();
	}
	*/
	background.draw();

	for (auto& p : players)
	{
		p->draw();
	}
 //enemy.draw();
    playerHeart.draw();

	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
		e->getComponent<EnemyComponent>().heart.draw();
	}
	for (auto& m : meds)
	{
		m->draw();
	}
	label.draw();
	weapon.draw();
	gameOver.draw();
	shop.draw();
	enemy1shop.draw();
	enemy2shop.draw();
	enemy3shop.draw();
	heartShop.draw();
	bulletShop.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

