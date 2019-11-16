#include <KOMO/komo.h>

//===========================================================================

void tutorialBasics(){
  rai::Configuration C("model.g");

  KOMO komo;
  /* there are essentially three things that KOMO needs to be specified:
   * 1) the kinematic model
   * 2) the timing parameters (duration/phases, number os time slices per phase)
   * 3) the tasks */

  //-- setting the model; false -> NOT calling collision detection (SWIFT) -> faster
  komo.setModel(C, false);

  //-- the timing parameters: 2 phases, 20 time slices, 5 seconds, k=2 (acceleration mode)
  komo.setTiming(2, 20, 5., 2);

  //-- default tasks for transition costs
  komo.setSquaredQAccVelHoming();
  komo.setSquaredQuaternionNorms(-1., -1., 1e1); //when the kinematics includes quaternion joints, keep them roughly regularized

  //-- simple tasks, called low-level

  //in phase-time [1,\infty] position-difference between "endeff" and "target" shall be zero (eq objective)
  komo.addObjective({1.,-1.}, FS_positionDiff, {"endeff", "target"}, OT_eq, {1e0});

  //in phase-time [1,\infty] quaternion-difference between "endeff" and "target" shall be zero (eq objective)
  komo.addObjective({1., -1.}, FS_quaternionDiff, {"endeff", "target"}, OT_eq, {1e1});
  //I don't always recommend setting quaternion tasks! This is only for testing here. As an alternative, one can use alignment tasks as in test/KOMO/komo

  //slow down around phase-time 1. (not measured in seconds, but phase)
  komo.setSlow(1., -1., 1e1);

  //-- call the optimizer
  komo.reset();
  komo.run();
  //  komo.checkGradients(); //this checks all gradients of the problem by finite difference
  komo.getReport(true); //true -> plot the cost curves
  for(uint i=0;i<2;i++) komo.displayTrajectory(.1, true); //play the trajectory

  /* next step:
   *
   * Have a look at all the other set*** methods, which all add tasks to the KOMO problem. Look into
   * their implementation: they mainly just call setTask(..) with various TaskMaps.
   *
   * The last three arguments of setTask are important:
   *
   * type allows to define whether this is a sumOfSqr, equality, or inequality task
   *
   * target defines the target value in the task space; {} is interpreted as the zero vector
   *
   * order=0 means that the task is about the position(absolute value) in task space
   * order=1 means that the task is about the velocity in task space
   * order=2 means that the task is about the acceleration in task space
   *
   * For instance, setSquaredQAccelerations sets a tasks about the acceleration in the identity map
   *
   * Next, perhaps learn about all the available taskMaps, or implement new differentiable MappingSuccess
   *
   */
}

//===========================================================================

void tutorialInverseKinematics(){
  /* The only difference is that the timing parameters are set differently and the tranision
   * costs need to be velocities (which is just sumOfSqr of the difference to initialization).
   * All tasks should refer to phase-time 1. Internally, the system still created a banded-diagonal
   * Hessian representation, which is some overhead. It then calles exactly the same constrained optimizers */

  rai::Configuration C("model.g");

  KOMO komo;
  komo.setModel(C, false);

  //-- the timing parameters: 1 phase, 1 time slice
  komo.setTiming(1, 1);

  //-- default tasks for transition costs
  komo.setSquaredQAccVelHoming(1., -1., 0., 1., 1e-2);
  komo.setSquaredQuaternionNorms(-1., -1., 1e3); //when the kinematics includes quaternion joints, keep them roughly regularized

  //-- simple tasks, called low-level
  komo.addObjective({}, FS_positionDiff, {"endeff", "target"}, OT_eq, {1e0});
  komo.addObjective({}, FS_quaternionDiff, {"endeff", "target"}, OT_eq, {1e1});

  //-- call the optimizer
  komo.reset();
  komo.run();
  //  komo.checkGradients(); //this checks all gradients of the problem by finite difference
  komo.getReport(); //true -> plot the cost curves
  for(uint i=0;i<2;i++) komo.displayTrajectory(.1, true); //play the trajectory
}

//===========================================================================

int main(int argc,char** argv){
  rai::initCmdLine(argc,argv);

  tutorialBasics();

  tutorialInverseKinematics();

  return 0;
}
