#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <sys/time.h>

using namespace std;

std::default_random_engine generator;
std::normal_distribution<double> distribution(0.0, 1.0);

double timeval_diff(const struct timeval& t1, const struct timeval& t2)
{
	return (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1e6;
}

// Pricing a European vanilla call option with a Monte Carlo method
double monte_carlo_call_price(size_t num_sims, double S,
	double K, double r, double v, double T) 
{
  const double S_adjust = S * exp(T*(r-0.5*v*v));
  double payoff_sum = 0.0;

  #ifdef USE_OPENMP
  #pragma omp parallel for reduction(+:payoff_sum) num_threads(4)
  #endif
  for (int i=0; i<num_sims; i++) {
  	double deviate = distribution(generator);    
    double S_cur = S_adjust * exp(sqrt(v*v*T)*deviate);
    payoff_sum += std::max(S_cur - K, 0.0);
  }

  return (payoff_sum / (double)num_sims) * exp(-r*T);
}

int main()
{
  struct timeval wall_start, wall_end;
  struct timezone wall_tz_start, wall_tz_end;
  double S = 100.0;  // Option price                                                                       
  double K = 100.0;  // Strike price                                                                                  
  double r = 0.05;   // Risk-free rate (5%)                                                                           
  double v = 0.2;    // Volatility of the underlying (20%)                                                            
  double T = 1.0;    // One year until expiry
  for (size_t N = 10000; N <= 100000000; N *= 10) 
  {
  	clock_t cpu_begin = clock();
  	gettimeofday(&wall_start, &wall_tz_start);
  	double price = monte_carlo_call_price(N, S, K, r, v, T);
  	clock_t cpu_end = clock();
  	gettimeofday(&wall_end, &wall_tz_end);
  	double cpu_elapsed_seconds = double(cpu_end - cpu_begin) / CLOCKS_PER_SEC;	
  	double wall_elapsed_seconds = timeval_diff(wall_start, wall_end);
  	cout << "N = " << N 
  	<< ", CPU time = " << cpu_elapsed_seconds 
  	<< ", Wall time = " << wall_elapsed_seconds
  	<< " price=" << price << endl;
  }
}
