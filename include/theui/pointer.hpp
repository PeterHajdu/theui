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
      auto me(std::find_if( std::begin(siblings),std::end(siblings),
          [current = m_window](const auto& sibling)
          {
            return current == sibling.get();
          } ));

      assert(me != std::end(siblings));

      auto next(me+1);
      if (std::end(siblings) == next)
      {
        next = std::begin(siblings);
      }

      m_window = next->get();
    }

    Window& selected() const
    {
      return *m_window;
    }

  private:
    Window* m_window;
};

}
}

