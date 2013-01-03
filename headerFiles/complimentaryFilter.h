#include "headerFiles/pVector.h"
#include "headerFiles/simpsonIteration.h" //we'll see what this changes to


pVector complimentaryFilter(pVector accel, pVector gyrot0, pVector gyrot1, pVector gyrot2, double t0, double t1, double t2, double ALPHA, pVector anglet1)
{
  /*current time step t2 (so T=t-1 is t1 and T=t-2 is t0)*/
  pVector anglet2;
  anglet2.set(X, accel.component(X)*(1-ALPHA)+ALPHA*(anglet1+simpsonIteration(gyrot0.component(X),gyrot1.component(X),gyrot2.component(X),t0,t1,t2)));
  anglet2.set(Y, accel.component(Y)*(1-ALPHA)+ALPHA*(anglet1+simpsonIteration(gyrot0.component(Y),gyrot1.component(Y),gyrot2.component(Y),t0,t1,t2)));
  anglet2.set(Z, accel.component(Z)*(1-ALPHA)+ALPHA*(anglet1+simpsonIteration(gyrot0.component(Z),gyrot1.component(Z),gyrot2.component(Z),t0,t1,t2)));
  return anglet2;
}
