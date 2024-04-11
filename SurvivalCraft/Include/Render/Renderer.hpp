#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include "Render/Camera.hpp"
#include "Render/Mesh.hpp"

namespace SurvivalCraft
{
	namespace Render
	{
		class Renderer
		{

		public:
			
			void Register(const Mesh& mesh)
			{
				registeredMeshes[mesh.GetName()] = mesh;
			}

			Mesh& Get(const std::string& name)
			{
				return registeredMeshes[name];
			}

			void Render(const Camera& camera)
			{
				for (auto& [key, mesh] : registeredMeshes)
					mesh.Render(camera);
			}

			void CleanUp()
			{
				for (auto& [key, mesh] : registeredMeshes)
					mesh.CleanUp();
			}

		private:
			
			std::unordered_map<std::string, Mesh> registeredMeshes;

		};
	}
}

#endif // !RENDERER_HPP