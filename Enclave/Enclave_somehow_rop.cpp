/*
 * Copyright (C) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Enclave.h"
#include "Enclave_t.h"  /* print_string */

char testarray[0x10000000];

/*
 * do_eexit_by_rop:
 *   Invokes EEXIT (ENCLU) leaf instruction using ROP
 */
extern "C" void tcommit()
{
//	printf("transaction commit.\n");
}

extern "C" void tabort(int rdi)
{
//	printf("transaction abort: %d.\n", rdi);
}

void do_eexit_by_rop2()
{
	  __asm__ __volatile__(
        //  "movq %0, %%rax\n\t"
	  		  "xBegin .L0\n\t"
	  		 // "call tcommit\n\t"
        //  "movq $1000, %%rsi\n\t"
       //   "movq (%%rax), %%rbx\n\t"
	  		  "xEnd\n\t"	

	  		  "jmp exit\n\t"
	  		  ".L0:\n\t"
	  		  "call tabort\n\t"  // if call tcommit, then transaction abort here
			    "exit:\n\t"
          : 
          : "r" (&testarray)
          : "rax", "rbx", "rsi"
	  	);
}

void enclave_main()
{
    do_eexit_by_rop2();
}
