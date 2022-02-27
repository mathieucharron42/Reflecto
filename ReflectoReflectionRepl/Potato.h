#pragma once

#include <string>

class Potato
{
public:
	void Bake()
	{
		IsBaked = true;
	}

	void AddToppings()
	{
		Calory += 100;
		Weight += 5;
	}

	std::string Type;
	float Weight;
	int32_t Calory;
	bool IsBaked;
};