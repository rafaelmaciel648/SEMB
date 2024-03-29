/* Copyright (c) 2010  Paulo Costa
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

// ATMega328p version

#ifndef	_TIMER_TOOLS_H
#define	_TIMER_TOOLS_H	1

#include <stdint.h>
#include <ctype.h>

// definable constants
#define	CTRL_FREQ		        1		 //	KHz
#define	PRESCALER		       64

/*
 * Struct of a miliseconds timer
 */
typedef struct {
  uint16_t start_value, delta;
} mili_timer;

// Initializer function
void init_mili_timers(void);

// Get the current value of microcontroller time
uint16_t get_ms_count(void);

// Start timer 'timer' to count until 'delta_time'
void start_timer(mili_timer* timer, uint16_t delta_time);

// Get timer status. Return '1' when timer finish count, if not return '0'
uint8_t get_timer(mili_timer* timer);

// Return current value of 'timer'
uint16_t get_timer_time(mili_timer* timer);

uint16_t elapsed_time_ms(uint16_t start_count);
uint16_t check_elapsed_time_ms(uint16_t start_count, uint16_t total_count);

#endif


