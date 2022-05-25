// Copyright (c) 2022 Malik Allen

#pragma once

#include <string>

struct Extent
{
	uint32_t width;
	uint32_t height;
};

struct Window_Properties
{
	std::string name;
	Extent extent;
};