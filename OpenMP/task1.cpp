#include <omp.h>
#include <iostream>

double pi(unsigned steps)
{ 
    double t1 = omp_get_wtime();
    double sum = 0.0;
    double dx = 1.0 / steps;
    double x = 0.0;
    for (int i = 0; i < steps; ++i)
    {
        sum += 4.0 / (1.0 + x * x);
        x += dx;
    }
    double t2 = omp_get_wtime();
	printf("time: %lf\n", t2 - t1);
    return sum * dx;
}

double pi_parallel_for(unsigned steps)
{
    double sum = 0.0;
    const double dx = 1.0 / steps;
    #pragma omp parallel for
    for (int i = 0; i < steps; ++i)
    {
        double x = i * dx;
        sum += 4.0 / (1.0 + x * x);

    }
    return sum * dx;
}


double pi_reduction(unsigned steps)
{
    double t1 = omp_get_wtime();
    double sum = 0.0;
    const double dx = 1.0 / steps; 
    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < steps; ++i)
    {
        double x = i * dx;
        sum += 4.0 / (1.0 + x * x);
    }
    double t2 = omp_get_wtime();
	printf("time (omp): %lf\n", t2 - t1);
    return sum * dx;
}

int main()
{
    unsigned steps = 2000;
    std::cout << "pi: " << pi(steps) << std::endl;
    std::cout << "pi (parallel for): " << pi_parallel_for(steps) << std::endl;
    std::cout << "pi (reduction): " << pi_reduction(steps) << std::endl;

    return 0;
}