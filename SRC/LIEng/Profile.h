#pragma once

typedef unsigned long long uprof;

inline uprof rdtsc() {
	__asm rdtsc
}