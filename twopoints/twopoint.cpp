//Two-point algorithm for function maximization.
//Function and continuous first derivative are f.value and f.der.
//It is assumed that f.value approaches negative infinity
//if the absolute value of the argument approaches infinity.
//It is also assumed that f.value has a unique critical point.
//Maximum number of iterations is maxit.
//If change in approximation to maximum is less
//than tol, then iterations cease.
//The largest permitted step is stepmax>0.
//The improvement check is b>1.
#include<cstdlib>
#include<functional>
#include<cmath>
#include<float.h>
using namespace std;
double davidon(double,double,double,double,double,double);

struct fd1
{
    double value;
    double der;
};
    
struct twopointvar
{
    double locmax;
    double max;
    double der;
};
twopointvar twopointvarf(double x,function <fd1(double)> f)
{
    fd1 resultf;
    twopointvar result;
    resultf=f(x);
    result.locmax=x;
    result.max=resultf.value;
    result.der=resultf.der;
       
    return result;
};
double modit(double,double,double,double,double);
void rebound(double,double,double &,double &);
twopointvar twopoint(const int maxit,const double tol,const double start1,
                         const double start2,const double stepmax,const double b,
                         function<fd1(double)> f)
{
    double d,deltaf,lower,upper,x,y,z;
    int i;
    twopointvar varx,vary,varz;
// x is oldest value.  y is next value.
    x=start1;
    y=start2;
// lower is lower bound for location of maximum.
    lower=-DBL_MAX;
// upper is upper bound for location of maximum.
    upper=DBL_MAX;
    
// varx is location, function, and first derivative for x.
    varx=twopointvarf(x,f);
    vary=twopointvarf(y,f);
// Stop if derivative is 0.
    if(varx.der==0.0) return varx;
    if(vary.der==0.0) return vary;
    rebound(x,varx.der,lower,upper);
    rebound(y,vary.der,lower,upper);
// Switch order.
    if(varx.max>vary.max)
    {
        z=y;
        y=x;
        x=z;
        varz=vary;
        vary=varx;
        varx=varz;
    }
// Up to maxit iterations.
    for(i=0;i<maxit;i++)
    {
// The proposed step.
        z=davidon(x,y,varx.max,vary.max,varx.der,vary.der);
// Truncate if needed.
        z=modit(y,z,stepmax,lower,upper);
        varz=twopointvarf(z,f);
// Update limits.
        if(varz.der==0.0)return varz;
        rebound(z,varz.der,lower,upper);
        deltaf=varz.max-fmax(varx.max,vary.max);
        if(deltaf>=b*fabs(varz.der)*fmax(fabs(z-x),fabs(z-y))&&deltaf<tol)return varz;
        
// Prepare for next step.
        x=y;
        varx=vary;
        y=z;
        vary=varz;

    }
    return vary;
}

