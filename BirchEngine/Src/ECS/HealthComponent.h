#pragma once
#include "Components.h"

class HealthComponent : public Component
{	
private:
	

public:

	int HP;
	int MaxHealth = 5;
	HealthComponent(int health){
		HP = health;
		std::cout << "HP: " <<  HP << std::endl;
	}

	void takeDamage(int damage)
	{
		
		HP -= damage;
		
		if (HP <= 0)
		{
			std::cout << "You Ded";
		}

		
	
	}

	
};