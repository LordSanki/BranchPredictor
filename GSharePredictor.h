#ifndef __GSHARE_PREDICTOR_H__
#define __GSHARE_PREDICTOR_H__

#include <BimodalPredictor.h>
#include <CustomTypes.h>
#include <AddressDecoder.h>
#include <iostream>

namespace BranchPrediction
{
  struct GlobalHistoryCounter
  {
    GlobalHistoryCounter(ui32 global_size, ui32 local_size)
    {
      _counter = DEFAULT_GLOBAL_HISTORY_COUNTER;
      _validation_mask = 0U;
      for(ui32 i=0;i<local_size;i++)
        _validation_mask = (_validation_mask<<1U) +1U;
      _shift = local_size-global_size;
      _left_shift_mask=0U;
      for(ui32 i=0;i<global_size;i++)
        _left_shift_mask = (_left_shift_mask<<1U) +1U;

      _right_shift_mask = (global_size>0)?1U<<(global_size-1):0U;
    }
    ui32 _counter;
    ui32 _shift;
    ui32 _left_shift_mask;
    ui32 _right_shift_mask;
    ui32 _validation_mask;
    ui32 operator^(ui32 pc) const
    {
      ui32 mask = _counter<<_shift;
      return ((pc^mask)&_validation_mask);
    }
    void operator<<(bool cond)
    {
      _counter = (_counter<<1U)&_left_shift_mask;
      if(cond) _counter++;
    }
    void operator>>(bool cond)
    {
      _counter = (_counter>>1U);
      if(cond) _counter = _counter|_right_shift_mask;
    }
    ui32 operator() () {return _counter;}
  };
  typedef GlobalHistoryCounter GHCount;

  class GSharePredictor : public BimodalPredictor
  {
    public:
      GSharePredictor(ui32 local_history_size, ui32 global_history_size=0,
          const LocalHistoryCustomizer customLH = LocalHistoryCustomizer())
        :BimodalPredictor(local_history_size, customLH)
        ,_global_history(global_history_size, local_history_size)
      {
        _global_size = global_history_size;
        name("GSHARE");
      }

      // function to read data
      virtual bool predictCustom(ui32 pc, bool actual)
      {
        ui32 index = _indexGen(pc);
        index = _global_history^index;
        bool predicted = _local_history[index];
        _global_history>>actual;
        _local_history[index](actual);
        return predicted;
      }

    protected:
      // global branch histor size
      ui32 _global_size;


      // counter for storing global history
      GlobalHistoryCounter _global_history;

  };
};

#endif //__GSHARE_PREDICTOR_H__

