#pragma once
#ifndef PRNG_H
#define	PRNG_H

#include "Timer.h"

template <class _Ty, size_t _Wx, size_t _Nx, size_t _Mx, size_t _Rx, _Ty _Px, size_t _Ux, _Ty _Dx, size_t _Sx, _Ty _Bx,
	size_t _Tx, _Ty _Cx, size_t _Lx, _Ty _Fx>
class NumberGenerator
{
public:
	NumberGenerator()
	{
		minRange = _Ty{ 0 };
		maxRange = ~_Ty{ 0 };
		seed = pTimer->GetHighPrecisionTime().Count();
		RandomInit();
	}
	NumberGenerator(_Ty minRange, _Ty maxRange) :NumberGenerator()
	{
		SetRange(minRange, maxRange);
	}

	void ReInitGenerator()
	{
		seed = pTimer->GetHighPrecisionTime().Count();
		RandomInit();
	}

	void SetRange(_Ty minRange, _Ty maxRange)
	{
		if (minRange > maxRange)
		{
			this->maxRange = minRange;
			this->minRange = maxRange;
		}
		else
		{
			this->minRange = minRange;
			this->maxRange = maxRange;
		}
	}
	_Ty GetMin()
	{
		return minRange;
	}

	_Ty GetMax()
	{
		return maxRange;
	}

	_Ty GetSeed()
	{
		return seed;
	}

	_Ty Randomize()
	{
		_Ty y = (m_mt[m_index] & MASK_UPPER) | (m_mt[(m_index + 1) % N] & MASK_LOWER);

		if (y & 1u)
			m_mt[m_index] = m_mt[(m_index + M) % N] ^ (y >> 1) ^ A;
		else
			m_mt[m_index] = m_mt[(m_index + M) % N] ^ (y >> 1) ^ 0;

		y = m_mt[m_index];
		y ^= (y >> U) & WORD_MASK;
		y ^= (y << S) & B;
		y ^= (y << T) & C;
		y ^= (y >> L);

		y = minRange + y % (maxRange - minRange + 1);

		m_index = (m_index + 1) % N;

		return y;
	}
private:
	_Ty minRange = 0, maxRange = 1000;
	_Ty seed;

	static const size_t W = _Wx;
	static const size_t N = _Nx;
	static const size_t M = _Mx;
	static const size_t R = _Rx;
	static const _Ty    A = _Px;
	static const size_t U = _Ux;
	static const _Ty    D = _Dx;
	static const size_t S = _Sx;
	static const _Ty    B = _Bx;
	static const size_t T = _Tx;
	static const _Ty    C = _Cx;
	static const size_t L = _Lx;
	static const _Ty    F = _Fx;

	static const _Ty WORD_MASK = ~((~_Ty{ 0 } << (W - 1)) << 1);
	static const _Ty MASK_UPPER = (WORD_MASK << R) & WORD_MASK;
	static const _Ty MASK_LOWER = ~MASK_UPPER & WORD_MASK;


	_Ty m_index = 0;
	_Ty m_mt[N];
private:

	void RandomInit()
	{
		_Ty prev = m_mt[0] = seed & WORD_MASK;
		for (m_index = 1; m_index < N; m_index++)
			prev = m_mt[m_index] = (m_index + F * (prev ^ (prev >> (W - 2)))) & WORD_MASK;
		m_index = N;
	}
};

using mt32bit = NumberGenerator<unsigned int, 32, 624, 397, 31, 0x9908b0df, 11, 0xffffffff, 7, 0x9d2c5680, 15,
	0xefc60000, 18, 1812433253>;

using mt64bit = NumberGenerator<unsigned long long, 64, 312, 156, 31, 0xb5026f5aa96619e9ULL, 29,
	0x5555555555555555ULL, 17, 0x71d67fffeda60000ULL, 37, 0xfff7eee000000000ULL, 43, 6364136223846793005ULL>;

using default_number_generator = mt64bit;

extern SmartPointer<default_number_generator> pNumGenerator;

#endif
