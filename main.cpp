#include <iostream>
#include <cstdlib>
#include <Cache.h>
#include <MainMemory.h>
#include <ResultGenerator.h>
#include <TraceReader.h>
#include <exception>
#include <GSharePredictor.h>
#include <cstring>

using namespace std;
using namespace CacheSimulator;
using namespace BranchPrediction;

int main(int argc, char **argv)
{
  if(    (argc < 6)
      || ((strcmp(argv[1],"bimodal") == 0) && (argc != 6))
      || ((strcmp(argv[1],"gshare") == 0)  && (argc != 7))
      || ((strcmp(argv[1],"hybrid") == 0)  && (argc != 8))
    )
  {
    cout<<"invalid number of arguments"<<endl;
    cout<<"sim bimodal <M2> <BTB_SIZE> <BTB_ASSOC> <TRACE_FILE>"<<endl;
    cout<<"sim gshare <M1> <N> <BTB_SIZE> <BTB_ASSOC> <TRACE_FILE>"<<endl;
    cout<<"sim hybrid <K> <M1> <N> <M2> <BTB_SIZE> <BTB_ASSOC> <TRACE_FILE>"<<endl;
    return -1;
  }

  ui16 block_size = 32;//atoi(argv[e_BLOCKSIZE]);
  ui16 btb_size       = atoi(argv[argc -3]);//argv[e_L1_SIZE]);
  ui16 btb_assoc      = atoi(argv[argc -2]);//argv[e_L1_ASSOC]);
  ui16 M1=0,M2=0,N=0,K=0;
 
  if(strcmp(argv[1],"bimodal") == 0) M2 = atoi(argv[2]);
  if(strcmp(argv[1],"gshare") == 0)  M1 = atoi(argv[2]);
  if(strcmp(argv[1],"gshare") == 0)  N  = atoi(argv[3]);
  if(strcmp(argv[1],"hybrid") == 0)  K  = atoi(argv[2]);
  if(strcmp(argv[1],"hybrid") == 0)  M1 = atoi(argv[3]);
  if(strcmp(argv[1],"hybrid") == 0)  N  = atoi(argv[4]);
  if(strcmp(argv[1],"hybrid") == 0)  M2 = atoi(argv[5]);

  ReplacementPolicy::Types rP  = ReplacementPolicy::e_LRU;
  WritePolicy wP        = e_WBWA;

  try
  {
//    Cache *btb = new Cache( block_size, btb_size, btb_assoc, rP, wP, (Memory*)new MainMemory());
//    btb->init(); 
//    btb->name("L1");

    BranchPredictor *p1 = new GSharePredictor(M1,N);
    BranchPredictor *p2 = new BimodalPredictor(M2);
//    GSharePredictor *h1 = new HybridPredictor(K,p1,p2);

//    h1->init();
//    h1->name("CHOOSER");

    BranchPredictor *p = NULL;
    if(strcmp(argv[1],"bimodal") == 0) p = p2;
    if(strcmp(argv[1],"gshare") == 0)  p = p1;
//    if(strcmp(argv[1],"hybrid") == 0)  p = h1;
    ResultGenerator rGen(argc, argv);
    for(TraceReader tReader(argv[argc-1]); tReader; tReader++)
    {
      tReader>>p;
    }
    rGen<<p;
    delete p1;
    delete p2;
//    delete h1;
//    delete btb;

  }
  catch(const exception &e)
  {
    cout<<e.what()<<endl;
  }


  return 0;
}

