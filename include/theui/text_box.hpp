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
        const Size& window_size )
      : Window(top_left, window_size)
    {
      set_content_and_resize_if_needed( content );
      m_dispatcher.register_listener< Resized >( [ this ]( const Resized& ){ handle_resize(); } );
    }

    using Line = Texts;
    using Lines = std::vector< Texts >;
    const Lines& lines() const
    {
      return m_lines;
    }

    void set_content( const Texts& content )
    {
      set_content_and_resize_if_needed( content );
    }

  private:
    void handle_resize()
    {
      Line collapsed_lines;
      for ( const auto& line : m_lines )
      {
        std::copy(
            std::begin( line ), std::end( line ),
            std::back_inserter( collapsed_lines ) );
      }
      set_content_and_resize_if_needed( collapsed_lines );
    }


    void set_content_and_resize_if_needed( const Texts& content )
    {
      const auto previous_number_of_lines( m_lines.size() );
      split_up( content ).swap( m_lines );

      if ( previous_number_of_lines != m_lines.size() )
      {
        update_height_to_fit_text();
      }
    }


    void update_height_to_fit_text()
    {
      const int needed_height(
          m_lines.back().back().height() * m_lines.size() );
      const int keep_width( size().width );
      resize( { keep_width, needed_height } );
    }


    //todo: extract to function outside of TextBox
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
