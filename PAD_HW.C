/*****************************************************
 *																	  *
 *																	  *
 * Pad read stuff												  *
 *																	  *
 *																	  *
 *****************************************************/


/*
	how pad read works...
	inittap is setup to read 2 packets ( one for left and one for right port )
	this reads 34 bytes ( a packet ) into port_packet[0] and [1]

	if pad is connected directly to a port,
	data is read as a Pad_data structure....
	
	if a multitap is connected to a port,
	data is read as a multitap_packet structure
	This holds the data for upto four pads connected
	to the multitap...

	if Transstatus != 0xff, packet can be read.
	dataFormat top 4 bits = controller type
	eg std_pad, analog_joy, multitap etc

	Data output is as follows....
	[0-3] are for left port regardless of whether Tap connected or not
	[4-7] are for right port
	joydata[8]  = digital pad presses
	analog_lx[8] = analog controller left stick x pos
	analog_rx[8] = analog controller right stick x pos
	analog_ly[8] = analog controller left stick y pos
	analog_ry[8] = analog controller right stick y pos

*/



extern WORD pads_connected;

extern WORD pads_connected_list[];
extern WORD multitaps_connected_list[];

#include "pad_hw.h"

#define DUALSHOCK 7	//ctrller.h does not contain any ref to pad type 7 as yet so i'll put my own in.Nah


// structure for an analog pad
typedef struct
{
	UBYTE transStatus;  /* 0xff = no pad, bad pad, bad transmission */
	UBYTE dataFormat;   /* Top 4 bits = type of controller */

	UWORD digital_buttons;               /* Bit mask of plain keys. */
   UBYTE right_x;
   UBYTE right_y;
   UBYTE left_x;
   UBYTE left_y;
}Single_pad_packet;

typedef struct
{
	UBYTE transStatus;  /* 0xff = no pad, bad pad, bad transmission */
	UBYTE dataFormat;   /* Top 4 bits = type of controller */
	Single_pad_packet single_pad_packet[4];
}Multitap_packet;


typedef struct
{
	union
		{
		Multitap_packet multitap_packet;
		Single_pad_packet pad_direct_packet;
		}data;

}Controller_packet;


Controller_packet lt_port_packet;
Controller_packet rt_port_packet;


WORD cur_pad;

void setup_pads(void);
void read_controllers();

WORD joydata[8];
WORD analog_lx[8];
WORD analog_ly[8];
WORD analog_rx[8];
WORD analog_ry[8];

void init_pad_hardware()
{
	printf("doing inittap\n");
	InitTAP(&lt_port_packet,MAX_CONTROLLER_BYTES, &rt_port_packet,MAX_CONTROLLER_BYTES);
	StartTAP();

	pads_connected=0;
}



void read_machine_pad_data()
{
WORD cnt,type;

//Pad_data *pad_direct;

Single_pad_packet *single_pad;

//*** Check left port for controller or multitap ***


	pads_connected=0;
	
	for ( cnt=0;cnt<8;cnt++)
		{
		analog_lx[cnt] = analog_ly[cnt] = analog_rx[cnt] = analog_ry[cnt] = 0;
		pads_connected_list[cnt]=NO;
		}

	multitaps_connected_list[0] = 
	multitaps_connected_list[1] = 0;


//******** LEFT PORT **************
	if ( lt_port_packet.data.pad_direct_packet.transStatus != 0xff )
		{
		// can now read packet data for left port

		// find out what is plugged into directly into left port....

		type = (lt_port_packet.data.pad_direct_packet.dataFormat &0xf0) >> 4;

		//FntPrint("joy type=%d\n",type);

		if ( type == MULTI_TAP )
			{
			multitaps_connected_list[0] = YES;

		//** Found a multi tap plugged into left port **
			for ( cnt=0;cnt<4;cnt++ )
				{
				//*** Check all controllers plugged into multitap ***

				single_pad = &lt_port_packet.data.multitap_packet.single_pad_packet[cnt];

				if ( lt_port_packet.data.multitap_packet.transStatus != 0xff )
					{
					if ( single_pad->transStatus != 0xff )
						{

						//** Get type of pad in this controller port **
						type = (single_pad->dataFormat &0xf0) >> 4;

						if ( (type == STD_PAD) || (type == DUALSHOCK ) )
							{
							//lt_port_pad_types[cnt] = type; 

							pads_connected++;
							joydata [ cnt ]  =~single_pad->digital_buttons;
							pads_connected_list[cnt] = 1;

							if ( (type == DUALSHOCK) )
								{
								analog_lx[ 0+cnt ] = single_pad->left_x;
								analog_ly[ 0+cnt ] = single_pad->left_y;
								analog_rx[ 0+cnt ] = single_pad->right_x;
								analog_ry[ 0+cnt ] = single_pad->right_y;
								}
							}


						}
					}
				}
			}
		else
			{
			if ( (type == STD_PAD) || (type == DUALSHOCK) )
				{
				pads_connected++;
				pads_connected_list[0] = 1;
			//** Found an analog or standard pad plugged directly into playstation **
				single_pad = &lt_port_packet.data.pad_direct_packet;
				//lt_port_pad_types[0] = (single_pad->dataFormat &0xf0) >> 4;
				joydata [ 0 ]  =~single_pad->digital_buttons;

				if ( (type == DUALSHOCK) )
					{
					analog_lx[ 0 ] = single_pad->left_x;
					analog_ly[ 0 ] = single_pad->left_y;
					analog_rx[ 0 ] = single_pad->right_x;
					analog_ry[ 0 ] = single_pad->right_y;
					}
				}
			}

		}





//******** RIGHT PORT **************
	if ( rt_port_packet.data.pad_direct_packet.transStatus != 0xff )
		{
		// can now read packet data for left port

		// find out what is plugged into directly into left port....

		type = (rt_port_packet.data.pad_direct_packet.dataFormat &0xf0) >> 4;

		if ( type == MULTI_TAP )
			{
			multitaps_connected_list[1] = YES;

		//** Found a multi tap plugged into left port **
			for ( cnt=0;cnt<4;cnt++ )
				{
				//*** Check all controllers plugged into multitap ***

				single_pad = &rt_port_packet.data.multitap_packet.single_pad_packet[cnt];

				if ( rt_port_packet.data.multitap_packet.transStatus != 0xff )
					{
					if ( single_pad->transStatus != 0xff )
						{

						//** Get type of pad in this controller port **
						type = (single_pad->dataFormat &0xf0) >> 4;

						if ( (type == STD_PAD) || (type == DUALSHOCK))
							{
							pads_connected++;
							pads_connected_list[cnt+4] = 1;
							//rt_port_pad_types[cnt] = type; 

							joydata [ 4+cnt ]  =~single_pad->digital_buttons;

							if ( (type == DUALSHOCK) )
								{
								analog_lx[ 4+cnt ] = single_pad->left_x;
								analog_ly[ 4+cnt ] = single_pad->left_y;
								analog_rx[ 4+cnt ] = single_pad->right_x;
								analog_ry[ 4+cnt ] = single_pad->right_y;
								}
							}


						}
					}
				}
			}
		else
			{
			if ( (type == STD_PAD) || (type == DUALSHOCK) )
				{
			//** Found an analog or standard pad plugged directly into playstation **
				pads_connected++;
				pads_connected_list[4] = 1;
				single_pad = &rt_port_packet.data.pad_direct_packet;
				//rt_port_pad_types[4] = (single_pad->dataFormat &0xf0) >> 4;
				joydata [ 4 ]  =~single_pad->digital_buttons;

				if ( (type == DUALSHOCK))
					{
					analog_lx[ 4 ] = single_pad->left_x;
					analog_ly[ 4 ] = single_pad->left_y;
					analog_rx[ 4 ] = single_pad->right_x;
					analog_ry[ 4 ] = single_pad->right_y;
					}
				}
			}

		}

}


