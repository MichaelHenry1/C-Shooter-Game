#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "windows.h"
#include <sstream>
#include <string>


class EnemyComponent : public Component
{

	int speed = 0;
	int enemyHealth = 0;

  


public:
	int type = 0;

	Entity& heart;
	Vector2D EnemyFollow;

	EnemyComponent(int sp, int enemyhealth, Entity& h, int Type) : speed(sp), enemyHealth(enemyhealth), heart(h), type(Type)
	{}
	



	
	void heartPos(int xpos,int ypos) {

	    xpos = heart.addComponent<TransformComponent>().position.x;
		ypos = heart.getComponent<TransformComponent>().position.y;

	}


	void update() override
	{

		


		//move enemy by enenemyfollow
		entity->getComponent<TransformComponent>().position.x += EnemyFollow.x;
		entity->getComponent<TransformComponent>().position.y += EnemyFollow.y;

		if (entity->getComponent<HealthComponent>().HP <= 0) {
			
			//entity->getComponent<TransformComponent>().position.x = 9999999999999999;
			entity->destroy();
		
		}
		
	

	

		if (type == 0) {
			heart.getComponent<TransformComponent>().position.x = entity->getComponent<TransformComponent>().position.x + 10 + (5 * (5 - entity->getComponent<HealthComponent>().HP));
			heart.getComponent<TransformComponent>().position.y = entity->getComponent<TransformComponent>().position.y - 50 + (10 * (5 - entity->getComponent<HealthComponent>().HP));
			heart.getComponent<TransformComponent>().scale = entity->getComponent<HealthComponent>().HP;
		
			}
		else if (type == 1) {
			heart.getComponent<TransformComponent>().position.x = entity->getComponent<TransformComponent>().position.x + 45 + (5 * (5 - entity->getComponent<HealthComponent>().HP));
			heart.getComponent<TransformComponent>().position.y = entity->getComponent<TransformComponent>().position.y - 50 + (10 * (5 - entity->getComponent<HealthComponent>().HP));
			heart.getComponent<TransformComponent>().scale = entity->getComponent<HealthComponent>().HP;
			
		}
		else if (type == 2) {
			heart.getComponent<TransformComponent>().position.x = entity->getComponent<TransformComponent>().position.x + 85 + (5 * (5 - entity->getComponent<HealthComponent>().HP));
			heart.getComponent<TransformComponent>().position.y = entity->getComponent<TransformComponent>().position.y - 50 + (10 * (5 - entity->getComponent<HealthComponent>().HP));
			heart.getComponent<TransformComponent>().scale = entity->getComponent<HealthComponent>().HP;
		
		}
	}


	


};
	


































//SDL_Rect enemyCol = enemy.getComponent<ColliderComponent>().collider;

//Vector2D enemyFollow = Vector2D((playerPos.x - enemy.getComponent<TransformComponent>().position.x), (playerPos.y - enemy.getComponent<TransformComponent>().position.y)).Normalize() * 2;

/*if (enemy.getComponent<HealthComponent>().HP > 0) {

	enemy.getComponent<TransformComponent>().position.x += enemyFollow.x;
	enemy.getComponent<TransformComponent>().position.y += enemyFollow.y;
}
else {
	enemy.getComponent<TransformComponent>().position.x = 9999999999999999;
}
*/
/*if (Collision::AABB(cCol, enemyCol))
{
	enemy.getComponent<TransformComponent>().position -= enemyFollow * 2;
}
	}*/

	/*if (Collision::AABB(enemy.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
	{
		std::cout << enemy.getComponent<HealthComponent>().HP << std::endl;
		enemy.getComponent<HealthComponent>().takeDamage(1);
		p->destroy();

	}
	
	}

	heart.getComponent<TransformComponent>().position.x = enemy.getComponent<TransformComponent>().position.x + 30 + (5 * (5 - enemy.getComponent<HealthComponent>().HP));
	heart.getComponent<TransformComponent>().position.y = enemy.getComponent<TransformComponent>().position.y - 50 + (10 * (5 - enemy.getComponent<HealthComponent>().HP));

	heart.getComponent<TransformComponent>().scale = enemy.getComponent<HealthComponent>().HP;
	

	*/
	
