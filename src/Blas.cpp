#include "einsums/Blas.hpp"

#include "backends/cblas/cblas.hpp"
#include "backends/netlib/Netlib.hpp"
#include "backends/onemkl/onemkl.hpp"
#include "backends/vendor/Vendor.hpp"

#include <fmt/format.h>
#include <stdexcept>

namespace einsums::blas {

void initialize() {
#if defined(SYCL_LANGUAGE_VERSION)
    ::einsums::backend::onemkl::initialize();
#endif
}

void finalize() {
#if defined(SYCL_LANGUAGE_VERSION)
    ::einsums::backend::onemkl::finalize();
#endif
}

void sgemm(char transa, char transb, int m, int n, int k, float alpha, const float *a, int lda, const float *b, int ldb, float beta,
           float *c, int ldc) {
    ::einsums::backend::vendor::sgemm(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

void dgemm(char transa, char transb, int m, int n, int k, double alpha, const double *a, int lda, const double *b, int ldb, double beta,
           double *c, int ldc) {
    ::einsums::backend::vendor::dgemm(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

void cgemm(char transa, char transb, int m, int n, int k, std::complex<float> alpha, const std::complex<float> *a, int lda,
           const std::complex<float> *b, int ldb, std::complex<float> beta, std::complex<float> *c, int ldc) {
    ::einsums::backend::vendor::cgemm(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}
void zgemm(char transa, char transb, int m, int n, int k, std::complex<double> alpha, const std::complex<double> *a, int lda,
           const std::complex<double> *b, int ldb, std::complex<double> beta, std::complex<double> *c, int ldc) {
    ::einsums::backend::vendor::zgemm(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

void dgemv(char transa, int m, int n, double alpha, const double *a, int lda, const double *x, int incx, double beta, double *y, int incy) {
    ::einsums::backend::vendor::dgemv(transa, m, n, alpha, a, lda, x, incx, beta, y, incy);
}

auto dsyev(char job, char uplo, int n, double *a, int lda, double *w, double *work, int lwork) -> int {
    return ::einsums::backend::vendor::dsyev(job, uplo, n, a, lda, w, work, lwork);
}

auto dgesv(int n, int nrhs, double *a, int lda, int *ipiv, double *b, int ldb) -> int {
    return ::einsums::backend::vendor::dgesv(n, nrhs, a, lda, ipiv, b, ldb);
}

void dscal(int n, double alpha, double *vec, int inc) {
    ::einsums::backend::vendor::dscal(n, alpha, vec, inc);
}

auto ddot(int n, const double *x, int incx, const double *y, int incy) -> double {
    return ::einsums::backend::vendor::ddot(n, x, incx, y, incy);
}

void daxpy(int n, double alpha_x, const double *x, int inc_x, double *y, int inc_y) {
    ::einsums::backend::vendor::daxpy(n, alpha_x, x, inc_x, y, inc_y);
}

void dger(int m, int n, double alpha, const double *x, int inc_x, const double *y, int inc_y, double *a, int lda) {
    ::einsums::backend::vendor::dger(m, n, alpha, x, inc_x, y, inc_y, a, lda);
}

auto dgetrf(int m, int n, double *a, int lda, int *ipiv) -> int {
    return ::einsums::backend::vendor::dgetrf(m, n, a, lda, ipiv);
}

auto dgetri(int n, double *a, int lda, const int *ipiv, double *work, int lwork) -> int {
    return ::einsums::backend::vendor::dgetri(n, a, lda, (int *)ipiv, work, lwork);
}

auto dlange(char norm_type, int m, int n, const double *A, int lda, double *work) -> double {
    return ::einsums::backend::vendor::dlange(norm_type, n, m, A, lda, work);
}

auto dgesdd(char jobz, int m, int n, double *a, int lda, double *s, double *u, int ldu, double *vt, int ldvt, double *work, int lwork,
            int *iwork) -> int {
#if defined(EINSUMS_HAVE_LAPACKE) || defined(EINSUMS_HAVE_MKL_LAPACKE)
    return ::einsums::backend::cblas::dgesdd(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, iwork);
#else
    throw std::runtime_error("dgesdd not implemented.");
#endif
}

} // namespace einsums::blas