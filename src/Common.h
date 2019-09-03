#pragma once

enum class ImageCollectionType
{
	Sequence,
	Stack
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

