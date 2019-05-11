#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include "HealthComponent.h"

class KeyboardController : public Component
{
public:

	TransformComponent * transform;
	SpriteComponent * sprite;
	//HealthComponent * health;
	int bulletStrength = 1;
	bool movingLeft = false;
	bool Fire = false;
	bool  canPress = false;
	bool shop = false;
	bool canMove = true;
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	
	}

	void update() override
	{


		if (Game::event.type == SDL_KEYDOWN)
		{
			if (canMove == true) 
				switch (Game::event.key.keysym.sym) {

				case SDLK_w:
					transform->velocity.y = -1;
					sprite->Play("Walk");
					movingLeft = false;
					break;
				case SDLK_a: transform->velocity.x = -1;
					sprite->Play("Walk");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					movingLeft = true;
					break;
				case SDLK_d: transform->velocity.x = 1;
					sprite->Play("Walk");
					movingLeft = false;
					break;
				case SDLK_s:
					transform->velocity.y = 1;
					sprite->Play("Walk");
					movingLeft = false;
					break;


				default:

					break;
				}

				switch (Game::event.key.keysym.sym)
				{
				case SDLK_f:
					shop = !shop;
					break;
				case SDLK_1:
					//enemy1 Coin +1
					break;
				case SDLK_2:
					//enemy2 Coin +1
					break;
				case SDLK_3:
					//enemy3 Coin +1
					break;
				case SDLK_5:
					bulletStrength += 1;
					break;
				case SDLK_4:
					if (shop == true) {
						entity->getComponent<HealthComponent>().takeDamage(-1);
						entity->getComponent<HealthComponent>().MaxHealth += 1;
						
					}
					break;
				default:
					break;
				}
			}
		
				
		
		

		if (Game::event.type == SDL_MOUSEBUTTONDOWN){
			if (Game::event.button.button == SDL_BUTTON_LEFT)
			{
				if (canPress == true) {
					Fire = true;
					canPress = false;
				}

			}



			
		}

		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			if (Game::event.button.button == SDL_BUTTON_LEFT)
			{
				 canPress = true;

			}




		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->Play("Idle");
				break;
			case SDLK_a: transform->velocity.x = 0;
				sprite->Play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_d: transform->velocity.x = 0;
				sprite->Play("Idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->Play("Idle");
				break;
			case SDLK_f:

				break;
			case SDLK_e:
				Game::isRunning = false;
				
			default:
				break;
			}
		}


	}
};