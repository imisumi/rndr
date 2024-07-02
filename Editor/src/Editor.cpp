#include "Rndr.h"
#include "Rndr/Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Rndr
{

	class Editor : public Application
	{
	public:
		Editor()
			: Application("Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Editor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new Editor();
	}
}