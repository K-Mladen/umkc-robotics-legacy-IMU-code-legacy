std::atan(1)/45; //(pi/4)/45 = pi/180
  double s = sin(rot.magnitude()*rad),
c = cos(rot.magnitude()*rad);
     
double xDir = (c+pow(rot.unitComponent(X),2)*(1-c))*vec.component(X) +
(rot.unitComponent(X)*rot.unitComponent(Y)*(1-c)
- rot.unitComponent(Z)*s)*vec.component(Y) +
(rot.unitComponent(X)*rot.unitComponent(Z)*(1-c)
+ rot.unitComponent(Y)*s)*vec.component(Z);
										     
double yDir = (rot.unitComponent(Y)*rot.unitComponent(X)*(1-c)
+ rot.unitComponent(Z)*s)*vec.component(X) +
(c+pow(rot.unitComponent(Y),2)*(1-c))*vec.component(Y) +
(rot.unitComponent(Y)*rot.unitComponent(Z)*(1-c)
- rot.unitComponent(X)*s)*vec.component(Z);
double zDir =  (rot.unitComponent(Z)*rot.unitComponent(X)*(1-c)
- rot.unitComponent(Y)*s)*vec.component(X) +
(rot.unitComponent(Z)*rot.unitComponent(Y)*(1-c)
+ rot.unitComponent(X)*s)*vec.component(Y) +
(c+pow(rot.unitComponent(Z),2)*(1-c))*vec.component(Z);
printf("After Rotating:: %f %f %f", xDir, yDir, zDir);
cout << endl;																														  
vec.set(xDir, yDir, zDir);
return;*/

