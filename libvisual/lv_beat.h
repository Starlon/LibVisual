/* Libvisual - The audio visualisation framework.
 * 
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Scott Sibley <scott@starlon.net>
 * Adapted from Winamp's AVS plugin. See below.
 *
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/*
  LICENSE
  -------
Copyright 2005 Nullsoft, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer. 

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution. 

  * Neither the name of Nullsoft nor the names of its contributors may be used to 
    endorse or promote products derived from this software without specific prior written permission. 
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef _LV_BEAT_H
#define _LV_BEAT_H

#include <libvisual/lv_time.h>

VISUAL_BEGIN_DECLS

#define VISUAL_BEAT(obj)				(VISUAL_CHECK_CAST ((obj), VisBeat))
#define VISUAL_BEAT_ADV(obj)       (VISUAL_CHECK_CAST ((obj), VisBeatAdv))

#define BEAT_REAL    1
#define BEAT_GUESSED 2

#define BEAT_MAX_BPM 170
#define BEAT_MIN_BPM 60

#define BEAT_BETTER_CONF_ADOPT      2
#define BEAT_TOP_CONF_ADOPT         8
#define BEAT_MIN_STICKY             8
#define BEAT_STICKY_THRESHOLD       70
#define BEAT_STICKY_THRESHOLD_LOW   85

#define BEAT_ADV_MAX 200
#define BEAT_ADV_SIZE 2048

typedef struct _VisBeat VisBeat;
typedef struct _VisBeatType VisBeatType;
typedef struct _VisBeatPeak VisBeatPeak;
typedef struct _VisBeatAdv VisBeatAdv;

typedef enum {
    VISUAL_BEAT_SLIDE_IN,
    VISUAL_BEAT_SLIDE_OUT
} VisBeatSlider;

typedef enum {
    VISUAL_BEAT_ALGORITHM_PEAK,
    VISUAL_BEAT_ALGORITHM_ADV
} VisBeatAlgorithm;

struct _VisBeatType {
    clock_t TC;
    int type;
};

struct _VisBeatPeak {
    int beat_peak1;
    int beat_peak2;
    int beat_cnt;
    int beat_peak1_peak;
};

struct _VisBeatAdv {
    VisObject   obj;

    VisTime     lastDetect;
    int32_t cfg_sensitivity;      // How sensitive the algorithm is to loudness
    int32_t cfg_max_detect;     // Max bpm to throttle input
    int32_t cfg_thick_on_beats; // Should we create thick lines?
    int32_t aged;               // smoothed out loudness
    int32_t lowest;             // quietest poin in the current beat
    int32_t elapsed;            // frames since last beat
    int isquiet;                // was previous frame quiet
    int prevbeat;               // period of previous beat
    int32_t *beathistory;
    int32_t beatbase;
    int32_t beatquiet;
    int32_t thick;
    int32_t quiet;
};

struct _VisBeat {
    VisObject obj;

    VisBeatPeak peak;
    VisBeatAdv *adv;

    int cfg_smartbeat;
    int cfg_smartbeatsticky;
    int cfg_smartbeatresetnewsong;
    int cfg_smartbeatonlysticky;
    int sticked;
    int arbVal, skipVal;        // Values of arbitrary beat and beat skip
    int bpm, confidence, confidence1, confidence2; // Calculated BPM  (realtime), confidence computation
    clock_t lastTC;             // Last beat tick count
    clock_t startTC;            // Time we started
    VisBeatType *TCHist;         // History of last 8 beats
    int *smoother;            // History of last 8 BPM values, used to smooth changes
    int *half_discriminated;    // Discriminated beats table
    int *half_discriminated2;   // Discriminated beats table
    int hdPos;                  // Position of discriminating table
    int hdPos2;                 // Position of discriminating table
    int smPtr, smSize;          // Smoother pointer and size
    int TCHistPtr;               // Tick count of history pointer
    int TCHistSize;              // Tick count of history size
    int offIMax;                // Max divisor/multiplier used to guess/discriminate beats
    int lastBPM;                // Last calculated BPM, used by the smoother to detect new entry
    int insertionCount;          // Remembers how many beats were guessed
    clock_t predictionLastTC;   // Last tick count guessed/accepted beat
    clock_t avg;                // Average tick count interval between beats
    clock_t avg2;               // Average tick count interval between beats
    int skipCount;              // Beat counter used by beat skipper
    int inInc, outInc;          // +1/-1, Used by the nifty beatsynced sliders
    int inSlide, outSlide;      // Position of sliders
    int oldInSlide, oldOutSlide;// Used by timer to detect changes in sliders
    int oldsticked;             // Used by timer to detect changes in sticked state
    char *txt;              // Contains txt about the current BPM and confidence
    int halfCount, doubleCount; // Counter used to autodetect if double/half beat needed
    int TCUsed;                 // Remembers how many beats in the history were actually used for computationT/
    int predictionBpm;          // Contains BPM actually used to prediction (eliminates BPM drivings)
    int oldDisplayBpm, oldDisplayConfidence; // Detects stuff to redraw
    int bestConfidence;         // Best confidence we had so far
    int forceNewBeat;           // Force new BPM adoption
    int topConfidenceCount;     // Used to decide when to adpot new beat;
    int stickyConfidenceCount;  // Used to decided when to go sticky
    uint8_t doResyncBpm;
    int new_song;
    VisTimer timer;
    unsigned char logtab[256];
};

VisBeat *visual_beat_new(void);
int visual_beat_init(VisBeat *beat);
int visual_beat_refine_beat(VisBeat *beat, int beat_in);
int visual_beat_change_song(VisBeat *beat);
int visual_beat_reset_adapt(VisBeat *beat);
int visual_beat_slider_get(VisBeat *beat, VisBeatSlider slider);
int visual_beat_set_config(VisBeat *beat, int smartbeat, int smartbeatsticky, int smartbeatresetnewsong, int smartbeatonlysticky);
int visual_beat_set_smartbeat(VisBeat *beat, int smartbeat);
int visual_beat_set_smartbeat_sticky(VisBeat *beat, int smartbeatsticky);
int visual_beat_set_smartbeat_reset_on_newsong(VisBeat *beat, int smartbeatnewsong);
int visual_beat_set_smartbeat_only_sticky(VisBeat *beat, int smartbeatonlysticky);
VisBeatPeak *visual_beat_get_peak(VisBeat *beat);
VisBeatAdv *visual_beat_get_adv(VisBeat *beat);
char *visual_beat_get_info(VisBeat *beat);

VisBeatAdv *visual_beat_adv_new(void);
int visual_beat_adv_init(VisBeatAdv *adv);
int visual_beat_adv_set_config(VisBeatAdv *adv, int sensitive, int max_bpm, int thick_on_beats);
int visual_beat_adv_set_sensitivity(VisBeatAdv *adv, int sensitive);
int visual_beat_adv_set_max_detect(VisBeatAdv *adv, int max_detect);
int visual_beat_adv_set_thick_on_beats(VisBeatAdv *adv, int thick_on_beats);


VISUAL_END_DECLS

#endif /* _LV_BEAT_H */
