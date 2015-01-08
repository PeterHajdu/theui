#include <theui/text_box.hpp>

#include "lorem_ipsum.hpp"

#include <memory>

#include <igloo/igloo.h>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

class Text
{
  public:
    using Container = std::vector< Text >;
    Text() = default;
    Text( const char* word )
      : word( word )
    {
    }

    int width() const
    {
      return word.size();
    }

    std::string word;
};

bool operator==( const Text& a, const Text& b )
{
  return a.word == b.word;
}

std::ostream& operator<<( std::ostream& output, const Text& input )
{
  output << input.word;
  return output;
}

std::istream& operator>>( std::istream& input, Text& output )
{
  input >> output.word;
  output.word.append( " " );
  return input;
}

Text::Container tokenize( const char * text )
{
  std::stringstream stream( text );
  return Text::Container(
      std::istream_iterator< Text >( stream ),
      std::istream_iterator< Text >() );
}

Text to_token( const char* token )
{
  Text text( token );
  text.word += " ";
  return text;
}

using TextBox = the::ui::TextBox< Text >;
}

Describe( a_text_box )
{
  void SetUp()
  {
    text_box = std::make_unique<test::TextBox>(
        content,
        top_left,
        size );
  }

  It(is_a_window)
  {
    AssertThat(dynamic_cast<the::ui::Window*>(text_box.get()) != nullptr, Equals(true));
  }

  It(sets_the_correct_window_size_and_coordinate)
  {
    AssertThat(text_box->top_left().x, Equals(top_left.x));
    AssertThat(text_box->top_left().y, Equals(top_left.y));
    AssertThat(text_box->size().width, Equals(size.width));
    AssertThat(text_box->size().height, Equals(size.height));
  }

  It(splits_up_text_in_the_first_line)
  {
    const auto& first_line( text_box->lines().front() );
    AssertThat( first_line, Contains( test::to_token( "Lorem" ) ) );
    AssertThat( first_line, Contains( test::to_token( "ipsum" ) ) );
    AssertThat( first_line, !Contains( test::to_token( "dolor" ) ) );
  }

  It(splits_up_text_in_the_following_lines)
  {
    const auto& second_line( text_box->lines()[ 1 ] );
    AssertThat( second_line, Contains( test::to_token( "dolor" ) ) );
    AssertThat( second_line, Contains( test::to_token( "sit" ) ) );
    AssertThat( second_line, Contains( test::to_token( "amet," ) ) );
  }

  It(splits_up_text_in_the_last_line)
  {
    const auto& last_line( text_box->lines().back() );
    AssertThat( last_line, Contains( test::to_token( "laborum." ) ) );
  }

  It(splits_up_text_after_being_resized)
  {
    text_box->resize( new_size );
    const auto& first_line( text_box->lines().front() );
    AssertThat( first_line, Contains( test::to_token( "Lorem" ) ) );
    AssertThat( first_line, Contains( test::to_token( "ipsum" ) ) );
    AssertThat( first_line, Contains( test::to_token( "dolor" ) ) );
    AssertThat( first_line, Contains( test::to_token( "sit" ) ) );
    AssertThat( first_line, !Contains( test::to_token( "amet," ) ) );
  }

  It(lets_see)
  {
    text_box->resize( { 40, 200 } );
    std::cout << std::endl;
    for ( const auto& line : text_box->lines() )
    {
      for ( const auto& word : line )
      {
        std::cout << word;
      }
      std::cout << std::endl;
    }
  }

  std::unique_ptr<test::TextBox> text_box;
  const the::ui::Window::Coordinate top_left{70, 75};
  const the::ui::Size size{ 16 , 200 };
  const the::ui::Size new_size{ 22 , 200 };
  const test::Text::Container content{ test::tokenize( test::lorem_ipsum ) };
};

