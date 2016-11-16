namespace robust_utils {

  using namespace atomic::tiny_ad;

  // logspace_add
  // template<class T>
  // T fmax2(T x, T y) {return (x < y) ? y : x;}
  template<class T>
  T logspace_add (const T &logx, const T &logy) {
    // Was:
    //  fmax2 (logx, logy) + log1p (exp (-fabs (logx - logy)));
    return ( logx < logy ?
             logy + log1p (exp (logx - logy)) :
             logx + log1p (exp (logy - logx)) );
  }

  /** \brief Robust negative binomial

      Parameterized via

      log_mu           := log(mu)
      log_var_minus_mu := log(var - mu)

      \internal

      Formulas:
      log( p / (1-p) ) =     log_mu - log_var_minus_mu
      log( n )         = 2 * log_mu - log_var_minus_mu

      var = exp(log_mu) + exp(log_var_minus_mu)
      log(var) = logspace_add( log_mu, log_var_minus_mu )

      log(1 - p) = log_var_minus_mu - logspace_add( log_mu, log_var_minus_mu )
      log( p )   = log_mu           - logspace_add( log_mu, log_var_minus_mu )

  */
  template <class Float>
  inline Float dnbinom_robust(const Float &x,
                              const Float &log_mu,
                              const Float &log_var_minus_mu,
                              int give_log = 0) {
    // Float p = mu / var;
    // Float n = mu * p / (1. - p);
    Float log_var = logspace_add( log_mu, log_var_minus_mu );
    Float log_p   =     log_mu - log_var;
    Float log_n   = 2 * log_mu - log_var_minus_mu;
    Float n = exp(log_n);  // NB: exp(log_n) could over/underflow
    Float logres = n * log_p;
    if (x != 0) {
      Float log_1mp = log_var_minus_mu - log_var;
      logres += lgamma(x + n) - lgamma(n) - lgamma(x + 1.) + x * log_1mp;
    }
    return ( give_log ? logres : exp(logres) );
  }

}
