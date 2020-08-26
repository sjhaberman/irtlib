//Newton-Raphson algorithm for function maximization of a continuously
//differentiable real function of real vectors of dimension p on a
//nonempty open convex set O of p-dimensional vectors.  The gradient of f.value
//is f.grad.  The Hessian matrix is f.hess.
//The strict pseudoconcavity condition described in the document
//convergence.pdf is assumed to apply for the real number a.  For the starting
//vector start, it is assumed that the value of f.value at start exceeds a.
//Parameters used are defined in gaparams.
//The maximum number of main iterations is gaparams.maxit.
//The maximum number of secondary iterations per main iteration
//is gaparams.maxits.
//The maximum fraction of a step toward a boundary is
//gaparams.eta.
//For secondary iterations, the improvement check
//is gaparams.gamma1<1.
//The cosine check  is gaparams.gamma2.
//The largest permitted step length is gaparams.kappa>0.
//If a main iteration leads to a change of the function f less
//than gaparams.tol, then iterations cease.
#include<armadillo>
using namespace std;
using namespace arma;
struct f2v
{
    double value;
    vec grad;
    mat hess;
};
struct maxf2v
{
    vec locmax;
    double max;
    vec grad;
    mat hess;
};
struct paramnr
{
    int maxit;
    int maxits;
    double eta;
    double gamma1;
    double gamma2;
    double kappa;
    double tol;
};
maxf2v maxf2vvar(const vec & y,const f2v &f2y);
maxf2v maxlin2(const paramnr &nrparams,const vec & v,maxf2v & vary0,const function <f2v(vec)> f);
maxf2v nrv(const paramnr&nrparams,const vec &start,function<f2v(vec)> f)
{
    f2v fy0;
    int i;
    maxf2v vary0,vary1;
    vec v;
// Function settings at start.
    fy0=f(start);
    vary0=maxf2vvar(start,fy0);
// Return if starting impossible.
    if(isnan(vary0.max)) return vary0;
// Iterations.
    for(i=0;i<nrparams.maxit;i++)
    {
// Stop if gradient of zero.
        if(!any(vary0.grad)) return vary0;
// Find Newton-Raphson step if possible.
        if ((-vary0.hess).is_sympd())
        {
            v=solve(-vary0.hess,vary0.grad);
            if(dot(v,vary0.grad)<nrparams.gamma1*norm(v,2)*norm(vary0.grad,2))v=vary0.grad;
        }
        else
        {
            v=vary0.grad;
        }
// Line search.
        vary1 = maxlin2(nrparams,v,vary0,f);
//  Convergence check
        if(vary1.max<vary0.max+nrparams.tol) return vary1;
        vary0=vary1;
    }
    return vary1;
}
