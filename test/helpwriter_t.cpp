// Copyright (c) 2018, 2019 Marko Mahnič
// License: MIT. See LICENSE in the root of the project.

#include "../src/argparser.h"
#include "../src/helpformatter.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>

using namespace argparse;

namespace {
std::vector<std::string_view> splitLines( std::string_view text )
{
    std::vector<std::string_view> output;
    size_t start = 0;
    auto delims = "\n\r";

    auto isWinEol = [&text]( auto pos ) {
       return text[pos] == '\r' && text[pos+1] == '\n';
    };

    while ( start < text.size() ) {
       const auto stop = text.find_first_of( delims, start );

       if ( start != stop )
          output.emplace_back( text.substr( start, stop-start ) );

       if ( stop == std::string_view::npos )
          break;

       start = stop + ( isWinEol( stop ) ? 2 : 1 );
    }

    return output;
}
}

namespace {
std::string loremIpsum123_19w =
   "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
   "do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
}

TEST( WriterTest, shouldSplitTextIntoWordsAtWhitespace )
{
   std::string text = loremIpsum123_19w;
   std::stringstream strout;
   Writer writer( strout );
   auto words = writer.splitIntoWords( text );

   EXPECT_EQ( 19, words.size() );
}

TEST( WriterTest, shouldReformatText )
{
   std::string text = loremIpsum123_19w;
   std::stringstream strout;
   Writer writer( strout, 27 );

   writer.write( text );
   auto written = strout.str();
   auto lines = splitLines( written );

   EXPECT_LT( 4, lines.size() );

   for ( auto line : lines )
      EXPECT_GE( 27, line.size() );
}

TEST( WriterTest, shouldIndentFormattedText )
{
   std::string text = loremIpsum123_19w;
   std::stringstream strout;
   Writer writer( strout, 27 );
   writer.setIndent( 3 );

   writer.write( text );
   auto written = strout.str();
   auto lines = splitLines( written );

   EXPECT_LT( 4, lines.size() );

   for ( auto line : lines )
      EXPECT_GE( 27, line.size() );

   for ( auto line : lines ) {
      EXPECT_EQ( "   ", line.substr(0, 3) );
      EXPECT_NE( "    ", line.substr(0, 4) );
   }
}