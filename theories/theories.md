# Booting process
The **bootloader** boots up the **kernel** by calling the external *kernel_main* function from the *kernel.c* file. The identification of this bootloader is by it's unique bootloader sign, which will define it as a bootloader for **GRUB** and that is, **0x1BADB002**.

# Memory and VGA
The memory while developing an OS, is *RAM* **( Random Access Memory)**. The address **0xB8000** is the address to the VGA video memory. We can write to this block of memory directly by creating a **char*** to that address. This memory block has two parts, for text and colour. **They can be accessed using +1 the counter in the loop while printing.**

# The Global Descriptor Table or GDT
The **GDT** is used to seperate each **memory segments** to later differenciate in them. There are 6 entries intotal in a GDT. They are as follows:
- ### Null Segment (For backup)
- ### Kernel Code Segment (For code)
    - It can be executed
    - It can be read
    - Only kernel can access it
    - It is not writable
    - It grows in upward direction
- ### Kernel Data Segment (For data)
    - It can be read
    - It cannot be executed
    - Only kernel can access it
    - It is not writable
    - It grows in upward direction
- ### User Code Segment (For code of user apps)
    - It can read
    - It can be executed
    - It has the lowest privilage and user can access it
    - It is not writable
    - It grows upward in direction
- ### User Data Segment (For data of user apps)
    - It can be read
    - It cannot be executed
    - It has the lowest privilage and user can access it
    - It is not writable
    - It grows upward in memory
- Task State Segment (For multi-tasking and switching PID's)

# Structure and Access Bytes in GDT
The **structure** of **GDT** is bit unique. The following image represents the structure of a GDT:

![image info](./pictures/gdt.png) 

In this structure, there are the following things:
- ### Limit
    - This is the address till where the segment can extend. This field is of 16 bits and the next limit block is ignored and merged into it and in place of that, we place the flags field.
- ### Base (Low)
    - This is the low 16 bits of the address.
- ### Base (Middle)
    - This is the middle 8 bits of the address.
- ### Access Bytes
    - These are the 8 bits of the entry which defines the type, access, direction of growth and other useful things for the entry.
- ### Flags
    - As I told earlier, we skip the limit after access bytes. These flags are for some other useful things which access bytes don't define. One of them is merging the base addresses spread in the entry.
- ### Base (High)
    - These are the high 8 bits of the address.In total, the base is of 32 bits.

## Access Bytes
Now, we have a specific section to talk about ***access bytes*** as they are the most complex field in the GDT. The following is the structure of a access bit that itself defines the important fields such as, type, access, direction of growth etc.

![image info](./pictures/access_bytes.png)

The following fields are present in it:
- ### P (Present)
    - This tells if the segment is present or not. It must be set to (1) if it is a valid segment. The null segment is just for spacing and backup, hence it doesn't has it.
- ### DPL (Privilage Level)
    - It tells the privilage level of the segment. If it is (0), it has the highest privilage or the kernel privilage. If it is (3) or in binary, (11), it has the lowest privilage or the user privilage.
- ### S (Segment Type)
    - It tells the type of the segment. If it is (0), it has to be a system segment (like Task State Segment). If it is (1), it has to be a code or data segment.
- ### E (Executable Bit)
    - It tells wheater the segment is executable or not. If it is (0), the segment cannot be executed and if it is (1), it can be executed.
- ### DC (Direction Bit)
    - It tells in which direction will the segment grow. If it is set to (0), it grows upwards and if set to (1), it grows downwards in the stack.
- ### RW (Read/Write)
    - It tells if the segment is readable or not. If it is (1), it readable else if it is (0), it is not readable.
- ### A (Accessed Bit)
    - It tells if the segment has been accessed. By default, it should be (0), means it is not accessed. It is accessed, it will be automatically set to (1) by the PiC and CPU.

# Task State Segment (TSS)
The ***Task State Segment*** or *TSS* is used to switch context. It is accessible by the user. Context switching means to switch from one task to another. It saves the state of the current task. If whe close the OS, the task is restored to its normal state using the *TSS*.


# Interrupt Descriptor Table (IDT)
An **interrupt** is a signal to the OS to stop the current work and focus on that particular interrupt. They can occur from varoius reasons such as divison by zero, Unimplemented GDT with IDT, key press etc. There are 256 different interrupts. They are broadly classified in three categories:

- ### OS Reserved (0-31)
    - These interrupts are only for the OS and only the kernel has the permission to solve them. This includes errors like division by zero in the kernel code.

- ### Hardware (32-47)
    - These interrupts are generated from hardware and again, only kernel can handle it. This includes like key press.
- ### Software (Hardware-255)
    - These interrupts are generated from the software in the OS like from the instruction INT. It also overlaps with **Hadware Interrupts** as they can be handled by the user too by the INT instruction as ou know.

# The structure of IDT
It is a lot like GDT. But, in place of base spread all in the memory, we use the segment hex code directly. Here is a representation:

![image info](./pictures/idt.png)

Just there are few interesting fields:
- ### Gate Type
    - It tells us wheater it is a trap, interrupt or task gate. 32-bits or 64-bits and other useful info. We have a dedicated section after this.
- ### Offset
    - The offset is the offset (means the approx) location of the handler.

# Flags in IDT
***Flags*** is a field in the *IDT* that tells the type, bits etc information for the IDT Gate. This field is this part of the IDT:

![image info](./pictures/flags_idt.png)

It has the following fields:
- ### Present (P)
    - It defines if a IDT gate is valid or not. Same as the GDT, it must be set to (1) for a gate to be valid
- ### DPL (Privilage Level)
    - It defines the privilage level of the IDT gate. Means, which ring can call it. Kernel or user?
- ### 0
    - It is always set to 0
- ### Gate Type
    - This defines which type of IDT gate is it. It contains the following types:
        - #### Task Gate
            - It is a task gate. Means, it gets executed for a task. It doesn't require a offset and so to use it, the offset must be 0. It hex code is 0x5
        - #### Trap Gate
            - It means that once the interrupt occurs, the kernel is trapped and forced to panic. Windows' Blue Screen of Death is a good example for this type of gate. The hex codes are as follows:
                - ##### For 16 bits
                    - 0x7
                - ##### For 32 bits
                    - 0xF
        - #### Interrupt Gate
            - It tells that once the interrupt occurs, handle it and then come back from where the kernel left. A key press or an assembly INT instruction are some daily examples for this. The hex codes are as follows:
                - ##### For 16 bits
                    - 0x6
                - ##### For 32 bits
                    - 0xE

# The Programmable Interrupt Controller (PiC)
The ***Programmable Interrupt Controller*** also popularly called ***PiC*** (pronounced **Pi-K**) is responsible for receiving and handling interrupts. It is essential to set up PiC before loading the IDT. There are some codes to set up the PiC. We will come back to it later. There are two PiC's, the Master PiC and the Slave PiC. The Master PiC has two ports and so the Slave PiC. They are as follows:

- ### For Master PiC
    - #### Command Port
        - 0x20
    - #### Data Port
        - 0x21
- ### For Slave PiC
    - #### Command Port
        - 0xA0
    - #### Data Port 
        - 0xA1

# The PiC Codes
The PiC Codes are:
- ### For Initialising PiC
    - First of all, to initialize the PiC, we cand use the code **0x11** which is **0x10 | 0x01**. Here, the 0x10 is the init code and 0x01 is the offset.
- ### For Mask/Unmask
    - Now, it gets complicated. Whenever we have to use some interrupts, we have to unmask them by sending the respective codes to both the PiC's data port. They are as follows if we want to unmask all the IRQS:
        - #### For Master PiC
            - ##### 0x20
                - It means unmask all IRQS present on the Master PiC
        - #### For Slave PiC
            - ##### 0x28
                - It means unmask all IRQs present on the Slave PiC
- ### For Custom Means
    - There are various more codes that all cannot be remembered. However, they can be found in the code.

