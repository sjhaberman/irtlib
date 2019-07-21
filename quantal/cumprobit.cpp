//Log likelihood component, derivative, and second derivative
//for logit model with response y and parameter beta.
#include<armadillo>
using namespace arma;

struct fd2v
{
    double value;
    vec grad;
    mat hess;
};


fd2v cumprobit(int y,vec beta)
{

    double p,q,r;
    int i;
    fd2v results;
    
    results.value=0.0;
    results.grad=zeros(beta.n_elem);
    results.hess=zeros(beta.n_elem,beta.n_elem);
    for(i=0;i<beta.n_elem;i++)
    {
        p=normcdf(beta(i));
        q=1.0-p;
        r=normpdf(beta(i));
       
        
        if(i<y)
        {
            results.value=results.value+log(p);
            results.grad(i)=r/p;
            results.hess(i,i)=-(beta(i)+results.grad(i))*results.grad(i);
        }
        else
        {
            results.value=results.value+log(q);
            results.grad(i)=-r/q;
            results.hess(i,i)=-(beta(i)+results.grad(i))*results.grad(i);
            break;
        }
    }
    
    return results;
}