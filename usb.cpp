
#include "usb.h"

/*
sends first packet which contains file name and packets number then sends file data packets, Each packet is 64-Bytes, 
If the file size not O(64) bytes then there is a not complete packet, so it’s completed with zeros and sent to MCU as last packet.
To verify data, The PC sends the packet then receive it from MCU compares it 
If correct send packet of ones to MCU, else send packet of zeros and resend the data packet,
If not correct packet repeated for 10 times, an error massage returns to the user.
input parameters :
arr   is the file stream to be sent
name  is the file name

if success returns 1 else returns 0


*/

int usb_send( unsigned char *arr, unsigned char *name)
{
     libusb_context *ctx = NULL;
     libusb_device_handle *handle;

    int length=64,
        r = 1,
        read_64=0,
        k=0,
        actual_write=0;     //represents number of actual bytes which were send to MCU or Recieved

    unsigned int    ret,
        i=0,
        last_flag=0,
        counter_64=0;       //counts number of packets each packet size is 64BYTE

    unsigned long int chars_counter=0;
    unsigned char name_size[64]={0};            //to carry name and size of target circuit
    unsigned char last_64[64]={0};              //to carry last non-completed packet (complete with zeros)
    unsigned char zeros[64]={0};                //sent to MCU if verification failed to resend the packet
    unsigned char ones[64]={1,1,1,1,1,1,1,1};   //sent to MCU if verification Done
    unsigned char read_arr[64]={0};             //temporary array to carry read packets
    unsigned int Error_Count =0;
    
	libusb_init(&ctx);
	libusb_set_debug(ctx, 3);

    //Open Device with VendorID and ProductID
	handle = libusb_open_device_with_vid_pid(ctx,
				USB_VENDOR_ID, USB_PRODUCT_ID);

	if (!handle) {
		return 0;
	}

	//Claim Interface 0 from the device the device must be claimed before sending
    r = libusb_claim_interface(handle, 0);
	if (r < 0) {
		return 0;
	}

    
    while(name[i]!='\0')
    {
        name_size[i]=name[i];
        i++;
    }

    name_size[i++]='\0';

    while(arr[chars_counter] != '\0' || arr[chars_counter+1] != '\0' || arr[chars_counter+2] != '\0' ||  arr[chars_counter+3] != '\0')
    {
        chars_counter+=4;
    }
    chars_counter+=4;

    if(chars_counter%64!=0)   //there is a need to send last bulk & complete it with zeros
    {
        last_flag=1;
        counter_64++;        //If there is a fraction increase packets counter
    }

    counter_64+=(chars_counter/64);
    
    for(k=4;k>=0;k--)
    {
        name_size[i+k]=(counter_64%10);
        counter_64=counter_64/10;
    }

    counter_64=(chars_counter/64);

	/////To Send Name and Size
	ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, name_size, length,
			&actual_write, USB_TIMEOUT);

	
	for(k=0;k<counter_64;)
    {
        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, arr+64*k, length,
                &actual_write, USB_TIMEOUT);

        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_IN, read_arr, length,
                &actual_write, USB_TIMEOUT);

                for(int i=0;i<64;i++)
                {
                    if(arr[64*k+i]!=read_arr[i])
                    {

                        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, zeros, length,
                        &actual_write, USB_TIMEOUT);
                        Error_Count++;
                        if(Error_Count==10)
                          return 0;

                        break;
                    }
                    if(i==63)
                    {
                    Error_Count=0;
                    ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, ones, length,
                    &actual_write, USB_TIMEOUT);

                    k++;
                    }
                }




    }

	int z=0;
    if(last_flag)
    while((arr[counter_64*64+z]!='\0' || arr[counter_64*64+z+1]!='\0'|| arr[counter_64*64+z+2]!='\0' || arr[counter_64*64+z+3]!='\0'))
    {
        last_64[z]	=arr[counter_64*64+z];
		last_64[z+1]=arr[counter_64*64+z+1];
		last_64[z+2]=arr[counter_64*64+z+2];
		last_64[z+3]=arr[counter_64*64+z+3];
        z+=4;
    }

    while(last_flag)
    {

        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, last_64, length,
                &actual_write, USB_TIMEOUT);

        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_IN, read_arr, length,
                &actual_write, USB_TIMEOUT);


                for(int i=0;i<64;i++)
                {
                    if(last_64[i]!=read_arr[i])
                    {

                        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, zeros, length,
                        &actual_write, USB_TIMEOUT);
                        Error_Count++;
                        if(Error_Count==10)
                          return 0;

                        break;
                    }
                    if(i==63)
                    {
                    Error_Count=0;
                    ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, ones, length,
                    &actual_write, USB_TIMEOUT);
                        last_flag=0;
                    }
                }

    }

    printf("\nVerification DONE \n");       

    libusb_close(handle);
	libusb_exit(NULL);

    return 1;

}
