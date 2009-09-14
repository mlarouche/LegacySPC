#!/usr/bin/env ruby
# LegacySPC - A portable object-oriented SPC emulator.
# Copyright (c) 2007 by Michaël Larouche <larouche@kde.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; version 2 of the
# License
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

class DisassemblerEntry
	attr_accessor :numBytes
	attr_accessor :opcodeName
	attr_accessor :arguments

 	def initialize(numBytes, opcodeName, args)
		@numBytes = numBytes
		@opcodeName = opcodeName
		@arguments = args
	end
end

# Input data
opcodeListString = <<LIST
  MOV    A,#inm      E8    2
  MOV    A,(X)       E6    1
  MOV    A,(X)+      BF    1
  MOV    A,dp        E4    2
  MOV    A,dp+X      F4    2
  MOV    A,abs       E5    3
  MOV    A,abs+X     F5    3
  MOV    A,abs+Y     F6    3
  MOV    A,(dp+X)    E7    2
  MOV    A,(dp)+Y    F7    2
  MOV    X,#inm      CD    2
  MOV    X,dp        F8    2
  MOV    X,dp+Y      F9    2
  MOV    X,abs       E9    3
  MOV    Y,#inm      8D    2
  MOV    Y,dp        EB    2
  MOV    Y,dp+X      FB    2
  MOV    Y,abs       EC    3
  MOV    (X),A       C6    1
  MOV    (X)+,A      AF    1
  MOV    dp,A        C4    2
  MOV    dp+X,A      D4    2
  MOV    abs,A       C5    3
  MOV    abs+X,A     D5    3
  MOV    abs+Y,A     D6    3
  MOV    (dp+X),A    C7    2
  MOV    (dp)+Y,A    D7    2
  MOV    dp,X        D8    2
  MOV    dp+Y,X      D9    2
  MOV    abs,X       C9    3
  MOV    dp,Y        CB    2
  MOV    dp+X,Y      DB    2
  MOV    abs,Y       CC    3
  MOV    A,X         7D    1
  MOV    A,Y         DD    1
  MOV    X,A         5D    1
  MOV    Y,A         FD    1
  MOV    X,SP        9D    1
  MOV    SP,X        BD    1
  MOV    dp,dp       FA    3
  MOV    dp,#inm     8F    3
  ADC    A,#inm      88    2
  ADC    A,(X)       86    1
  ADC    A,dp        84    2
  ADC    A,dp+X      94    2
  ADC    A,abs       85    3
  ADC    A,abs+X     95    3
  ADC    A,abs+Y     96    3
  ADC    A,(dp+X)    87    2
  ADC    A,(dp)+Y    97    2
  ADC    (X),(Y)     99    1
  ADC    dp,dp       89    3
  ADC    dp,#inm     98    3
  SBC    A,#inm      A8    2
  SBC    A,(X)       A6    1
  SBC    A,dp        A4    2
  SBC    A,dp+X      B4    2
  SBC    A,abs       A5    3
  SBC    A,abs+X     B5    3
  SBC    A,abs+Y     B6    3
  SBC    A,(dp+X)    A7    2
  SBC    A,(dp)+Y    B7    2
  SBC    (X),(Y)     B9    1
  SBC    dp,dp       A9    3
  SBC    dp,#inm     B8    3
  CMP    A,#inm      68    2
  CMP    A,(X)       66    1
  CMP    A,dp        64    2
  CMP    A,dp+X      74    2
  CMP    A,abs       65    3
  CMP    A,abs+X     75    3
  CMP    A,abs+Y     76    3
  CMP    A,(dp+X)    67    2
  CMP    A,(dp)+Y    77    2
  CMP    (X),(Y)     79    1
  CMP    dp,dp       69    3
  CMP    dp,#inm     78    3
  CMP    X,#inm      C8    2
  CMP    X,dp        3E    2
  CMP    X,abs       1E    3
  CMP    Y,#inm      AD    2
  CMP    Y,dp        7E    2
  CMP    Y,abs       5E    3
  AND    A,#inm      28    2
  AND    A,(X)       26    1
  AND    A,dp        24    2
  AND    A,dp+X      34    2
  AND    A,abs       25    3
  AND    A,abs+X     35    3
  AND    A,abs+Y     36    3
  AND    A,(dp+X)    27    2
  AND    A,(dp)+Y    37    2
  AND    (X),(Y)     39    1
  AND    dp,dp       29    3
  AND    dp,#inm     38    3
  OR     A,#inm      08    2
  OR     A,(X)       06    1
  OR     A,dp        04    2
  OR     A,dp+X      14    2
  OR     A,abs       05    3
  OR     A,abs+X     15    3
  OR     A,abs+Y     16    3
  OR     A,(dp+X)    07    2
  OR     A,(dp)+Y    17    2
  OR     (X),(Y)     19    1
  OR     dp,dp       09    3
  OR     dp,#inm     18    3
  EOR    A,#inm      48    2
  EOR    A,(X)       46    1
  EOR    A,dp        44    2
  EOR    A,dp+X      54    2
  EOR    A,abs       45    3
  EOR    A,abs+X     55    3
  EOR    A,abs+Y     56    3
  EOR    A,(dp+X)    47    2
  EOR    A,(dp)+Y    57    2
  EOR    (X),(Y)     59    1
  EOR    dp,dp       49    3
  EOR    dp,#inm     58    3
  INC      A         BC    1
  INC      dp        AB    2
  INC     dp+X       BB    2
  INC     abs        AC    3
  INC      X         3D    1
  INC      Y         FC    1
  DEC      A         9C    1
  DEC      dp        8B    2
  DEC     dp+X       9B    2
  DEC     abs        8C    3
  DEC      X         1D    1
  DEC      Y         DC    1
  ASL      A         1C    1
  ASL      dp        0B    2
  ASL     dp+X       1B    2
  ASL     abs        CC    3
  LSR      A         5C    1
  LSR      dp        4B    2
  LSR     dp+X       5B    2
  LSR     abs        4C    3
  ROL      A         3C    1
  ROL      dp        2B    2
  ROL     dp+X       3B    2
  ROL     abs        2C    3
  ROR      A         7C    1
  ROR      dp        6B    2
  ROR     dp+X       7B    2
  ROR     abs        6C    3
  XCN      A         9F    1
  MOVW     YA,dp     BA    2
  MOVW     dp,YA     DA    2
  INCW     dp        3A    2
  DECW     dp        1A    2
  ADDW    YA,dp      7A    2
  SUBW    YA,dp      9A    2
  CMPW    YA,dp      5A    2
  MUL      YA        CF    1
  DIV      YA,X      9E    1
  DAA       A        DF    1
  DAS       A        BE    1
  BRA     rel        2F    2
  BEQ     rel        F0    2
  BNE     rel        D0    2
  BCS     rel        B0    2
  BCC     rel        90    2
  BVS     rel        70    2
  BVC     rel        50    2
  BMI     rel        30    2
  BPL     rel        10    2
  CBNE   dp,rel      2E    3
  CBNE  dp+X,rel     DE    3
  DBNZ   dp,rel      6E    3
  DBNZ    Y,rel      FE    2
  JMP     abs        5F    3
  JMP    (abs+X)     1F    3
  CALL     abs       3F    3
  PCALL     rel      4F    2
  BRK       nil      0F    1
  RET       nil      6F    1
  RET1      nil      7F    1
  PUSH      A        2D    1
  PUSH      X        4D    1
  PUSH      Y        6D    1
  PUSH     PSW       0D    1
  POP       A        AE    1
  POP       X        CE    1
  POP       Y        EE    1
  POP      PSW       8E    1
  TSET1    abs       0E    3
  TCLR1    abs       4E    3
  CLRC     nil       60    1
  SETC     nil       80    1
  NOTC     nil       ED    1
  CLRV     nil       E0    1
  CLRP     nil       20    1
  SETP     nil       40    1
  EI       nil       A0    1
  DI       nil       C0    1
  NOP      nil       00    1
  SLEEP    nil       EF    1
  STOP     nil       FF    1
  BBC0    dp,rel     13    3
  BBC1    dp,rel     33    3
  BBC2    dp,rel     53    3
  BBC3    dp,rel     73    3
  BBC4    dp,rel     93    3
  BBC5    dp,rel     B3    3
  BBC6    dp,rel     D3    3
  BBC7    dp,rel     F3    3
  BBS0    dp,rel     03    3
  BBS1    dp,rel     23    3
  BBS2    dp,rel     43    3
  BBS3    dp,rel     63    3
  BBS4    dp,rel     83    3
  BBS5    dp,rel     A3    3
  BBS6    dp,rel     C3    3
  BBS7    dp,rel     E3    3
  CLR0    dp         02    2
  CLR1    dp         22    2
  CLR2    dp         42    2
  CLR3    dp         62    2
  CLR4    dp         82    2
  CLR5    dp         A2    2
  CLR6    dp         C2    2
  CLR7    dp         E2    2
  SET0    dp         12    2
  SET1    dp         32    2
  SET2    dp         52    2
  SET3    dp         72    2
  SET4    dp         92    2
  SET5    dp         B2    2
  SET6    dp         D2    2
  SET7    dp         F2    2
  AND1   C,rel,bit   4A    3
  AND1   C,/mem,bit  6A    3
  OR1    C,rel,bit   0A    3
  OR1    C,/mem,bit  2A    3
  EOR1   C,rel,bit   8A    3
  NOT1   rel,bit     EA    3
  MOV1   C,rel,bit   AA    3
  MOV1   rel,bit,C   CA    3
LIST

# Associate an opcode to a entry
opcodeEntryHash = {}
# List of opcode for the CPU instructions
opcodeList = []

opcodeEntryHashRubyCode = "opcodeEntryHash = { "

# Parse the input date to extract the opcodes and argument information
opcodeListString.split("\n").each do |line|
	lineComponents = line.strip.split(" ")
	
	opcodeList << lineComponents[0]

	opcodeName = "\"#{lineComponents[0]}\""
	argumentsList = []

	if lineComponents[1] != "nil" then
		lineComponents[1].split(',').each do |arg|
			argumentsList << "\"#{arg}\""
		end
	end

	opcodeHex = lineComponents[2]
	numBytes = lineComponents[3]

	opcodeEntryHashRubyCode += "0x#{opcodeHex} => DisassemblerEntry.new(#{numBytes},#{opcodeName}, [#{argumentsList.join(",")}]),"
end
opcodeEntryHashRubyCode += "}"

# Add UNK to the list
opcodeList << "UNK"

# Remove duplicate in list
opcodeList = opcodeList.uniq.sort

# Populate the opcodeEntryHash hash.
eval opcodeEntryHashRubyCode

argumentEnumHash =
{
	"A" => "A_Argument",
	"X" => "X_Argument",
	"Y" => "Y_Argument",
	"YA" => "YA_Argument",
	"SP" => "StackPointer_Argument",
	"PSW" => "ProgramStatusWard_Argument",
	"#inm" => "Immediate_Argument",
	"(X)" => "IndirectX_Argument",
	"(X)+" => "IndirectXAutoIncrement_Argument",
	"(Y)" => "IndirectY_Argument",
	"dp" => "DirectPage_Argument",
	"dp+X" => "DirectPagePlusX_Argument",
	"dp+Y" => "DirectPagePlusY_Argument",
	"rel" => "Relative_Argument",
	"abs" => "Absolute_Argument",
	"abs+X" => "AbsolutePlusX_Argument",
	"abs+Y" => "AbsolutePlusY_Argument",
	"(dp+X)" => "IndirectDirectPagePlusX_Argument",
	"(dp)+Y" => "IndirectDirectPagePlusY_Argument",
	"(abs+X)" => "IndirectAbsolutePlusX_Argument",
	"C" => "Carry_Argument",
	"bit" => "Bit_Argument",
	"/mem" => "ComplementBit_Argument"
}

# Generate the table for the opcode name
# along with the enum index list
prettyOpcodeName = []
prettyOpcodeEnumList = []
formattedOpcodeNameList = []

index = 0
opcodeList.each do |opcode|
	prettyOpcodeName  << "#{opcode}_Pretty"
	prettyOpcodeEnumList << "#{opcode}_Pretty = #{index}"
	formattedOpcodeNameList << "\"#{opcode}\""
	index += 1
end

puts "const char prettyOpcodeName[][10] = {"
puts formattedOpcodeNameList.join(",")
puts "};"
puts ""

puts "enum PrettyOpcodeIndex {"
puts prettyOpcodeEnumList.join(",\n")
puts "};"
puts ""

# Generate the dissambler table
disassemblerTableList = []

(0..0xFF).each do |opcodeHex|
	entry = opcodeEntryHash[opcodeHex]

	if entry == nil then
		entry = DisassemblerEntry.new(1, "UNK", [])
	end

	argumentList = []
	if entry.arguments.size == 0 then
		argumentList = [-1,-1,-1]
	else
		entry.arguments.each do |arg|
			argumentList << argumentEnumHash[arg]
		end
		if argumentList.size == 1 then
			argumentList << [-1,-1]
		elsif argumentList.size == 2 then
			argumentList << -1
		end
	end

	opcodeIndex = opcodeList.index(entry.opcodeName)
	disassemblerTableList << "DisassemblerEntry(0x#{opcodeHex.to_s(16)}, #{prettyOpcodeName[opcodeIndex]}, #{entry.numBytes}, #{argumentList.join(",")})"
end

puts "enum ArgumentTypeEnum {"
puts argumentEnumHash.values.join(",\n")
puts "};"
puts ""

puts "DisassemblerEntry disassemblerTable[0xFF+1] = {"
puts disassemblerTableList.join(",\n")
puts "};"