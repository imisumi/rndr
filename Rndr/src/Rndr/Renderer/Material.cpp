#include "Material.h"

#include "Rndr/Core/Log.h"


namespace Rndr
{
	void Material::Bind() const
	{
		m_Shader->Bind();
	}

	void Material::Unbind() const
	{
		m_Shader->Unbind();
	}












	Ref<Material> Material::Create()
	{
		return CreateRef<Material>();
	}

	//? ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//? MaterialLibrary



	void MaterialLibrary::Add(const Ref<Material>& material)
	{
		auto& name = material->GetShader()->GetName();
		Add(name, material);
	}

	void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material)
	{
		RNDR_CORE_ASSERT(!Exists(name), "Material already exists!");
		m_Materials[name] = material;
	}

	Ref<Material> MaterialLibrary::Get(const std::string& name)
	{
		RNDR_CORE_ASSERT(Exists(name), "Material not found!");
		return m_Materials[name];
	}

	bool MaterialLibrary::Exists(const std::string& name) const
	{
		return m_Materials.find(name) != m_Materials.end();
	}

} // namespace Rndr