#pragma once

#include <theui/window.hpp>

namespace the
{
namespace ui
{

class Pointer
{
  public:
    Pointer(Window& window)
      : m_window(&window)
    {}

    void next()
    {
      const auto& siblings(m_window->parent()->children());
      auto next(find_selected(siblings)+1);


      if (std::end(siblings) == next)
      {
        next = std::begin(siblings);
      }

      m_window = next->get();
    }

    void previous()
    {
      const auto& siblings(m_window->parent()->children());
      auto selected(find_selected(siblings));

      auto previous(selected == std::begin(siblings) ?
          std::end(siblings) - 1 :
          selected - 1 );

      m_window = previous->get();
    }

    Window& selected() const
    {
      return *m_window;
    }

  private:
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

