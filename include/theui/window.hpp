#pragma once

#include <thectci/dispatcher.hpp>
#include <memory>
#include <vector>
#include <functional>

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
    bool operator==( const Size& other ) const
    {
      return
        width == other.width &&
        height == other.height;
    }
};

class Resized
{
  public:
    add_ctci( "the_ui_resized" );
};

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

        bool operator==( const Coordinate& other ) const
        {
          return
            x == other.x &&
            y == other.y;
        }
    };

    Window( Restructure window_restructure = do_nothing_window_restructure )
      : m_parent(nullptr)
      , m_window_restructure( std::move( window_restructure ) )
      , m_is_visible( true )
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
      , m_is_visible( true )
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

    void delete_child( const Window* window )
    {
      auto to_be_deleted( std::find_if( std::begin( m_children ), std::end( m_children ),
            [ window ]( const Pointer& child )
            {
              return child.get() == window;
            } ) );

      if ( to_be_deleted == std::end( m_children ) )
      {
        return;
      }

      m_children.erase( to_be_deleted );
      restructure();
    }

    void clear()
    {
      m_children.clear();
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

    Coordinate bottom_right() const
    {
      return {
        m_top_left_corner.x + m_size.width,
        m_top_left_corner.y + m_size.height };
    }

    const Size& size() const
    {
      return m_size;
    }

    void move_to( const Coordinate& new_top_left )
    {
      m_top_left_corner = new_top_left;
      restructure();
      set_visibility();
    }

    void resize( const Size& new_size )
    {
      resize_and_notify( new_size );
      restructure();
    }

    void move_and_resize( const Coordinate& new_top_left, const Size& new_size )
    {
      m_top_left_corner = new_top_left;
      resize_and_notify( new_size );
      restructure();
    }

    bool is_visible() const
    {
      return m_is_visible;
    }

    void request_restructure()
    {
      was_restructure_requested = true;
    }

  private:
    void set_visibility()
    {
      if ( nullptr == parent() )
      {
        return;
      }

      m_is_visible =
        top_left().x >= parent()->top_left().x &&
        top_left().y >= parent()->top_left().y &&
        bottom_right().y <= parent()->bottom_right().y &&
        bottom_right().x <= parent()->bottom_right().x;
    }

    void resize_and_notify( const Size& new_size )
    {
      m_size = new_size;
      set_visibility();
      m_dispatcher.dispatch( the::ui::Resized() );
    }

    volatile bool was_restructure_requested{ false };
    void restructure()
    {
      do
      {
        was_restructure_requested = false;
        m_window_restructure( *this );
      }
      while( was_restructure_requested );
    }

    Container m_children;
    Window* m_parent;
    Coordinate m_top_left_corner;
    Size m_size;
    Restructure m_window_restructure;
    bool m_is_visible;

  protected:
    the::ctci::Dispatcher m_dispatcher;
};

}
}
