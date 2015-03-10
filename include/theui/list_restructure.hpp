#pragma once

#include <theui/window.hpp>

namespace the
{

namespace ui
{
class Window;

inline void
list_window_restructure( const Window& parent )
{
  const auto& x( parent.top_left().x );
  const auto& width( parent.size().width );
  const int child_height( parent.size().height / parent.children().size() );

  int next_y( parent.top_left().y );

  for ( const auto& child : parent.children() )
  {
    child->move_and_resize(
        { x, next_y },
        { width, child_height } );
    next_y += child_height;
  }
}

}

}

