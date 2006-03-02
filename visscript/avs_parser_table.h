#ifndef _AVS_PARSERTABLES_H
#define _AVS_PARSERTABLES_H 1

#define AVS_PARSER_STATES 45
#define AVS_PARSER_TOKENS 19

static AvsParserState parserstate[AVS_PARSER_STATES][AVS_PARSER_TOKENS] = {
	/*  0 */ {
		R( 4, 1, 0),R( 4, 1, 0),R( 4, 1, 0),S( 1, 0, 0),S( 2, 0, 0),R( 4, 1, 0),
		R( 4, 1, 0),R( 4, 1, 0),R( 4, 1, 0),S( 3, 0, 0),S( 4, 0, 0),R( 4, 1, 0),
		R( 4, 1, 0),R( 4, 1, 0),R( 4, 1, 0),S( 5, 0, 0),S( 6, 0, 0),R( 4, 1, 0),
		R( 4, 1, 0),
	},
	/*  1 */ {
		R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),
		R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),
		R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),R(26, 8, 1),S(12, 0, 0),R(26, 8, 1),
		R(26, 8, 1),
	},
	/*  2 */ {
		D(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),
		R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),
		R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),R(25, 8, 1),
		R(25, 8, 1),
	},
	/*  3 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/*  4 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/*  5 */ {
		D( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),
		R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),
		R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),R( 7, 3, 1),
		R( 7, 3, 1),
	},
	/*  6 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/*  7 */ {
		A( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/*  8 */ {
		R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),
		R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),
		R( 2, 1, 1),R( 2, 1, 1),R( 2, 1, 1),S( 5, 0, 0),R( 2, 1, 1),R( 2, 1, 1),
		R( 2, 1, 1),
	},
	/*  9 */ {
		D( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),
		R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),
		R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),R( 3, 1, 1),
		R( 3, 1, 1),
	},
	/* 10 */ {
		R( 5, 2, 1),R( 5, 2, 1),R( 5, 2, 1),R( 5, 2, 1),R( 5, 2, 1),R( 5, 2, 1),
		S(18, 0, 0),S(19, 0, 0),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R( 5, 2, 1),R( 5, 2, 1),R( 5, 2, 1),R( 5, 2, 1),
		R( 5, 2, 1),
	},
	/* 11 */ {
		D( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),
		R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),
		R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),R( 8, 4, 1),
		R( 8, 4, 1),
	},
	/* 12 */ {
		D(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),
		R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),
		R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),R(27, 9, 0),
		R(27, 9, 0),
	},
	/* 13 */ {
		D(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),
		R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),
		R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),R(10, 4, 2),
		R(10, 4, 2),
	},
	/* 14 */ {
		D( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),
		R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),
		R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),R( 9, 4, 2),
		R( 9, 4, 2),
	},
	/* 15 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),
		S(18, 0, 0),S(19, 0, 0),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S(27, 0, 0),
		E( 0, 0, 0),
	},
	/* 16 */ {
		D( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),
		R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),
		R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),R( 1, 0, 2),
		R( 1, 0, 2),
	},
	/* 17 */ {
		R(30,10, 0),R(30,10, 0),R(30,10, 0),S( 1, 0, 0),S( 2, 0, 0),R(30,10, 0),
		R(30,10, 0),R(30,10, 0),R(30,10, 0),S( 3, 0, 0),S( 4, 0, 0),R(30,10, 0),
		R(30,10, 0),R(30,10, 0),R(30,10, 0),R(30,10, 0),S( 6, 0, 0),R(30,10, 0),
		R(30,10, 0),
	},
	/* 18 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 19 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 20 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 21 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 22 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 23 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 24 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 25 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 26 */ {
		R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),
		R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),
		R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(21, 6, 0),R(20, 5, 0),
		R(20, 5, 0),
	},
	/* 27 */ {
		D(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),
		R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),
		R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),R(19, 4, 3),
		R(19, 4, 3),
	},
	/* 28 */ {
		R(29,10, 1),R(29,10, 1),R(29,10, 1),R(29,10, 1),R(29,10, 1),R(29,10, 1),
		S(18, 0, 0),S(19, 0, 0),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(29,10, 1),R(29,10, 1),R(29,10, 1),R(29,10, 1),
		R(29,10, 1),
	},
	/* 29 */ {
		D( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),
		R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),
		R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),R( 6, 2, 3),
		R( 6, 2, 3),
	},
	/* 30 */ {
		R(18, 4, 3),R(18, 4, 3),R(18, 4, 3),R(18, 4, 3),R(18, 4, 3),R(18, 4, 3),
		S(18, 0, 0),S(19, 0, 0),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(18, 4, 3),R(18, 4, 3),R(18, 4, 3),R(18, 4, 3),
		R(18, 4, 3),
	},
	/* 31 */ {
		R(17, 4, 3),R(17, 4, 3),R(17, 4, 3),R(17, 4, 3),R(17, 4, 3),R(17, 4, 3),
		R(17, 4, 3),R(17, 4, 3),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(17, 4, 3),R(17, 4, 3),R(17, 4, 3),R(17, 4, 3),
		R(17, 4, 3),
	},
	/* 32 */ {
		R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),
		R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),R(16, 4, 3),
		R(16, 4, 3),
	},
	/* 33 */ {
		R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),
		R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),R(11, 4, 3),
		R(11, 4, 3),
	},
	/* 34 */ {
		R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),
		R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),R(12, 4, 3),
		R(12, 4, 3),
	},
	/* 35 */ {
		D(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),
		R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),
		R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),R(13, 4, 3),
		R(13, 4, 3),
	},
	/* 36 */ {
		D(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),
		R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),
		R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),R(14, 4, 3),
		R(14, 4, 3),
	},
	/* 37 */ {
		D(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),
		R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),
		R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),R(15, 4, 3),
		R(15, 4, 3),
	},
	/* 38 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S(40, 0, 0),
		S(41, 0, 0),
	},
	/* 39 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 40 */ {
		D(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),
		R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),
		R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),R(28, 8, 5),
		R(28, 8, 5),
	},
	/* 41 */ {
		D(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),
		R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),
		R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),R(23, 7, 0),
		R(23, 7, 0),
	},
	/* 42 */ {
		R(22, 5, 2),R(22, 5, 2),R(22, 5, 2),R(22, 5, 2),R(22, 5, 2),R(22, 5, 2),
		S(18, 0, 0),S(19, 0, 0),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(22, 5, 2),R(22, 5, 2),R(22, 5, 2),R(22, 5, 2),
		R(22, 5, 2),
	},
	/* 43 */ {
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 1, 0, 0),S( 2, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 3, 0, 0),S( 4, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),E( 0, 0, 0),S( 6, 0, 0),E( 0, 0, 0),
		E( 0, 0, 0),
	},
	/* 44 */ {
		R(24, 5, 4),R(24, 5, 4),R(24, 5, 4),R(24, 5, 4),R(24, 5, 4),R(24, 5, 4),
		S(18, 0, 0),S(19, 0, 0),S(20, 0, 0),S(21, 0, 0),S(22, 0, 0),S(23, 0, 0),
		S(24, 0, 0),S(25, 0, 0),R(24, 5, 4),R(24, 5, 4),R(24, 5, 4),R(24, 5, 4),
		R(24, 5, 4),
	},
};

static unsigned char parsetranslate[256] = {
	 0,  0,  3,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0, 13,  8,  0, 16, 17, 11,  9, 18, 10,  0, 12, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15,  0,  6,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7, 
};

static signed char parsergotostate[45][11] = {
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,13,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,14,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,15,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8,17,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,28,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,30,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,31,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,32,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,33,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,34,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,35,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,36,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,37,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,42,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,44,38,39,43,11,26,29, },
	{ -1, 7, 8, 9,10,38,39,43,11,26,29, },
};

static AvsParserErrorState parsererror[AVS_PARSER_STATES] = {
	{  0, 19, 0x00018618 },
	{ 14, 19, 0x00000004 },
	{  0,  0, 0x00000000 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0,  0, 0x00000000 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00000001 },
	{  0, 19, 0x00008000 },
	{  0,  0, 0x00000000 },
	{  2, 19, 0x00000ff0 },
	{  0,  0, 0x00000000 },
	{  0,  0, 0x00000000 },
	{  0,  0, 0x00000000 },
	{  0,  0, 0x00000000 },
	{  0, 19, 0x00023fc0 },
	{  0,  0, 0x00000000 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  0, 19, 0x00010618 },
	{  5, 19, 0x00003000 },
	{  0,  0, 0x00000000 },
	{  2, 19, 0x00000ff0 },
	{  0,  0, 0x00000000 },
	{  2, 19, 0x00000ff0 },
	{  0, 18, 0x00003f00 },
	{  0, 13, 0x00001e00 },
	{  0, 19, 0x00003800 },
	{  0, 19, 0x00003800 },
	{  0,  0, 0x00000000 },
	{  0,  0, 0x00000000 },
	{  0,  0, 0x00000000 },
	{  0, 19, 0x00060000 },
	{  0, 19, 0x00010618 },
	{  0,  0, 0x00000000 },
	{  0,  0, 0x00000000 },
	{  2, 19, 0x00000ff0 },
	{  0, 19, 0x00010618 },
	{  2, 19, 0x00000ff0 },
};

static char *parsertokenname[19] = {
	"$end", "error", "$undefined", "IDENTIFIER", "CONSTANT", "FUNCTION", 
	"'='", "'|'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY", "';'", 
	"'('", "')'", "','", 
};

#endif /* !_AVS_PARSERTABLES_H */
