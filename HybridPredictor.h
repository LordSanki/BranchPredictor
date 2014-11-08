#ifndef __HYBRID_PREDICTOR_H__
#define __HYBRID_PREDICTOR_H__

#include <GSharePredictor.h>
#include <BimodalPredictor.h>
#include <CustomTypes.h>
#include <AddressDecoder.h>
#include <iostream>

namespace BranchPrediction
{
  class HybridPredictor : public BimodalPredictor
  {
    public:
      HybridPredictor(ui32 hybrid_chooser_history_size,
                      ui32 gshare_local_history_size,
                      ui32 gshare_global_history_size,
                      ui32 bimodal_local_history_size)
        :BimodalPredictor(hybrid_chooser_history_size, LocalHistoryCustomizer(1,3,0,1))
        ,_gshare_global_history(gshare_global_history_size, gshare_local_history_size)
        ,_gshare_local_history(1U<<gshare_local_history_size)
        ,_bimodal_local_history(1U<<bimodal_local_history_size)
        ,_gshare_index_gen(gshare_local_history_size)
        ,_bimodal_index_gen(bimodal_local_history_size)
      {
        name("CHOOSER");
      }

      // function to read data
      virtual bool predictCustom(ui32 pc, bool actual)
      {
        bool hybrid_prediction;
        ui32 bimodal_index = _bimodal_index_gen(pc);
        bool bimodal_prediction = _bimodal_local_history[bimodal_index];
        ui32 gshare_index = _gshare_global_history^(_gshare_index_gen(pc));
        bool gshare_prediction = _gshare_local_history[gshare_index];

        ui32 chooser_index = _indexGen(pc);
        bool choose_gshare = _local_history[chooser_index];
        if(choose_gshare)
        {
          hybrid_prediction = gshare_prediction;
          _gshare_local_history[gshare_index](actual);
        }
        else
        {
          hybrid_prediction = bimodal_prediction;
          _bimodal_local_history[bimodal_index](actual);
        }
        _gshare_global_history>>actual;

        if(gshare_prediction != bimodal_prediction)
        {
          _local_history[chooser_index](gshare_prediction == actual);
        }

        return hybrid_prediction;
      }
      ui32 bimodalContents(ui32 index) const {return _bimodal_local_history[index]();}
      ui32 gshareContents(ui32 index) const {return _gshare_local_history[index]();}
      ui32 bimodalSize() const {return _bimodal_local_history.size();}
      ui32 gshareSize() const {return _gshare_local_history.size();}

    protected:
      // global branch histor size
      ui32 _global_size;

      GHCount _gshare_global_history;
      LocalHistoryStore _gshare_local_history;
      LocalHistoryStore _bimodal_local_history;
      const AddressDecoder _gshare_index_gen;
      const AddressDecoder _bimodal_index_gen;
      const LocalHistoryCustomizer _default_customizer;
  };
};

#endif //__HYBRID_PREDICTOR_H__

