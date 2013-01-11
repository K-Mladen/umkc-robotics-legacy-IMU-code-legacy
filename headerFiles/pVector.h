/******************************************************************************
*  pVector is a physics-vector, to avoid confusion with the C++ vector class. *
* it has cartesian components, as well as a magnitude and a set of unit       *
* vectors that correspond in direction to the components but with magnitude 1 *
*  pDirection stands for pVectorDirection, and is a simplified way to keep    *
* track of which component is being referenced.  I could have just used 0,1,2 *
* but this will make it easier to adjust the code if someone decides to mount *
* the phidget sideways.  Maybe it won't matter since the calibration should   *
* take care of that but in either case this way is a bit clearer I hope.      *
*  Definitions in pVector.cpp                        --MK                     *
******************************************************************************/

#ifndef PVECTOR_H
#define PVECTOR_H

using namespace std;

enum pDirection
{
  X = 0,
  Y = 1,
  Z = 2,
  x = 0,
  y = 1,
  z = 2
};

class pVector
{
    private:
    double comp[3], mag, unitcomp[3];
    bool magflag;
      bool unitflag [3]; //set to 1 when value is calculated
    //helper functions
    void resetFlags();

    public:
      //constructors & destructors
      pVector();
      pVector(double comp0, double comp1, double comp2);
      pVector(double comp[3]);
      virtual ~pVector();
      //accessors
      double component(int dir);
      double component(pDirection dir);  //Enumerated for clarity:
                                          // which component is desired and
                                          // in case IMU is mounted sideways.
      double magnitude();
      double unitComponent(pDirection dir); //Enumerated as above
      //mutators
      void set(pDirection dir, double val); //Enumerated as above
      void set(double val[3]);
      void set(double valX, double valY, double valZ);
      void print();

      //operators
//      const pVector operator=(const pVector& rhs);
      pVector operator+=(const pVector& other);
      const pVector operator+(const pVector& other) const;
      pVector operator-=(const pVector& other);
      const pVector operator-(const pVector& other) const;
      double& operator[] (const int nIndex);
      double& operator[] (const pDirection nIndex);
};


#endif // PVECTOR_H
