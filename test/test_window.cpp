#include <theui/window.hpp>
#include <thectci/id.hpp>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

class AnEvent
{
  public:
    add_ctci( "test_an_event" );
};

class Window : public the::ui::Window
{
  public:
    Window()
    {
      m_dispatcher.register_listener< AnEvent >(
          [ this ]( const AnEvent& event )
          {
            handle_event( event );
          } );
    }

    bool was_event_dispatched{ false };
    void handle_event( const AnEvent& )
    {
      was_event_dispatched = true;
    }
};

}

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
    void SetUp()
    {
      window = std::make_unique<the::ui::Window>( top_left, size );
    }

    void assert_top_left_is( const the::ui::Window::Coordinate& expected_top_left )
    {
      AssertThat( window->top_left().x, Equals( expected_top_left.x ) );
      AssertThat( window->top_left().y, Equals( expected_top_left.y ) );
    }

    void assert_size_is( const the::ui::Window::Size& expected_size )
    {
      AssertThat( window->size().width, Equals( expected_size.width ) );
      AssertThat( window->size().height, Equals( expected_size.height ) );
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
    const the::ui::Window::Size size{ 100, 200 };
    const the::ui::Window::Size another_size{ 101, 201 };
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

    It(calls_the_restructure_function_after_child_creation)
    {
      window->create_child();
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
    const the::ui::Window::Size size{ 100, 200 };
    bool was_restructure_called;
    size_t number_of_restructure_calls;
  };

  Describe( event_dispatch )
  {
    void SetUp()
    {
      window = std::make_unique< test::Window >();
    }

    It( can_dispatch_an_event_to_a_single_window )
    {
      window->dispatch( test::AnEvent() );
      AssertThat( window->was_event_dispatched, Equals( true ) );
    }

    std::unique_ptr< test::Window > window;
  };

};

