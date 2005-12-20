/* Libvisual-plugins - Standard plugins for libvisual
 * 
 * Copyright (C) 2002, 2003, 2004, 2005 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id: table.h,v 1.3 2005-12-20 18:49:14 synap Exp $
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

#ifndef _OINK_TABLE_H
#define _OINK_TABLE_H

#define OINK_TABLE_NORMAL_SIZE	1200
#define OINK_TABLE_LARGE_SIZE	12000

float _oink_table_sin[OINK_TABLE_NORMAL_SIZE];
float _oink_table_cos[OINK_TABLE_NORMAL_SIZE];
float _oink_table_sinlarge[OINK_TABLE_LARGE_SIZE];
float _oink_table_coslarge[OINK_TABLE_LARGE_SIZE];

void _oink_table_init ();

#endif /* _OINK_TABLE_H */
