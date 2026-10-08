#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include <iostream>
#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

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

		if (m_Image)
		{
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });
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
		
		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) 
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth,m_ViewportHeight, ImageFormat::RGBA);
			m_ImageBuffer.resize(m_ViewportWidth * m_ViewportHeight);
		}

		float center_x = m_ViewportWidth / 2;
		float center_y = m_ViewportHeight / 2;
		float radius = 64000;
		

		for (uint32_t x = 0; x < m_ViewportWidth; x++)
		{
			float dx = x - center_x;
			
			for (uint32_t y = 0; y < m_ViewportHeight; y++)
			{
				//float dx = x - center_x;
				if (dx*dx + (y-center_y)*(y-center_y) < radius)
				{
					m_ImageBuffer[x  + y * m_ViewportWidth] = Random::UInt();
					m_ImageBuffer[x + y * m_ViewportWidth] |= 0xff000000;
				}
				else
				{
					m_ImageBuffer[x + y * m_ViewportWidth] = 0x00000000;
				}
			}
		}

		m_Image->SetData(m_ImageBuffer.data());
		m_LastRender = timer.ElapsedMillis();

	}
private:
	std::shared_ptr<Image> m_Image;
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