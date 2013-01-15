
#incldue "../headerFiles/pVector.h"


double* get(pVector vec, pVector rot)
{
  double theMatrix[3][3];


  theMatrix[0][0] = (c+pow(rot.unitComponent(X),2)*(1-c));
  theMatrix[0][1] = (rot.unitComponent(X)*rot.unitComponent(Y)*(1-c)
                     - rot.unitComponent(Z)*s);
  theMatrix[0][2] = (rot.unitComponent(X)*rot.unitComponent(Z)*(1-c)
                     + rot.unitComponent(Y)*s);

  theMatrix[1][0] = (rot.unitComponent(Y)*rot.unitComponent(X)*(1-c)
                     + rot.unitComponent(Z)*s);
  theMatrix[1][1] = (c+pow(rot.unitComponent(Y),2)*(1-c));
  theMatrix[1][2] = (rot.unitComponent(Y)*rot.unitComponent(Z)*(1-c)
                     - rot.unitComponent(X)*s);

  theMatrix[2][0] = (rot.unitComponent(Z)*rot.unitComponent(X)*(1-c)
                     - rot.unitComponent(Y)*s);
  theMatrix[2][1] = (rot.unitComponent(Z)*rot.unitComponent(Y)*(1-c)
                     + rot.unitComponent(X)*s);
  theMatrix[2][2] = (c+pow(rot.unitComponent(Z),2)*(1-c));

  return theMatrix;
}

