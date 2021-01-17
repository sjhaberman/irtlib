//Find maximum likelihood estimates for response model
//Responses are y.
//Predictors are x.
//Weights are w.
//Conjugate gradient method is used.
#include<armadillo>
using namespace arma;
using namespace std;
using namespace std::placeholders;
struct f1v
{
    double value;
    vec grad;
};
struct maxf1v
{
    vec locmax;
    double max;
    vec grad;
};
struct params
{
    int maxit;
    int maxits;
    double eta;
    double gamma1;
    double gamma2;
    double kappa;
    double tol;
};
struct model
{
  char type;
  char transform;
};
struct resp
{
  ivec iresp;
  vec dresp;
};
struct xsel
{
  bool all;
  ivec list;
};
struct dat
{
     model choice;
     double weight;
     resp dep;
     vec offset;
     mat indep;
     xsel xselect;
};
maxf1v conjgrad(const params &,const vec &, function<f1v(vec &)>);
f1v genresplik1(const vector<dat> & , const vec &);
maxf1v genrespmle1(const params & mparams, const vector<dat> & data, const vec & start)
{
    maxf1v results;
    int p;
    p=start.n_elem;
    results.locmax.set_size(p);
    results.grad.set_size(p);
    auto f=std::bind(genresplik1,data,_1);
    results=conjgrad(mparams,start,f);
    return results;
}