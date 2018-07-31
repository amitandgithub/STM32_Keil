//******************
// CLASS: HwButtonPoll
//
// DESCRIPTION:
//  This is the class for the push button interface with Microcontroller
//
// CREATED: 10-01-2018, by Amit Chaudhary
//
// FILE: $FILE_FNAME$
//

#include "GpioInput.hpp"

namespace Bsp
{
  
class HwButtonPoll : public GpioInput
{
public:
  typedef bool ButtonInput_t;
  HwButtonPoll();
  ~HwButtonPoll();
  virtual HwInit();
  virtual ButtonInput_t Read();
  
};

} // namespace Bsp