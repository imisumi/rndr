// #include "ViewportPanel.h"

// #include "imgui.h"

// #include "Rndr/Core/Base.h"

// #include "Rndr/Core/Application.h"

// namespace Rndr
// {
// 	void ViewportPanel::OnImGuiRender()
// 	{
// 		RNDR_CORE_ASSERT(m_Scene, "Scene not set");

// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
// 		ImGui::Begin("Viewport");

// 		auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

// 			m_ViewportFocused = ImGui::IsWindowFocused();
// 			m_ViewportHovered = ImGui::IsWindowHovered();
// 			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

// 			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
// 			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

// 			uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
// 			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

// 			auto windowSize = ImGui::GetWindowSize();
// 			auto minBound = ImGui::GetWindowPos();
// 			minBound.x += viewportOffset.x;
// 			minBound.y += viewportOffset.y;

// 			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
// 			m_ViewportBounds[0] = { minBound.x, minBound.y };
// 			m_ViewportBounds[1] = { maxBound.x, maxBound.y };

// 			if (ImGui::BeginDragDropTarget())
// 			{
// 				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
// 				{
// 					std::filesystem::path path((const wchar_t*)payload->Data);
// 					RNDR_CORE_INFO("Dropped file: {0}", path.string());
// 					// print file extension
// 					std::string extension = path.extension().string();
// 					RNDR_CORE_INFO("Extension: {0}", extension);
// 					// OpenScene(path);
// 					if (extension == ".fbx" || extension == ".obj")
// 						LoadMeshEntity(path);
// 				}
// 				ImGui::EndDragDropTarget();
// 			}

			

// 			// Gizmos
// 			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
// 			if (selectedEntity && m_GizmoType != -1)
// 			{
// 				ImGuizmo::SetOrthographic(false);
// 				ImGuizmo::SetDrawlist();

// 				float windowWidth = (float)ImGui::GetWindowWidth();
// 				float windowHeight = (float)ImGui::GetWindowHeight();
// 				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

// 				// Editor camera
// 				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
// 				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

// 				// Entity transform
// 				auto& tc = selectedEntity.GetComponent<TransformComponent>();
// 				glm::mat4 localTransform = tc.GetTransform();
// 				glm::mat4 worldTransform = tc.GetWorldTransform();

// 				// Snapping
// 				bool snap = Input::IsKeyPressed(RNDR_KEY_LEFT_CONTROL);
// 				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
// 				// Snap to 45 degrees for rotation
// 				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
// 					snapValue = 45.0f;

// 				float snapValues[3] = { snapValue, snapValue, snapValue };


// 				glm::mat4 worldTransformCopy = worldTransform;
// 				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
// 					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(worldTransformCopy),
// 					nullptr, snap ? snapValues : nullptr);

// 				if (ImGuizmo::IsUsing())
// 				{
// 					// glm::vec3 translation, rotation, scale;
// 					// Math::DecomposeTransform(worldTransform, translation, rotation, scale);

// 					// glm::vec3 deltaRotation = rotation - tc.Rotation;
// 					// tc.Translation = translation;
// 					// tc.Rotation += deltaRotation;
// 					// tc.Scale = scale;
// 					// m_SceneHierarchyPanel.UpdateWorldTransform(selectedEntity);


// 					glm::vec3 worldTranslation, worldRotation, worldScale;
// 					Math::DecomposeTransform(worldTransform, worldTranslation, worldRotation, worldScale);
// 					glm::vec3 translation, rotation, scale;
// 					Math::DecomposeTransform(worldTransformCopy, translation, rotation, scale);

// 					tc.Translation = translation - worldTranslation + tc.Translation;
// 					tc.Rotation = rotation - worldRotation + tc.Rotation;
// 					tc.Scale = scale - worldScale + tc.Scale;

// 					m_SceneHierarchyPanel.UpdateWorldTransform(selectedEntity);
// 				}
// 			}
// 		ImGui::End();
// 		ImGui::PopStyleVar();
// 	}
// }