#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lv_common.h"
#include "lv_color.h"

/**
 * @defgroup VisColor VisColor
 * @{
 */

/**
 * Creates a new VisColor structure
 *
 * @return A newly allocated VisColor.
 */
VisColor *visual_color_new ()
{
	VisColor *color;

	color = visual_mem_new0 (VisColor, 1);

	return color;
}

/**
 * Frees the VisColor. This frees the VisColor data structure.
 *
 * @param color Pointer to a VisColor that needs to be freed.
 *
 * @return 0 on succes -1 on error.
 */
int visual_color_free (VisColor *color)
{
	visual_log_return_val_if_fail (color != NULL, -1);

	visual_mem_free (color);

	return 0;
}

/**
 * Fills the VisColor it's rgb values from hsv colorspace values.
 *
 * @param color Pointer to a VisColor which rgb values are filled.
 * @param h Hue value for the hsv colorspace, ranging from 0 to 360.
 * @param s Saturation value for the hsv colorspace, ranging from 0 to 1.
 * @param v Value value for the hsv colorspace, ranging from 0 to 1.
 *
 * @return 0 on succes -1 on error.
 */
int visual_color_from_hsv (VisColor *color, float h, float s, float v)
{
	int i;
	float f, w, q, t, r = 0, g = 0, b = 0;

	visual_log_return_val_if_fail (color != NULL, -1);

	if (s == 0.0)
		s = 0.000001;

	if (h == 360.0)
		h = 0.0;

	h = h / 60.0;
	i = (int) h;
	f = h - i;
	w = v * (1.0 - s);
	q = v * (1.0 - (s * f));
	t = v * (1.0 - (s * (1.0 - f)));

	switch (i) {
		case 0: r = v; g = t; b = w; break;
		case 1: r = q; g = v; b = w; break;
		case 2: r = w; g = v; b = t; break;
		case 3: r = w; g = q; b = v; break;
		case 4: r = t; g = w; b = v; break;
		case 5: r = v; g = w; b = q; break;

		default:
			break;
	}

	color->r = (float) r * 255;
	color->g = (float) g * 255;
	color->b = (float) b * 255;
	
	return 0;
}

/**
 * Creates hsv colorspace values from a VisColor
 *
 * @param color Pointer to a VisColor from which hsv colorspace values are created.
 * @param h Float pointer to a hue value for the hsv colorspace, ranging from 0 to 360.
 * @param s Float pointer to a saturation value for the hsv colorspace, ranging from 0 to 1.
 * @param v Float pointer to a value value for the hsv colorspace, ranging from 0 to 1.
 *
 * @return 0 on succes -1 on error.
 */
int visual_color_to_hsv (VisColor *color, float *h, float *s, float *v)
{

        float max, min, delta, r, g, b;

	visual_log_return_val_if_fail (color != NULL, -1);

	r = (float) color->r / 255.0;
	g = (float) color->g / 255.0;
	b = (float) color->b / 255.0;

	max = r;
	if (g > max)
		max = g;

	if (b > max)
		max = b;

	min = r;
	if (g < min)
		min = g;

	if (b < min)
		min = b;

	*v = max;

	if (max != 0.0)
		*s = (max - min) / max;
	else
		*s = 0.0;

	if (*s == 0.0) {
		*h = 0.0;
	} else {
		delta = max - min;

		if (r == max)
			*h = (g - b) / delta;
		else if (g == max)
			*h = 2.0 + (b - r) / delta;
		else if (b == max)
			*h = 4.0 + (r - g) / delta;

		*h = *h * 60.0;

		if (*h < 0.0)
			*h = *h + 360;
	}


	return 0;
}

/**
 * @}
 */

