#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace the
{
namespace ui
{

class Window;
void do_nothing_window_restructure( const Window& ) {}

class Window
{
  public:
    using Pointer = std::unique_ptr<the::ui::Window>;
    using Container = std::vector<Pointer>;
    using Restructure = std::function< void( const Window& ) >;

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

    Window( Restructure window_restructure = do_nothing_window_restructure )
      : m_window_restructure( std::move( window_restructure ) )
    {
    }

    Window(
        const Coordinate& coordinate,
        const Size& size,
        Restructure window_restructure = do_nothing_window_restructure )
      : m_top_left_corner( coordinate )
      , m_size( size )
      , m_window_restructure( std::move( window_restructure ) )
    {
    }

    template <typename...Ts>
    Window& create_child( Ts&&...ts )
    {
      m_children.push_back( std::make_unique< Window >( std::forward< Ts >( ts )... ) );
      restructure();
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

    void move_to( const Coordinate& new_top_left )
    {
      m_top_left_corner = new_top_left;
      restructure();
    }

    void resize( const Size& new_size )
    {
      m_size = new_size;
      restructure();
    }

    void move_and_resize( const Coordinate& new_top_left, const Size& new_size )
    {
      m_top_left_corner = new_top_left;
      m_size = new_size;
      restructure();
    }

  private:
    void restructure()
    {
      m_window_restructure( *this );
    }

    Container m_children;
    Coordinate m_top_left_corner;
    Size m_size;
    Restructure m_window_restructure;
};
}
}
