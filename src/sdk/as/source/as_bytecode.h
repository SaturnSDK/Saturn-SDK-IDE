/*
   AngelCode Scripting Library
   Copyright (c) 2003-2005 Andreas J�nsson

   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.

   The original version of this library can be located at:
   http://www.angelcode.com/angelscript/

   Andreas J�nsson
   andreas@angelcode.com
*/


//
// as_bytecode.h
//
// A class for constructing the final byte code
//



#ifndef AS_BYTECODE_H
#define AS_BYTECODE_H

#include "as_bytecodedef.h"
#include "as_types.h"
#include "as_array.h"

BEGIN_AS_NAMESPACE

#define BYTECODE_SIZE  4
#define MAX_DATA_SIZE  8
#define MAX_INSTR_SIZE (BYTECODE_SIZE+MAX_DATA_SIZE)

class asCModule;
class asCScriptEngine;
class cByteInstruction;

class asCByteCode
{
public:
	asCByteCode();
	~asCByteCode();

	void ClearAll();

	int GetSize();

	void Finalize();

	bool IsComplex();
		
	int Optimize();
	void ExtractLineNumbers();
	bool IsCombination(cByteInstruction *curr, asBYTE bc1, asBYTE bc2);
	bool IsCombination(cByteInstruction *curr, asBYTE bc1, asBYTE bc2, asBYTE bc3);
	cByteInstruction *ChangeFirstDeleteNext(cByteInstruction *curr, asBYTE bc);
	cByteInstruction *DeleteFirstChangeNext(cByteInstruction *curr, asBYTE bc);
	cByteInstruction *DeleteInstruction(cByteInstruction *instr);
	void RemoveInstruction(cByteInstruction *instr);
	void InsertBefore(cByteInstruction *before, cByteInstruction *instr);
	bool MatchPattern(cByteInstruction *curr);
	cByteInstruction *OptimizePattern(cByteInstruction *curr);
	bool CanBeSwapped(cByteInstruction *curr);
	int  ResolveJumpAddresses();
	int  FindLabel(int label, cByteInstruction *from, cByteInstruction **dest, int *positionDelta);

	void AddPath(asCArray<cByteInstruction *> &paths, cByteInstruction *instr, int stackSize);

	void Output(asBYTE *array);
	void AddCode(asCByteCode *bc);

	void PostProcess();
	void DebugOutput(const char *name, asCModule *module, asCScriptEngine *engine);

	int  GetLastCode();
	int  RemoveLastCode();

	void GetVarsUsed(asCArray<int> &vars);
	bool IsVarUsed(int offset);
	void ExchangeVar(int oldOffset, int newOffset);

	void Label(short label);
	void Line(int line, int column);
	void Call(int bc, int funcID, int pop);
	void Alloc(int bc, int objID, int funcID, int pop);
	void Ret(int pop);
	void JmpP(asDWORD max);
	void Destructor(int bc, asDWORD destr, int sfOffset);

	int InsertFirstInstrDWORD(int bc, asDWORD param);
	int InsertFirstInstrQWORD(int bc, asQWORD param);
	int Instr(int bc);
	int InstrQWORD(int bc, asQWORD param);
	int InstrDOUBLE(int bc, double param);
	int InstrDWORD(int bc, asDWORD param);
	int InstrWORD(int bc, asWORD param);
	int InstrBYTE(int bc, asBYTE param);
	int InstrSHORT(int bc, short param);
	int InstrFLOAT(int bc, float param);
	int InstrINT(int bc, int param);

	int Pop (int numDwords);
	int Push(int numDwords);

	asCArray<int> lineNumbers;
	int largestStackUsed;

protected:
	int AddInstruction();
	int AddInstructionFirst();

	cByteInstruction *first;
	cByteInstruction *last;
};

class cByteInstruction
{
public:
	cByteInstruction();

	void AddAfter(cByteInstruction *nextCode);
	void AddBefore(cByteInstruction *nextCode);
	void Remove();

	int  GetSize();
	int  GetStackIncrease();

	cByteInstruction *next;
	cByteInstruction *prev;

	asDWORD op;
	asBYTE arg[MAX_DATA_SIZE];
	int size;
	int stackInc;

	// Testing
	bool marked;
	int stackSize;
};

END_AS_NAMESPACE

#endif
