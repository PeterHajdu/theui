#include <theui/text_box.hpp>
#include <theui/list_restructure.hpp>

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

    int height() const
    {
      return 2;
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

  It(sets_window_size_and_coordinate_passed_by_the_constructor_except_the_height )
  {
    AssertThat(text_box->top_left().x, Equals(top_left.x));
    AssertThat(text_box->top_left().y, Equals(top_left.y));
    AssertThat(text_box->size().width, Equals(size.width));
  }

  It(increases_its_height_to_be_able_to_hold_the_text_after_splitting_it_to_many_lines)
  {
    const int number_of_lines( text_box->lines().size() );
    AssertThat( text_box->size().height, Equals( number_of_lines * content.back().height() ) );
  }

  It(requests_a_restructure_after_height_increase)
  {
    const the::ui::Size thinner_parent_size{ size.width - 5, size.height };
    size_t number_of_restructure_calls( 0 );
    the::ui::Window parent(
        { 0, 0 },
        thinner_parent_size,
        [ &number_of_restructure_calls ]( const auto& parent )
        {
          the::ui::front_from_top_with_fixed_height( parent );
          ++number_of_restructure_calls;
        } );
    parent.add_child( std::move( text_box ) );
    AssertThat( number_of_restructure_calls, Equals( 2u ) );
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

  It(lets_you_assign_new_content)
  {
    const char * new_text( "new content" );
    const test::Text::Container new_content{ test::tokenize( new_text ) };
    text_box->set_content( new_content );

    const auto& first_line( text_box->lines().front() );
    AssertThat( first_line, Contains( test::to_token( "new" ) ) );
    AssertThat( first_line, Contains( test::to_token( "content" ) ) );
  }

  std::unique_ptr<test::TextBox> text_box;
  const the::ui::Window::Coordinate top_left{70, 75};
  const the::ui::Size size{ 16 , 2 };
  const the::ui::Size new_size{ 22 , 200 };
  const test::Text::Container content{ test::tokenize( test::lorem_ipsum ) };
};

