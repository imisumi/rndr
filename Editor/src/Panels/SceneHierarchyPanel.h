#pragma once

#include <imgui/imgui.h>


#include "Rndr/Scene/Scene.h"

#include "Rndr/Core/Base.h"

#include "Rndr/Scene/Entity.h"

namespace Rndr
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		~SceneHierarchyPanel();


		SceneHierarchyPanel(const Ref<Scene>& scene);


		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext = { entt::null, nullptr };
	};
	

}