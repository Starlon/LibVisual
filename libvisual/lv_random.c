#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lv_random.h"

/* Thanks Burkhard Plaum <plaum@ipf.uni-stuttgart.de> for these values and some other hints */
#define val_a 1664525L		/* as suggested by Knuth */
#define val_c 1013904223L	/* as suggested by H.W. Lewis and is a prime close to 2^32 * (sqrt(5) - 2)) */

VisRandomContext __lv_internal_random_context;

/**
 * @defgroup VisRandom VisRandom
 * @{
 */


/**
 * Creates a new VisRandomContext data structure.
 *
 * @param seed The seed to be used to initialize the VisRandomContext with.
 *
 * @return A newly allocated VisRandomContext, or NULL on error.
 */
VisRandomContext *visual_random_context_new (uint32_t seed)
{
	VisRandomContext *rcontext;

	rcontext = visual_mem_new0 (VisRandomContext, 1);
	visual_random_context_set_seed (rcontext, seed);

	return rcontext;
}

/**
 * Frees the VisRandomContext. This frees the VisRandomContext data structure.
 *
 * @param rcontext Pointer to a VisRandomContext that needs to be freed.
 *
 * @return 0 on succes -1 on error.
 */
int visual_random_context_free (VisRandomContext *rcontext)
{
	visual_log_return_val_if_fail (rcontext != NULL, -1);

	visual_mem_free (rcontext);

	return 0;
}

/**
 * Set the seed to for a VisRandomContext.
 *
 * @param rcontext Pointer to the VisRandomContext for which the seed it set.
 * @param seed The seed which is set in the VisRandomContext.
 *
 * @return 0 on succes -1 on error.
 */
int visual_random_context_set_seed (VisRandomContext *rcontext, uint32_t seed)
{
	visual_log_return_val_if_fail (rcontext != NULL, -1);

	rcontext->seed = seed;
	rcontext->seed_state = seed;

	return 0;
}

/**
 * Get the seed that has been set to the VisRandomContext. This returns
 * the initial seed. Not the state seed.
 *
 * @see visual_random_context_get_seed_state
 *
 * @param rcontext The pointer to the VisRandomContext of which the initial random seed is requested.
 *
 * @return The initial random seed.
 */
uint32_t visual_random_context_get_seed (const VisRandomContext *rcontext)
{
	visual_log_return_val_if_fail (rcontext != NULL, 0);

	return rcontext->seed;
}

/**
 * Get the current state seed for the VisRandomContext.
 *
 * @see visual_random_context_get_seed
 *
 * @param rcontext The pointer to the VisRandomContext of which the state seed is requested.
 *
 * @return The current state seed for the randomizer.
 */
uint32_t visual_random_context_get_seed_state (const VisRandomContext *rcontext)
{
	visual_log_return_val_if_fail (rcontext != NULL, 0);

	return rcontext->seed_state;
}

/**
 * Gives a random integer using the VisRandomContext as context for the randomizer.
 *
 * @param rcontext The pointer to the VisRandomContext in which the state of the randomizer is set.
 *
 * @return A pseudo random integer.
 */
uint32_t visual_random_context_int (VisRandomContext *rcontext)
{
	visual_log_return_val_if_fail (rcontext != NULL, 0);

	return (rcontext->seed_state = val_a * rcontext->seed_state + val_c);
}

/**
 * Gives a random integer ranging between min and max using the VisRandomContext as context
 * for the randomizer.
 * This function may use floating point instructions. Remeber, this will break
 * things if used inside of MMX code.
 *
 * @param rcontext The pointer to the VisRandomContext in which the state of the randomizer is set.
 * @param min The minimum for the output.
 * @param max The maximum for the output.
 *   
 * @return A pseudo random integer confirm to the minimum and maximum.
 */
uint32_t visual_random_context_int_range (VisRandomContext *rcontext, int min, int max)
{
#if VISUAL_RANDOM_FAST_FP_RND
	/* Uses fast floating number generator and two divisions elimitated.
	 * More than 2 times faster than original.
	 */
	float fm = min; /* +10% speedup... */

	visual_log_return_val_if_fail (rcontext != NULL, 0);

	return visual_random_context_float (rcontext) * (max - min + 1) + fm;
#else
	visual_log_return_val_if_fail (rcontext != NULL, 0);

	return (visual_random_context_int (rcontext) / (VISUAL_RANDOM_MAX / (max - min + 1))) + min;
#endif
}

/**
 * Gives a random double precision floating point value
 * using the VisRandomContext as context for the randomizer.
 *
 * @param rcontext The pointer to the VisRandomContext in which the state of the randomizer is set.
 *
 * @return A pseudo random integer.
 */
double visual_random_context_double (VisRandomContext *rcontext)
{
#if VISUAL_RANDOM_FAST_FP_RND
	union {
		unsigned int i[2];
		double d;
	} value;
#endif
	uint32_t irnd;

	visual_log_return_val_if_fail (rcontext != NULL, -1);

	irnd = (rcontext->seed_state = val_a * rcontext->seed_state + val_c);
#if VISUAL_RANDOM_FAST_FP_RND
	/* This saves floating point division (20 clocks on AXP, 
	 * 38 on P4) and introduces store-to-load data size mismatch penalty
	 * and substraction op.
	 * Faster on AXP anyway :)
	 */

	value.i[0] = (irnd << 20);
	value.i[1] = 0x3ff00000 | (irnd >> 12);

	return value.d - 1.0;
#else
	return (double) irnd / VISUAL_RANDOM_MAX;
#endif
}

/**
 * Gives a random single precision floating point value
 * using the VisRandomContext as context for the randomizer.
 *
 * @param rcontext The pointer to the VisRandomContext in which the state of the randomizer is set.
 *
 * @return A pseudo random integer.
 */
float visual_random_context_float (VisRandomContext *rcontext)
{
#if VISUAL_RANDOM_FAST_FP_RND
	union {
		unsigned int i;
		float f;
	} value;
#endif
	uint32_t irnd;

	visual_log_return_val_if_fail (rcontext != NULL, -1);

	irnd = (rcontext->seed_state = val_a * rcontext->seed_state + val_c);
#if VISUAL_RANDOM_FAST_FP_RND
	/* Saves floating point division. Introduces substraction.
	 * Yet faster! :)
	 */

	value.i = 0x3f800000 | (t >> 9);

	return value.f - 1.0f;
#else
	return (float) irnd / VISUAL_RANDOM_MAX;
#endif
}

/**
 * Function which returns 1 with a propability of p (0.0 <= p <= 1.0) using the VisRandomContext
 * as context for the randomizer.
 *
 * @param rcontext The pointer to the VisRandomContext in which the state of the randomizer is set.
 * @param a The float to be used in the decide.
 *
 * @returns 0 or 1, or -1 on error.
 */
int visual_random_context_decide (VisRandomContext *rcontext, float a)
{
	visual_log_return_val_if_fail (rcontext != NULL, -1);

	return visual_random_context_float (rcontext) <= a;
}

/**
 * @}
 */

