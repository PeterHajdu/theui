#include <theui/window.hpp>
#include <thectci/id.hpp>

#include "test_window.hpp"

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( a_window )
{
  Describe( children )
  {
    void SetUp()
    {
      window = std::make_unique<the::ui::Window>( top_left, size );
    }

    It(owns_children_windows)
    {
      window->add_child( std::make_unique< the::ui::Window >() );
      const auto& children(window->children());
      AssertThat(children, HasLength(1u));
    }

    It(returns_the_new_child_reference)
    {
      auto& new_child( window->add_child( std::make_unique< the::ui::Window >() ) );
      AssertThat(&new_child, Equals(window->children().back().get()));
    }

    It(is_visible_by_default)
    {
      AssertThat( window->is_visible(), Equals( true ) );
    }

    It(becomes_invisible_if_it_is_moved_outside_the_borders_of_parent_window)
    {
      auto& new_child( window->add_child( std::make_unique< the::ui::Window >( top_left, size ) ) );
      const the::ui::Window::Coordinate left_of_parent{ top_left.x - 1, top_left.y };
      new_child.move_to( left_of_parent );
      AssertThat( new_child.is_visible(), Equals( false ) );

      const the::ui::Window::Coordinate above_parent{ top_left.x, top_left.y - 1 };
      new_child.move_to( above_parent );
      AssertThat( new_child.is_visible(), Equals( false ) );

      const the::ui::Window::Coordinate below_parent{ top_left.x, top_left.y + 1 };
      new_child.move_to( below_parent );
      AssertThat( new_child.is_visible(), Equals( false ) );

      const the::ui::Window::Coordinate right_of_parent{ top_left.x + 1, top_left.y };
      new_child.move_to( right_of_parent );
      AssertThat( new_child.is_visible(), Equals( false ) );
    }

    It(becomes_invisible_if_it_is_resized_outside_the_borders_of_parent_window)
    {
      auto& new_child( window->add_child( std::make_unique< the::ui::Window >( top_left, size ) ) );
      const the::ui::Size wider_than_parent{ size.width + 1, size.height };
      new_child.resize( wider_than_parent );
      AssertThat( new_child.is_visible(), Equals( false ) );

      const the::ui::Size higher_than_parent{ size.width, size.height + 1 };
      new_child.resize( higher_than_parent );
      AssertThat( new_child.is_visible(), Equals( false ) );
    }

    It(sets_parent_window)
    {
      auto& new_child( window->add_child( std::make_unique< the::ui::Window >() ) );
      AssertThat(new_child.parent(), Equals(window.get()));
    }

    It(can_delete_a_children)
    {
      auto& new_child_1( window->add_child( std::make_unique< the::ui::Window >() ) );
      auto& new_child_2( window->add_child( std::make_unique< the::ui::Window >() ) );
      window->delete_child( &new_child_1 );
      AssertThat(&new_child_1, !Equals(window->children().back().get()));
      AssertThat(&new_child_2, Equals(window->children().back().get()));
    }

    It(handles_non_existent_child_deletion)
    {
      window->delete_child( nullptr );
    }

    It(can_delete_all_children)
    {
      auto& new_child_1( window->add_child( std::make_unique< the::ui::Window >() ) );
      auto& new_child_2( window->add_child( std::make_unique< the::ui::Window >() ) );
      window->clear();
      AssertThat(&new_child_1, !Equals(window->children().back().get()));
      AssertThat(&new_child_2, !Equals(window->children().back().get()));
    }

    the::ui::Window::Pointer window;
    const the::ui::Window::Coordinate top_left{ 1, 2 };
    const the::ui::Size size{ 100, 200 };
  };

  Describe( dimensions )
  {
    void SetUp()
    {
      window = std::make_unique<the::ui::Window>( top_left, size );
    }

    void assert_top_left_is( const the::ui::Window::Coordinate& expected_top_left )
    {
      AssertThat( window->top_left(), Equals( expected_top_left ) );
    }

    void assert_size_is( const the::ui::Size& expected_size )
    {
      AssertThat( window->size(), Equals( expected_size ) );
    }

    It(has_two_dimensions)
    {
      assert_top_left_is( top_left );
    }

    It(can_be_moved)
    {
      window->move_to( another_top_left );
      assert_top_left_is( another_top_left );
    }

    It(calculates_bottom_right)
    {
      const the::ui::Window::Coordinate expected_bottom_right_corner{
        top_left.x + size.width,
        top_left.y + size.height };
      AssertThat( window->bottom_right(), Equals( expected_bottom_right_corner ) );
    }

    It(can_be_resized)
    {
      window->resize( another_size );
      assert_size_is( another_size );
    }

    It(can_be_moved_and_resized_with_one_call)
    {
      window->move_and_resize( another_top_left, another_size );
      assert_top_left_is( another_top_left );
      assert_size_is( another_size );
    }

    const the::ui::Window::Coordinate top_left{1, 2};
    const the::ui::Window::Coordinate another_top_left{10, 20};
    const the::ui::Size size{ 100, 200 };
    const the::ui::Size another_size{ 101, 201 };
    the::ui::Window::Pointer window;
  };

  Describe( window_restructure )
  {
    void SetUp()
    {
      was_restructure_called = false;
      number_of_restructure_calls = 0;
      window = std::make_unique< the::ui::Window >(
          top_left,
          size,
          [ &was_called = was_restructure_called,
            &number_of_calls = number_of_restructure_calls ]( const the::ui::Window& )
          {
            ++number_of_calls;
            was_called = true;
          } );
    }

    It(calls_the_restructure_function_after_child_addition)
    {
      window->add_child( std::make_unique< the::ui::Window >() );
      AssertThat( was_restructure_called, Equals( true ) );
    }

    It(calls_the_restructure_function_after_child_deletion)
    {
      const auto& new_window( window->add_child( std::make_unique< the::ui::Window >() ) );
      was_restructure_called = false;
      window->delete_child( &new_window );
      AssertThat( was_restructure_called, Equals( true ) );
    }

    It(calls_the_restructure_function_when_moved)
    {
      window->move_to( { 1, 2 } );
      AssertThat( was_restructure_called, Equals( true ) );
    }

    It(calls_the_restructure_function_when_resized)
    {
      window->resize( { 543, 234 } );
      AssertThat( was_restructure_called, Equals( true ) );
    }

    It(calls_the_restructure_function_only_once_when_resized_and_moved)
    {
      window->move_and_resize( { 543, 234 }, { 543, 234 } );
      AssertThat( number_of_restructure_calls, Equals( 1u ) );
    }

    the::ui::Window::Pointer window;
    const the::ui::Window::Coordinate top_left{0, 0};
    const the::ui::Size size{ 100, 200 };
    bool was_restructure_called;
    size_t number_of_restructure_calls;
  };

  Describe( event_dispatch )
  {
    void add_some_children()
    {
      for ( int i( 0 ); i < 5; ++i )
      {
        the::ui::Window& child( window->add_child( std::make_unique< test::Window >() ) );
        for ( int i( 0 ); i < 2; ++i )
        {
          child.add_child( std::make_unique< test::Window >() );
        }
      }
    }

    void SetUp()
    {
      window = std::make_unique< test::Window >();
      add_some_children();
    }

    It( can_dispatch_an_event_to_a_single_window )
    {
      window->dispatch( test::AnEvent() );
      AssertThat( window->was_event_dispatched, Equals( true ) );
    }

    It( can_broadcast_an_event_to_children )
    {
      window->broadcast( test::AnEvent() );
      for ( const auto& child : window->children() )
      {
        const test::Window& test_window( static_cast< const test::Window& >( *child ) );
        AssertThat( test_window.was_event_dispatched, Equals( true ) );
      }
    }

    It( dispatches_broadcasted_message_to_itself )
    {
      window->broadcast( test::AnEvent() );
      AssertThat( window->was_event_dispatched, Equals( true ) );
    }

    It( dispatches_broadcasted_message_to_the_grandchildren )
    {
      window->broadcast( test::AnEvent() );
      for ( const auto& child : window->children() )
      {
        for ( const auto& grandchild : child->children() )
        {
          const test::Window& test_window( static_cast< const test::Window& >( *grandchild ) );
          AssertThat( test_window.was_event_dispatched, Equals( true ) );
        }
      }
    }

    std::unique_ptr< test::Window > window;
  };

  Describe( resize_events )
  {
    void SetUp()
    {
      window = std::make_unique< test::Window >();
    }

    It( dispatches_resized_event_when_resized )
    {
      window->resize( { 123, 987 } );
      AssertThat( window->was_resized, Equals( true ) );
    }

    It( dispatches_resized_event_when_moved_and_resized )
    {
      window->move_and_resize( { 123, 987 }, { 123, 987 } );
      AssertThat( window->was_resized, Equals( true ) );
    }

    std::unique_ptr< test::Window > window;
  };

};

