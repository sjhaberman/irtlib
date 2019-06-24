//Newton-Raphson algorithm for function maximization of a continuously
//differentiable real function of real vectors of dimension p.
//The function, gradient, and Hessian are
//f.value, f.grad, and f.hess.
//The function should approach negative infinity whenever the vector norm of the function argument
//approaches infinity.  The Hessian matrix is continuous.
//Maximum number of main iterations is maxit.
//Maximum number of secondary iterations per main iteration is maxits.
//If change in approximation to maximum is less
//than tol, then iterations cease.
//The largest permitted step is stepmax>0.
//The improvement check is b>1.
//The minimum ratio of inner product of tentative step and gradient versus inner product of
//gradient and gradient must at least be mult>0.
//See Chapter 3 of Analysis of Frequency Data by S. J. Haberman for a very closely-related
//algorithm.



#include<armadillo>
using namespace std;
using namespace arma;

struct fd2v
{
    double value;
    vec grad;
    mat hess;
};

struct nrvvar
{
    vec locmax;
    double max;
    vec grad;
    mat hess;
};
nrvvar nrvvarf(vec x,function<fd2v(vec)> f)
{
    fd2v resultf;
    nrvvar result;
    resultf=f(x);
    result.locmax=x;
    result.max=resultf.value;
    result.grad=resultf.grad;
    result.hess=resultf.hess;
    return result;
};

nrvvar nrswv(const int,
            const double,const double,vec,nrvvar,
            function<fd2v(vec)>);
nrvvar nrvs(const int maxit,const int maxits,const double tol,const vec start,
        const double stepmax,const double b,const double mult,
        function<fd2v(vec)> f)
{
    
    
   
    double c,d;
    int i;
    
    nrvvar varx,vary;
    vec v,x,y;

// Function settings at start.
    varx=nrvvarf(start,f);

// Iterations.
    for(i=0;i<maxit;i++)
    {
// Stop if gradient of zero.
        if(!any(varx.grad)) return varx;
// Get direction.
        
        if ((-varx.hess).is_sympd())
        {
            v=solve(-varx.hess,varx.grad);
            if(dot(v,varx.grad)<mult*dot(varx.grad,varx.grad))v=varx.grad;
            
        }
        else
        {
            v=varx.grad;
        }





// Step truncation.
        
        d=norm(v,"inf");
        if(d<=stepmax)
        {
            c=1.0;
        }
        else
        {
            c=stepmax/d;
        }
        v=c*v;
        
        vary = nrswv(maxit,stepmax,b,v,varx,f);
        
        if(vary.max<varx.max+tol) return vary;

        varx=vary;
        x=y;
    }
    return vary;
}

