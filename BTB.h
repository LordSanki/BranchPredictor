#ifndef __BTB_H__
#define __BTB_H__
#include <CustomTypes.h>
#include <Cache.h>
#include <MainMemory.h>
#include <ReplacementPolicy.h>
#include <BranchPredictor.h>
#include <cstdio>
namespace BranchPrediction
{
class BTB
{
  public:
    BTB(BranchPredictor *p, ui32 btb_size, ui32 btb_assoc)
    {
      if(btb_size >0)
      {
        _btb = new CacheSimulator::Cache(4, btb_size, btb_assoc,
            CacheSimulator::ReplacementPolicy::e_LRU, CacheSimulator::e_WBWA, new CacheSimulator::MainMemory());
        _btb->init(); 
        _btb->name("BTB");
      }
      else
      {
        _btb = NULL;
      }
      _p = p;
      _btb_forced_miss = 0;
    }

    bool read(ui32 addr, bool taken)
    {
      if(_btb)
      {
        if(_btb->read(addr))
        {
          return _p->predict(addr,taken);
        }
        else
        {
          if(taken)
            _btb_forced_miss++;
          return false;
        }
      }
      else
      {
        return _p->predict(addr,taken);
      }
    }
    CacheSimulator::Cache * cache() {return _btb;}
    BranchPredictor * predictor() {return _p;}
    ui32 btbForcedMisses() {return _btb_forced_miss;}
  private:
    CacheSimulator::Cache *_btb;
    BranchPredictor *_p;
    ui32 _btb_forced_miss;
};
};
#endif //__BTB_H__

