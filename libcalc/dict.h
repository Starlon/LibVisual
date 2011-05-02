/* dict.h -- symbol dictionary structures
 *
 * Copyright (C) 2001 Janusz Gregorczyk <jgregor@kki.net.pl>
 *
 * This file is part of xvs.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 
 */

#ifndef Included_DICT_H
#define Included_DICT_H

/* A variable. */
typedef struct {
  char *name;
  double value;
} var_t;

/* A symbol dictionary. */
typedef struct {
  /* Variables. */
  var_t *variables;
  int v_count;
  int v_space;
} symbol_dict_t;

/* Prototypes. */
symbol_dict_t *dict_new (void);
void dict_free (symbol_dict_t *dict);

int dict_lookup (symbol_dict_t *calc, const char *name, double defval);
double* dict_variable (symbol_dict_t *calc, const char *var_name);
double* dict_new_variable (symbol_dict_t *calc, const char *var_name, double defval);

#endif /* Included_DICT_H */
