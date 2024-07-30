#pragma once

#include "Rndr/Core/Base.h"
#include "Rndr/Scene/Scene.h"
#include "Rndr/Scene/Entity.h"

namespace Rndr {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

		//TODO: Move this to Scene class
		void UpdateWorldTransform(Entity entity);
	private:
		void DeleteEntityNode(Entity entity);
		void ReccursiveDeleteEntityNode(Entity entity);

		bool DrawEntityNode(Entity entity);
		void DrawChildEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		void BeginDragDropSource(UUID id, const std::string& tag);
		bool BeginDragDropTarget(Entity entity);

		void ReccursiveUpdateWorldTransform(Entity entity);


		void RenderEntityIcon(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
