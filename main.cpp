#include <cstring>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <Cache.h>
#include <MainMemory.h>
#include <ResultGenerator.h>
#include <TraceReader.h>
#include <BimodalPredictor.h>
#include <GSharePredictor.h>
#include <HybridPredictor.h>

using namespace std;
using namespace CacheSimulator;
using namespace BranchPrediction;

int main(int argc, char **argv)
{
  if(    (argc < 6)
      || ((strcmp(argv[1],"bimodal") == 0) && (argc != 6))
      || ((strcmp(argv[1],"gshare") == 0)  && (argc != 7))
      || ((strcmp(argv[1],"hybrid") == 0)  && (argc != 9))
    )
  {
    cout<<"invalid number of arguments"<<endl;
    cout<<"sim bimodal <M2> <BTB_SIZE> <BTB_ASSOC> <TRACE_FILE>"<<endl;
    cout<<"sim gshare <M1> <N> <BTB_SIZE> <BTB_ASSOC> <TRACE_FILE>"<<endl;
    cout<<"sim hybrid <K> <M1> <N> <M2> <BTB_SIZE> <BTB_ASSOC> <TRACE_FILE>"<<endl;
    return -1;
  }

  ui16 btb_size       = atoi(argv[argc -3]);//argv[e_L1_SIZE]);
  ui16 btb_assoc      = atoi(argv[argc -2]);//argv[e_L1_ASSOC]);
  ui16 M1=0,M2=0,N=0,K=0;
 
  if(strcmp(argv[1],"bimodal") == 0)
  {
    M2 = atoi(argv[2]);
  }
  if(strcmp(argv[1],"gshare") == 0)
  {
    M1 = atoi(argv[2]);
    N  = atoi(argv[3]);
  }
  if(strcmp(argv[1],"hybrid") == 0)
  {
    K  = atoi(argv[2]);
    M1 = atoi(argv[3]);
    N  = atoi(argv[4]);
    M2 = atoi(argv[5]);
  }

  try
  {
    BranchPredictor *gshare = new GSharePredictor(M1,N);
    BranchPredictor *bimodal = new BimodalPredictor(M2);
    BranchPredictor *hybrid = new HybridPredictor(K,M1,N,M2);

    BranchPredictor *p = NULL;
    if(strcmp(argv[1],"bimodal") == 0) p = bimodal;
    if(strcmp(argv[1],"gshare") == 0)  p = gshare;
    if(strcmp(argv[1],"hybrid") == 0)  p = hybrid;
    
    BTB *btb = new BTB(p, btb_size, btb_assoc);

    ResultGenerator rGen(argc, argv);
    for(TraceReader tReader(argv[argc-1]); tReader; tReader++)
    {
      tReader>>btb;
    }
    if(btb_size > 0)
      rGen<<btb;
    else
      rGen<<p;
    delete bimodal;
    delete gshare;
    delete hybrid;
    delete btb;
  }
  catch(const exception &e)
  {
    cout<<e.what()<<endl;
  }


  return 0;
}

