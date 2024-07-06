#include "Rndr.h"
#include "Rndr/Core/EntryPoint.h"
// #include "EditorLayer.h"
#include "SandBox3D.h"

namespace Rndr
{

	class Editor : public Application
	{
	public:
		Editor()
			: Application("Editor")
		{
			PushLayer(new Sandbox3D());
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