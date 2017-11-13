#include <Kin/kin.h>
#include <Kin/frame.h>
#include <Gui/opengl.h>
#include <KOMO/komo.h>

//===========================================================================

void TEST(Easy){
  mlr::KinematicWorld K("arm.g");
  cout <<"configuration space dim=" <<K.q.N <<endl;
  KOMO komo;
  komo.setModel(K);
  komo.setPathOpt(1., 100, 5.);

  komo.setPosition(1., 1., "endeff", "target", OT_sumOfSqr);
  komo.setSlowAround(1., .02);
  komo.setCollisions(false);

  komo.reset();
//  komo.setSpline(5);
  komo.run();
//  komo.checkGradients();
  komo.plotTrajectory();
  komo.getReport(true);
  for(uint i=0;i<2;i++) komo.displayTrajectory();
}

//===========================================================================

void TEST(Align){
  mlr::KinematicWorld K("arm.g");
  cout <<"configuration space dim=" <<K.q.N <<endl;
  KOMO komo;
  komo.setModel(K);
  komo.setPathOpt(1., 100, 5.);

  komo.setPosition(1., 1., "endeff", "target");
  komo.setOrientation(1., 1., "endeff", "target", OT_eq);
  komo.setSlowAround(1., .02);
  komo.setCollisions(true);

  komo.reset();
  komo.run();
  komo.plotTrajectory();
  komo.getReport(true);
  for(uint i=0;i<2;i++) komo.displayTrajectory();
}

//===========================================================================

void TEST(PR2){
  //NOTE: this uses a 25-DOF whole-body-motion model of thbe PR2
  mlr::KinematicWorld K("model.g");
  K.optimizeTree();
  makeConvexHulls(K.frames);
  K.calc_fwdPropagateFrames();
  cout <<"configuration space dim=" <<K.q.N <<endl;
  double rand = mlr::getParameter<double>("KOMO/moveTo/randomizeInitialPose", .0);
  if(rand){
    rnd.seed(mlr::getParameter<uint>("rndSeed", 0));
    rndGauss(K.q,rand,true);
    K.setJointState(K.q);
  }

  KOMO komo;
  komo.setModel(K);
  komo.setPathOpt(1., 100, 10.);
  komo.setPosition(1., 1., "endeff", "target");
  komo.setSlowAround(1., .02);
  komo.setCollisions(false);

  komo.reset();
//  komo.setSpline(10);
  komo.run();
  komo.plotTrajectory();
  komo.getReport(true);
  for(uint i=0;i<2;i++) komo.displayTrajectory();
}

//===========================================================================

void TEST(FinalPosePR2){
  mlr::KinematicWorld K("model.g");
  K.optimizeTree();
  makeConvexHulls(K.frames);
  cout <<"configuration space dim=" <<K.q.N <<endl;
  arr x = finalPoseTo(K, *K.getFrameByName("endeff"), *K.getFrameByName("target"));
  K.setJointState(x.reshape(x.N));
  K.gl().watch();
}

//===========================================================================

int main(int argc,char** argv){
  mlr::initCmdLine(argc,argv);

  testEasy();
  testAlign();
  testPR2();

  return 0;
}


