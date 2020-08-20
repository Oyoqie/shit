#include <windows.h>
#include <stdio.h>
#include <chrono>
#include <omp.h>

typedef unsigned int UINT;

int main() {
	UINT result = 0;
	UINT h = 32 / omp_get_max_threads();

	auto start = std::chrono::steady_clock::now();

	#pragma omp parallel num_threads(omp_get_max_threads())
	{
		for (UINT j = 0; j < 10; ++j) {
			UINT g = 0;
			UINT t = omp_get_thread_num();

			for (UINT i = 0; i < 0x04ffffff; ++i)
				for (UINT k = 0; k < 15; ++k)
					g ^= 0x15 << k * 2;

			#pragma omp flush(result)
			if (g == 0x15555551) {
				result ^= g & (3 << (t * h));
				printf("thread %d complete, result %x\n", t, result);
			}
			else
				printf("thread %d error (invalid result)\n", t);
		}
	}

	auto end = std::chrono::steady_clock::now();

    using milliseconds_duration = std::chrono::duration<float, std::chrono::seconds::period>;

	printf("%.8fs took.\n", milliseconds_duration(end - start).count());

	return 0;
}