#pragma once

#include <cmath>
#include <string>
#include <stdint.h>

#include <iostream>


class Potato
{
public:
	static Potato BuildRusset()
	{
		Potato russetPotato;
		russetPotato.Type = "Russet";
		russetPotato.Weight = 173.0; //g
		russetPotato.Calories = 161; // calories
		russetPotato.Sodium = 17; //mg
		russetPotato.Proteins = 4.3f; //g
		russetPotato.IsBaked = false;
		return russetPotato;
	}

	bool Bake()
	{
		if (!IsBaked)
		{
			std::cout << "Baking potato" << std::endl;
			IsBaked = true;
			return true;
		}
		else
		{
			std::cout << "Potato is already baked" << std::endl;
			return false;
		}
	}

	void AddButter()
	{
		const float Tsp = 1.f;
		const int32_t AddedCalories = static_cast<int32_t>(ceil(Tsp * 35)); // calories
		std::cout << "Adding " << Tsp << " tsp of butter for " << AddedCalories << " calories" << std::endl;
		Calories += AddedCalories;
	}

	void AddSalt(int32_t amount)
	{
		const float Tsp = 0.3f;
		std::cout << "Adding " << amount << "mg of sodium" << std::endl;
		Sodium += amount;
	}

	void ChangeType(std::string type)
	{
		std::cout << "Chaing type from " << Type << " to " << type << std::endl;
		Type = type;
	}

	std::string Type;
	float Weight; //g
	int32_t Calories; // calories
	int32_t Sodium; //mg
	float Proteins; //g
	bool IsBaked;
};