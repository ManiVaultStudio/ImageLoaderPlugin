#pragma once

/**
 * Image resampling filter
 * Defines image resampling filters for image downsampling
 */
enum class ImageResamplingFilter
{
	Box,			/** Box filter */
	Bilinear,		/** Bilinear filter */
	BSpline,		/** B-spline filter */
	Bicubic,		/** B-cubic filter */
	CatmullRom,		/** Catmull-rom filter */
	Lanczos			/** Lanczos filter */
};

template <typename E>
constexpr auto ult(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}