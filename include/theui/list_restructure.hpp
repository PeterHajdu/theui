#pragma once

#include <theui/window.hpp>

namespace the
{

namespace ui
{
class Window;

class WindowResizer
{
  public:
    inline
    WindowResizer( const Window& parent )
      : m_parent( parent )
      , m_parent_x( parent.top_left().x )
      , m_parent_width( parent.size().width )
    {
    }


    inline void
    fit_with_equal_height() const
    {
      const int child_height( m_parent.size().height / m_parent.children().size() );
      int next_y( m_parent.top_left().y );

      for ( const auto& child : m_parent.children() )
      {
        resize_with_y_and_height( next_y, child_height, *child );
        next_y += child_height;
      }
    }


    inline void
    front_from_top_with_fixed_height() const
    {
      int next_y( m_parent.top_left().y );

      for ( const auto& child : m_parent.children() )
      {
        const int original_height( child->size().height );
        resize_with_y_and_height( next_y, original_height, *child );
        next_y += original_height;
      }
    }


    inline void
    back_from_bottom_with_fixed_height() const
    {
      int next_y( m_parent.bottom_right().y );

      const auto& children( m_parent.children() );
      for (
          auto child_it( children.rbegin() );
            child_it != children.rend();
            ++child_it )
      {
        Window& child( **child_it );
        const int original_height( child.size().height );
        next_y -= original_height;
        resize_with_y_and_height( next_y, original_height, child );
      }
    }


  private:
    inline void
    resize_with_y_and_height( int y, int height, Window& child ) const
    {
      child.move_and_resize(
          { m_parent_x, y },
          { m_parent_width, height } );
    }

    const Window& m_parent;
    const int m_parent_x;
    const int m_parent_width;
};

inline void
fit_with_equal_height( const Window& parent )
{
  WindowResizer( parent ).fit_with_equal_height();
}

inline void
front_from_top_with_fixed_height( const Window& parent )
{
  WindowResizer( parent ).front_from_top_with_fixed_height();
}

inline void
back_from_bottom_with_fixed_height( const Window& parent )
{
  WindowResizer( parent ).back_from_bottom_with_fixed_height();
}

}

}

