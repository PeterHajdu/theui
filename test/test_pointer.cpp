#include <theui/pointer.hpp>
#include <theui/window.hpp>

#include <memory>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_pointer )
{
  the::ui::Window* nth_child(size_t n)
  {
    return root_window->children()[n-1].get();
  }

  void point_to(size_t n)
  {
    initializing_window = nth_child(n);
    pointer = std::make_unique< the::ui::Pointer > ( *initializing_window );
  }

  void SetUp()
  {
    root_window = std::make_unique< the::ui::Window > ();
    for ( int i(0); i < children_count; ++i)
    {
      root_window->add_child(std::make_unique< the::ui::Window > ());
    }
    point_to(1);
  }

  It(points_to_the_initializing_window)
  {
    AssertThat(&pointer->selected(), Equals(initializing_window));
  }

  It(can_jump_to_next_window)
  {
    pointer->next();
    AssertThat(&pointer->selected(), Equals(nth_child(2)));
  }

  It(jumps_to_the_first_window_from_the_last)
  {
    point_to(children_count);
    pointer->next();
    AssertThat(&pointer->selected(), Equals(nth_child(1)));
  }

  It(can_jump_to_previous_window)
  {
    point_to(children_count);
    pointer->previous();
    AssertThat(&pointer->selected(), Equals(nth_child(children_count-1)));
  }

  It(jumps_to_the_last_window_from_the_first)
  {
    pointer->previous();
    AssertThat(&pointer->selected(), Equals(nth_child(children_count)));
  }


  std::unique_ptr< the::ui::Window > root_window;
  the::ui::Window* initializing_window;
  std::unique_ptr< the::ui::Pointer > pointer;
  const int children_count{3};
};

