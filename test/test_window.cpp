#include <theui/window.hpp>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( a_window )
{
  Describe( children )
  {
    void SetUp()
    {
      window = std::make_unique<the::ui::Window>();
    }

    It(owns_children_windows)
    {
      window->create_child();
      const auto& children(window->children());
      AssertThat(children, HasLength(1u));
    }

    It(returns_the_new_child_reference)
    {
      auto& new_child(window->create_child());
      AssertThat(&new_child, Equals(window->children().back().get()));
    }

    the::ui::Window::Pointer window;
  };

  Describe( dimensions )
  {
    It(has_two_dimensions)
    {
      the::ui::Window window(top_left, size);
      AssertThat( window.top_left().x, Equals( top_left.x ) );
      AssertThat( window.top_left().y, Equals( top_left.y ) );
      AssertThat( window.size().width, Equals( size.width ) );
      AssertThat( window.size().height, Equals( size.height ) );
    }

    const the::ui::Window::Coordinate top_left{0, 0};
    const the::ui::Window::Size size{ 100, 200 };
  };
};

