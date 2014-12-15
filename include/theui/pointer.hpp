#pragma once

#include <theui/window.hpp>
#include <thectci/id.hpp>

namespace the
{
namespace ui
{

class Arrive
{
  public:
    add_ctci( "the_ui_arrive" );
};

class Leave
{
  public:
    add_ctci( "the_ui_leave" );
};

class Pointer
{
  public:
    Pointer(Window& window)
    {
      arrive_to( &window );
    }

    void next()
    {
      const auto& siblings(m_window->parent()->children());
      auto next(find_selected(siblings)+1);


      if (std::end(siblings) == next)
      {
        next = std::begin(siblings);
      }

      arrive_to( next->get() );
    }

    void previous()
    {
      const auto& siblings(m_window->parent()->children());
      auto selected(find_selected(siblings));

      auto previous(selected == std::begin(siblings) ?
          std::end(siblings) - 1 :
          selected - 1 );

      arrive_to( previous->get() );
    }

    Window& selected() const
    {
      return *m_window;
    }

    void enter()
    {
      const auto first_child( std::begin( m_window->children() ) );
      if ( first_child == std::end( m_window->children() ) )
      {
        return;
      }

      arrive_to( first_child->get() );
    }

    void escape()
    {
      auto parent_window( m_window->parent() );
      if ( nullptr == parent_window )
      {
        return;
      }

      arrive_to( parent_window );
    }

  private:
    void arrive_to( the::ui::Window* new_window )
    {
      if ( m_window != nullptr )
      {
        m_window->dispatch( Leave() );
      }

      m_window = new_window;
      new_window->dispatch( Arrive() );
    }

    Window::Container::const_iterator find_selected(const Window::Container& siblings) const
    {
      auto selected (std::find_if( std::begin(siblings),std::end(siblings),
          [current = m_window](const auto& sibling)
          {
            return current == sibling.get();
          } ));

      assert(selected != std::end(siblings));

      return selected;
    }


    Window* m_window;
};

}
}

