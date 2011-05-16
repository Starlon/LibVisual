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
#ifndef _R_BLEND_H_
#define _R_BLEND_H_

#include "config.h"
#include "avs_globals.h"

uint32_t  BLEND(uint32_t a, uint32_t b);
uint32_t  BLEND_MAX(uint32_t a, uint32_t b);
uint32_t  BLEND_MIN(uint32_t a, uint32_t b);
uint32_t  BLEND_AVG(uint32_t a, uint32_t b);
uint32_t  BLEND_SUB(uint32_t a, uint32_t b);
uint32_t  BLEND_ADJ_NOMMX(unsigned char blendtable[256][256], uint32_t a, uint32_t b, int v);
uint32_t  BLEND_MUL(unsigned char blendtable[256][256], uint32_t a, uint32_t b);
void BLEND_LINE(uint32_t *fb, uint32_t color, unsigned char blendtable[256][256], int mode);
uint32_t BLEND4(unsigned char blendtable[256][256], uint32_t *p1, uint32_t w, int xp, int yp);
uint32_t BLEND4_16(unsigned char **blendtable, uint32_t *p1, uint32_t w, int xp, int yp);
void mmx_mulblend_block(unsigned char blendtable[256][256], uint32_t *output, uint32_t *input, int l);
void mmx_adjblend_block(unsigned char blendtable[256][256], uint32_t *o, uint32_t *in1, uint32_t *in2, int len, int v);
void mmx_addblend_block(uint32_t *output, uint32_t *input, int l);
void mmx_avgblend_block(uint32_t *output, uint32_t *input, int l);

#endif
