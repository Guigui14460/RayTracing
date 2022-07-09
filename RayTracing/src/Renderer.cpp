#include "Renderer.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage) {
		// no resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec2 coord = { x / (float)m_FinalImage->GetWidth(), y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.f - 1.f; // -1 -> 1
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	glm::vec3 rayOrigin(0.f, 0.f, 2.f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.f);
	rayDirection = glm::normalize(rayDirection);
	float radius = .5f;

	// (bx^2 + by^2)t^2 + 2t(axbx + ayby) + (ax^2 + ay^2 - r^2) = 0
	// where
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	// quadratic formula discriminant
	// b^2 - 4ac
	float discriminant = b * b - 4.f * a * c;

	if (discriminant >= 0.f)
		return 0xffff00ff;

	return 0xff000000;
}
