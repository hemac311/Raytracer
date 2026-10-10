#include "Renderer.h"
#include "Walnut/Random.h"
#include <cmath>


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
			glm::vec2 coord = { (float)x / m_FinalImage->GetWidth(),(float)y / m_FinalImage->GetHeight()};
			coord = coord * 2.0f - 1.0f; // map from -1 to 1
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

	uint32_t imageWidth = m_FinalImage->GetWidth();
	uint32_t imageHeight = m_FinalImage->GetHeight();

	float center_x = 0;
	float center_y = 0;
	float aspect = (float)imageWidth / (float)imageHeight;
	float dx = (coord.x - center_x) * aspect;
	float dy = coord.y; - center_y ;
	float dxSqr = dx * dx;
	float dySqr = dy * dy;
	float radius = 0.5;


	glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	rayDirection = glm::normalize(rayDirection);

	//Render Circle
	/*
	if (dxSqr + dySqr < radius * radius)
	{
		auto pixel = Walnut::Random::UInt();
		pixel |= 0xff000000;
		return pixel;
	}
	else
	{
		return 0x00000000;
	}
	*/

	// Render Coordinates
	/*
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	return 0xff000000 | (g<<8) | r;
	*/

	// Ray Tracing
	uint8_t r = (uint8_t)((coord.x + 1.0f) / 2.0f * 255.0f);
	uint8_t g = (uint8_t)((coord.y + 1.0f) / 2.0f * 255.0f);

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	// quadratic formular discriminant
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0)
	{
		float hitDistance = (-b + std::sqrt(discriminant)) / (2 * a); //quadratic formular, first result
		uint8_t shading = (uint8_t)(hitDistance * 100.0f);
		return 0xff000000 | shading;
	}
	else
	{
		return 0x00000000;
	}
}
