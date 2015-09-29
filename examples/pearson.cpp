#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cmath>

extern "C" {

inline size_t matrix_idx(size_t n_row_size,
    size_t n_column_size, size_t i, size_t j) {
#if MATRIX_ORDER_ROW_MAJOR
	return i * n_row_size + j;
#else
	return j * n_column_size + i;
#endif
}

double* matrix_new(size_t n_row_sz, size_t n_col_sz)
{
	return (double*) calloc(n_row_sz * n_col_sz, sizeof(double));
}

void matrix_delete(double* mtx)
{
	free(mtx);
}

inline double matrix_get(double* mtx, size_t n_row_sz,
    size_t n_col_sz, size_t i, size_t j) {
	return mtx[matrix_idx(n_row_sz, n_col_sz, i, j)];
}

inline void matrix_set(double* mtx, size_t n_row_sz,
    size_t n_col_sz, size_t i, size_t j, double val) {
	mtx[matrix_idx(n_row_sz, n_col_sz, i, j)] = val;
}

double* create_random_matrix(size_t n_row_sz, size_t n_col_sz)
{
	double* random_mtx = matrix_new(n_row_sz, n_col_sz);
	for (size_t i = 0; i < n_row_sz * n_col_sz; i++) {
		random_mtx[i] = rand();
	}
	return random_mtx;
}

double* calc_correlation_matrix(double* series_mtx, 
	size_t n_series, size_t n_samples) 
{
	double * corr_mtx = matrix_new(n_series, n_series);
	for (size_t sr1 = 0; sr1 < n_series; sr1++) {
		for (size_t sr2 = 0; sr2 < sr1; sr2++) {			
			double sr1mean = 0.0, sr2mean = 0.0;
			for (size_t sm = 0; sm < n_samples; sm++) {
				sr1mean += matrix_get(series_mtx, n_series, n_samples, sr1, sm);
				sr2mean += matrix_get(series_mtx, n_series, n_samples, sr2, sm);
			}
			sr1mean /= n_samples;
			sr2mean /= n_samples;
			double sum_cross = 0.0, sumsq1 = 0.0, sumsq2 = 0.0;
			for (size_t sm = 0; sm < n_samples; sm++) {
				double v1 = matrix_get(series_mtx, n_series, n_samples, sr1, sm);
				double v2 = matrix_get(series_mtx, n_series, n_samples, sr2, sm);
				sum_cross += (v1 - sr1mean) * (v2 - sr2mean);
				sumsq1 += (v1 - sr1mean) * (v1 - sr1mean);
				sumsq2 += (v2 - sr2mean) * (v2 - sr2mean);
			}
			double pearson_r = sum_cross / sqrt(sumsq1 * sumsq2);
			matrix_set(corr_mtx, n_series, n_series, sr1, sr2, pearson_r);
			matrix_set(corr_mtx, n_series, n_series, sr2, sr1, pearson_r);
		}
		matrix_set(corr_mtx, n_series, n_series, sr1, sr1, 1.0);
	}
	return corr_mtx;
}

double bench(size_t n_series, size_t n_samples)
{	
	double* random_mtx = create_random_matrix(n_series, n_samples);
	clock_t begin = clock();
	double* corr_mtx = calc_correlation_matrix(random_mtx, 
		n_series, n_samples);
	clock_t end = clock();
	double elapsed_seconds = double(end - begin) / CLOCKS_PER_SEC;	
	matrix_delete(corr_mtx);
	matrix_delete(random_mtx);
	return elapsed_seconds;
}

int main() {

	for (size_t problem_size = 256; problem_size <= 2048; problem_size *= 2) {
		double elapsed_seconds = bench(problem_size, problem_size);
		printf("%lu x %lu: %lf s\n", problem_size, problem_size, elapsed_seconds);
	}
	return 0;
}

}