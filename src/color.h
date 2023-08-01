#pragma once
#include <iostream>

class Color {
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  Color() : r(0), g(0), b(0), a(255) {}
  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : r(red), g(green), b(blue), a(alpha) {}

  Color operator+(const Color& other) const {
    uint8_t newRed = static_cast<uint8_t>(r + other.r);
    uint8_t newGreen = static_cast<uint8_t>(g + other.g);
    uint8_t newBlue = static_cast<uint8_t>(b + other.b);
    uint8_t newAlpha = static_cast<uint8_t>(a + other.a);

    // Clamping values to the range [0, 255]
    if (newRed > 255)
      newRed = 255;

    if (newGreen > 255)
      newGreen = 255;

    if (newBlue > 255)
      newBlue = 255;

    if (newAlpha > 255)
      newAlpha = 255;

    return Color(newRed, newGreen, newBlue, newAlpha);
  }

  Color operator*(float scalar) const {
    uint8_t newRed = static_cast<uint8_t>(r * scalar);
    uint8_t newGreen = static_cast<uint8_t>(g * scalar);
    uint8_t newBlue = static_cast<uint8_t>(b * scalar);
    uint8_t newAlpha = static_cast<uint8_t>(a * scalar);

    // Clamping values to the range [0, 255]
    if (newRed > 255)
      newRed = 255;

    if (newGreen > 255)
      newGreen = 255;

    if (newBlue > 255)
      newBlue = 255;

    if (newAlpha > 255)
      newAlpha = 255;

    return Color(newRed, newGreen, newBlue, newAlpha);
  }

  friend std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "RGBA: (" << static_cast<int>(color.r) << ", " << static_cast<int>(color.g) << ", " << static_cast<int>(color.b) << ", " << static_cast<int>(color.a) << ")";
    return os;
  }
};
