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

enum pDirection
{
    X = 0;
    Y = 1;
    Z = 2;
};

class pVector
{
    private:
    double comp[3], magnitude, unitcomp[3];
    bool magflag=0, unitflag[3]={0,0,0}; //set to 1 when value is calculated
    //helper functions
    resetFlags();

    public:
        //constructors & destructors
        pVector();
        pVector(double comp[0], double comp[1], double comp[2]);
        virtual ~pVector();
        //accessors
        double getComponent(pDirection dir);  //Enumerated for clarity:
                                            // which component is desired and
                                            // in case IMU is mounted sideways.
        double getMagnitude();
        double getUnitComponent(pDirection dir); //Enumerated as above
        //mutators
        void setComponent(pDirection dir, double val); //Enumerated as above
        //operators
        pVector& operator=(const pVector& other);
        pVector& operator+=(const pVector& other);
        const pVector& operator+(const pVector& other) const;
        pVector& operator-=(const pVector& other);
        const pVector& operator-(const pVector& other) const;

};

#endif // PVECTOR_H
