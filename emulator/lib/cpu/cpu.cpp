#include "cpu.h"
#include <stdint.h>

uint32_t cpu_cycle(void)
{
	uint8_t b, t;
	uint16_t s;
	uint32_t i;
	
	if(halted)
	{
		c.cycles += 1;
		uint32_t delta = (c.cycles - c.lastcycles);
		c.lastcycles = c.cycles;
		return delta;
	}
	
    /* not sure what any of this does yet

	interrupt_flush();

	b = mem_get_byte(c.PC);
	
	if (halt_bug) {
		halt_bug = false;
	} else {
		++c.PC;
	}

	*/
	switch(b)
	{
		case 0x00:	/* NOP */
			c.cycles += 1;
		break;
		case 0x01:	/* LD BC, imm16 */
			s = mem_get_word(c.PC);
			set_BC(s);
			c.PC += 2;
			c.cycles += 3;
		break;
		case 0x02:	/* LD (BC), A */
			mem_write_byte(get_BC(), c.A);
			c.cycles += 2;
		break;
		case 0x03:	/* INC BC */
			set_BC(get_BC()+1);
			c.cycles += 2;
		break;
		case 0x04:	/* INC B */
			set_H((c.B&0xF) == 0xF);
			c.B++;
			set_Z(!c.B);
			set_N(0);
			c.cycles += 1;
		break;
		case 0x05:	/* DEC B */
			c.B--;
			set_Z(!c.B);
			set_N(1);
			set_H((c.B & 0xF) == 0xF);
			c.cycles += 1;
		break;
		case 0x06:	/* LD B, imm8 */
			c.B = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x07:	/* RLCA */
			RLC(7);
			set_Z(0);
			c.cycles += 1;
		break;
		case 0x08:	/* LD (imm16), SP */
			mem_write_word(mem_get_word(c.PC), c.SP);
			c.PC += 2;
			c.cycles += 5;
		break;
		case 0x09:	/* ADD HL, BC */
			i = get_HL() + get_BC();
			set_N(0);
			set_C(i >= 0x10000);
			set_H((i&0xFFF) < (get_HL()&0xFFF));
			set_HL(i&0xFFFF);
			c.cycles += 2;
		break;
		case 0x0A:	/* LD A, (BC) */
			c.A = mem_get_byte(get_BC());
			c.cycles += 2;
		break;
		case 0x0B:	/* DEC BC */
			s = get_BC();
			s--;
			set_BC(s);
			c.cycles += 2;
		break;
		case 0x0C:	/* INC C */
			set_H((c.C&0xF) == 0xF);
			c.C++;
			set_Z(!c.C);
			set_N(0);
			c.cycles += 1;
		break;
		case 0x0D:	/* DEC C */
			set_H((c.C&0xF) == 0);
			c.C--;
			set_Z(!c.C);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x0E:	/* LD C, imm8 */
			c.C = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x0F:	/* RRCA */
			RRC(7);
			set_Z(0);
			c.cycles += 1;
		break;
		case 0x10: /* STOP */
			// TODO
			c.PC++;
			c.cycles += 1;
		break;
		case 0x11:	/* LD DE, imm16 */
			s = mem_get_word(c.PC);
			set_DE(s);
			c.PC += 2;
			c.cycles += 3;
		break;
		case 0x12:	/* LD (DE), A */
			mem_write_byte(get_DE(), c.A);
			c.cycles += 2;
		break;
		case 0x13:	/* INC DE */
			s = get_DE();
			s++;
			set_DE(s);
			c.cycles += 2;
		break;
		case 0x14:	/* INC D */
			set_H((c.D&0xF) == 0xF);
			c.D++;
			set_Z(!c.D);
			set_N(0);
			c.cycles += 1;
		break;
		case 0x15:	/* DEC D */
			c.D--;
			set_Z(!c.D);
			set_N(1);
			set_H((c.D & 0xF) == 0xF);
			c.cycles += 1;
		break;
		case 0x16:	/* LD D, imm8 */
			c.D = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x17:	/* RLA */
			RL(7);
			set_Z(0);
			c.cycles += 1;
		break;
		case 0x18:	/* JR rel8 */
			c.PC += (signed char)mem_get_byte(c.PC) + 1;
			c.cycles += 3;
		break;
		case 0x19:	/* ADD HL, DE */
			i = get_HL() + get_DE();
			set_H((i&0xFFF) < (get_HL()&0xFFF));
			set_HL(i);
			set_N(0);
			set_C(i > 0xFFFF);
			c.cycles += 2;
		break;
		case 0x1A:	/* LD A, (DE) */
			c.A = mem_get_byte(get_DE());
			c.cycles += 2;
		break;
		case 0x1B:	/* DEC DE */
			s = get_DE();
			s--;
			set_DE(s);
			c.cycles += 2;
		break;
		case 0x1C:	/* INC E */
			set_H((c.E&0xF) == 0xF);
			c.E++;
			set_Z(!c.E);
			set_N(0);
			c.cycles += 1;
		break;
		case 0x1D:	/* DEC E */
			c.E--;
			set_Z(!c.E);
			set_N(1);
			set_H((c.E & 0xF) == 0xF);
			c.cycles += 1;
		break;
		case 0x1E:	/* LD E, imm8 */
			c.E = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x1F:	/* RR A */
			RR(7);
			set_Z(0);
			c.cycles += 1;
		break;
		case 0x20:	/* JR NZ, rel8 */
			if(flag_Z == 0)
			{
				c.PC += (signed char)mem_get_byte(c.PC) + 1;
				c.cycles += 3;
			} else {
				c.PC += 1;
				c.cycles += 2;
			}
		break;
		case 0x21:	/* LD HL, imm16 */
			s = mem_get_word(c.PC);
			set_HL(s);
			c.PC += 2;
			c.cycles += 3;
		break;
		case 0x22:	/* LDI (HL), A */
			i = get_HL();
			mem_write_byte(i, c.A);
			i++;
			set_HL(i);
			c.cycles += 2;
		break;
		case 0x23:	/* INC HL */
			s = get_HL();
			s++;
			set_HL(s);
			c.cycles += 2;
		break;
		case 0x24:	/* INC H */
			c.H++;
			set_Z(!c.H);
			set_H((c.H&0xF) == 0);
			set_N(0);
			c.cycles += 1;
		break;
		case 0x25:	/* DEC H */
			c.H--;
			set_Z(!c.H);
			set_N(1);
			set_H((c.H & 0xF) == 0xF);
			c.cycles += 1;
		break;
		case 0x26:	/* LD H, imm8 */
			c.H = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x27:	/* DAA */
			s = c.A;

			if(flag_N)
			{
				if(flag_H)
					s = (s - 0x06)&0xFF;
				if(flag_C)
					s -= 0x60;
			}
			else
			{
				if(flag_H || (s & 0xF) > 9)
					s += 0x06;
				if(flag_C || s > 0x9F)
					s += 0x60;
			}

			c.A = s;
			set_H(0);
			set_Z(!c.A);
			if(s >= 0x100)
				set_C(1);
			c.cycles += 1;
		break;
		case 0x28:	/* JR Z, rel8 */
			if(flag_Z == 1)
			{
				c.PC += (signed char)mem_get_byte(c.PC) + 1;
				c.cycles += 3;
			} else {
				c.PC += 1;
				c.cycles += 2;
			}
		break;
		case 0x29:	/* ADD HL, HL */
			i = get_HL()*2;
			set_H((i&0x7FF) < (get_HL()&0x7FF));
			set_C(i > 0xFFFF);
			set_HL(i);
			set_N(0);
			c.cycles += 2;
		break;
		case 0x2A:	/* LDI A, (HL) */
			s = get_HL();
			c.A = mem_get_byte(s);
			set_HL(s+1);
			c.cycles += 2;
		break;
		case 0x2B: 	/* DEC HL */
			set_HL(get_HL()-1);
			c.cycles += 2;
		break;
		case 0x2C:	/* INC L */
			c.L++;
			set_Z(!c.L);
			set_N(0);
			set_H((c.L & 0xF) == 0x00);
			c.cycles += 1;
		break;
		case 0x2D:	/* DEC L */
			c.L--;
			set_Z(!c.L);
			set_N(1);
			set_H((c.L & 0xF) == 0xF);
			c.cycles += 1;
		break;
		case 0x2E:	/* LD L, imm8 */
			c.L = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x2F:	/* CPL */
			c.A = ~c.A;
			set_N(1);
			set_H(1);
			c.cycles += 1;
		break;
		case 0x30:	/* JR NC, rel8 */
			if(flag_C == 0)
			{
				c.PC += (signed char)mem_get_byte(c.PC) + 1;
				c.cycles += 3;
			} else {
				c.PC += 1;
				c.cycles += 2;
			}
		break;
		case 0x31:	/* LD SP, imm16 */
			c.SP = mem_get_word(c.PC);
			c.PC += 2;
			c.cycles += 3;
		break;
		case 0x32:	/* LDD (HL), A */
			i = get_HL();
			mem_write_byte(i, c.A);
			set_HL(i-1);
			c.cycles += 2;
		break;
		case 0x33:	/* INC SP */
			c.SP++;
			c.cycles += 2;
		break;
		case 0x34:	/* INC (HL) */
			t = mem_get_byte(get_HL());
			t++;
			mem_write_byte(get_HL(), t);
			set_Z(!t);
			set_N(0);
			set_H((t & 0xF) == 0);
			c.cycles += 3;
		break;
		case 0x35:	/* DEC (HL) */
			t = mem_get_byte(get_HL());
			t--;
			mem_write_byte(get_HL(), t);
			set_Z(!t);
			set_N(1);
			set_H((t & 0xF) == 0xF);
			c.cycles += 3;
		break;
		case 0x36:	/* LD (HL), imm8 */
			t = mem_get_byte(c.PC++);
			mem_write_byte(get_HL(), t);
			c.cycles += 3;
		break;
		case 0x37:	/* SCF */
			set_N(0);
			set_H(0);
			set_C(1);
			c.cycles += 1;
		break;
		case 0x38:  /* JR C, rel8 */
			if(flag_C == 1)
			{
				c.PC += (signed char)mem_get_byte(c.PC) + 1;
				c.cycles += 3;
			} else {
				c.PC += 1;
				c.cycles += 2;
			}
		break;
		case 0x39:	/* ADD HL, SP */
			i = get_HL() + c.SP;
			set_H((i&0x7FF) < (get_HL()&0x7FF));
			set_C(i > 0xFFFF);
			set_N(0);
			set_HL(i);
			c.cycles += 2;
		break;
		case 0x3A:	/* LDD A, (HL) */
			c.A = mem_get_byte(get_HL());
			set_HL(get_HL()-1);
			c.cycles += 2;
		break;
		case 0x3B:	/* DEC SP */
			c.SP--;
			c.cycles += 2;
		break;
		case 0x3C:	/* INC A */
			c.A++;
			set_Z(!c.A);
			set_H((c.A&0xF) == 0);
			set_N(0);
			c.cycles += 1;
		break;
		case 0x3D:	/* DEC A */
			c.A--;
			set_Z(!c.A);
			set_N(1);
			set_H((c.A & 0xF) == 0xF);
			c.cycles += 1;
		break;
		case 0x3E:	/* LD A, imm8 */
			c.A = mem_get_byte(c.PC++);
			c.cycles += 2;
		break;
		case 0x3F:	/* CCF */
			set_N(0);
			set_H(0);
			set_C(!flag_C);
			c.cycles += 1;
		break;
		case 0x40:	/* LD B, B */
			c.B = c.B;
			c.cycles += 1;
		break;
		case 0x41:	/* LD B, C */
			c.B = c.C;
			c.cycles += 1;
		break;
		case 0x42:	/* LD B, D */
			c.B = c.D;
			c.cycles += 1;
		break;
		case 0x43:	/* LD B, E */
			c.B = c.E;
			c.cycles += 1;
		break;
		case 0x44:	/* LD B, H */
			c.B = c.H;
			c.cycles += 1;
		break;
		case 0x45:	/* LD B, L */
			c.B = c.L;
			c.cycles += 1;
		break;
		case 0x46:	/* LD B, (HL) */
			c.B = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x47:	/* LD B, A */
			c.B = c.A;
			c.cycles += 1;
		break;
		case 0x48:	/* LD C, B */
			c.C = c.B;
			c.cycles += 1;
		break;
		case 0x49:	/* LD C, C */
			c.C = c.C;
			c.cycles += 1;
		break;
		case 0x4A:	/* LD C, D */
			c.C = c.D;
			c.cycles += 1;
		break;
		case 0x4B:	/* LD C, E */
			c.C = c.E;
			c.cycles += 1;
		break;
		case 0x4C:	/* LD C, H */
			c.C = c.H;
			c.cycles += 1;
		break;
		case 0x4D:	/* LD C, L */
			c.C = c.L;
			c.cycles += 1;
		break;
		case 0x4E:	/* LD C, (HL) */
			c.C = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x4F:	/* LD C, A */
			c.C = c.A;
			c.cycles += 1;
		break;
		case 0x50:	/* LD D, B */
			c.D = c.B;
			c.cycles += 1;
		break;
		case 0x51:	/* LD D, C */
			c.D = c.C;
			c.cycles += 1;
		break;
		case 0x52:	/* LD D, D */
			c.D = c.D;
			c.cycles += 1;
		break;
		case 0x53:	/* LD D, E */
			c.D = c.E;
			c.cycles += 1;
		break;
		case 0x54:	/* LD D, H */
			c.D = c.H;
			c.cycles += 1;
		break;
		case 0x55:	/* LD D, L */
			c.D = c.L;
			c.cycles += 1;
		break;
		case 0x56:	/* LD D, (HL) */
			c.D = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x57:	/* LD D, A */
			c.D = c.A;
			c.cycles += 1;
		break;
		case 0x58:	/* LD E, B */
			c.E = c.B;
			c.cycles += 1;
		break;
		case 0x59:	/* LD E, C */
			c.E = c.C;
			c.cycles += 1;
		break;
		case 0x5A:	/* LD E, D */
			c.E = c.D;
			c.cycles += 1;
		break;
		case 0x5B:	/* LD E, E */
			c.E = c.E;
			c.cycles += 1;
		break;
		case 0x5C:	/* LD E, H */
			c.E = c.H;
			c.cycles += 1;
		break;
		case 0x5D:	/* LD E, L */
			c.E = c.L;
			c.cycles += 1;
		break;
		case 0x5E:	/* LD E, (HL) */
			c.E = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x5F:	/* LD E, A */
			c.E = c.A;
			c.cycles += 1;
		break;
		case 0x60:	/* LD H, B */
			c.H = c.B;
			c.cycles += 1;
		break;
		case 0x61:	/* LD H, C */
			c.H = c.C;
			c.cycles += 1;
		break;
		case 0x62:	/* LD H, D */
			c.H = c.D;
			c.cycles += 1;
		break;
		case 0x63:	/* LD H, E */
			c.H = c.E;
			c.cycles += 1;
		break;
		case 0x64:	/* LD H, H */
			c.H = c.H;
			c.cycles += 1;
		break;
		case 0x65:	/* LD H, L */
			c.H = c.L;
			c.cycles += 1;
		break;
		case 0x66:	/* LD H, (HL) */
			c.H = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x67:	/* LD H, A */
			c.H = c.A;
			c.cycles += 1;
		break;
		case 0x68:	/* LD L, B */
			c.L = c.B;
			c.cycles += 1;
		break;
		case 0x69:	/* LD L, C */
			c.L = c.C;
			c.cycles += 1;
		break;
		case 0x6A:	/* LD L, D */
			c.L = c.D;
			c.cycles += 1;
		break;
		case 0x6B:	/* LD L, E */
			c.L = c.E;
			c.cycles += 1;
		break;
		case 0x6C:	/* LD L, H */
			c.L = c.H;
			c.cycles += 1;
		break;
		case 0x6D:	/* LD L, L */
			c.L = c.L;
			c.cycles += 1;
		break;
		case 0x6E:	/* LD L, (HL) */
			c.L = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x6F:	/* LD L, A */
			c.L = c.A;
			c.cycles += 1;
		break;
		case 0x70:	/* LD (HL), B */
			mem_write_byte(get_HL(), c.B);
			c.cycles += 2;
		break;
		case 0x71:	/* LD (HL), C */
			mem_write_byte(get_HL(), c.C);
			c.cycles += 2;
		break;
		case 0x72:	/* LD (HL), D */
			mem_write_byte(get_HL(), c.D);
			c.cycles += 2;
		break;
		case 0x73:	/* LD (HL), E */
			mem_write_byte(get_HL(), c.E);
			c.cycles += 2;
		break;
		case 0x74:	/* LD (HL), H */
			mem_write_byte(get_HL(), c.H);
			c.cycles += 2;
		break;
		case 0x75:	/* LD (HL), L */
			mem_write_byte(get_HL(), c.L);
			c.cycles += 2;
		break;
		case 0x76: {	/* HALT */
			if (!IME && (IF & IE & 0x1F)) {
				halt_bug = true;
			} else {
				halted = 1;
			}
			c.cycles += 1;
		}
		break;
		case 0x77:	/* LD (HL), A */
			mem_write_byte(get_HL(), c.A);
			c.cycles += 2;
		break;
		case 0x78:	/* LD A, B */
			c.A = c.B;
			c.cycles += 1;
		break;
		case 0x79:	/* LD A, C */
			c.A = c.C;
			c.cycles += 1;
		break;
		case 0x7A:	/* LD A, D */
			c.A = c.D;
			c.cycles += 1;
		break;
		case 0x7B:	/* LD A, E */
			c.A = c.E;
			c.cycles += 1;
		break;
		case 0x7C:	/* LD A, H */
			c.A = c.H;
			c.cycles += 1;
		break;
		case 0x7D:	/* LD A, L */
			c.A = c.L;
			c.cycles += 1;
		break;
		case 0x7E:	/* LD A, (HL) */
			c.A = mem_get_byte(get_HL());
			c.cycles += 2;
		break;
		case 0x7F:	/* LD A, A */
			c.A = c.A;
			c.cycles += 1;
		break;
		case 0x80:	/* ADD B */
			i = c.A + c.B;
			set_H((c.A&0xF)+(c.B&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x81:	/* ADD C */
			i = c.A + c.C;
			set_H((c.A&0xF)+(c.C&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x82:	/* ADD D */
			i = c.A + c.D;
			set_H((c.A&0xF)+(c.D&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x83:	/* ADD E */
			i = c.A + c.E;
			set_H((c.A&0xF)+(c.E&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x84:	/* ADD H */
			i = c.A + c.H;
			set_H((c.A&0xF)+(c.H&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x85:	/* ADD L */
			i = c.A + c.L;
			set_H((c.A&0xF)+(c.L&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x86:	/* ADD (HL) */
			i = c.A + mem_get_byte(get_HL());
			set_H((i&0xF) < (c.A&0xF));
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0x87:	/* ADD A */
			i = c.A + c.A;
			set_H((c.A&0xF)+(c.A&0xF) > 0xF);
			set_C(i > 0xFF);
			set_N(0);
			c.A = i;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x88:	/* ADC B */
			i = c.A + c.B + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.B&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.B + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x89:	/* ADC C */
			i = c.A + c.C + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.C&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.C + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x8A:	/* ADC D */
			i = c.A + c.D + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.D&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.D + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x8B:	/* ADC E */
			i = c.A + c.E + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.E&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.E + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x8C:	/* ADC H */
			i = c.A + c.H + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.H&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.H + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x8D:	/* ADC L */
			i = c.A + c.L + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.L&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.L + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x8E:	/* ADC (HL) */
			t = mem_get_byte(get_HL());
			i = c.A + t + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (t&0xF) + flag_C) >= 0x10);
			c.A = c.A + t + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0x8F:	/* ADC A */
			i = c.A + c.A + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (c.A&0xF) + flag_C) >= 0x10);
			c.A = c.A + c.A + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x90:	/* SUB B */
			set_C((c.A - c.B) < 0);
			set_H(((c.A - c.B)&0xF) > (c.A&0xF));
			c.A -= c.B;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x91:	/* SUB C */
			set_C((c.A - c.C) < 0);
			set_H(((c.A - c.C)&0xF) > (c.A&0xF));
			c.A -= c.C;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x92:	/* SUB D */
			set_C((c.A - c.D) < 0);
			set_H(((c.A - c.D)&0xF) > (c.A&0xF));
			c.A -= c.D;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x93:	/* SUB E */
			set_C((c.A - c.E) < 0);
			set_H(((c.A - c.E)&0xF) > (c.A&0xF));
			c.A -= c.E;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x94:	/* SUB H */
			set_C((c.A - c.H) < 0);
			set_H(((c.A - c.H)&0xF) > (c.A&0xF));
			c.A -= c.H;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x95:	/* SUB L */
			set_C((c.A - c.L) < 0);
			set_H(((c.A - c.L)&0xF) > (c.A&0xF));
			c.A -= c.L;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x96:	/* SUB (HL) */
			t = mem_get_byte(get_HL());
			set_C((c.A - t) < 0);
			set_H(((c.A - t)&0xF) > (c.A&0xF));
			c.A -= t;
			set_Z(!c.A);
			set_N(1);
			c.cycles += 2;
		break;
		case 0x97:	/* SUB A */
			set_C(0);
			set_H(0);
			c.A = 0;
			set_Z(1);
			set_N(1);
			c.cycles += 1;
		break;
		case 0x98:	/* SBC B */
			t = flag_C + c.B;
			set_H(((c.A&0xF) - (c.B&0xF) - flag_C) < 0);
			set_C((c.A - c.B - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x99:	/* SBC C */
			t = flag_C + c.C;
			set_H(((c.A&0xF) - (c.C&0xF) - flag_C) < 0);
			set_C((c.A - c.C - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x9A:	/* SBC D */
			t = flag_C + c.D;
			set_H(((c.A&0xF) - (c.D&0xF) - flag_C) < 0);
			set_C((c.A - c.D - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x9B:	/* SBC E */
			t = flag_C + c.E;
			set_H(((c.A&0xF) - (c.E&0xF) - flag_C) < 0);
			set_C((c.A - c.E - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x9C:	/* SBC H */
			t = flag_C + c.H;
			set_H(((c.A&0xF) - (c.H&0xF) - flag_C) < 0);
			set_C((c.A - c.H - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x9D:	/* SBC L */
			t = flag_C + c.L;
			set_H(((c.A&0xF) - (c.L&0xF) - flag_C) < 0);
			set_C((c.A - c.L - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0x9E:	/* SBC (HL) */
			t = mem_get_byte(get_HL());
			b = flag_C + t;
			set_H(((c.A&0xF) - (t&0xF) - flag_C) < 0);
			set_C((c.A - t - flag_C) < 0);
			set_N(1);
			c.A -= b;
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0x9F:	/* SBC A */
			t = flag_C + c.A;
			set_H(((c.A&0xF) - (c.A&0xF) - flag_C) < 0);
			set_C((c.A - c.A - flag_C) < 0);
			set_N(1);
			c.A -= t;
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0xA0:	/* AND B */
			c.A &= c.B;
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xA1:	/* AND C */
			c.A &= c.C;
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xA2:	/* AND D */
			c.A &= c.D;
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xA3:	/* AND E */
			c.A &= c.E;
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xA4:	/* AND H */
			c.A &= c.H;
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xA5:	/* AND L */
			c.A &= c.L;
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xA6:	/* AND (HL) */
			c.A &= mem_get_byte(get_HL());
			set_Z(!c.A);
			set_H(1);
			set_N(0);
			set_C(0);
			c.cycles += 2;
		break;
		case 0xA7:	/* AND A */
			set_H(1);
			set_N(0);
			set_C(0);
			set_Z(!c.A);
			c.cycles += 1;
		break;
		case 0xA8:	/* XOR B */
			c.A ^= c.B;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xA9:	/* XOR C */
			c.A ^= c.C;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xAA:	/* XOR D */
			c.A ^= c.D;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xAB:	/* XOR E */
			c.A ^= c.E;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xAC:	/* XOR H */
			c.A ^= c.H;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xAD:	/* XOR L */
			c.A ^= c.L;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xAE:	/* XOR (HL) */
			c.A ^= mem_get_byte(get_HL());
			c.F = (!c.A)<<7;
			c.cycles += 2;
		break;
		case 0xAF:	/* XOR A */
			c.A = 0;
			c.F = 0x80;
			c.cycles += 1;
		break;
		case 0xB0:	/* OR B */
			c.A |= c.B;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB1:	/* OR C */
			c.A |= c.C;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB2:	/* OR D */
			c.A |= c.D;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB3:	/* OR E */
			c.A |= c.E;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB4:	/* OR H */
			c.A |= c.H;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB5:	/* OR L */
			c.A |= c.L;
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB6:	/* OR (HL) */
			c.A |= mem_get_byte(get_HL());
			c.F = (!c.A)<<7;
			c.cycles += 2;
		break;
		case 0xB7:	/* OR A */
			c.F = (!c.A)<<7;
			c.cycles += 1;
		break;
		case 0xB8:	/* CP B */
			set_C((c.A - c.B) < 0);
			set_H(((c.A - c.B)&0xF) > (c.A&0xF));
			set_Z(c.A == c.B);
			set_N(1);
			c.cycles += 1;
		break;
		case 0xB9:	/* CP C */
			set_Z(c.A == c.C);
			set_H(((c.A - c.C)&0xF) > (c.A&0xF));
			set_N(1);
			set_C((c.A - c.C) < 0);
			c.cycles += 1;
		break;
		case 0xBA:	/* CP D */
			set_Z(c.A == c.D);
			set_H(((c.A - c.D)&0xF) > (c.A&0xF));
			set_N(1);
			set_C((c.A - c.D) < 0);
			c.cycles += 1;
		break;
		case 0xBB:	/* CP E */
			set_Z(c.A == c.E);
			set_H(((c.A - c.E)&0xF) > (c.A&0xF));
			set_N(1);
			set_C((c.A - c.E) < 0);
			c.cycles += 1;
		break;
		case 0xBC:	/* CP H */
			set_Z(c.A == c.H);
			set_H(((c.A - c.H)&0xF) > (c.A&0xF));
			set_N(1);
			set_C((c.A - c.H) < 0);
			c.cycles += 1;
		break;
		case 0xBD:	/* CP L */
			set_Z(c.A == c.L);
			set_H(((c.A - c.L)&0xF) > (c.A&0xF));
			set_N(1);
			set_C((c.A - c.L) < 0);
			c.cycles += 1;
		break;
		case 0xBE:	/* CP (HL) */
			t = mem_get_byte(get_HL());
			set_Z(c.A == t);
			set_H(((c.A - t)&0xF) > (c.A&0xF));
			set_N(1);
			set_C((c.A - t) < 0);
			c.cycles += 2;
		break;
		case 0xBF:	/* CP A */
			set_Z(1);
			set_H(0);
			set_N(1);
			set_C(0);
			c.cycles += 1;
		break;
		case 0xC0:	/* RET NZ */
			if(!flag_Z)
			{
				c.PC = mem_get_word(c.SP);
				c.SP += 2;
				c.cycles += 5;
			} else {
				c.cycles += 2;
			}
		break;
		case 0xC1:	/* POP BC */
			s = mem_get_word(c.SP);
			set_BC(s);
			c.SP += 2;
			c.cycles += 3;
		break;
		case 0xC2:	/* JP NZ, mem16 */
			if(flag_Z == 0)
			{
				c.PC = mem_get_word(c.PC);
				c.cycles += 4;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xC3:	/* JP imm16 */
			c.PC = mem_get_word(c.PC);
			c.cycles += 4;
		break;
		case 0xC4:	/* CALL NZ, imm16 */
			if(flag_Z == 0)
			{
				c.SP -= 2;
				mem_write_word(c.SP, c.PC+2);
				c.PC = mem_get_word(c.PC);
				c.cycles += 6;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xC5:	/* PUSH BC */
			c.SP -= 2;
			mem_write_word(c.SP, get_BC());
			c.cycles += 4;
		break;
		case 0xC6:	/* ADD A, imm8 */
			t = mem_get_byte(c.PC++);
			set_C((c.A + t) >= 0x100);
			set_H(((c.A + t)&0xF) < (c.A&0xF));
			c.A += t;
			set_N(0);
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0xC7:	/* RST 00 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0;
			c.cycles += 4;
		break;
		case 0xC8:	/* RET Z */
			if(flag_Z == 1)
			{
				c.PC = mem_get_word(c.SP);
				c.SP += 2;
				c.cycles += 5;
			} else {
				c.cycles += 2;
			}
		break;
		case 0xC9:	/* RET */
			c.PC = mem_get_word(c.SP);
			c.SP += 2;
			c.cycles += 4;
		break;
		case 0xCA:	/* JP z, mem16 */
			if(flag_Z == 1)
			{
				c.PC = mem_get_word(c.PC);
				c.cycles += 4;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xCB:	/* RLC/RRC/RL/RR/SLA/SRA/SWAP/SRL/BIT/RES/SET */
			decode_CB(mem_get_byte(c.PC++));
			c.cycles += 1;
		break;
		case 0xCC:	/* CALL Z, imm16 */
			if(flag_Z == 1)
			{
				c.SP -= 2;
				mem_write_word(c.SP, c.PC+2);
				c.PC = mem_get_word(c.PC);
				c.cycles += 6;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xCD:	/* call imm16 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC+2);
			c.PC = mem_get_word(c.PC);
			c.cycles += 6;
		break;
		case 0xCE:	/* ADC a, imm8 */
			t = mem_get_byte(c.PC++);
			i = c.A + t + flag_C >= 0x100;
			set_N(0);
			set_H(((c.A&0xF) + (t&0xF) + flag_C) >= 0x10);
			c.A = c.A + t + flag_C;
			set_C(i);
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0xCF:	/* RST 08 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x0008;
			c.cycles += 4;
		break;
		case 0xD0:	/* RET NC */
			if(flag_C == 0)
			{
				c.PC = mem_get_word(c.SP);
				c.SP += 2;
				c.cycles += 5;
			} else {
				c.cycles += 2;
			}
		break;
		case 0xD1:	/* POP DE */
			s = mem_get_word(c.SP);
			set_DE(s);
			c.SP += 2;
			c.cycles += 3;
		break;
		case 0xD2:	/* JP NC, mem16 */
			if(flag_C == 0)
			{
				c.PC = mem_get_word(c.PC);
				c.cycles += 4;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xD4:	/* CALL NC, mem16 */
			if(flag_C == 0)
			{
				c.SP -= 2;
				mem_write_word(c.SP, c.PC+2);
				c.PC = mem_get_word(c.PC);
				c.cycles += 6;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xD5:	/* PUSH DE */
			c.SP -= 2;
			mem_write_word(c.SP, get_DE());
			c.cycles += 4;
		break;
		case 0xD6:	/* SUB A, imm8 */
			t = mem_get_byte(c.PC++);
			set_C((c.A - t) < 0);
			set_H(((c.A - t)&0xF) > (c.A&0xF));
			c.A -= t;
			set_N(1);
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0xD7:	/* RST 10 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x0010;
			c.cycles += 4;
		break;
		case 0xD8:	/* RET C */
			if(flag_C == 1)
			{
				c.PC = mem_get_word(c.SP);
				c.SP += 2;
				c.cycles += 5;
			} else {
				c.cycles += 2;
			}
		break;
		case 0xD9:	/* RETI */
			c.PC = mem_get_word(c.SP);
			c.SP += 2;
			c.cycles += 4;
			IME = 1;
		break;
		case 0xDA:	/* JP C, mem16 */
			if(flag_C)
			{
				c.PC = mem_get_word(c.PC);
				c.cycles += 4;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xDC:	/* CALL C, mem16 */
			if(flag_C == 1)
			{
				c.SP -= 2;
				mem_write_word(c.SP, c.PC+2);
				c.PC = mem_get_word(c.PC);
				c.cycles += 6;
			} else {
				c.PC += 2;
				c.cycles += 3;
			}
		break;
		case 0xDE:	/* SBC A, imm8 */
			t = mem_get_byte(c.PC++);
			b = flag_C;
			set_H(((t&0xF) + flag_C) > (c.A&0xF));
			set_C(t + flag_C > c.A);
			set_N(1);
			c.A -= (b + t);
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0xDF:	/* RST 18 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x0018;
			c.cycles += 4;
		break;
		case 0xE0:	/* LD (FF00 + imm8), A */
			t = mem_get_byte(c.PC++);
			mem_write_byte(0xFF00 + t, c.A);
			c.cycles += 3;
		break;
		case 0xE1:	/* POP HL */
			i = mem_get_word(c.SP);
			set_HL(i);
			c.SP += 2;
			c.cycles += 3;
		break;
		case 0xE2:	/* LD (FF00 + C), A */
			mem_write_byte(0xFF00 + c.C, c.A);
			c.cycles += 2;
		break;
		case 0xE5:	/* PUSH HL */
			c.SP -= 2;
			mem_write_word(c.SP, get_HL());
			c.cycles += 4;
		break;
		case 0xE6:	/* AND A, imm8 */
			t = mem_get_byte(c.PC++);
			set_N(0);
			set_H(1);
			set_C(0);
			c.A = t & c.A;
			set_Z(!c.A);
			c.cycles += 2;
		break;
		case 0xE7:	/* RST 20 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x20;
			c.cycles += 4;
		break;
		case 0xE8:	/* ADD SP, imm8 */
			i = mem_get_byte(c.PC++);
			set_Z(0);
			set_N(0);
			set_C(((c.SP+i)&0xFF) < (c.SP&0xFF));
			set_H(((c.SP+i)&0xF) < (c.SP&0xF));
			c.SP = c.SP + (signed char)i;
			c.cycles += 4;
		break;
		case 0xE9:	/* JP HL */
			c.PC = get_HL();
			c.cycles += 1;
		break;
		case 0xEA:	/* LD (mem16), a */
			s = mem_get_word(c.PC);
			mem_write_byte(s, c.A);
			c.PC += 2;
			c.cycles += 4;
		break;
		case 0xEE:	/* XOR A, imm8 */
			c.A ^= mem_get_byte(c.PC++);
			c.F = (!c.A)<<7;
			c.cycles += 2;
		break;
		case 0xEF:	/* RST 28 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x28;
			c.cycles += 4;
		break;
		case 0xF0:	/* LD A, (FF00 + imm8) */
			t = mem_get_byte(c.PC++);
			c.A = mem_get_byte(0xFF00 + t);
			c.cycles += 3;
		break;
		case 0xF1:	/* POP AF */
			s = mem_get_word(c.SP);
			set_AF(s&0xFFF0);
			c.SP += 2;
			c.cycles += 3;
		break;
		case 0xF2:	/* LD A, (FF00 + c) */
			c.A = mem_get_byte(0xFF00 + c.C);
			c.cycles += 2;
		break;
		case 0xF3:	/* DI */
			c.cycles += 1;
			IME = 0;
		break;
		case 0xF5:	/* PUSH AF */
			c.SP -= 2;
			mem_write_word(c.SP, get_AF());
			c.cycles += 4;
		break;
		case 0xF6:	/* OR A, imm8 */
			c.A |= mem_get_byte(c.PC++);
			c.F = (!c.A)<<7;
			c.cycles += 2;
		break;
		case 0xF7:	/* RST 30 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x30;
			c.cycles += 4;
		break;
		case 0xF8:	/* LD HL, SP + imm8 */
			i = mem_get_byte(c.PC++);
			set_N(0);
			set_Z(0);
			set_C(((c.SP+i)&0xFF) < (c.SP&0xFF));
			set_H(((c.SP+i)&0xF) < (c.SP&0xF));
			set_HL(c.SP + (signed char)i);
			c.cycles += 3;
		break;
		case 0xF9:	/* LD SP, HL */
			c.SP = get_HL();
			c.cycles += 2;
		break;
		case 0xFA:	/* LD A, (mem16) */
			s = mem_get_word(c.PC);
			c.A = mem_get_byte(s);
			c.PC += 2;
			c.cycles += 4;
		break;
		case 0xFB:	/* EI */
			interrupt_enable();
			c.cycles += 1;
		break;
		case 0xFE:	/* CP a, imm8 */
			t = mem_get_byte(c.PC++);
			set_Z(c.A == t);
			set_N(1);
			set_H(((c.A - t)&0xF) > (c.A&0xF));
			set_C(c.A < t);
			c.cycles += 2;
		break;
		case 0xFF:	/* RST 38 */
			c.SP -= 2;
			mem_write_word(c.SP, c.PC);
			c.PC = 0x0038;
			c.cycles += 4;
		break;
		default: {
			static char errstr[50];
			sprintf(errstr, "Unhandled opcode: %02X at %04X.", b, c.PC);
			espeon_faint(errstr);
		}
		break;
	}