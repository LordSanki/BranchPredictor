#ifndef __BIMODAL_PREDICTOR_H__
#define __BIMODAL_PREDICTOR_H__

#include <BranchPredictor.h>
#include <CustomTypes.h>
#include <AddressDecoder.h>
#include <iostream>

namespace BranchPrediction
{
  class BimodalPredictor : public BranchPredictor
  {
    public:
      BimodalPredictor(ui32 local_history_size)
        :_indexGen(local_history_size)
      {
        _local_size = local_history_size;
      
        name("BIMODAL");
      
        _num_registers = 1U<<_local_size;
        // populating the local history register file
        for(ui32 i=0; i<_num_registers; i++)
          _local_history.push_back(LHCount());
      }

      // function to predict branch
      virtual bool predictCustom(ui32 pc, bool actual)
      {
        ui32 index = _indexGen(pc);
        bool predicted = (_local_history[index]() > LOCAL_HISTORY_COUNTER_THRESH);
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
      // struct to mentain local history counters
      struct LocalHistoryCounter
      {
        ui8 _counter;
        LocalHistoryCounter() {_counter = DEFAULT_LOCAL_HISTORY_COUNTER;}
        ui8 operator() () const {return _counter;}
        void operator++(i32) 
        {
          if(_counter < MAX_LOCAL_HISTORY_COUNTER_VALUE)
            _counter++;
        }
        void operator--(i32)
        {
          if(_counter > MIN_LOCAL_HISTORY_COUNTER_VALUE)
            _counter--;
        }
      };
      typedef LocalHistoryCounter LHCount;
      typedef std::vector<LHCount> LocalHistoryStore;
      // register file for local history counters
      LocalHistoryStore _local_history;

      // object to help extract index from PC
      const AddressDecoder _indexGen;

  };
};

#endif //__GSHARE_PREDICTOR_H__

