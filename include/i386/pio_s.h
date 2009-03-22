
#warning use pio.h


#if 1

#ifdef SLOW_IO_BY_JUMPING
#define __SLOW_DOWN_IO "\njmp 1f\n1:\tjmp 1f\n1:"
#else
#define __SLOW_DOWN_IO "\noutb %%al,$0x80"
#endif

#ifdef REALLY_SLOW_IO
#define __FULL_SLOW_DOWN_IO __SLOW_DOWN_IO __SLOW_DOWN_IO __SLOW_DOWN_IO __SLOW_DOWN_IO
#else
#define __FULL_SLOW_DOWN_IO __SLOW_DOWN_IO
#endif

/*
 * Talk about misusing macros..
 */
#define __OUT1(s,x) \
extern __inline void out##s(unsigned x value, unsigned short port) {

#define __OUT2(s,s1,s2) \
__asm__ __volatile__ ("out" #s " %" s1 "0,%" s2 "1"

#define __OUT(s,s1,x) \
__OUT1(s,x) __OUT2(s,s1,"w") : : "a" (value), "Nd" (port)); } \
__OUT1(s##_p,x) __OUT2(s,s1,"w") __FULL_SLOW_DOWN_IO : : "a" (value), "Nd" (port));} \

#define __IN1(s) \
extern inline RETURN_TYPE in##s(unsigned short port) { RETURN_TYPE _v;

#define __IN2(s,s1,s2) \
__asm__ __volatile__ ("in" #s " %" s2 "1,%" s1 "0"

#define __IN(s,s1,i...) \
__IN1(s) __IN2(s,s1,"w") : "=a" (_v) : "Nd" (port) ,##i ); return _v; } \
__IN1(s##_p) __IN2(s,s1,"w") __FULL_SLOW_DOWN_IO : "=a" (_v) : "Nd" (port) ,##i ); return _v; } \

#define __INS(s) \
static inline void ins##s(unsigned short port, void * addr, unsigned long count) \
{ __asm__ __volatile__ ("cld ; rep ; ins" #s \
: "=D" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }

#define __OUTS(s) \
static inline void outs##s(unsigned short port, const void * addr, unsigned long count) \
{ __asm__ __volatile__ ("cld ; rep ; outs" #s \
: "=S" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }

/*
#define RETURN_TYPE unsigned char
__IN(b,"")
#undef RETURN_TYPE
#define RETURN_TYPE unsigned short
__IN(w,"")
#undef RETURN_TYPE
#define RETURN_TYPE unsigned int
__IN(l,"")
#undef RETURN_TYPE

__OUT(b,"b",char)
__OUT(w,"w",short)
__OUT(l,,int)
*/
__INS(b)
__INS(w)
__INS(l)

__OUTS(b)
__OUTS(w)
__OUTS(l)


#else


/*	$OpenBSD: pio.h,v 1.5 1997/11/10 23:40:45 niklas Exp $	*/
/*	$NetBSD: pio.h,v 1.13 1996/03/08 20:15:23 cgd Exp $	*/

/*
 * Copyright (c) 1993, 1995 Charles M. Hannum.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Charles M. Hannum.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _I386_PIO_H_
#define _I386_PIO_H_

/*
 * Functions to provide access to i386 programmed I/O instructions.
 *
 * The in[bwl]() and out[bwl]() functions are split into two varieties: one to
 * use a small, constant, 8-bit port number, and another to use a large or
 * variable port number.  The former can be compiled as a smaller instruction.
 */


#ifdef __OPTIMIZE__

#define	__use_immediate_port(port) \
	(__builtin_constant_p((port)) && (port) < 0x100)

#else

#define	__use_immediate_port(port)	0

#endif

#if 0

#define	inb(port) \
	(__use_immediate_port(port) ? __inbc(port) : __inb(port))

static __inline u_int8_t
__inbc(int port)
{
	u_int8_t data;
	__asm __volatile("inb %1,%0" : "=a" (data) : "id" (port));
	return data;
}

static __inline u_int8_t
__inb(int port)
{
	u_int8_t data;
	__asm __volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

#endif

static //__inline
void
insb(int port, void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\tinsb"			:
								:
			 "d" (port), "D" (addr), "c" (cnt)	:
			 "%edi", "%ecx", "memory");
}

#if 0

#define	inw(port) \
	(__use_immediate_port(port) ? __inwc(port) : __inw(port))

static __inline u_int16_t
__inwc(int port)
{
	u_int16_t data;
	__asm __volatile("inw %1,%0" : "=a" (data) : "id" (port));
	return data;
}

static __inline u_int16_t
__inw(int port)
{
	u_int16_t data;
	__asm __volatile("inw %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

#endif

static __inline void
insw(int port, void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\tinsw"			:
								:
			 "d" (port), "D" (addr), "c" (cnt)	:
			 "%edi", "%ecx", "memory");
}

#if 0

#define	inl(port) \
	(__use_immediate_port(port) ? __inlc(port) : __inl(port))

static __inline u_int32_t
__inlc(int port)
{
	u_int32_t data;
	__asm __volatile("inl %1,%0" : "=a" (data) : "id" (port));
	return data;
}

static __inline u_int32_t
__inl(int port)
{
	u_int32_t data;
	__asm __volatile("inl %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

#endif

static __inline void
insl(int port, void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\tinsl"			:
								:
			 "d" (port), "D" (addr), "c" (cnt)	:
			 "%edi", "%ecx", "memory");
}

#if 0

#define	outb(port, data) \
	(__use_immediate_port(port) ? __outbc(port, data) : __outb(port, data))

static __inline void
__outbc(int port, u_int8_t data)
{
	__asm __volatile("outb %0,%1" : : "a" (data), "id" (port));
}

static __inline void
__outb(int port, u_int8_t data)
{
	__asm __volatile("outb %0,%w1" : : "a" (data), "d" (port));
}

#endif

static __inline void
outsb(int port, const void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\toutsb"		:
								:
			 "d" (port), "S" (addr), "c" (cnt)	:
			 "%esi", "%ecx");
}

#if 0

#define	outw(port, data) \
	(__use_immediate_port(port) ? __outwc(port, data) : __outw(port, data))

static __inline void
__outwc(int port, u_int16_t data)
{
	__asm __volatile("outw %0,%1" : : "a" (data), "id" (port));
}

static __inline void
__outw(int port, u_int16_t data)
{
	__asm __volatile("outw %0,%w1" : : "a" (data), "d" (port));
}
#endif

#if 0
//static __inline void
void
outsw(int port, const void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\toutsw"		:
								:
			 "d" (port), "S" (addr), "c" (cnt)	:
			 "%esi", "%ecx");
}
#endif
#if 0

#define	outl(port, data) \
	(__use_immediate_port(port) ? __outlc(port, data) : __outl(port, data))

static __inline void
__outlc(int port, uint32_t data)
{
	__asm __volatile("outl %0,%1" : : "a" (data), "id" (port));
}

static __inline void
__outl(int port, uint32_t data)
{
	__asm __volatile("outl %0,%w1" : : "a" (data), "d" (port));
}

#endif

static __inline void
outsl(int port, const void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\toutsl"		:
								:
			 "d" (port), "S" (addr), "c" (cnt)	:
			 "%esi", "%ecx");
}

#endif /* _I386_PIO_H_ */

#endif

