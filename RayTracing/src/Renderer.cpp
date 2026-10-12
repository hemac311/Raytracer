#include "Renderer.h"
#include "Walnut/Random.h"
#include <cmath>

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (color.r * 255.0f);
		uint8_t g = (color.g * 255.0f);
		uint8_t b = (color.b * 255.0f);
		uint8_t a = (color.a * 255.0f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}
}

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
	float aspect = (float)imageWidth / (float)imageHeight;


	glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);

	glm::vec3 lightDir(lightPosX, lightPosY, lightPosZ);
	lightDir = glm::normalize(lightDir);


	//call PixelShader
	for (uint32_t y = 0; y < imageHeight; y++)
	{
		for (uint32_t x = 0; x < imageWidth; x++)
		{
			glm::vec2 coord = { (float)x / imageWidth,(float)y / imageHeight};
			coord = coord * 2.0f - 1.0f; // map from -1 to 1
			int pos = x + y * imageWidth;
			glm::vec3 rayDirection(coord.x * aspect, coord.y, -1.0f);
			rayDirection = glm::normalize(rayDirection);
			glm::vec4 color = PixelShader(rayOrigin, rayDirection, lightDir);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageBuffer[pos] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageBuffer.data());

}

std::shared_ptr<Walnut::Image> Renderer::GetFinalImage()
{
	return m_FinalImage;
}

glm::vec4 Renderer::PixelShader(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 lightDir)
{
	// Ray Tracing
	float radius = 0.5;
	glm::vec3 sphereOrigin(0.0f, 0.0f, 0.0f);


	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	// quadratic formular discriminant
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0)
	{
		//float t0 = (-b + std::sqrt(discriminant)) / (2.0f * a); 
		float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a); //quadratic formular, first hit

		//glm::vec3 hitLoc1 = rayOrigin + rayDirection * t0;
		glm::vec3 hitLoc2 = rayOrigin + rayDirection * t1;
		 
		glm::vec3 normal = glm::normalize(hitLoc2 - sphereOrigin);

		float lightShading = std::max(0.0f,(((glm::dot(normal, -lightDir)) * 0.5f + 0.5f)));
		
		// Preview Normals:
		//return glm::vec4(normal * 0.5f + 0.5f, 1.0f);

		glm::vec3 sphereColor(1, 0, 0);
		glm::vec3 shading = lightShading * sphereColor;

		return glm::vec4(shading, 1.0f);
	}
	else
	{
		return glm::vec4(0, 0, 0, 1);
	}
}


uint32_t Renderer::Rasterizer(glm::vec2 coord)
{
	uint32_t imageWidth = m_FinalImage->GetWidth();
	uint32_t imageHeight = m_FinalImage->GetHeight();

	float center_x = 0;
	float center_y = 0;
	float aspect = (float)imageWidth / (float)imageHeight;
	float dx = (coord.x - center_x) * aspect;
	float dy = coord.y; -center_y;
	float dxSqr = dx * dx;
	float dySqr = dy * dy;
	float radius = 0.5;

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
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	return 0xff000000 | (g << 8) | r;

}