#pragma once

#include <Qt>

namespace ConanQTOSGDemo {
[[nodiscard]] inline unsigned int qtButtonToOSGCode(
    const Qt::MouseButton& qtButton) {
  switch (qtButton) {
    case Qt::LeftButton:
      return 1;
    case Qt::MiddleButton:
      return 2;
    case Qt::RightButton:
      return 3;
    default:
      return 0;
  }
}
}  // namespace ConanQTOSGDemo
