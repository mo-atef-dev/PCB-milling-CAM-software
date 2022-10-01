
#ifndef USB
#define USB

#define USB_VENDOR_ID	    0x1111                          /* USB vendor ID used by the device  */
#define USB_PRODUCT_ID	    0x0001                          /* USB product ID used by the device */
#define USB_ENDPOINT_IN	    (LIBUSB_ENDPOINT_IN  | 0x81)    /* endpoint in address 				 */
#define USB_ENDPOINT_OUT	(LIBUSB_ENDPOINT_OUT | 0x01)    /* endpoint out address 			 */
#define USB_TIMEOUT	         0                              /* Connection timeout (in ms) 		 */

#include <libusb.h>
#include <stdio.h>

/*
int usb_send( unsigned char *arr, unsigned char *name);
Summary		: This will send an array(file) from PC to MCU with a name.
Param arr	: The file data to be sent.
Param name	: The file name.
Param stream: The stream to set.
Returns		: 0 if there was a problem, 1 if everything went okay.
Note 1		: the arr must end with 4 zeros.
Note 2		: put the function call in while(!usb_end()); to recall its self if data wasn't correctly send
*/

int usb_send( unsigned char *arr, unsigned char *name);

#endif
