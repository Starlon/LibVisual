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
