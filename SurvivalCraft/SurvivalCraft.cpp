#include "Core/SurvivalCraft.hpp"

int main()
{
	SurvivalCraftMain* main = new SurvivalCraftMain();

	main->Initialize();

	main->CleanUp();

	delete main;

	return 0;
}