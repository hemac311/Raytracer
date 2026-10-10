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

private:
	uint32_t PixelShader(glm::vec2 coord);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	std::vector<uint32_t> m_ImageBuffer;

};