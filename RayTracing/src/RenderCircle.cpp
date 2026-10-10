#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include <iostream>
#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Viewport Settings");

		if (ImGui::Button("Start Render"))
		{
			m_StartRender = true;
			Render();
		}

		if (ImGui::Button("Render once"))
		{
			Render();
		}
		ImGui::Text("Last render %.2fms", m_LastRender);


		if (ImGui::Button("Stop Render"))
		{
			m_StartRender = false;
		}

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		

		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		std::shared_ptr<Image> finalImage = m_Renderer.GetFinalImage();
		if (finalImage)
		{
			ImGui::Image(finalImage->GetDescriptorSet(), { (float)finalImage->GetWidth(), (float)finalImage->GetHeight() });
		}

		ImGui::End();
		ImGui::PopStyleVar();

		if (m_StartRender)
		{
			Render();
		}
	}

	void Render()
	{
		
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render();
		
		m_LastRender = timer.ElapsedMillis();

	}
private:
	Renderer m_Renderer;
	float m_LastRender = 0.0f;
	bool m_StartRender = false;

	//std::shared_ptr<uint32_t> m_TestBuffer;

	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;
	std::vector<uint32_t> m_ImageBuffer;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "RayTracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}