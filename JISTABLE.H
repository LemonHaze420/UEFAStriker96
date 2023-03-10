




BYTE jis_to_ascii_table[]={
//ascii codes, starting at 0x8140

	32,	//	space == jis 0x8140
	32,32,
	44,	//	,	== jis 0x8143 etc
	46,	//	.
	32,
	58,	//	:
	59,	//	;
	63,	//	?
	33,	//	!
	32,32,32,32, 32,
	94,	//	^

//8150
	126,	//	~
	95,	//	_
	32,32,32,32,32,32,32,32,32,
	45,	// alone in the dark requires this to be a minus ( 0x815b )
	32,32,
	47,	// backslash
	32,

//8160
	32,32,
	124,	//	|
	32,32,
	96,	//	`
	39,	//	'
	32,
	34,	//	"
	40,	//	(
	41,	//	)
	32,32,
	91,	//	[
	93,	//	]
	123,	//	{

//8170
	125,	//	}
	60,	//	<
	62,	//	>
	32,32,32,32,32,32,32,32,
	43,	//	+
	45,	//	-
	32,32,32,

	32,
	61,	//	=
	32,32,32,32,32,32,32,32,32,32,32,32,32,
	92,	//	\

	36,	//	$
	32,
	32,
	37,	//	%
	35,	//	#
	38,	//	&
	42,	//	)
	64,	//	@
	32,32,32,32,32,32,32,32,

	//1a0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1b0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1c0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1d0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1e0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1f0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//200
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//210
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//220
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//230
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//240
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	48, //0

	//250
	49, //1-9
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	32,32,32,32,32,32,32,

	//260
	65,	//'A-Z'
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,

	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	32,32,32,32,32,32,

	32,
	97,		//'a-z'
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,

	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	32,
	32,
	32,
	32,
	32,

};

UWORD ascii_to_jis_table[]={
// jis codes, from ascii 32-126
	0x8140,0x8149,0x8168,0x8194,0x8190,0x8193,0x8195,0x8166,0x8169,0x816a,
	0x8196,0x817b,0x8143,0x817c,0x8144,0x815e,0x824f,0x8250,0x8251,0x8252,
	0x8253,0x8254,0x8255,0x8256,0x8257,0x8258,0x8146,0x8147,0x8171,0x8181,
	0x8172,0x8148,0x8197,0x8260,0x8261,0x8262,0x8263,0x8264,0x8265,0x8266,
	0x8267,0x8268,0x8269,0x826a,0x826b,0x826c,0x826d,0x826e,0x826f,0x8270,
	0x8271,0x8272,0x8273,0x8274,0x8275,0x8276,0x8277,0x8278,0x8279,0x816d,
	0x818f,0x816e,0x814f,0x8151,0x8165,0x8281,0x8282,0x8283,0x8284,0x8285,
	0x8286,0x8287,0x8288,0x8289,0x828a,0x828b,0x828c,0x828d,0x828e,0x828f,
	0x8290,0x8291,0x8292,0x8293,0x8294,0x8295,0x8296,0x8297,0x8298,0x8299,
	0x829a,0x816f,0x8162,0x8170,0x8150,
};




BYTE convert_jis_to_ascii ( UWORD jis )
{
UWORD jis_code,ascii_out;
LONG del;

BYTE text[2];

	jis_code = (jis&0xff)*256;
	jis_code+= jis/256;

	if ( (jis_code <0x8140) || (jis_code >0x829a) )
		{
		return 0; // illegal jis code
		}
	else
		{
		ascii_out = jis_to_ascii_table[jis_code-0x8140];
		if ( ascii_out == 0 )
			{
			printf("jis_code %x can't be converted ,",jis_code);
			printf("substituting a '-'\n");
			ascii_out='-';
			}

		return ascii_out;



		}

} 



UWORD convert_ascii_to_jis ( BYTE ascii )
{
	if ( (ascii<32) || (ascii >126) )
		return 0;	//illegal ascii code
	else
		{
		return ascii_to_jis_table[ascii-32];
		}
} 





void convert_string_from_jis_to_ascii ( UWORD *src, BYTE *dest )
{
	while( 1 )
		{
		*dest = convert_jis_to_ascii( *src );
		if ( *dest == 0 )
			break;
		else
			{
			src++;
			dest++;
			}
		
		}
}

void convert_string_from_ascii_to_jis ( BYTE *src, UWORD *dest )
{
UWORD jis_code;

	while( 1 )
		{
		jis_code = convert_ascii_to_jis( *src );
		if ( jis_code == 0 )
			break;
		else
			{
			*dest = ((jis_code&0xff)<<8) + ((jis_code/256)<<0);
			src++;
			dest++;
			*dest=0;
			}
		
		}
}



