/* Libvisual-plugins - Standard plugins for libvisual
 * 
 * Copyright (C) 2004, 2005 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *	    Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and
 *	    4Front Technologies
 *
 * $Id:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <libvisual/libvisual.h>

#define NUM_BANDS	16

typedef struct {
	GLfloat y_angle;
	GLfloat y_speed;
	GLfloat x_angle;
	GLfloat x_speed;
	GLfloat z_angle;
	GLfloat z_speed;
	GLfloat heights[16][16];
	GLfloat scale;

	int transparant;
} GLtestPrivate;

int lv_gltest_init (VisPluginData *plugin);
int lv_gltest_cleanup (VisPluginData *plugin);
int lv_gltest_requisition (VisPluginData *plugin, int *width, int *height);
int lv_gltest_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
int lv_gltest_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *lv_gltest_palette (VisPluginData *plugin);
int lv_gltest_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

static void draw_bars (GLtestPrivate *priv);
static void draw_rectangle (GLtestPrivate *priv, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
static void draw_bar (GLtestPrivate *priv, GLfloat x_offset, GLfloat z_offset, GLfloat height, GLfloat red, GLfloat green, GLfloat blue);
static void draw_bars (GLtestPrivate *priv);

/* Main plugin stuff */
const VisPluginInfo *get_plugin_info (int *count)
{
	static const VisActorPlugin actor[] = {{
		.requisition = lv_gltest_requisition,
		.palette = lv_gltest_palette,
		.render = lv_gltest_render,
		.depth = VISUAL_VIDEO_DEPTH_GL
	}};

	static const VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_ACTOR,

		.plugname = "lv_gltest",
		.name = "libvisual GL analyser",
		.author = "Original by:  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies, Port by: Dennis Smit <ds@nerds-incorporated.org>",
		.version = "0.1",
		.about = "The Libvisual GL analyzer plugin",
		.help =  "This plugin shows an openGL bar analyzer like the xmms one.",

		.init = lv_gltest_init,
		.cleanup = lv_gltest_cleanup,
		.events = lv_gltest_events,

		.plugin = VISUAL_OBJECT (&actor[0])
	}};
	
	*count = sizeof (info) / sizeof (*info);

	return info;
}

int lv_gltest_init (VisPluginData *plugin)
{
	GLtestPrivate *priv;
	VisParamContainer *paramcontainer = visual_plugin_get_params (plugin);
	
	static const VisParamEntry params[] = {
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("transparant bars",	TRUE),
		VISUAL_PARAM_LIST_END
	};
	
	int x, y;

	/* UI Vars */
	VisUIWidget *checkbox;

	priv = visual_mem_new0 (GLtestPrivate, 1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	visual_param_container_add_many (paramcontainer, params);

	checkbox = visual_ui_checkbox_new ("Transparant bars", TRUE);
	visual_ui_mutator_set_param (VISUAL_UI_MUTATOR (checkbox), visual_param_container_get (paramcontainer, "transparant bars"));

	visual_plugin_set_userinterface (plugin, checkbox);

	/* GL setting up the rest! */	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	glFrustum (-1, 1, -1, 1, 1.5, 10);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glBlendFunc (GL_SRC_ALPHA,GL_ONE);

	for (x = 0; x < 16; x++) {
		for (y = 0; y < 16; y++) {
			priv->heights[y][x] = 0.0;
		}
	}

	priv->scale = 1.0 / log (256.0);

	priv->x_speed = 0.0;
	priv->y_speed = 0.5;
	priv->z_speed = 0.0;
	priv->x_angle = 20.0;
	priv->y_angle = 45.0;
	priv->z_angle = 0.0;

	return 0;
}

int lv_gltest_cleanup (VisPluginData *plugin)
{
	GLtestPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	VisUIWidget *ui;

	/* Destroy the VisUI tree */
	ui = visual_plugin_get_userinterface (plugin);
	visual_object_unref (VISUAL_OBJECT (ui));

	visual_mem_free (priv);

	return 0;
}

int lv_gltest_requisition (VisPluginData *plugin, int *width, int *height)
{
	int reqw, reqh;

	reqw = *width;
	reqh = *height;

	if (reqw < 1)
		reqw = 1;

	if (reqh < 1)
		reqh = 1;

	*width = reqw;
	*height = reqh;

	return 0;
}

int lv_gltest_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
	GLfloat ratio;
	
	visual_video_set_dimension (video, width, height);

	ratio = (GLfloat) width / (GLfloat) height;

	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	gluPerspective (45.0, ratio, 0.1, 100.0);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	return 0;
}

int lv_gltest_events (VisPluginData *plugin, VisEventQueue *events)
{
	GLtestPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	VisEvent ev;
	VisParamEntry *param;

	while (visual_event_queue_poll (events, &ev)) {
		switch (ev.type) {
			case VISUAL_EVENT_RESIZE:
				lv_gltest_dimension (plugin, ev.resize.video,
						ev.resize.width, ev.resize.height);
				break;

			case VISUAL_EVENT_PARAM:
				param = ev.param.param;

				if (visual_param_entry_is (param, "transparant bars")) {
					priv->transparant = visual_param_entry_get_integer (param);

					if (priv->transparant == FALSE)
						glDisable (GL_BLEND);
					else
						glEnable (GL_BLEND);
				}

			default: /* to avoid warnings */
				break;
		}
	}

	return 0;
}

VisPalette *lv_gltest_palette (VisPluginData *plugin)
{
	return NULL;
}

int lv_gltest_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
	GLtestPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	int i,c;
	int y;
	GLfloat val;

	int xscale[] = {0, 1, 2, 3, 5, 7, 10, 14, 20, 28, 40, 54, 74, 101, 137, 187, 255};

	for (y = 15; y > 0; y--)
	{
		for(i = 0; i < 16; i++)
		{
			priv->heights[y][i] = priv->heights[y - 1][i];
		}
	}

	for (i = 0; i < NUM_BANDS; i++)
	{
		for(c = xscale[i], y = 0; c < xscale[i + 1]; c++)
		{
			if(audio->freq[0][c] > y)
				y = audio->freq[0][c];
		}
		y >>= 7;
		if (y > 0)
			val = (log(y) * priv->scale);
		else
			val = 0;


		priv->heights[0][i] = val;


	}

	priv->x_angle += priv->x_speed;
	if (priv->x_angle >= 360.0)
		priv->x_angle -= 360.0;

	priv->y_angle += priv->y_speed;
	if (priv->y_angle >= 360.0)
		priv->y_angle -= 360.0;

	priv->z_angle += priv->z_speed;
	if (priv->z_angle >= 360.0)
		priv->z_angle -= 360.0;

	draw_bars (priv);

	return 0;
}

/* Drawing stuff */
static void draw_rectangle (GLtestPrivate *priv, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
	if (y1 == y2) {

		glVertex3f (x1, y1, z1);
		glVertex3f (x2, y1, z1);
		glVertex3f (x2, y2, z2);

		glVertex3f (x2, y2, z2);
		glVertex3f (x1, y2, z2);
		glVertex3f (x1, y1, z1);
	} else {
		glVertex3f (x1, y1, z1);
		glVertex3f (x2, y1, z2);
		glVertex3f (x2, y2, z2);

		glVertex3f (x2, y2, z2);
		glVertex3f (x1, y2, z1);
		glVertex3f (x1, y1, z1);
	}
}

static void draw_bar (GLtestPrivate *priv, GLfloat x_offset, GLfloat z_offset, GLfloat height, GLfloat red, GLfloat green, GLfloat blue)
{
	GLfloat width = 0.1;

	glColor3f (red,green,blue);
	draw_rectangle (priv, x_offset, height, z_offset, x_offset + width, height, z_offset + 0.1);
	draw_rectangle (priv, x_offset, 0, z_offset, x_offset + width, 0, z_offset + 0.1);

	glColor3f (0.5 * red, 0.5 * green, 0.5 * blue);
	draw_rectangle (priv, x_offset, 0.0, z_offset + 0.1, x_offset + width, height, z_offset + 0.1);
	draw_rectangle (priv, x_offset, 0.0, z_offset, x_offset + width, height, z_offset );

	glColor3f (0.25 * red, 0.25 * green, 0.25 * blue);
	draw_rectangle (priv, x_offset, 0.0, z_offset , x_offset, height, z_offset + 0.1);
	draw_rectangle (priv, x_offset + width, 0.0, z_offset , x_offset + width, height, z_offset + 0.1);
}

static void draw_bars (GLtestPrivate *priv)
{
	int x,y;
	GLfloat x_offset, z_offset, r_base, b_base;

	glClearColor (0,0,0,0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix ();
	glTranslatef (0.0,-0.5,-5.0);
	glRotatef (priv->x_angle,1.0,0.0,0.0);
	glRotatef (priv->y_angle,0.0,1.0,0.0);
	glRotatef (priv->z_angle,0.0,0.0,1.0);

	glBegin (GL_TRIANGLES);
	for (y = 0; y < 16; y++)
	{
		z_offset = -1.6 + ((15 - y) * 0.2);

		b_base = y * (1.0 / 15);
		r_base = 1.0 - b_base;

		for(x = 0; x < 16; x++)
		{
			x_offset = -1.6 + (x * 0.2);
			draw_bar (priv, x_offset, z_offset, priv->heights[y][x], r_base - (x * (r_base / 15.0)), x * (1.0 / 15), b_base);
		}
	}
	glEnd ();

	glPopMatrix ();
}
