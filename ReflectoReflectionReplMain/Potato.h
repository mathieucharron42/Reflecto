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

	void Bake()
	{
		if (!IsBaked)
		{
			std::cout << "Baking potato" << std::endl;
			IsBaked = true;
		}
		else
		{
			std::cout << "Potato is already baked" << std::endl;
		}
	}

	void AddButter()
	{
		const float Tsp = 1.f;
		const int32_t AddedCalories = static_cast<int32_t>(ceil(Tsp * 35)); // calories
		std::cout << "Adding " << Tsp << " tsp of butter for " << AddedCalories << " calories" << std::endl;
		Calories += AddedCalories;
	}

	void AddSalt()
	{
		const float Tsp = 0.3f;
		const int32_t AddedSodium = static_cast<int32_t>(ceil(Tsp * 2324)); // mg
		std::cout << "Adding " << Tsp << " tsp of salt for " << AddedSodium << "mg of sodium" << std::endl;
		Sodium += AddedSodium;
	}

	std::string Type;
	float Weight; //g
	int32_t Calories; // calories
	int32_t Sodium; //mg
	float Proteins; //g
	bool IsBaked;
};