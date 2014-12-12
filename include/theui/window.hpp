#pragma once

#include <memory>
#include <vector>

namespace the
{
namespace ui
{
class Window
{
  public:
    using Pointer = std::unique_ptr<the::ui::Window>;
    using Container = std::vector<Pointer>;
    class Coordinate
    {
      public:
        int x;
        int y;
    };

    class Size
    {
      public:
        int width;
        int height;
    };

    Window() = default;

    Window(const Coordinate& coordinate, const Size& size )
      : m_top_left_corner( coordinate )
      , m_size( size )
    {}

    template <typename...Ts>
    Window& create_child( Ts&&...ts )
    {
      m_children.push_back( std::make_unique< Window >( std::forward< Ts >( ts )... ) );
      return *m_children.back();
    }

    const Container& children() const
    {
      return m_children;
    }

    const Coordinate& top_left() const
    {
      return m_top_left_corner;
    }

    const Size& size() const
    {
      return m_size;
    }

  private:
    Container m_children;
    Coordinate m_top_left_corner;
    Size m_size;
};
}
}
