#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Event/EventSystem.hpp"
#include "Render/ShaderManager.hpp"
#include "Render/TextureManager.hpp"

#define VertexAttribute(index, size, rawType, type, transpose, attribute) \
	glEnableVertexAttribArray(index); \
	glVertexAttribPointer(index, size, rawType, transpose, sizeof(type), (void*)offsetof(type, attribute));	\

using namespace SurvivalCraft::Event;

namespace SurvivalCraft
{
	namespace Render
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec2 textureCoordinates;

			static Vertex Create(const glm::vec3& position, const glm::vec3& color, const glm::vec2& textureCoordinates)
			{
				return { position, color, textureCoordinates };
			}
		};

		class Mesh;

		EventHeader(MeshCreationEvent)

			Mesh& mesh;

		explicit MeshCreationEvent(Mesh& mesh) : mesh(mesh) {}

		EventFooter;

		EventHeader(MeshGenerationEvent)

			Mesh& mesh;

		std::vector<Vertex>& vertices;
		std::vector<unsigned int>& indices;
		std::unordered_map<std::string, GLuint>& buffers;

		explicit MeshGenerationEvent(Mesh& mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::unordered_map<std::string, GLuint>& buffers) : mesh(mesh), vertices(vertices), indices(indices), buffers(buffers) {}

		EventFooter;

		EventHeader(MeshDeletionEvent)

			const std::string& name;

		explicit MeshDeletionEvent(const std::string& name) : name(name) {}

		EventFooter;

		class Mesh
		{

		public:
			
			void GenerateSquare()
			{
				vertices = 
				{
					Vertex::Create({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }),
					Vertex::Create({  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }),
					Vertex::Create({  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }),
					Vertex::Create({ -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f })
				};

				indices = 
				{
					0, 1, 2,
					2, 3, 0
				};
			}

			void Generate()
			{
				texture.Generate(TextureData::Create(TextureWrapMode::REPEAT, TextureFilterMode::NEAREST));
				eventSystem.Broadcast(TextureGenerationEvent(texture));

				glGenVertexArrays(1, &VAO);
				glBindVertexArray(VAO);

				glGenBuffers(1, &buffers["VBO"]);
				glBindBuffer(GL_ARRAY_BUFFER, buffers["VBO"]);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

				if (!indices.empty()) 
				{
					glGenBuffers(1, &buffers["EBO"]);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers["EBO"]);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
				}

				eventSystem.Broadcast(MeshGenerationEvent(*this, vertices, indices, buffers));

				VertexAttribute(0, 3, GL_FLOAT, Vertex, GL_FALSE, position);
				VertexAttribute(1, 3, GL_FLOAT, Vertex, GL_FALSE, color);
				VertexAttribute(2, 2, GL_FLOAT, Vertex, GL_FALSE, textureCoordinates);

				glBindVertexArray(0);

				shader.Use();
				shader.SetUniform("diffuse", 0);
			}

			void Render(const Camera& camera)
			{
				shader.Use();

				shader.SetUniform("projection", camera.GetProjectionMatrix());
				shader.SetUniform("view", camera.GetViewMatrix());
				shader.SetUniform("model", transform.GetMatrix());

				glActiveTexture(GL_TEXTURE0);

				texture.Use();

				glBindVertexArray(VAO);

				if (!indices.empty())
					glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
				else
					glDrawArrays(GL_TRIANGLES, 0, vertices.size());
				
				glBindVertexArray(0);

				GLenum error = glGetError();

				if (error != GL_NO_ERROR)
					Logger_ThrowError("GL_ERROR", "OpenGL error: " + std::to_string(error), false);
			}

			std::string GetName() const
			{ 
				return name; 
			}

			void SetData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
			{
				this->vertices = vertices;
				this->indices = indices;
			}

			void CleanUp()
			{
				eventSystem.Broadcast(MeshDeletionEvent(name));

				texture.CleanUp();

				eventSystem.Broadcast(TextureDeletionEvent(texture.GetName()));

				glDeleteVertexArrays(1, &VAO);

				for(auto& [key, value] : buffers)
					glDeleteBuffers(1, &value);
			}

			static void StaticInitialize(ShaderManager& shaderManager, TextureManager& textureManager, EventSystem& eventSystem)
			{
				Mesh::shaderManager = shaderManager;
				Mesh::textureManager = textureManager;
				Mesh::eventSystem = eventSystem;
			}

			static Mesh Create(const std::string& name, const std::string& shader, const std::string& texture, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
			{
				Mesh out = {};

				out.name = name;
				out.shader = shaderManager.Get(shader);
				out.texture = textureManager.Get(texture);
				out.vertices = vertices;
				out.indices = indices;
				out.transform = Transform::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

				eventSystem.Broadcast(MeshCreationEvent(out));

				return out;
			}
			
			Transform transform;

		private:

			std::string name;

			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			std::unordered_map<std::string, GLuint> buffers =
			{
				{ "VBO", 0 },
				{ "EBO", 0 }
			};

			GLuint VAO = 0;

			Shader shader;
			Texture texture;

			static ShaderManager shaderManager;
			static TextureManager textureManager;
			static EventSystem eventSystem;

		};

		ShaderManager Mesh::shaderManager = {};
		TextureManager Mesh::textureManager = {};
		EventSystem Mesh::eventSystem = {};
	}
}

#endif // !MESH_HPP