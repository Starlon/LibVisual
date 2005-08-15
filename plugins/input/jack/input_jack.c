/* Libvisual-plugins - Standard plugins for libvisual
 * 
 * Copyright (C) 2004, 2005 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <gettext.h>

#include <jack/jack.h>

#include <libvisual/libvisual.h>

#define PCM_BUF_SIZE	1024

typedef struct {
	jack_client_t *client;
	jack_port_t *input_port;

	int shutdown;

	short fakebuf[PCM_BUF_SIZE];
} JackPrivate;

static int process_callback (jack_nframes_t nframes, void *arg);
static void shutdown_callback (void *arg);

int inp_jack_init (VisPluginData *plugin);
int inp_jack_cleanup (VisPluginData *plugin);
int inp_jack_upload (VisPluginData *plugin, VisAudio *audio);

const VisPluginInfo *get_plugin_info (int *count)
{
	static VisInputPlugin input[] = {{
		.upload = inp_jack_upload
	}};

	static VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_INPUT,

		.plugname = "jack",
		.name = "jack",
		.author = "Dennis Smit <ds@nerds-incorporated.org>",
		.version = "0.1",
		.about = N_("Jackit capture plugin"),
		.help = N_("Use this plugin to capture PCM data from the jackd daemon"),

		.init = inp_jack_init,
		.cleanup = inp_jack_cleanup,

		.plugin = VISUAL_OBJECT (&input[0])
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int inp_jack_init (VisPluginData *plugin)
{
	JackPrivate *priv;
	const char **ports;

#if ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
#endif

	visual_log_return_val_if_fail (plugin != NULL, -1);

	priv = visual_mem_new0 (JackPrivate, 1);
	visual_log_return_val_if_fail (priv != NULL, -1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	if ((priv->client = jack_client_new (_("Libvisual jackit capture"))) == NULL) {
		visual_log (VISUAL_LOG_CRITICAL, _("jack server probably not running"));
		return -1;
	}

	jack_set_process_callback (priv->client, process_callback, priv);
	jack_on_shutdown (priv->client, shutdown_callback, priv);

	priv->input_port = jack_port_register (priv->client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	if (jack_activate (priv->client) == 1) {
		visual_log (VISUAL_LOG_CRITICAL, _("Cannot activate the jack client"));

		return -1;
	}

	if ((ports = jack_get_ports (priv->client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput)) == NULL) {
		visual_log (VISUAL_LOG_CRITICAL, _("Cannot find any physical capture ports"));

		return -1;
	}

	if (jack_connect (priv->client, ports[0], jack_port_name (priv->input_port))) {
		visual_log (VISUAL_LOG_CRITICAL, _("Cannot connect input ports"));

		visual_mem_free (ports);

		return -1;
	}

	visual_mem_free (ports);

	return 0;
}

int inp_jack_cleanup (VisPluginData *plugin)
{
	JackPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	visual_log_return_val_if_fail( plugin != NULL, -1 );
	priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	visual_log_return_val_if_fail( priv != NULL, -1 );

	if (priv->client != NULL)
		jack_client_close (priv->client);

	visual_mem_free (priv);

	return 0;
}

int inp_jack_upload (VisPluginData *plugin, VisAudio *audio)
{
	JackPrivate *priv = NULL;
	int i;

	visual_log_return_val_if_fail(audio != NULL, -1);
	visual_log_return_val_if_fail(plugin != NULL, -1);

	priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	visual_log_return_val_if_fail(priv != NULL, -1);

	if (priv->shutdown == TRUE) {
		visual_log (VISUAL_LOG_CRITICAL, _("The jack server seems to have shutdown"));

		return -1;
	}

	for (i = 0; i < PCM_BUF_SIZE && i < 1024; i += 2) {
		audio->plugpcm[0][i >> 1] = priv->fakebuf[i];
		audio->plugpcm[1][i >> 1] = priv->fakebuf[i + 1];
	}

	return 0;
}

static int process_callback (jack_nframes_t nframes, void *arg)
{
	JackPrivate *priv = arg;
	jack_default_audio_sample_t *in;
	int i;

	in = (jack_default_audio_sample_t *) jack_port_get_buffer (priv->input_port, nframes);

	visual_mem_set (&priv->fakebuf, 0, sizeof (priv->fakebuf));

	for (i = 0; i < nframes && i < 1024; i++)
		priv->fakebuf[i] = in[i] * 32767;

	return 0;
}

static void shutdown_callback (void *arg)
{
	JackPrivate *priv = arg;

	priv->shutdown = TRUE;
}

