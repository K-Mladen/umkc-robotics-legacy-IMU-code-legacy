 a datapoint, keeping a running total of these datapoints          **   implements simpsons rule
*
*******************************************************************************/

//pt[i] being the value of the ith element
//ust[i] being the time at which the ith element was sampled
double simpsonIteration(double pt0, double pt1, double pt2,
                         int ust0, int ust1, int ust2)
{
  /*microseconds to seconds*/
  double t0 = ust0/1000000.0,
         t1 = ust1/1000000.0,
         t2 = ust2/1000000.0;
  /*make sure time interval is constant*/
  if ((t1 - t0)==(t2 - t1))
  {
    /*if so, do simpsons rule normally*/
    return ((t1-t0)/3*(pt0 + 4*pt1 + pt2));
  } else {
    /*Otherwise, split simpsons rule into smaller parts, and add them*/
    double result0, result1;
	 result0 = (t1-t0)/3*(pt0+2*pt1);
	 result1 = (t2-t1)/3*(2*pt1+pt2);
	 return (result0+result1);
  }
}
