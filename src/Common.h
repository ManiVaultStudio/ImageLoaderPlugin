#pragma once

#include <vector>

enum class ImageCollectionType
{
	Sequence,
	Stack,
	MultiPartSequence
};

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