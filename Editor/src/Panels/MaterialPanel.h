#pragma once

#include "Rndr/Renderer/Material.h"

namespace Rndr
{
	class MaterialPanel
	{
	public:
		MaterialPanel() = default;
		~MaterialPanel();

		void SetMaterialLibrary(Ref<MaterialLibrary> materialLibrary) { m_MaterialLibrary = materialLibrary; }

		void OnImGuiRender();
	private:
		Ref<MaterialLibrary> m_MaterialLibrary;
		Ref<Material> m_SelectedMaterial;
	};
}