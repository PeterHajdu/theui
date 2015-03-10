#include <theui/list_restructure.hpp>
#include <theui/window.hpp>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

class WindowRestructureBundle
{
  public:
    WindowRestructureBundle(
        const the::ui::Window::Coordinate& top_left,
        const the::ui::Size& size,
        the::ui::Window::Restructure window_restructure,
        const size_t number_of_children )
      : m_window_restructure( std::move( window_restructure ) )
      , m_parent( std::make_unique< the::ui::Window >( top_left, size ) )
      , parent( *m_parent )
    {
      add_children( number_of_children );
    }

    void restructure()
    {
      m_window_restructure( parent );
    }

    template < typename F >
    void assert_each_child( F f )
    {
      const auto& children( parent.children() );
      AssertThat(
          std::all_of( std::begin( children ), std::end( children ), f ),
          Equals( true ) );
    }

    const the::ui::Window::Coordinate original_child_top_left{ 34, 87 };
    const the::ui::Size original_child_size{ 234, 987 };
  private:
    void add_children( size_t n )
    {
      for ( size_t i( 0 ); i < n; ++i )
      {
        m_parent->add_child( std::make_unique< the::ui::Window >(
              original_child_top_left,
              original_child_size ) );
      }
    }

    the::ui::Window::Restructure m_window_restructure;
    the::ui::Window::Pointer m_parent;

  public:
    const the::ui::Window& parent;
};

}

Describe( a_list_window_restructure )
{

  void SetUp()
  {
    restructure_bundle = std::make_unique< test::WindowRestructureBundle >(
        top_left,
        size,
        the::ui::list_window_restructure,
        number_of_children );
    restructure_bundle->restructure();
  }

  const the::ui::Window::Container& children()
  {
    return restructure_bundle->parent.children();
  }

  It( sets_the_top_left_x_coordinate_to_the_parents )
  {
    restructure_bundle->assert_each_child(
          [ x = top_left.x ]( const auto& child ) -> bool
          {
            return x == child->top_left().x;
          } );
  }

  It( sets_the_width_to_the_parents )
  {
    restructure_bundle->assert_each_child(
          [ width = size.width ]( const auto& child ) -> bool
          {
            return child->size().width == width;
          } );
  }

  It( sets_the_equal_size_y )
  {
    restructure_bundle->assert_each_child(
          [ height = size.height / number_of_children ]( const auto& child ) -> bool
          {
            return child->size().height == int( height );
          } );
  }

  It( sets_the_y_coordinate )
  {
    int next_y( restructure_bundle->parent.top_left().y );
    const auto child_height( restructure_bundle->parent.size().height / number_of_children );

    for ( const auto& child : children() )
    {
      AssertThat( child->top_left().y, Equals( next_y ) );
      next_y += child_height;
    }
  }

  const size_t number_of_children{ 5u };
  std::unique_ptr< test::WindowRestructureBundle > restructure_bundle;
  const the::ui::Window::Coordinate top_left{ 100, 200 };
  const the::ui::Size size{ 1000, 2000 };
};

Describe( a_list_window_restructure_with_fixed_height_from_top )
{

  void SetUp()
  {
    restructure_bundle = std::make_unique< test::WindowRestructureBundle >(
        top_left,
        size,
        the::ui::list_window_restructure_with_fixed_height_from_top,
        number_of_children );
    restructure_bundle->restructure();
  }

  const the::ui::Window::Container& children()
  {
    return restructure_bundle->parent.children();
  }

  It( sets_the_top_left_x_coordinate_to_the_parents )
  {
    restructure_bundle->assert_each_child(
          [ x = top_left.x ]( const auto& child ) -> bool
          {
            return x == child->top_left().x;
          } );
  }

  It( sets_the_width_to_the_parents )
  {
    restructure_bundle->assert_each_child(
          [ width = size.width ]( const auto& child ) -> bool
          {
            return child->size().width == width;
          } );
  }

  It( does_not_touch_original_height )
  {
    restructure_bundle->assert_each_child(
          [ original_height = restructure_bundle->original_child_size.height ]( const auto& child ) -> bool
          {
            return child->size().height == original_height;
          } );
  }

  It( increments_y_coordinates_with_the_original_height_of_the_previous_children )
  {
    int next_y( restructure_bundle->parent.top_left().y );
    const auto child_height( restructure_bundle->original_child_size.height );

    for ( const auto& child : children() )
    {
      AssertThat( child->top_left().y, Equals( next_y ) );
      next_y += child_height;
    }
  }

  const size_t number_of_children{ 5u };
  std::unique_ptr< test::WindowRestructureBundle > restructure_bundle;
  const the::ui::Window::Coordinate top_left{ 100, 200 };
  const the::ui::Size size{ 1000, 2000 };
};

