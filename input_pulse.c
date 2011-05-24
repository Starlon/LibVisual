/*    This file is part of Xmms2-libvisual.
 *
 *    Xmms2-libvisual is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Xmms2-libvisual is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Xmms2-libvisual.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <libvisual/libvisual.h>

#define PCM_BUF_SIZE 1024

typedef struct {
    pa_threaded_mainloop *mainloop;
    pa_context *context;
    pa_stream *stream;
    pa_proplist *proplist;
    pa_sample_spec sample_spec;

    const char *server;
} pulse_priv_t;

int inp_pulse_init( VisPluginData *plugin );
int inp_pulse_cleanup( VisPluginData *plugin );
int inp_pulse_upload( VisPluginData *plugin, VisAudio *audio );
int inp_pulse_events(VisPluginData *plugin, VisEventQueue *events);

int result_get_string(xmmsc_result_t *res, const char *key, const char **buf);
int result_get_int(xmmsc_result_t *res, const char *key, int *num);

VISUAL_PLUGIN_API_VERSION_VALIDATOR

const VisPluginInfo *get_plugin_info( int *count ) {
    static VisInputPlugin input[] = {{
        .upload = inp_pulse_upload
    }};

    static VisPluginInfo info[] = {{
        .type = VISUAL_PLUGIN_TYPE_INPUT,
        .plugname = "pulse",
        .name = "Pulse audio input plugin",
        .author = "Scott Sibley <scott@starlon.net>",
        .version = "$Revision: 1463 $",
        .about = "Use data exported from pulse audio",
        .help = "",
        .license = VISUAL_PLUGIN_LICENSE_GPL,

        .init = inp_pulse_init,
        .cleanup = inp_pulse_cleanup,
        .events = inp_pulse_events,

        .plugin = VISUAL_OBJECT(&input[0])
    }};

    *count = sizeof(info) / sizeof(*info);

    return info;
}

static void context_state_cb(pa_context *c, void *userdata)
{
	pulse_priv_t *priv = userdata;
	switch(pa_context_get_state(c)) {
		case PA_CONTEXT_READY:
		case PA_CONTEXT_TERMINATED:
		case PA_CONTEXT_FAILED:
			pa_threaded_mainloop_signal( priv->mainloop, 0);
			break;
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
			break;
	}
}

static int pulse_create_stream(pulse_priv_t *priv)
{
	const char *name;
}


static void stream_request_cb (pa_stream *s, size_t length, void *userdata)
{

}

static void stream_underflow_cb (pa_stream * s, void *userdata)
{
}

static void stream_overflow_cb (pa_stream * s, void *userdata)
{
}

static void stream_latency_update_cb (pa_stream *s, void *userdata)
{
	const pa_timing_info *info;
	pa_usec_t source_usec;
	info = pa_stream_get_timing_info(s);
	source_usec = 0;
}

int inp_pulse_init( VisPluginData *plugin ) {
    pulse_priv_t *priv;
    const char *err_buf;
    pa_channel_map channel_map;
    const char *name;
    VisParamContainer *paramcontainer = visual_plugin_get_params(plugin);

    priv = visual_mem_malloc0(sizeof(pulse_priv_t));
    priv->mainloop = pa_threaded_mainloop_new();
    pa_threaded_mainloop_start(priv->mainloop);
    pa_threaded_mainloop_lock(priv->mainloop);

    pa_context_set_state_callback(priv->context, context_state_cb, priv);

    priv->server = "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor";

    pa_context_connect(priv->context, priv->server, 0, NULL);
    for(;;) {
	pa_context_state_t state;
	state = pa_context_get_state(priv->context);
	if(!PA_CONTEXT_IS_GOOD (STATE)) {
		goto unlock_and_fail;
	}
	if(state == PA_CONTEXT_READY)
		break;

	pa_threaded_mainloop_wait(priv->mainloop);
    }

    priv->stream = pa_stream_new(priv->context, "Record Stream", &priv->sample_spec, &channel_map);
    visual_object_set_private(VISUAL_OBJECT(plugin), priv);

    const pa_channel_map *m = pa_stream_get_channel_map(priv->stream);

    pa_stream_set_state_callback(priv->stream, stream_state_cb, priv);
    pa_stream_set_read_callback (priv->stream, stream_request_cb, priv);
    pa_stream_set_underflow_callback (pulsesrc->stream, stream_underflow_cb, priv);
    pa_stream_set_overflow_callback (pulsesrc->stream, stream_overflow_cb, priv);
    pa_stream_set_latency_update_callback (pulsesrc->stream, stream_latency_update_cb, priv);

unlock_and_fail:
    pa_threaded_mainloop_unlock (priv->mainloop);
    return VISUAL_OK;
}

int inp_pulse_cleanup( VisPluginData *plugin ) {
    pulse_priv_t *priv = NULL;

    visual_log_return_val_if_fail( plugin != NULL, VISUAL_ERROR_GENERAL);
    priv = visual_object_get_private(VISUAL_OBJECT( plugin));
    visual_log_return_val_if_fail( priv != NULL, VISUAL_ERROR_GENERAL);

    xmmsc_visualization_shutdown(priv->connection, priv->vis);

    visual_mem_free (priv);
    return VISUAL_OK;
}

int inp_pulse_upload( VisPluginData *plugin, VisAudio *audio )
{
    pulse_priv_t *priv = NULL;
    short pcm_data[PCM_BUF_SIZE];
    xmmsc_result_t *res;
    xmmsv_t *val;
    VisBuffer buffer;
    VisSongInfo *songinfo;
    int id, time;
    const char *dictbuf;
    int dictnum;
    int length = VISUAL_BEAT_ADV_MAX;

    visual_log_return_val_if_fail( audio != NULL, -VISUAL_ERROR_GENERAL);
    visual_log_return_val_if_fail( plugin != NULL, -VISUAL_ERROR_GENERAL);

    priv = visual_object_get_private(VISUAL_OBJECT(plugin));

    visual_log_return_val_if_fail( priv != NULL, -VISUAL_ERROR_GENERAL);

    pa_threaded_mainloop_lock(priv->mainloop);

    visual_plugin_events_pump(plugin);

    songinfo = priv->songinfo;

    while(length > 0) {
	size_t l;
	if(!priv->read_buffer) {
	    for(;;) {
		    if(pa_stream_peek (priv->stream, &priv->read_buffer, priv->read_buffer_length) < 0)
				goto unlock;
	            if(priv->read_buffer && priv->read_bufer_length)
			break;
	
	            pa_threaded_mainloop_wait (priv->mainloop);
            }
	}
	l = length
	memcpy(data, priv->read_buffer, l);
        priv->read_buffer = (const guint8 *) priv->read_buffer + l;
	priv->read_buffer_length-=l;
	data = (uint8_t *) data + l;
	length -= l;
	sum+=l;
	if(priv->read_buffer_length <= 0) {
		if(pa_stream_drop (priv->stream) < 0)
			goto unlock;
	}
    }

unlock:
    pa_threaded_mainloo_unlock(priv->mainloop);

    return 0;
}

int inp_pulse_events(VisPluginData *plugin, VisEventQueue *events) {
    pulse_priv_t *priv = visual_object_get_private (VISUAL_OBJECT(plugin));
    VisEvent ev;
    VisParamEntry *param;

    while(visual_event_queue_poll(events, &ev))
    {
        switch(ev.type)
        {
            case VISUAL_EVENT_PARAM:
                param = ev.event.param.param;
                if(visual_param_entry_get_type(param) == VISUAL_PARAM_ENTRY_TYPE_OBJECT)
                {
                    priv->songinfo = VISUAL_SONGINFO(visual_param_entry_get_object(param));
                }
                break;
            default:
                break;

        }
    }
    return 0;
}
