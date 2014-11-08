#ifndef __MAIN_MEMORY_H__
#define __MAIN_MEMORY_H__

#include <Memory.h>
#include <CustomTypes.h>
#include <iostream>
namespace CacheSimulator
{
  class MainMemory: public Memory
  {
    public:
      MainMemory()
      {
        _next = NULL;
        name("MainMemory");
        _valid = true;
      }
      ~MainMemory() 
      {
      }
      //function to initialize memory
      void initC()
      {
      }
      // function to read data
      bool readC(ui32 addr)
      {
        return true;
      }
      // function to write data
      bool writeC(ui32 addr, ui8 data)
      {
        return true;
      }
  };
};

#endif //__MAIN_MEMORY_H__

