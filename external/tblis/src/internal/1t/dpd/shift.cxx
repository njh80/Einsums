#include "util.hpp"
#include "shift.hpp"
#include "internal/1t/dense/shift.hpp"

namespace tblis
{
namespace internal
{

template <typename T>
void shift(const communicator& comm, const config& cfg,
           T alpha, T beta, bool conj_A, const dpd_marray_view<T>& A,
           const dim_vector& idx_A_A)
{
    unsigned nirrep = A.num_irreps();
    unsigned ndim = A.dimension();

    stride_type nblock = 1;
    for (unsigned i = 0;i < ndim-1;i++) nblock *= nirrep;

    irrep_vector irreps(ndim);
    unsigned irrep = A.irrep();

    for (stride_type block = 0;block < nblock;block++)
    {
        assign_irreps(ndim, irrep, nirrep, block, irreps, idx_A_A);

        if (is_block_empty(A, irreps)) continue;

        auto local_A = A(irreps);

        shift<T>(comm, cfg, local_A.lengths(), alpha, beta, conj_A, local_A.data(),
                 local_A.strides());
    }
}

#define FOREACH_TYPE(T) \
template void shift(const communicator& comm, const config& cfg, \
                    T alpha, T beta, bool conj_A, const dpd_marray_view<T>& A, \
                    const dim_vector&);
#include "configs/foreach_type.h"

}
}
