#ifndef mathy
#define mathy

double simpsonIteration(double pt0, double pt1, double pt2,
    int ust0, int ust1, int ust2);
pVector rotatePOV(pVector vec, pVector rot);
pVector orientation(pVector angle);
pVector filter(pVector accel, pVector gyrot, double alpha);

#endif
