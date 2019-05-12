# CS33 Attack Lab Workshop: How to Break Code Using Gets
## 1. Setting up the Demo

### Intro: What's inside this repository?
This repository contains sample code that illustrates a simple buffer overflow attack, packaged as a "tarball" (.tgz file, basically what you're working with for the attack lab).

### Step 1: Extract the files
Download the winterfell.tgz file, and move it to the SEASnet servers (for Windows, use WinSCP; for Mac, use Cyberduck, etc.), into the folder of your choice. Change directories (by using `cd directory-name`) into that folder, then type `tar -xvf winterfell.tgz` to extract the files inside of it. You'll see the following files added to your current folder:

1. sample.c - source code for the demo
2. hex2raw - executable provided by Professor Reinman; turns a .txt file representing bytes (in hex) into the corresponding ASCII versions that can be used as input for the attack lab.
3. attack.txt - answer to the sample attack lab.
4. Makefile - don't worry about this too much; this is just to make compiling the code easier for you

You can confirm these files were added properly by listing the contents of your directory (type `ls`).

### Step 2: Compile the code

`$ make`

This generates an executable (called "sample") that contains all the info you'll need to perform a successful buffer overflow attack. Now you're ready to attack!

## 2. Demo Overview

### Goal

Perform a simple buffer overflow attack by overwriting the return address of the caller function with the address of another function (in this case, that "another function" is the nightking function).

### Program Setup

There is a function, getbuf(), in the sample executable that looks like this:

`
unsigned getbuf()
{
    char buf[SOMENUMBER];   
    gets(buf);
    return 1;
}
`

The main() function of this program simply calls the getbuf() function, prints out an "not successful" message, then returns 0.

## 3. Walkthrough

### Step 1: Run the code

Let's try out the executable! We can run the program with the following command:

`$ ./sample`

This will return a prompt waiting for our input. Go ahead and type whatever you want into the terminal, and press ENTER. Odds are, you'll get a segmentation fault (if you corrupted the stack) or a message telling you to try again.


### Step 2: Examine the code

`$ objdump -d sample`

This will dump out all the assembly code in the executable. There is one section in particular that looks interesting:

![alt text](https://github.com/JuliaB1/CS33-Attack-Lab-Workshop-S19/blob/master/img/nightking.PNG "omg it's the night king")

This is the nightking function we were looking for! We can see that the address for this function is 0x400532. This is the address we'll want to inject into the program, so we'll keep this number in mind for later.

Now, we need to figure out how much buffer to add before injecting our address. We can do this by examining the getbuf() function, which looks like this:

![alt text](https://github.com/JuliaB1/CS33-Attack-Lab-Workshop-S19/blob/master/img/getbuf.PNG "gets is weak lol")

Since we know that the main function simply calls getbuf() and then returns (as stated in the demo description), we know that we need to make sure we overwrite the return address that allows main() to continue executing after getbuf() returns. This way, when getbuf() returns, we can force the nightking() function to execute instead.

We can see that the function manipulates the stack in two ways: pushing a register onto the stack, and then manually moving down %rsp by 0x20 bytes. Since we're on a 64-bit system, the size of the register being pushed on the stack is 8 bytes. Doing some quick maths, we also know that 0x20 == 32 bytes. Thus, the total amount of padding we'll want to add is 40 bytes. After those 40 bytes, we can then place the address of the nightking() function, because that's where the return address for the caller (in this case, main()), would be, and we want to overwrite that address. Yay!


### Step 3: Launching the attack

We can create a new text file, say, "solution.txt", and put the following text into it:

00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
32 05 40 00 00 00 00 00

And then use hex2raw in order to feed this as input into the sample executable.
