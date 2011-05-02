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
#ifndef _R_DEFS_H_
#define _R_DEFS_H_

#include "config.h"
#include "avs_blend.h"

static int max(int x, int y)
{
    return x > y ? x : y;
}

static int  min(int x, int y)
{
    return x < y ? x : y;
}

unsigned int  BLEND(unsigned int a, unsigned int b)
{
	register unsigned int r,t;
	r=(a&0xff)+(b&0xff);
	t=min(r,0xff);
	r=(a&0xff00)+(b&0xff00);
	t|=min(r,0xff00);
	r=(a&0xff0000)+(b&0xff0000);
	t|=min(r,0xff0000);
	r=(a&0xff000000)+(b&0xff000000);
	return t|min(r,0xff000000);
}

#if 1
#define FASTMAX(x,y) max(x,y)
// (x-(((x-y)>>(32-1))&(x-y))) // hmm not faster :(
#define FASTMIN(x,y) min(x,y)
//(x+(((y-x)>>(32-1))&(y-x))) 
#else
#pragma warning( push, 1 )

 int FASTMAX(int x, int y)
{
  __asm
  {
    mov ecx, [x]
    mov eax, [y]
    sub	ecx, eax
  	cmc
  	and	ecx, edx
  	add	eax, ecx
  }
}
 int FASTMIN(int x, int y)
{
  __asm
  {
    mov ecx, [x]
    mov eax, [y]
    sub	ecx, eax
	  sbb	edx, edx
	  and	ecx, edx
	  add	eax, ecx
  }
}
#pragma warning( pop ) 

#endif

unsigned int  BLEND_MAX(unsigned int a, unsigned int b)
{
	register unsigned int t;
  int _a=a&0xff;
  int _b=b&0xff;
	t=FASTMAX(_a,_b);
  _a=a&0xff00; _b=b&0xff00;
  t|=FASTMAX(_a,_b);
  _a=a&0xff0000; _b=b&0xff0000;
  t|=FASTMAX(_a,_b);
	return t;
}

unsigned int  BLEND_MIN(unsigned int a, unsigned int b)
{
#if 1
	register unsigned int t;
  int _a=a&0xff;
  int _b=b&0xff;
	t=FASTMIN(_a,_b);
  _a=a&0xff00; _b=b&0xff00;
  t|=FASTMIN(_a,_b);
  _a=a&0xff0000; _b=b&0xff0000;
  t|=FASTMIN(_a,_b);
	return t;
#else
  __asm
  {
    mov ecx, [a]
    mov eax, [b]

    and ecx, 0xff
    and eax, 0xff

    mov esi, [a]
    mov ebx, [b]

    sub	ecx, eax
	  sbb	edx, edx

    and esi, 0xff00
    and ebx, 0xff00

	  and	ecx, edx
    sub	esi, ebx

	  sbb	edx, edx
	  add	eax, ecx

	  and	esi, edx
    mov ecx, [a]

	  add	ebx, esi
    and ecx, 0xff0000

    mov esi, [b]
    or eax, ebx

    and esi, 0xff0000

    sub	ecx, esi
	  sbb	edx, edx

	  and	ecx, edx
	  add	esi, ecx
    
    or eax, esi
  }
#endif
}

#ifdef FASTMAX
#undef FASTMAX
#undef FASTMIN
#endif


unsigned int  BLEND_AVG(unsigned int a, unsigned int b)
{
	return ((a>>1)&~((1<<7)|(1<<15)|(1<<23)))+((b>>1)&~((1<<7)|(1<<15)|(1<<23)));
}


unsigned int  BLEND_SUB(unsigned int a, unsigned int b)
{
	register int r,t;
	r=(a&0xff)-(b&0xff);
	t=max(r,0);
	r=(a&0xff00)-(b&0xff00);
	t|=max(r,0);
	r=(a&0xff0000)-(b&0xff0000);
	t|=max(r,0);
	r=(a&0xff000000)-(b&0xff000000);
	return t|max(r,0);
}

#ifdef NO_MMX
#define BLEND_ADJ BLEND_ADJ_NOMMX
#endif

unsigned int  BLEND_ADJ_NOMMX(unsigned char  **blendtable, unsigned int a, unsigned int b, int v)
{
	register int t;
	t=blendtable[a&0xFF][v]+blendtable[b&0xFF][0xFF-v];
	t|=(blendtable[(a&0xFF00)>>8][v]+blendtable[(b&0xFF00)>>8][0xFF-v])<<8;
	t|=(blendtable[(a&0xFF0000)>>16][v]+blendtable[(b&0xFF0000)>>16][0xFF-v])<<16;
	return t;
}

unsigned int  BLEND_MUL(unsigned char **blendtable, unsigned int a, unsigned int b)
{
	register int t;
	t=blendtable[a&0xFF][b&0xFF];
	t|=blendtable[(a&0xFF00)>>8][(b&0xFF00)>>8]<<8;
  t|=blendtable[(a&0xFF0000)>>16][(b&0xFF0000)>>16]<<16;
	return t;
}

void BLEND_LINE(unsigned int *fb, unsigned int color, unsigned char **blendtable, int mode)
{
  switch (mode&0xff)
  {
    case 1: *fb=BLEND(*fb,color); break;
    case 2: *fb=BLEND_MAX(*fb,color); break;
    case 3: *fb=BLEND_AVG(*fb,color); break;
    case 4: *fb=BLEND_SUB(*fb,color); break;
    case 5: *fb=BLEND_SUB(color,*fb); break;
    case 6: *fb=BLEND_MUL(blendtable, *fb,color); break;
    case 7: *fb=BLEND_ADJ_NOMMX(blendtable, *fb,color,(mode>>8)&0xff); break;
    case 8: *fb=*fb^color; break;
    case 9: *fb=BLEND_MIN(*fb,color); break;
    default: *fb=color; break;
  }
}

#endif
