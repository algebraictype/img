#include "include/round_to_int.hpp"

[[nodiscard]] uint32_t round_to_int(const double& x){

    uint64_t rounded(0);
    rounded = (x + 0.50) > (int)x ? (x + 0.50) : (x - 0.50);

    return rounded;
}
