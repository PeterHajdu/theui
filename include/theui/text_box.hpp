#pragma once

#include <theui/window.hpp>

#include <string>

namespace the
{
namespace ui
{
using CharSize = Size;

template < typename Text >
class TextBox: public Window
{
  public:
    using Texts = typename Text::Container;
    TextBox(
        const Texts& content,
        const Window::Coordinate& top_left,
        const Window::Size& window_size,
        const CharSize& char_size)
      : Window(top_left, window_size)
      , m_lines( split_up( content ) )
    {
    }

    using Line = Texts;
    using Lines = std::vector< Texts >;
    const Lines& lines() const
    {
      return m_lines;
    }

  private:
    Lines split_up( const Texts& content )
    {
      Lines lines;
      Line next_line;
      int current_width{ 0 };
      for ( const auto& word : content )
      {
        int next_width( current_width + word.width() );
        if ( next_width <= size().width )
        {
          next_line.push_back( word );
          current_width = next_width;
          continue;
        }

        lines.push_back( next_line );
        Line( 1, word ).swap( next_line );
        current_width = word.width();
      }

      const bool has_leftover{ !next_line.empty() };
      if ( has_leftover )
      {
        lines.push_back( next_line );
      }

      return lines;
    }

    Lines m_lines;

};
}
}
