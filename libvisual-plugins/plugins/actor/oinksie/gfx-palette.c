#include <math.h>
#include <string.h>

#include <libvisual/libvisual.h>

#include "common.h"
#include "misc.h"
#include "screen.h"
#include "gfx-palette.h"

void _oink_gfx_palette_init (OinksiePrivate *priv)
{
	priv->pal_startup = 1;
	
	priv->pal_data.fade_start = 1;
	priv->pal_data.fade_steps = 100;
	priv->pal_data.fade_poststeps = priv->pal_data.fade_steps;
	priv->pal_data.fade_poststop = 1;
}

void _oink_gfx_palette_save_old (OinksiePrivate *priv)
{
	/* If we haven't set up an fade yet, don't save the next old.
	 * this protects us from more event handles per screen draw
	 */
	if (priv->pal_data.pal_new == 1 && priv->pal_data.fade_stepsdone == priv->pal_data.fade_steps)
		visual_palette_copy (&priv->pal_old, &priv->pal_cur);
	
	priv->pal_data.fade_start = 1;
}

void _oink_gfx_palette_transform (OinksiePrivate *priv)
{
	int i;

	if (priv->pal_data.fade_start == 1)
	{
		priv->pal_data.fade_stepsdone = 0;
		
		for (i = 0; i < 256; i++)
		{
			priv->pal_fades[i].r = (float) (priv->pal_cur.colors[i].r - priv->pal_old.colors[i].r) / (float) priv->pal_data.fade_steps;
			priv->pal_fades[i].g = (float) (priv->pal_cur.colors[i].g - priv->pal_old.colors[i].g) / (float) priv->pal_data.fade_steps;
			priv->pal_fades[i].b = (float) (priv->pal_cur.colors[i].b - priv->pal_old.colors[i].b) / (float) priv->pal_data.fade_steps;

			priv->pal_fades[i].r_cur = priv->pal_old.colors[i].r;
			priv->pal_fades[i].g_cur = priv->pal_old.colors[i].g;
			priv->pal_fades[i].b_cur = priv->pal_old.colors[i].b;
		}
	
		priv->pal_data.fade_start = 0;
	}

	for (i = 0; i < 256; i++)
	{
		priv->pal_fades[i].r_cur += priv->pal_fades[i].r;
		priv->pal_fades[i].g_cur += priv->pal_fades[i].g;
		priv->pal_fades[i].b_cur += priv->pal_fades[i].b;
	
		priv->pal_old.colors[i].r = priv->pal_fades[i].r_cur;
		priv->pal_old.colors[i].g = priv->pal_fades[i].g_cur;
		priv->pal_old.colors[i].b = priv->pal_fades[i].b_cur;
	}

	priv->pal_data.fade_stepsdone++;

	if (priv->pal_data.fade_stepsdone >= priv->pal_data.fade_poststeps)
	{
		visual_palette_copy (&priv->pal_cur, &priv->pal_old);

		priv->pal_data.pal_new = 0;
		priv->pal_data.fade_start = 1;
		priv->pal_startup = 0;
	}
}

void _oink_gfx_palette_build (OinksiePrivate *priv, uint8_t funky)
{
	/* save old palette for transistion */
	_oink_gfx_palette_save_old (priv);
	_oink_gfx_palette_build_gradient (priv, funky);

	/* FIXME, changing fadesteps can fuck this up */
	if (priv->pal_data.fade_poststop == 1 && priv->pal_startup == 0) {
		switch (_oink_random_int (0, 5)) {
		case 0:
		case 1:
			priv->pal_data.fade_poststeps = _oink_random_int (60, priv->pal_data.fade_steps);
			break;

		case 2:
			priv->pal_data.fade_poststeps = _oink_random_int (40, priv->pal_data.fade_steps / 2);
			break;

		case 3:
			priv->pal_data.fade_poststeps = _oink_random_int (20, priv->pal_data.fade_steps / 3);
			break;
		case 4:
			priv->pal_data.fade_poststeps = _oink_random_int (80, priv->pal_data.fade_steps);
			break;

		default:
			priv->pal_data.fade_poststeps = priv->pal_data.fade_steps;
			break;
		}

	}

	priv->pal_data.pal_new = 1;	
}

void _oink_gfx_palette_color (OinksiePrivate *priv, int color, int red, int green, int blue)
{
	priv->pal_cur.colors[color].r = red << 2;
	priv->pal_cur.colors[color].g = green << 2;
	priv->pal_cur.colors[color].b = blue << 2;
} 

uint8_t _oink_gfx_palette_gradient_gen (OinksiePrivate *priv, uint8_t i, int mode)
{
	static const float __magic = 2.0 * PI/256;

	switch (mode)
	{
	case 0:
		return (i * i * i) >> 16; 
		break;

	case 1:
		return (i * i) >> 8;
		break;

	case 2:
		return (uint8_t) i;
		break;

	case 3:
		return (uint8_t) (128 * fabs (sin ( __magic * i )));
		break;

	case 4:
		return 0;
		break;

	default:
		break;
	}

	return 0;
}

void _oink_gfx_palette_build_gradient (OinksiePrivate *priv, uint8_t funky)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
 	
	if (funky == TRUE)
		i = 5;
	else
		i = 3;
	
	do
	{
		j = _oink_random_int (0, i - 1);
		k = _oink_random_int (0, i - 1);
		l = _oink_random_int (0, i - 1);		
		
	} while (j == k || j == l || l == k);
	
	for( i = 0; i <= 255; i++)
	{
		priv->pal_cur.colors[i].r = _oink_gfx_palette_gradient_gen (priv, i, j);
		priv->pal_cur.colors[i].g = _oink_gfx_palette_gradient_gen (priv, i, k);
		priv->pal_cur.colors[i].b = _oink_gfx_palette_gradient_gen (priv, i, l);
	}
}
