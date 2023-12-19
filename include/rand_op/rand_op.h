#ifndef RAND_OP_H_
#define RAND_OP_H_

#include <cassert>
#include <cstdint>
#include <random>

namespace rand_op {

// Return successful or not. If false, then this execution does not count.
template <typename R, typename E>
using op_t = bool (*)(R &, E &);

template <typename R, typename E>
void rand_op(R &rng, E &env, size_t num, op_t<R, E> ops[], uint64_t cnts[]) {
	uint64_t tot = 0;
	for (size_t i = 0; i < num; ++i) {
		tot += cnts[i];
	}
	while (tot > 0) {
		std::uniform_int_distribution<uint64_t> dist(0, tot - 1);
		uint64_t id = dist(rng);
		for (size_t i = 0; i < num; ++i) {
			if (id < cnts[i]) {
				if (ops[i](rng, env)) {
					tot -= 1;
					cnts[i] -= 1;
				}
				break;
			}
			id -= cnts[i];
		}
	}
	for (size_t i = 0; i < num; ++i) {
		assert(cnts[i] == 0);
	}
}

} // namespace rand_op

#endif // RAND_OP_H_
