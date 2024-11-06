#ifndef RECT_H
#define RECT_H

#pragma once

struct Rect {
	int x, y;     // Top-left corner of the rectangle
	int width, height;  // Dimensions of the rectangle

	Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

	bool intersects(const Rect& other) const {
		return !(x + width < other.x || x > other.x + other.width ||
			y + height < other.y || y > other.y + other.height);
	}
};

#endif // RECT_H