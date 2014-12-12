#include <theui/window_restructure.hpp>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_list_window_restructure )
{

  void SetUp()
  {
    window_restructure = the::ui::list_window_restructure;
  }

  It( is_a_window_restructure )
  {
  }

  the::ui::WindowRestructure window_restructure;
};

