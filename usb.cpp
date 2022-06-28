#include "usb.h"

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
    unsigned char zeros[64]={0};                //sent to MCU if verification failed to resend the file
    unsigned char ones[64]={1,1,1,1,1,1,1,1};   //sent to MCU if verification Done
    unsigned char read_arr[64]={0};             //temporary array to carry read packets

    /////////////////////////////////////////
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

    ////////////////////////////////////////

    while(name[i]!='\0')
    {
        name_size[i]=name[i];
        i++;
    }

    name_size[i++]='\0';

    while(arr[chars_counter] != '\0' && arr[chars_counter+1] != '\0' && arr[chars_counter+2] != '\0' &&  arr[chars_counter+3] != '\0')
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


	ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, name_size, length,
			&actual_write, USB_TIMEOUT);


    for(k=0;k<counter_64;k++)
    {
        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, arr+64*k, length,
                &actual_write, USB_TIMEOUT);
    }

    int z=0;
    while(last_flag && (arr[counter_64*64+z]!='\0'))
    {
        last_64[z]=arr[counter_64*64+z];
        z++;
    }
    if(last_flag)
    {

        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, last_64, length,
                &actual_write, USB_TIMEOUT);
    }

    unsigned char *ptr;
    ptr = new unsigned char[chars_counter+1];

    printf("Write : %s \n\n",arr);           //will be deleted


    //////////////////////////Verification///////////////////////
    while(read_64<counter_64)
    {
        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_IN, read_arr, length,
                &actual_write, USB_TIMEOUT);

        for( k=0;k<64;k++)
        {

            *(ptr+64*read_64+k)=read_arr[k];

        }

    read_64++;
    }

    if(last_flag)
    {
        ret = libusb_bulk_transfer(handle, USB_ENDPOINT_IN, read_arr, length,
                &actual_write, USB_TIMEOUT);

        for(k=0;k<(chars_counter-read_64*64);k++)
        {
            *(ptr+read_64*64+k)=read_arr[k];
        }
        ptr[read_64*64+k]='\0';

    }

    printf("Read  : %s \n\n",ptr);      //will be deleted


    k=0;
    while(k<=chars_counter)
    {
        if(ptr[k]!=arr[k])
        {
            printf("\nVerification Error \n");       //will be deleted
            ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, zeros, length,
                &actual_write, USB_TIMEOUT);
            return 0;
        }
        k++;
    }


    ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, ones, length,
                &actual_write, USB_TIMEOUT);
/////////////////////////////////////////////////////////////

    printf("\nVerification DONE \n");       //will be deleted

    libusb_close(handle);
	libusb_exit(NULL);

    return 1;

}


