#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <thectci/dispatcher.hpp>

namespace the
{
namespace ui
{

class Window;
inline void do_nothing_window_restructure( const Window& ) {}

class Size
{
  public:
    int width;
    int height;
};

class Window
{
  public:
    using Pointer = std::unique_ptr<the::ui::Window>;
    using Container = std::vector<Pointer>;
    using Restructure = std::function< void( const Window& ) >;
    using Size = Size;

    class Coordinate
    {
      public:
        int x;
        int y;
    };

    Window( Restructure window_restructure = do_nothing_window_restructure )
      : m_parent(nullptr)
      , m_window_restructure( std::move( window_restructure ) )
    {
    }

    Window(
        const Coordinate& coordinate,
        const Size& size,
        Restructure window_restructure = do_nothing_window_restructure )
      : m_parent(nullptr)
      , m_top_left_corner( coordinate )
      , m_size( size )
      , m_window_restructure( std::move( window_restructure ) )
    {
    }

    virtual ~Window() = default;

    template <typename T>
    void dispatch( const T& event )
    {
      m_dispatcher.dispatch( event );
    }

    template <typename T>
    void broadcast( const T& event )
    {
      dispatch( event );
      for ( auto& child : m_children )
      {
        child->broadcast( event );
      }
    }

    Window& add_child( Pointer&& window )
    {
      Window& temporary_reference(*window);
      m_children.emplace_back( std::move( window ) );
      temporary_reference.m_parent = this;
      restructure();
      return *m_children.back();
    }

    const Container& children() const
    {
      return m_children;
    }

    Window* parent() const
    {
      return m_parent;
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
    Window* m_parent;
    Coordinate m_top_left_corner;
    Size m_size;
    Restructure m_window_restructure;

  protected:
    the::ctci::Dispatcher m_dispatcher;
};
}
}
