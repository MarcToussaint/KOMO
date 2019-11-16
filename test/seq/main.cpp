#include <KOMO/komo-ext.h>

//===========================================================================

void TEST(KomoSequence){
  
  rai::Configuration K("model.g");
  K.optimizeTree(false);
  makeConvexHulls(K.frames);

  KOMO_ext komo;
  komo.setModel(K);
  komo.setPathOpt(2., 20, 10.);

  komo.setSquaredQAccVelHoming();

  komo.setGrasp(1., "humanR", "Long1");
  komo.setPlace(1.8, "humanR", "Long1", "tableL");
  komo.setSlowAround(1., .1, 1e3);

  komo.setGrasp(1., "humanL", "Long2");
  komo.setPlace(1.8, "humanL", "Long2", "tableR");

  komo.reset();
  komo.run();

  Graph result = komo.getReport(true);

  for(uint i=0;i<2;i++) if(!komo.displayTrajectory(.1, true)) break;
}

//===========================================================================

int main(int argc,char** argv){
  rai::initCmdLine(argc,argv);

  testKomoSequence();

  return 0;
}

