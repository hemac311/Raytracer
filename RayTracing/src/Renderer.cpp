#include "Renderer.h"
#include "Walnut/Random.h"


void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	m_ImageBuffer.resize(width * height);

}

void Renderer::Render()
{
	uint32_t imageWidth = m_FinalImage->GetWidth();
	uint32_t imageHeight = m_FinalImage->GetHeight();





	//call PixelShader
	for (uint32_t y = 0; y < imageHeight; y++)
	{
		for (uint32_t x = 0; x < imageWidth; x++)
		{
			glm::vec2 coord = { x,y };
			int pos = x + y * imageWidth;

			m_ImageBuffer[pos] = PixelShader(coord);
		}
	}
	m_FinalImage->SetData(m_ImageBuffer.data());


}

std::shared_ptr<Walnut::Image> Renderer::GetFinalImage()
{
	return m_FinalImage;
}

uint32_t Renderer::PixelShader(glm::vec2 coord)
{
	//Render Circle

	uint32_t imageWidth = m_FinalImage->GetWidth();
	uint32_t imageHeight = m_FinalImage->GetHeight();

	float center_x = imageWidth / 2;
	float center_y = imageHeight / 2;
	float radius = 64000;

	int x = coord.x;
	int y = coord.y;
	float dx = x - center_x;
	float dxSqr = dx * dx;

	if (dxSqr + (y - center_y) * (y - center_y) < radius)
	{
		auto pixel = Walnut::Random::UInt();
		pixel |= 0xff000000;
		return pixel;
	}
	else
	{
		return 0x00000000;
	}
}
