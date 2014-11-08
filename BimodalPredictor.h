#ifndef __BIMODAL_PREDICTOR_H__
#define __BIMODAL_PREDICTOR_H__

#include <BranchPredictor.h>
#include <CustomTypes.h>
#include <AddressDecoder.h>
#include <iostream>
#include <vector>

namespace BranchPrediction
{
  struct LocalHistoryCustomizer
  {
    LocalHistoryCustomizer(ui32 def_ = DEFAULT_LOCAL_HISTORY_COUNTER,
        ui32 max_ = MAX_LOCAL_HISTORY_COUNTER_VALUE,
        ui32 min_ = MIN_LOCAL_HISTORY_COUNTER_VALUE,
        ui32 thresh_ = LOCAL_HISTORY_COUNTER_THRESH)
      :def(def_),max(max_),min(min_),thresh(thresh_)
    {}
    const ui32 def;
    const ui32 max;
    const ui32 min;
    const ui32 thresh;
  };
  struct LocalHistoryCounter
  {
    ui8 _counter;
    LocalHistoryCounter(const LocalHistoryCustomizer customLH)
      :_customLH(customLH)
    {
      _counter = _customLH.def;
    }
    ui8 operator() () const {return _counter;}
    void operator++(i32) 
    {
      if(_counter < _customLH.max)
        _counter++;
    }
    void operator--(i32)
    {
      if(_counter > _customLH.min)
        _counter--;
    }
    private:
      const LocalHistoryCustomizer _customLH;
  };

  class BimodalPredictor : public BranchPredictor
  {
    public:
      BimodalPredictor(ui32 local_history_size, 
          const LocalHistoryCustomizer customLH = LocalHistoryCustomizer() )
        :_customLH(customLH)
        ,_local_history(1U<<local_history_size, LHCount(_customLH))
        ,_indexGen(local_history_size)
      {
        _local_size = local_history_size;
      
        name("BIMODAL");
      
        _num_registers = 1U<<_local_size;
      }

      // function to predict branch
      virtual bool predictCustom(ui32 pc, bool actual)
      {
        ui32 index = _indexGen(pc);
        bool predicted = (_local_history[index]() > _customLH.thresh);
        if (actual)
        {
          _local_history[index]++;
        }
        else
        {
          _local_history[index]--;
        }
        return predicted;
      }

      // number of local history registers
      ui32 numRegisters () const    { return _num_registers; }
      // contents of local history registers
      ui32 contents(ui32 index) { return _local_history[index](); }

    protected:
      // local branch history size
      ui32 _local_size;
      // number of local history registers
      ui32 _num_registers;
      
      const LocalHistoryCustomizer _customLH;
      // struct to mentain local history counters
      typedef LocalHistoryCounter LHCount;
      typedef std::vector<LHCount> LocalHistoryStore;
      // register file for local history counters
      LocalHistoryStore _local_history;

      // object to help extract index from PC
      const AddressDecoder _indexGen;

  };
};

#endif //__GSHARE_PREDICTOR_H__

