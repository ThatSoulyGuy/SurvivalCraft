#ifndef SURVIVAL_CRAFT_HPP
#define SURVIVAL_CRAFT_HPP

#include "Core/InputManager.hpp"
#include "Core/Logger.hpp"
#include "Core/Window.hpp"
#include "Entity/EntityPlayer.hpp"
#include "Event/EventSystem.hpp"
#include "Render/Camera.hpp"
#include "Render/Renderer.hpp"
#include "Render/ShaderManager.hpp"
#include "Render/TextureManager.hpp"

using namespace SurvivalCraft::Core;
using namespace SurvivalCraft::Entity;
using namespace SurvivalCraft::Event;
using namespace SurvivalCraft::Render;

class SurvivalCraftMain
{

public:
	
	Window window = {};
	InputManager inputManager = {};
	ShaderManager shaderManager = {};
	TextureManager textureManager = {};
	EventSystem eventSystem = {};
	Renderer renderer = {};

	EntityPlayer player = {};
	
	SurvivalCraftMain()
	{
		instance = this;
	}

	void Initialize()
	{
		Logger_WriteConsole("Hello OpenGL 3.4!", LogLevel::INFORMATION);

		window.Create("SurvivalCraft* 0.0.9", { -1, -1 }, { 750 + (750 * 0.5), 450 + (450 * 0.5) }, { 0.0f, 0.45f, 0.75f });

		eventSystem.Broadcast(PreInitializeEvent());

		shaderManager.Register(Shader::Create("default", "Shader/Default"));
		textureManager.Register(Texture::Create("atlas", "Texture/Atlas.png"));

		inputManager.Initialize(window);
		
		ShaderManager::StaticInitialize(eventSystem);
		Mesh::StaticInitialize(shaderManager, textureManager, eventSystem);
		Camera::StaticInitialize(window, eventSystem);
		EntityPlayer::StaticInitialize(inputManager);

		Logger_WriteConsole("Preinitialization complete!", LogLevel::INFORMATION);

		eventSystem.Broadcast(InitializeEvent());

		player = EntityPlayer::Create("player", { 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });

		Mesh mesh = Mesh::Create("triangle", "default", "atlas", {}, {});
		mesh.GenerateSquare();
		mesh.Generate();

		renderer.Register(mesh);

		Logger_WriteConsole("Initialization complete!", LogLevel::INFORMATION);

		while (!window.ShouldClose())
			Update();
	}

	void Update()
	{
		window.PreUpdate();

		player.Update();
		renderer.Render(player.camera);

		window.PostUpdate();
	}

	void CleanUp()
	{
		eventSystem.Broadcast(CleanUpEvent());

		shaderManager.CleanUp();
		renderer.CleanUp();
		window.CleanUp();
	}

	static SurvivalCraftMain* Instance()
	{
		if (!instance)
			instance = new SurvivalCraftMain();
		
		return instance;
	}

private:

	static SurvivalCraftMain* instance;

};

SurvivalCraftMain* SurvivalCraftMain::instance = nullptr;

#endif // !SURVIVAL_CRAFT_HPP