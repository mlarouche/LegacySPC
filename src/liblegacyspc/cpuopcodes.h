/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright (c) 2007 by Michaël Larouche <larouche@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef LEGACYSPC_OPCODES
#define LEGACYSPC_OPCODES

// TODO: Merge similar opcodes and verify the
//       addressing using opcode & addressing_mode
/**
 * @brief All the opcodes used by the SPC700 CPU
 */
enum CpuOpcodes
{
	// 8-Bit data transmission commands
	// Register <- Address
	Mov_A_ImmediateData = 0xE8,
	Mov_A_IndirectX = 0xE6,
	Mov_A_IndirectXAutoIncrement = 0xBF,
	Mov_A_DirectPage = 0xE4,
	Mov_A_DirectPagePlusX = 0xF4,
	Mov_A_Absolute = 0xE5,
	Mov_A_AbsolutePlusX = 0xF5,
	Mov_A_AbsolutePlusY = 0xF6,
	Mov_A_IndirectDirectPagePlusX = 0xE7,
	Mov_A_IndirectDirectPagePlusY = 0xF7,
	Mov_X_ImmediateData = 0xCD,
	Mov_X_DirectPage = 0xF8,
	Mov_X_DirectPagePlusY = 0xF9,
	Mov_X_Absolute = 0xE9,
	Mov_Y_ImmediateData = 0x8D,
	Mov_Y_DirectPage = 0xEB,
	Mov_Y_DirectPagePlusX = 0xFB,
	Mov_Y_Absolute = 0xEC,

	// Register -> Address
	Mov_IndirectX_A = 0xC6,
	Mov_IndirectXAutoIncrement_A = 0xAF,
	Mov_DirectPage_A = 0xC4,
	Mov_DirectPagePlusX_A = 0xD4,
	Mov_Absolute_A = 0xC5,
	Mov_AbsolutePlusX_A = 0xD5,
	Mov_AbsolutePlusY_A = 0XD6,
	Mov_IndirectDirectPagePlusX_A = 0xC7,
	Mov_IndirectDirectPagePlusY_A = 0xD7,
	Mov_DirectPage_X = 0xD8,
	Mov_DirectPagePlusY_X = 0xD9,
	Mov_Absolute_X = 0xC9,
	Mov_DirectPage_Y = 0xCB,
	Mov_DirectPagePlusX_Y = 0xDB,
	Mov_Absolute_Y = 0xCC,

	// Register <- Register
	Mov_A_X = 0x7D,
	Mov_A_Y = 0xDD,
	Mov_X_A = 0x5D,
	Mov_Y_A = 0xFD,
	Mov_X_SP = 0x9D,
	Mov_SP_X = 0xBD,
	Mov_DirectPage_DirectPage = 0xFA,
	Mov_DirectPage_ImmediateData = 0x8F,

	// 8-bit arithmetic operation commands
	Adc_ImmediateData = 0x88,
	Adc_IndirectX = 0x86,
	Adc_DirectPage = 0x84,
	Adc_DirectPagePlusX = 0x94,
	Adc_Absolute = 0x85,
	Adc_AbsolutePlusX = 0x95,
	Adc_AbsolutePlusY = 0x96,
	Adc_IndirectDirectPagePlusX = 0x87,
	Adc_IndirectDirectPagePlusY = 0x97,
	Adc_IndirectXY = 0x99,
	Adc_DirectPage_DirectPage = 0x89,
	Adc_DirectPage_ImmediateData = 0x98,

	Sbc_ImmediateData = 0xA8,
	Sbc_IndirectX = 0xA6,
	Sbc_DirectPage = 0xA4,
	Sbc_DirectPagePlusX = 0xB4,
	Sbc_Absolute = 0xA5,
	Sbc_AbsolutePlusX = 0xB5,
	Sbc_AbsolutePlusY = 0xB6,
	Sbc_IndirectDirectPagePlusX = 0xA7,
	Sbc_IndirectDirectPagePlusY = 0xB7,
	Sbc_IndirectXY = 0xB9,
	Sbc_DirectPage_DirectPage = 0xA9,
	Sbc_DirectPage_ImmediateData = 0xB8,

	Cmp_A_ImmediateData = 0x68,
	Cmp_A_IndirectX = 0x66,
	Cmp_A_DirectPage = 0x64,
	Cmp_A_DirectPagePlusX = 0x74,
	Cmp_A_Absolute = 0x65,
	Cmp_A_AbsolutePlusX = 0x75,
	Cmp_A_AbsolutePlusY = 0x76,
	Cmp_A_IndirectDirectPagePlusX = 0x67,
	Cmp_A_IndirectDirectPagePlusY = 0x77,
	Cmp_IndirectXY = 0x79,
	Cmp_DirectPage_DirectPage = 0x69,
	Cmp_DirectPage_ImmediateData = 0x78,
	Cmp_X_ImmediateData = 0xC8,
	Cmp_X_DirectPage = 0x3E,
	Cmp_X_Absolute = 0x1E,
	Cmp_Y_ImmediateData = 0xAD,
	Cmp_Y_DirectPage = 0x7E,
	Cmp_Y_Absolute = 0x5E,

	// 8-bit logic operation commands.
	And_ImmediateData = 0x28,
	And_IndirectX = 0x26,
	And_DirectPage = 0x24,
	And_DirectPagePlusX = 0x34,
	And_Absolute = 0x25,
	And_AbsolutePlusX = 0x35,
	And_AbsolutePlusY = 0x36,
	And_IndirectDirectPagePlusX = 0x27,
	And_IndirectDirectPagePlusY = 0x37,
	And_IndirectXY = 0x39,
	And_DirectPage_DirectPage = 0x29,
	And_DirectPage_ImmediateData = 0x38,
	
	Or_ImmediateData = 0x08,
	Or_IndirectX = 0x06,
	Or_DirectPage = 0x04,
	Or_DirectPagePlusX = 0x14,
	Or_Absolute = 0x05,
	Or_AbsolutePlusX = 0x15,
	Or_AbsolutePlusY = 0x16,
	Or_IndirectDirectPagePlusX = 0x07,
	Or_IndirectDirectPagePlusY = 0x17,
	Or_IndirectXY = 0x19,
	Or_DirectPage_DirectPage = 0x09,
	Or_DirectPage_ImmediateData = 0x18,

	Eor_ImmediateData = 0x48,
	Eor_IndirectX = 0x46,
	Eor_DirectPage = 0x44,
	Eor_DirectPagePlusX = 0x54,
	Eor_Absolute = 0x45,
	Eor_AbsolutePlusX = 0x55,
	Eor_AbsolutePlusY = 0x56,
	Eor_IndirectDirectPagePlusX = 0x47,
	Eor_IndirectDirectPagePlusY = 0x57,
	Eor_IndirectXY = 0x59,
	Eor_DirectPage_DirectPage = 0x49,
	Eor_DirectPage_ImmediateData = 0x58,

	// Increment and Deincrement commands
	Inc_A = 0xBC,
	Inc_DirectPage = 0xAB,
	Inc_DirectPagePlusX = 0xBB,
	Inc_Absolute = 0xAC,
	Inc_X = 0x3D,
	Inc_Y = 0xFC,

	Dec_A = 0x9C,
	Dec_DirectPage = 0x8B,
	Dec_DirectPagePlusX = 0x9B,
	Dec_Absolute = 0x8C,
	Dec_X = 0x1D,
	Dec_Y = 0xDC,
	
	// Shift, rotation commands
	Asl_A = 0x1C,
	Asl_DirectPage = 0x0B,
	Asl_DirectPagePlusX = 0x1B,
	Asl_Absolute = 0x0C,

	Lsr_A = 0x5C,
	Lsr_DirectPage = 0x4B,
	Lsr_DirectPageX = 0x5B,
	Lsr_Absolute = 0x4C,

	Rol_A = 0x3C,
	Rol_DirectPage = 0x2B,
	Rol_DirectPagePlusX = 0x3B,
	Rol_Absolute = 0x2C,

	Ror_A = 0x7C,
	Ror_DirectPage = 0x6B,
	Ror_DirectPagePlusX = 0x7B,
	Ror_Absolute = 0x6C,

	Xcn_A = 0x9F,

	// 16-bit transmission commands
	Movw_YA_DirectPage = 0xBA,
	Movw_DirectPage_YA = 0xDA,

	// 16-bit operation commands
	Incw_DirectPage = 0x3A,
	Decw_DirectPage = 0x1A,
	Addw_YA_DirectPage = 0x7A,
	Subw_YA_DirectPage = 0x9A,
	Cmpw_YA_DirectPage = 0x5A,

	// Multiplication & Division commands
	Mul = 0xCF,
	Div = 0x9E,
	
	// Decimal compensation commands
	Daa = 0xDF,
	Das = 0xBE,

	// Program Flow commands
	Bra_BranchAlways = 0x2F,
	Beq_BranchZ1 = 0xF0,
	Bne_BranchZ0 = 0xD0,
	Bcs_BranchC1 = 0xB0,
	Bcc_BranchC0 = 0x90,
	Bvs_BranchV1 = 0x70,
	Bvc_BranchV0 = 0x50,
	Bmi_BranchN1 = 0x30,
	Bpl_BranchN0 = 0x10,
	Bbc_BranchBit0 = 0x13,
	Bbc_BranchBit1 = 0x33,
	Bbc_BranchBit2 = 0x53,
	Bbc_BranchBit3 = 0x73,
	Bbc_BranchBit4 = 0x93,
	Bbc_BranchBit5 = 0xB3,
	Bbc_BranchBit6 = 0xD3,
	Bbc_BranchBit7 = 0xF3,
	Bbs_BranchBit0 = 0x03,
	Bbs_BranchBit1 = 0x23,
	Bbs_BranchBit2 = 0x43,
	Bbs_BranchBit3 = 0x63,
	Bbs_BranchBit4 = 0x83,
	Bbs_BranchBit5 = 0xA3,
	Bbs_BranchBit6 = 0xC3,
	Bbs_BranchBit7 = 0xE3,
	Cbne_DirectPage = 0x2E,
	Cbne_DirectPagePlusX = 0xDE,
	Dbnz_DirectPage = 0x6E,
	Dbnz_Y = 0xFE,
	Jmp = 0x5F,
	Jmp_X = 0x1F,

	// Sub-routine call return commands
	Call = 0x3F,
	Pcall = 0x4F,
	Tcall0 = 0x01,
	Tcall1 = 0x11,
	Tcall2 = 0x21,
	Tcall3 = 0x31,
	Tcall4 = 0x41,
	Tcall5 = 0x51,
	Tcall6 = 0x61,
	Tcall7 = 0x71,
	Tcall8 = 0x81,
	Tcall9 = 0x91,
	TcallA = 0xA1,
	TcallB = 0xB1,
	TcallC = 0xC1,
	TcallD = 0xD1,
	TcallE = 0xE1,
	TcallF = 0xF1,
	Brk = 0x0F,
	Ret = 0x6F,
	RetI = 0x7F,

	// Stack operations
	Push_A = 0x2D,
	Push_X = 0x4D,
	Push_Y = 0x6D,
	Push_Psw = 0x0D,
	Pop_A = 0xAE,
	Pop_X = 0xCE,
	Pop_Y = 0xEE,
	Pop_Psw = 0x8E,

	// Bit Operation commands
	Set0 = 0x02,
	Set1 = 0x22,
	Set2 = 0x42,
	Set3 = 0x62,
	Set4 = 0x82,
	Set5 = 0xA2,
	Set6 = 0xC2,
	Set7 = 0xE2,
	Clr0 = 0x12,
	Clr1 = 0x32,
	Clr2 = 0x52,
	Clr3 = 0x72,
	Clr4 = 0x92,
	Clr5 = 0xB2,
	Clr6 = 0xD2,
	Clr7 = 0xF2,
	Tset1 = 0x0E,
	Tclr1 = 0x4E,
	And1 = 0x4A,
	And1_Not = 0x6A,
	Or1 = 0x0A,
	Or1_Not = 0x2A,
	Eor1 = 0x8A,
	Not1 = 0xEA,
	Mov1_Store = 0xAA,
	Mov1_Read = 0xCA,

	// Program Status Flag Operation
	Clrc = 0x60,
	Setc = 0x80,
	Notc = 0xED,
	Clrv = 0xE0,
	Clrp = 0x20,
	Setp = 0x40,
	Ei = 0xA0,
	Di = 0xC0,

	// Other Commands.
	Nop = 0x00,
	Sleep = 0xEF,
	Stop = 0xFF
};

#endif
