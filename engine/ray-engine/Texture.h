#pragma once
#include <vector>
#include "Types.h"

namespace app {

	struct Texture {
		using Ptr = Texture*;
		Texture();
		Texture(const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned int components);
		Texture(const std::vector<float> &data, unsigned int width, unsigned int height, unsigned int components);
		ColorHDR texture2D(float u, float v);
		unsigned int stride();
	private:
		std::vector<float> m_data;
		unsigned int m_width, m_height;
		unsigned int m_component;
	};
}