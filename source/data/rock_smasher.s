
@{{BLOCK(rock_smasher)

@=======================================================================
@
@	rock_smasher, 1024x8@4, 
@	Transparent color : FF,00,FF
@	+ palette 256 entries, not compressed
@	+ 128 tiles not compressed
@	Total size: 512 + 4096 = 4608
@
@	Time-stamp: 2022-09-30, 10:12:26
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global rock_smasherTiles		@ 4096 unsigned chars
	.hidden rock_smasherTiles
rock_smasherTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000220,0x00022220,0x02222200,0x22112200,0x21112000,0x21122000,0x22220000
	.word 0x00000000,0x00000000,0x22222000,0x22222200,0x11112222,0x11111222,0x11111122,0x22211112
	.word 0x00000000,0x00000000,0x22222222,0x22222222,0x22112211,0x22112211,0x22112211,0x22222222
	.word 0x00000000,0x00000000,0x00222222,0x02222222,0x22222211,0x22222111,0x22221111,0x12222222
	.word 0x20000000,0x20000000,0x22000000,0x12000000,0x12200000,0x22200002,0x22222222,0x12211221
	.word 0x00000002,0x00000002,0x00000022,0x00000021,0x00000221,0x20000222,0x22222222,0x12211221

	.word 0x00000000,0x00000000,0x22222200,0x22222220,0x11222222,0x11122222,0x11112222,0x22222221
	.word 0x00000000,0x00000000,0x22222222,0x22222222,0x11221122,0x11221122,0x11221122,0x22222222
	.word 0x00000000,0x00000000,0x00022222,0x00222222,0x22221111,0x22211111,0x22111111,0x21111222
	.word 0x00000000,0x02200000,0x02222000,0x00222220,0x00221122,0x00021112,0x00022112,0x00002222
	.word 0x22220000,0x12220000,0x11222000,0x11122200,0x11112200,0x21112200,0x21112200,0x21112200
	.word 0x22222111,0x00022211,0x00000221,0x00000022,0x00000022,0x00000002,0x00000002,0x00000002
	.word 0x22222222,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x22222222,0x11112222,0x22222222,0x22222220,0x22222200,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x22221111,0x22222222,0x02222222,0x00222222,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x00000002,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x11122222,0x11222000,0x12200000,0x22000000,0x22000000,0x20000000,0x20000000,0x20000000
	.word 0x00002222,0x00002221,0x00022211,0x00222111,0x00221111,0x00221112,0x00221112,0x00221112
	.word 0x21112200,0x21112200,0x22222200,0x22222200,0x21112200,0x21112200,0x22222200,0x22222200
	.word 0x00000002,0x00000022,0x00000022,0x00000222,0x00022222,0x22222222,0x22222222,0x00022222

	.word 0x22222111,0x11122211,0x11111221,0x11111122,0x33333322,0x33333312,0x33223312,0x33223312
	.word 0x33333312,0x33333322,0x11111122,0x11111222,0x11122222,0x22222222,0x22222222,0x00022222
	.word 0x33333222,0x33333322,0x33333322,0x33333332,0x33223332,0x33223332,0x33333332,0x33333322
	.word 0x33333322,0x33333332,0x33223332,0x33223332,0x33333332,0x33333322,0x33333322,0x33333222
	.word 0x00022222,0x22222222,0x22222222,0x33322222,0x33333222,0x33333322,0x33333322,0x33333332
	.word 0x33223332,0x33223332,0x33333332,0x33333322,0x33333322,0x33333221,0x33322211,0x22222111
	.word 0x20000000,0x22000000,0x22000000,0x22200000,0x22222000,0x22222222,0x22222222,0x22222000
	.word 0x00221112,0x00221112,0x00222222,0x00222222,0x00221112,0x00221112,0x00222222,0x00222222

	.word 0x11112200,0x11112200,0x11122200,0x11222200,0x22222200,0x22222200,0x22222000,0x12220000
	.word 0x00000222,0x00000022,0x00000022,0x00000002,0x00000002,0x00000002,0x00000002,0x00000022
	.word 0x22222111,0x11122211,0x11111221,0x11111122,0x33333322,0x33223312,0x32332312,0x32332312
	.word 0x33223312,0x33333322,0x11111122,0x11111222,0x11122222,0x22222222,0x22222222,0x00022222
	.word 0x33333222,0x33333322,0x33333322,0x33223332,0x32332332,0x32332332,0x33223332,0x33333322
	.word 0x33333322,0x33223332,0x32332332,0x32332332,0x33223332,0x33333322,0x33333322,0x33333222
	.word 0x00022222,0x22222222,0x22222222,0x33322222,0x33333222,0x33333322,0x33333322,0x33223332
	.word 0x32332332,0x32332332,0x33223332,0x33333322,0x33333322,0x33333221,0x33322211,0x22222111

	.word 0x22200000,0x22000000,0x22000000,0x20000000,0x20000000,0x20000000,0x20000000,0x22000000
	.word 0x00221111,0x00221111,0x00222111,0x00222211,0x00222222,0x00222222,0x00022222,0x00002221
	.word 0x12222200,0x22211222,0x22211222,0x12222200,0x12220000,0x22220000,0x22222200,0x12211222
	.word 0x00000022,0x00000222,0x00022222,0x22222222,0x22222222,0x22112112,0x22112112,0x22112112
	.word 0x22222111,0x11122211,0x11111221,0x11111122,0x11221122,0x12112112,0x21111212,0x21111212
	.word 0x12112112,0x11221122,0x11111122,0x11111222,0x11122222,0x22222222,0x22222222,0x00022222
	.word 0x33333222,0x33333322,0x33223322,0x32332332,0x23333232,0x23333232,0x32332332,0x33223322
	.word 0x33223322,0x32332332,0x23333232,0x23333232,0x32332332,0x33223322,0x33333322,0x33333222

	.word 0x00022222,0x22222222,0x22222222,0x33322222,0x33333222,0x33333322,0x33223322,0x32332332
	.word 0x23333232,0x23333232,0x32332332,0x33223322,0x33333322,0x33333221,0x33322211,0x22222111
	.word 0x22000000,0x22200000,0x22222000,0x22222222,0x22222222,0x21121122,0x21121122,0x21121122
	.word 0x00222221,0x22211222,0x22211222,0x00222221,0x00002221,0x00002222,0x00222222,0x22211221
	.word 0x12211222,0x22222200,0x22220000,0x12220000,0x12222200,0x22211222,0x22211222,0x12222200
	.word 0x22112112,0x22112112,0x22112112,0x22222222,0x22222222,0x00022222,0x00000222,0x00000022
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x21121122,0x21121122,0x21121122,0x22222222,0x22222222,0x22222000,0x22200000,0x22000000
	.word 0x22211221,0x00222222,0x00002222,0x00002221,0x00222221,0x22211222,0x22211222,0x00222221
	.word 0x12220000,0x22222000,0x22222200,0x22222200,0x11222200,0x11122200,0x11112200,0x11112200
	.word 0x00000022,0x00000002,0x00000002,0x00000002,0x00000002,0x00000022,0x00000022,0x00000222

	.word 0x11122222,0x11222111,0x12211111,0x22111111,0x22333333,0x21333333,0x21332233,0x21332233
	.word 0x21333333,0x22333333,0x22111111,0x22211111,0x22222111,0x22222222,0x22222222,0x22222000
	.word 0x22233333,0x22333333,0x22333333,0x23333333,0x23332233,0x23332233,0x23333333,0x22333333
	.word 0x22333333,0x23333333,0x23332233,0x23332233,0x23333333,0x22333333,0x22333333,0x22233333
	.word 0x22222000,0x22222222,0x22222222,0x22222333,0x22233333,0x22333333,0x22333333,0x23333333
	.word 0x23332233,0x23332233,0x23333333,0x22333333,0x22333333,0x12233333,0x11222333,0x11122222
	.word 0x22000000,0x20000000,0x20000000,0x20000000,0x20000000,0x22000000,0x22000000,0x22200000
	.word 0x00002221,0x00022222,0x00222222,0x00222222,0x00222211,0x00222111,0x00221111,0x00221111

	.word 0x22222200,0x22222200,0x21112200,0x21112200,0x22222200,0x22222200,0x21112200,0x21112200
	.word 0x00022222,0x22222222,0x22222222,0x00022222,0x00000222,0x00000022,0x00000022,0x00000002
	.word 0x11122222,0x11222111,0x12211111,0x22111111,0x22333333,0x21332233,0x21323323,0x21323323
	.word 0x21332233,0x22333333,0x22111111,0x22211111,0x22222111,0x22222222,0x22222222,0x22222000
	.word 0x22233333,0x22333333,0x22333333,0x23332233,0x23323323,0x23323323,0x23332233,0x22333333
	.word 0x22333333,0x23332233,0x23323323,0x23323323,0x23332233,0x22333333,0x22333333,0x22233333
	.word 0x22222000,0x22222222,0x22222222,0x22222333,0x22233333,0x22333333,0x22333333,0x23332233
	.word 0x23323323,0x23323323,0x23332233,0x22333333,0x22333333,0x12233333,0x11222333,0x11122222

	.word 0x22222000,0x22222222,0x22222222,0x22222000,0x22200000,0x22000000,0x22000000,0x20000000
	.word 0x00222222,0x00222222,0x00221112,0x00221112,0x00222222,0x00222222,0x00221112,0x00221112
	.word 0x21112200,0x21112200,0x21112200,0x11112200,0x11122200,0x11222000,0x12220000,0x22220000
	.word 0x00000002,0x00000002,0x00000002,0x00000022,0x00000022,0x00000221,0x00022211,0x22222111
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x22222222
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x20000000,0x22222222
	.word 0x00000000,0x00000000,0x00000000,0x00222200,0x00222220,0x00222222,0x00222222,0x00002222
	.word 0x00000000,0x00000000,0x00000000,0x00222200,0x02222200,0x22222200,0x22222200,0x22220000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000002,0x22222222
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x22222222
	.word 0x20000000,0x20000000,0x20000000,0x22000000,0x22000000,0x12200000,0x11222000,0x11122222
	.word 0x00221112,0x00221112,0x00221112,0x00221111,0x00222111,0x00022211,0x00002221,0x00002222
	.word 0x22220000,0x21122000,0x21112000,0x22112200,0x02222200,0x00022220,0x00000220,0x00000000
	.word 0x22211112,0x11111122,0x11111222,0x11112222,0x22222200,0x22222000,0x00000000,0x00000000
	.word 0x22222222,0x22112211,0x22112211,0x22112211,0x22222222,0x22222222,0x00000000,0x00000000
	.word 0x12222222,0x22221111,0x22222111,0x22222211,0x02222222,0x00222222,0x00000000,0x00000000

	.word 0x00000221,0x00000022,0x00000002,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x12200000,0x22000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222221,0x11112222,0x11122222,0x11222222,0x22222220,0x22222200,0x00000000,0x00000000
	.word 0x22222222,0x11221122,0x11221122,0x11221122,0x22222222,0x22222222,0x00000000,0x00000000
	.word 0x21111222,0x22111111,0x22211111,0x22221111,0x00222222,0x00022222,0x00000000,0x00000000
	.word 0x00002222,0x00022112,0x00021112,0x00221122,0x00222220,0x02222000,0x02200000,0x00000000
	.word 0x11122222,0x11222111,0x12211111,0x22111111,0x22112211,0x21121121,0x21211112,0x21211112
	.word 0x21121121,0x22112211,0x22111111,0x22211111,0x22222111,0x22222222,0x22222222,0x22222000

	.word 0x22233333,0x22333333,0x22332233,0x23323323,0x23233332,0x23233332,0x23323323,0x22332233
	.word 0x22332233,0x23323323,0x23233332,0x23233332,0x23323323,0x22332233,0x22333333,0x22233333
	.word 0x22222000,0x22222222,0x22222222,0x22222333,0x22233333,0x22333333,0x22332233,0x23323323
	.word 0x23233332,0x23233332,0x23323323,0x22332233,0x22333333,0x12233333,0x11222333,0x11122222
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global rock_smasherPal		@ 512 unsigned chars
	.hidden rock_smasherPal
rock_smasherPal:
	.hword 0x7C1F,0x0000,0x7FFF,0x0800,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(rock_smasher)