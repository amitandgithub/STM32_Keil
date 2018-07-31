

#include"Page.hpp"
#include <cstring>
#include<stdio.h>

namespace App
{
   
Page::Page()
{
  std::memset(&PageText[0],'.',SIZE_OF_PAGE);
}


void Page::SetPageText(unsigned char Line, unsigned char Col, const char* pText, unsigned char Len)
{
  std::memcpy(&PageText[ ((Line % NO_OF_LINES_IN_PAGE) * NO_OF_CHARS_IN_LINE) + (Col%NO_OF_CHARS_IN_LINE) ],pText, Len%((unsigned char)(NO_OF_CHARS_IN_LINE-Col)) );
  //                       (Lines 0 - 5)                                          ( Col 0 - 11)                     ( Len 0-11 )
}

void Page::SetPageText(const char* pText)
{
  if(pText)
  {
      std::memcpy(&PageText[0],pText, SIZE_OF_PAGE );
  }
}


}
