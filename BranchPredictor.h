#ifndef __BRANCH_PREDICTOR_H__
#define __BRANCH_PREDICTOR_H__

#include <string>
#include <CustomTypes.h>

namespace BranchPrediction
{
  class BranchPredictor
  {
    public:
      // initializing function setting read and write counter to 0
      void BranchPredictor ()
      {
          _mispredictions=0;
          _num_branches=0;
      }

      // function to read data
      bool predict(ui32 pc, bool actual)
      {
        _num_branches++;
        bool predicted = predictCustom(pc,actual);
        if (predicted != actual)
        {
          _mispredictions++;
        }
        return predicted;
      }

      // number of local history registers
      ui32 numRegisters () const    { return _num_registers; }
      // number of branches
      ui32 numBranches () const { return _num_branches; }
      // getter for name
      std::string name () const { return _name; }
      // number of mispredictions      
      ui32 mispredictions () const  { return _mispredictions; }
      // contents of local history registers
      ui32 contents(ui32 index) { return _local_history[index](); }

    protected:
      //flag to store if cache configuration is valid
      bool predictCustom(ui32 pc, bool actual) = 0;
      // protected setter for name
      void name(std::string n) {_name = n;}

    private:
      // counter for tracking reads
      ui32 _mispredictions;
      // counter for tracking writes
      ui32 _num_branches;
      // name of predictor
      std::string _name
  };
};

#endif //__BRANCH_PREDICTOR_H__

