#ifndef __GSHARE_PREDICTOR_H__
#define __GSHARE_PREDICTOR_H__

#include <string>
#include <CustomTypes.h>
#include <AddressDecoder.h>
#include <iostream>

namespace BranchPrediction
{
  class GSharePredictor
  {
    public:
      GSharePredictor(ui32 local_history_size, ui32 global_history_size=0)
        :_global_history(global_history_size, local_history_size)
        ,_indexGen(local_history_size)
      {
        _local_size = local_history_size;
        _global_size = global_history_size;
      }
      // initializing function setting read and write counter to 0
      void init ()
      {
          _mispredictions=0;
          _num_branches=0;
          _valid = true;
          // populating the local history register file
          _num_registers = 1U<<_local_size;
          for(ui32 i=0; i<_num_registers; i++)
            _local_history.push_back(LHCount());
      }

      // function to read data
      bool predict(ui32 pc, bool actual)
      {
        _num_branches++;
        ui32 index = _indexGen(pc);
        //DO(index);
        index = _global_history^index;
        //DO(index);
        bool predicted = (_local_history[index]() > LOCAL_HISTORY_COUNTER_THRESH);
        //DO(_local_history[index]());
        if (predicted != actual)
        {
          _mispredictions++;
        }
        _global_history>>actual;
        if (actual)
        {
          _local_history[index]++;
        }
        else
        {
          _local_history[index]--;
        }
        //DO(_local_history[index]());
        //DO(_global_history());
        return predicted;
      }

      // number of local history registers
      ui32 numRegisters () const    { return _num_registers; }
      // number of branches
      ui32 numBranches () const { return _num_branches; }
      // getter for name
      std::string name () const { return _name; }
      // setter for name
      void name (std::string n) { _name = n;    }
      // validity check
      operator bool()           { return _valid;}
      // number of mispredictions      
      ui32 mispredictions () const  { return _mispredictions; }
      // contents of local history registers
      ui32 contents(ui32 index) { return _local_history[index](); }

    protected:
      //flag to store if cache configuration is valid
      bool _valid;

    private:
      // counter for tracking reads
      ui32 _mispredictions;
      // counter for tracking writes
      ui32 _num_branches;
      // name of memory
      std::string _name;
      // local branch history size
      ui32 _local_size;
      // number of local history registers
      ui32 _num_registers;
      // global branch histor size
      ui32 _global_size;
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

        private:
          const AddressDecoder _addrGen;
      };
      typedef GlobalHistoryCounter GHCount;

      // counter for storing global history
      GlobalHistoryCounter _global_history;

      // object to help extract index from PC
      const AddressDecoder _indexGen;

  };
};

#endif //__GSHARE_PREDICTOR_H__

