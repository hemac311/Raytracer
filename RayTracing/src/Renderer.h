#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();
	std::shared_ptr<Walnut::Image> GetFinalImage();


public:
	float lightPosX = 0.5f;
	float lightPosY = 0.5f;
	float lightPosZ = 0.5f;

private:
	uint32_t Rasterizer(glm::vec2 coord);
	glm::vec4 PixelShader(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 lightDir);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	std::vector<uint32_t> m_ImageBuffer;

};