#pragma once

#include <vector>

enum class ImageResamplingFilter
{
	Box,
	Bilinear,
	BSpline,
	Bicubic,
	CatmullRom,
	Lanczos
};

using FloatVector = std::vector<float>;