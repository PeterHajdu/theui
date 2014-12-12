#include <theui/window_restructure.hpp>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( a_list_window_restructure )
{

  It( is_instantiable )
  {
    the::ui::ListWindowRestructure list_window_restructure;
    (void)list_window_restructure;
  }

};

