Want to make minimal bootloader with AES CRC and UART
Processor after reset uses vector table of bootloader to find msp and reset handler
Find MSP and reset handler
Runs reset handler
Reset handler is the first piece of code to be executed.
It transfers data initialized variables from flash to ram
it initializes the data and zeroes the bss
it then jumps to the app using the apps vector table
it then sets the msp to the apps msp and runs the apps reset handler

So far i started by making linker scripts for app and bootloader
Script is made up of ENTRY MEMORY and SECTIONS
ENTRY gets the argument of RESET_HANDLER which is a symbol for the reset handler it resolves this to an address when linking
In memory we declared the origin and length of flash and ram
i set the length for the bootloader to be 32KB
As for sections we have .isr_vector, .text, .data ,.bss
.isr_vector contains the .isr_vector of all files 
.text contains the .text of all object files and .rodata 
.data contains the .data of all object files
.bss contains the .bss of all object files as well as COMMON which is left over uninitialized symbols
we also declared _estack, _etext, _sdata, _edata, _sbss and _ebss to mark start and end of each section as well as _sidata which symbolizes the data in flash
we also write the lma and vma of each section
Load memory address and virtual memory address
LMA is where the firmware is taken from is and vma is where we want to place it

Next we moved on to the startup code.
We declare the _estack as an extern to tell the compiler that there is such a variable outside the current file
we then made a vector table which is basically an array for MSP and the reset handler as well as all the other IRQ exceptions
For now well focus on the reset handler
So we extern declare all the start and ends of the sections we declared in the linker script
We then create pointers to the addresses of _sdata, _edata, _sbss and _ebss as well as _sidata so that we can begin to initialize data by copying _sidata at Flash over to _sdata at RAM as well as begin zeroing all the .bss
After that we call main() and add a while(1) infinite loop for safety in the unlikely event that main() returns
The vector table is given the attribute and named as .isr_vector
We also give a weak function/alias to the Interrupt ande exception addresses as Default_Handler still needs to be completed

Had a bug where reset handler was crashing. PC went to 0x0 instead of the breakpoint in reset handler. Used gdb to debug  Turns out i overshot the size of _estack due to thinking that RAM was 128kb instead 96 so it initialized to an invalid address

we now move on to bootloader.c where we will perform the jump from the bootloader to the app

uint32_t app_msp = *(uint32_t*)(APP_BASE);
uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U);
We want the variables to store the values not the addresses

In bootloader.c we create a jump to app function where we set the APP MSP and reset handler, disable interrupts, set the VTOR to the apps vector table and set the msp to the apps msp we then jump to the app reset handler

A bug that we got from this is that the call of the pointer function of the apps reset handler is being treated as a regular function call and hence the stack gets a return address and mixes the bootloader stack with the app stack The register instruction is blx but it should be bx. So we first declare apps msp and reset handler address, make a function pointer for the apps reset handler, disable interrupts, make VTOR the apps vector table set the apps msp and then call the reset handler

The jump is over 

we know implement the split decision of whether the bootloader stays in bootmode or jumps to the app with help of pressing the user button on the nucleo board. Currently we made do with a primitive delay for the user to press the button using a while loop but it should probably be proper Systick or something. For now well move on to the verification of firmware

CRC Cyclic Redundancy check is used to verify integrity of the firmware before the bootloader decides to jump to it.
It seems that sender and receiver agree on a divisor. The CRC that is the bits added to the firmware to check its integrity is divisor - 1 bits. So we first add divisor - 1 bits of 0 to the firmware. Then perform binary division of the firmware using the divisor. Binary division is just XORing and the remainder is the CRC. 

To implement this i need to now change the memory layout of the app. Since im gonna be adding a firmware header on top of the app

the struct firmware header contains tthe sentinel, device_id, length, version and crc. Since it would make a circular reference if we try to check crc of the app with the header included we will first set it to 0xffffffff and then fill it after calculation.

We create the struct give it the attribute of used so that the compiler doesn't optimize it away and also give it a section name since we are placing it after vector table of the app. 

Next we need to write the crc function itself.

CRC
we need byte mask and crc. We take data and length as parameters.
a for loop where we iterate through length
we transfer data to byte and xor the byte with crc.
we then for loop through the byte itself to go through individual bits
We 2s complement the crc ANDed with 1.
We convert crc to crc right shifted one XORed with the polynomial ANDed with mask
We return crc inverted

Now we need to create the tool for adding the crc and length into the image before it gets sent to the bootloader.
Im using c
WE use fopen to get the bin file through the file path
We use fseek and ftell to get the size
We then create a buffer.
We then use a for loop to find the header using the sentinel
We then set length to the size and the crc to 0xffffffff
We then run the crc32 algo we made over the entirety of the image.
We then insert the crc into the firmware header and return a True or False to indicate success or failure respectively

We've added the crc logic to the bootloader main.c 
We've used the app base and vector table size to reach the header 
The app size we defined using the value we gave for the linker script

We then implemented the usart driver.
This was very arduos because it wouldn't work for a long time until i gave up and just copy pasted an implmenetation of the driver that i had made that worked long time ago.

Notes for UART: Consider implementing ring buffer so to protect against packet loss

So now we move on to the flash driver for flash programming and erase.

As is for any peripheral we must enable the clock for the bus containing the flash peripheral which is AHB1.

In order to perform any flash operation on the flash registers we must first input 2 Keys in to the FLASH_KEYR register. 

In order to lock the flash peripheral we then write a bit to FLASH_CR's 31st bit place.

Erase
Before erasing we must first know some things. The STM32L4 erases flash by pages. Pages are 2KB each. This is called the erase granularity.

We will first wait for the FLASH_SR_BSY bit to be 0, which means we wait for any other flash operation to be finished. We then clear all previous error flags by writing one to them. We enable the page erase (PER) bit. We clear the page number selection (PNB) bits before inputing the page numbers. Formula for page numbers is (Page_Address - Flash_Base) / 0x800U. 0x800U is the size of a page which is 2KB. We then set the Start bit (STRT).
We again wait for the operation to finish using the FLASH_SR_BSY bit. If any of the error flags are set then we clear the page erase bit (PER) and exit. If not then we wait for the End of Operation (EOP) bit to be set and then clear it and clear the page erase bit (PER).

This above function is only erasing erasing pages hence to clear the whole flash, I call the function in a for loop with APP_BASE being the iterator and it being incrememnted by page size every iteration. The iterator will be the page address parameter for the function being called.

Write

Unlike the Erase function, The write function is not so straight forward. The bootloader will be receiving the bytes of the firmware through uart, Since the granularity of the write function is 8 bytes not only does it take too much RAM but it also wont take bytes from the UART since it sends one byte at a time. Instead we store the bytes in RAM until enough are collected for a write operation.
Thus, the write operation is divided into the following steps.
1)Create a buffer to store bytes in ram. I have created a uint8_t buffer of 256 bytes, since the size of uint8_t is 1 byte.
2)Write a function that uses the usart read byte function in a for loop that loops 256 times and stores each read byte into the buffer
3)Write a function that writes the bytes of the buffer into flash

The Write buffer to flash function has similar steps to the erase one. We intialize the flash peripheral, we unlock the flash peripheral. We clear all previous error flags and set the Programming flag (PG). The STM32 reference manual has a specific requirement for writing to flash. In which we do 2 32-bit writes at a time. So we a for loop that loops 256 bytes. Create a 32 bit word. Push 4 bytes at a time from the buffer into the word and write it, increment the flash address pointer, push another 4 bytes from the buffer into word and write it into flash and then the one iteration of the loop is over. Then we wait for the busy bit (BSY) to clear and check the error flags. Clear the End of Operation bit (EOP) ,Clear the programming bit (PG) and exit.

USART Protocol.
For the usart protocol, I decided to have the pc send the request, the bootloader will send a ready message and start accepting the bytes in packets of 256.
If the packet is accepted and written into the flash properly the bootloader sends an acknowledgement byte (ACK) and if it doesn't it sends (NACK) and the pc tries again. The maximum amount of retries is 5 per packet.

The function has 2 loops. In the first loop the bootloader keeps accepting bytes until it has received the firmware_header, Through which it can get the length of the firmware.
If the length of the header has been corrupted it will give a Failure message to the PC (F). If not then we'll use the length to calculate the length of the remaining firmware by subtracting the length from bytes received after the first loop. 
The second loop is used to accept the remaining bytes.

Once its done the function returns and we call the verify function which calls the crc function and compares the calculated crc to the crc written in the firmware header,
If the match then the bootloader sends the success message (S) if not then it send the Failure message (F).

2 Bugs were found. One where the 4 byte would not get accepted by the buffer and the PC uart script would fail. I used gdb and put a breakpoint at the loop of the taking bytes from the usart. I found out that this was due to the fact that i had activated the Interrupt for USART. So the polling usart protocol that I implemented and the interrupt usart were competing for bytes. For this version of the bootloader we will stick to polling usart.

Another bug was that the 3rd packet was being rejected by the bootloader with the message (NACK). Using gdb i put a breakpoint on all the possible return statements of the functions after i reached the return statement i discovered that this was the return statement of the if condition where the function returns if the length mentioned in the firmware header was bigger than the constant APP_SIZE. This was due to the fact that i had not run the firmware_header script which inputs the firmware_length and crc to the firmware header after calculating the crc. So the function read the default length of the unedited firmware binary and found firmware_length = 0xffffffff. 

This is the first version of this bootloader.
Future updates for version 2.
1)DMA for usart
2)Interrupts
3)AES encryption








