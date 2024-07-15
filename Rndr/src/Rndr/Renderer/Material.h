#pragma once

#include "Rndr/Core/Base.h"
#include "Rndr/Renderer/Shader.h"
#include "Rndr/Renderer/Texture.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iterator> // for std::iterator

namespace Rndr
{
	enum class TextureType
	{
		Diffuse = 0,
	};

	//TODO: abstract and move to OpenGLMaterial.h
	class Material
	{
	public:
		Material();
		virtual ~Material() = default;

		void Bind() const;
		void Unbind() const;

		void Use() const { Bind(); }

		Ref<Shader> GetShader() const { return m_Shader; }

		void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }


		// void SetColor(const glm::vec4& color) { m_Shader->SetFloat4("u_Color", color); }
		// void SetColor(const glm::vec3& color) { m_Shader->SetFloat3("u_Color", glm::vec4(color, 1.0f)); }
		// void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { m_Shader->SetFloat4("u_Color", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)); }
		// void SetColor(uint8_t r, uint8_t g, uint8_t b) { m_Shader->SetFloat3("u_Color", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f)); }
		// void SetColor(uint32_t hexColor) { m_Shader->SetFloat4("u_Color", glm::vec4((hexColor >> 16 & 0xFF) / 255.0f, (hexColor >> 8 & 0xFF) / 255.0f, (hexColor & 0xFF) / 255.0f, (hexColor >> 24 & 0xFF) / 255.0f)); }


		


		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }

		glm::vec4 GetColor() const { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }
		// get color value_ptr


		void SetTexture(const Ref<Texture2D>& texture, TextureType type = TextureType::Diffuse);
		uint32_t GetTextureID(TextureType type = TextureType::Diffuse) const;
		Ref<Texture2D> GetTexture(TextureType type = TextureType::Diffuse) const;


		static Ref<Material> Create();
	private:
		std::string m_Name;
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_Texture;


		glm::vec4 m_Color = { 0.8f, 0.2f, 0.1f, 1.0f };
		// glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> m_DiffuseMap;

	};



	class LineMaterial
	{
	public:
		LineMaterial() = default;
		virtual ~LineMaterial() = default;

		void Bind() const { m_Shader->Bind(); }
		void Unbind() const { m_Shader->Unbind(); }

		void SetColor(const glm::vec4& color) { m_Color = color; }
		glm::vec4 GetColor() const { return m_Color; }

		void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }
		Ref<Shader> GetShader() const { return m_Shader; }

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }

		static Ref<LineMaterial> Create(const Ref<Shader>& shader, const std::string& name = "LineMaterial")
		{
			Ref<LineMaterial> material = CreateRef<LineMaterial>();
			material->SetShader(shader);
			material->SetName(name);
			return material;
		}
	private:
		std::string m_Name;
		Ref<Shader> m_Shader;
		glm::vec4 m_Color = { 0.8f, 0.2f, 0.1f, 1.0f };
	};


	class MaterialLibrary
	{
	public:
		void Add(const Ref<Material>& material);
		void Add(const std::string& name, const Ref<Material>& material);

		Ref<Material> Get(const std::string& name);
		bool Exists(const std::string& name) const;




		// Iterator typedefs
		typedef typename std::unordered_map<std::string, Ref<Material>>::iterator iterator;
		typedef typename std::unordered_map<std::string, Ref<Material>>::const_iterator const_iterator;

		// Iterator functions
		iterator begin() { return m_Materials.begin(); }
		iterator end() { return m_Materials.end(); }
		const_iterator begin() const { return m_Materials.begin(); }
		const_iterator end() const { return m_Materials.end(); }
	private:
		std::unordered_map<std::string, Ref<Material>> m_Materials;
	};

	// class MaterialInstance
	// {
	// public:
	// 	MaterialInstance(const Ref<Material>& material);
	// 	~MaterialInstance() = default;

	// 	void Bind() const;

	// 	void SetColor(const glm::vec4& color) { m_Material->SetColor(color); }
	// 	void SetColor(const glm::vec3& color) { m_Material->SetColor(color); }
	// 	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { m_Material->SetColor(r, g, b, a); }
	// 	void SetColor(uint8_t r, uint8_t g, uint8_t b) { m_Material->SetColor(r, g, b); }
	// 	void SetColor(uint32_t hexColor) { m_Material->SetColor(hexColor); }

	// 	void SetTexture(const Ref<Texture2D>& texture) { m_Material->SetTexture(texture); }
	// 	void SetTexture(const std::string& name, const Ref<Texture2D>& texture) { m_Material->SetTexture(name, texture); }
	// 	void SetFloat(const std::string& name, float value) { m_Material->SetFloat(name, value); }
	// 	void SetFloat2(const std::string& name, const glm::vec2& value) { m_Material->SetFloat2(name, value); }
	// 	void SetFloat3(const std::string& name, const glm::vec3& value) { m_Material->SetFloat3(name, value); }
	// 	void SetFloat4(const std::string& name, const glm::vec4& value) { m_Material->SetFloat4(name, value); }
	// 	void SetMat4(const std::string& name, const glm::mat4& value) { m_Material->SetMat4(name, value); }

	// 	const std::string& GetName() const { return m_Material->GetName(); }
	// private:
	// 	Ref<Material> m_Material;
	// };
}