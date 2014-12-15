#include <theui/list_restructure.hpp>
#include <theui/window.hpp>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_list_window_restructure )
{

  void add_children( size_t n )
  {
    for ( size_t i( 0 ); i < n; ++i )
    {
      parent->add_child( std::make_unique< the::ui::Window >() );
    }

    children = &parent->children();
  }

  void SetUp()
  {
    restructure = the::ui::list_window_restructure;
    parent = std::make_unique< the::ui::Window >( top_left, size );
    add_children( 5u );
    restructure( *parent );
  }


  template < typename F >
  void assert_each_window( F f )
  {
    AssertThat(
        std::all_of( std::begin( *children ), std::end( *children ), f ),
        Equals( true ) );
  }

  It( sets_the_top_left_x_coordinate_to_the_parents )
  {
    assert_each_window(
          [ x = top_left.x ]( const auto& child ) -> bool
          {
            return x == child->top_left().x;
          } );
  }

  It( sets_the_size_x_to_the_parents )
  {
    assert_each_window(
          [ width = size.width ]( const auto& child ) -> bool
          {
            return child->size().width == width;
          } );
  }

  It( sets_the_equal_size_y )
  {
    assert_each_window(
          [ height = size.height / children->size() ]( const auto& child ) -> bool
          {
            return child->size().height == int( height );
          } );
  }

  It( sets_the_y_coordinate )
  {
    int next_y( parent->top_left().y );
    const auto child_height( parent->size().height / children->size() );

    for ( const auto& child : *children )
    {
      AssertThat( child->top_left().y, Equals( next_y ) );
      next_y += child_height;
    }
  }

  const the::ui::Window::Coordinate top_left{ 100, 200 };
  const the::ui::Window::Size size{ 1000, 2000 };
  the::ui::Window::Pointer parent;
  the::ui::Window::Restructure restructure;
  const the::ui::Window::Container* children;
};

