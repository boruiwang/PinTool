#include "pin.H"
#include <iostream>
#include <fstream>

/* ===================================================================== */
/* Names of all the flags*/
/* ===================================================================== */
#define DUMMY_FIXED_PIM_BEGIN "dummy_fixed_pim_begin"
#define DUMMY_FIXED_PIM_END "dummy_fixed_pim_end"
#define DUMMY_GEN_PIM_BEGIN "dummy_gen_pim_begin"
#define DUMMY_GEN_PIM_END "dummy_gen_pim_end"
#define DUMMY_MAIN_BEGIN "dummy_main_begin"
#define DUMMY_MAIN_END "dummy_main_end"

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */
std::ofstream LogFile;
bool fixed_pim_flag = false;
bool gen_pim_flag = false;
bool main_flag = false;

int fix_IterationTime = 0;
int gen_IterationTime = 0;

UINT64 com_count = 0;
UINT64 mem_count = 0;

UINT64 gen_pim_count = 0;
UINT64 gen_pim_mem_count = 0;

UINT64 fixed_pim_count = 0;
UINT64 fixed_pim_mem_count = 0;

/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "pimLog.out", "specify trace file name");

/* ===================================================================== */
/* Analysis routines                                                     */
/* ===================================================================== */
VOID set_fixed_pim_begin_flag()
{
    fixed_pim_flag = true;
    fix_IterationTime += 1;
}

VOID set_fixed_pim_end_flag()
{
    LogFile << "fix_K_" << fix_IterationTime << " = " << fixed_pim_count << endl;
    LogFile << "fix_M_" << fix_IterationTime << " = " << fixed_pim_mem_count << endl;    
    fixed_pim_flag = false;
}

VOID set_gen_pim_begin_flag()
{
    gen_IterationTime += 1;
    gen_pim_flag = true;
    LogFile << "Iteration " << gen_IterationTime << endl;
}

VOID set_gen_pim_end_flag()
{
    LogFile << "prog_K_" << gen_IterationTime << " = " << gen_pim_count << endl;
    LogFile << "prog_M_" << gen_IterationTime << " = " << gen_pim_mem_count << endl;
    gen_pim_flag = false;
}

VOID set_main_begin_flag() {
    main_flag = true;
    LogFile << "main_start" << main_flag << endl;
}

VOID set_main_end_flag() {
    main_flag = false;
    LogFile << "main_end" << main_flag << endl;
}

// count fix compute instructions
VOID count_fixed_com_ins()
{
    if(main_flag) {
        if(fixed_pim_flag) {
            fixed_pim_count += 1;
            com_count -= 1;
        }
    }
}

// count fix memory instructions
VOID count_fixed_mem_ins(VOID * ip, VOID * addr)
{
    if(main_flag) {
        if(fixed_pim_flag) {
            fixed_pim_mem_count += 1;
            mem_count -= 1;
        }
    }
}

// count gen compute instructions
VOID count_gen_com_ins()
{  
    if(main_flag) {
        if(gen_pim_flag)
            gen_pim_count += 1;
        else
            com_count += 1;
    }
}

// count gen memory instructions
VOID count_gen_mem_ins(VOID * ip, VOID * addr)
{   
    if(main_flag) {
        if(gen_pim_flag)
            gen_pim_mem_count += 1;
        else
            mem_count += 1;
    }
}

/* ===================================================================== */
/* Instrumentation routines                                              */
/* ===================================================================== */
   
VOID Image(IMG img, VOID *v)
{
    // Instrument the dummy_fixed_pim_begin() and dummy_fixed_pim_end() functions.

    //  Find the dummy_fixed_pim_begin() function.
    RTN fixed_pim_begin = RTN_FindByName(img, DUMMY_FIXED_PIM_BEGIN);
    if (RTN_Valid(fixed_pim_begin))
    {
        RTN_Open(fixed_pim_begin);
        // Instrument dummy_fixed_pim_begin() to set begin flag
        RTN_InsertCall(fixed_pim_begin, IPOINT_AFTER,
                        (AFUNPTR)set_fixed_pim_begin_flag,
                        IARG_END);
        RTN_Close(fixed_pim_begin);
    }

    // Find the dummy_fixed_pim_end() function.
    RTN fixed_pim_end = RTN_FindByName(img, DUMMY_FIXED_PIM_END);
    if (RTN_Valid(fixed_pim_end))
    {
        RTN_Open(fixed_pim_end);
        // Instrument dummy_fixed_pim_end() to set end flag
        RTN_InsertCall(fixed_pim_end, IPOINT_BEFORE, 
                        (AFUNPTR)set_fixed_pim_end_flag,
                        IARG_END);

        RTN_Close(fixed_pim_end);
    }

    //  Find the dummy_gen_pim_begin() function.
    RTN gen_pim_begin = RTN_FindByName(img, DUMMY_GEN_PIM_BEGIN);
    if (RTN_Valid(gen_pim_begin))
    {
        RTN_Open(gen_pim_begin);
        // Instrument dummy_gen_pim_begin() to set begin flag
        RTN_InsertCall(gen_pim_begin, IPOINT_AFTER,
                        (AFUNPTR)set_gen_pim_begin_flag,
                        IARG_END);
        RTN_Close(gen_pim_begin);
    }

    // Find the dummy_gen_pim_end() function.
    RTN gen_pim_end = RTN_FindByName(img, DUMMY_GEN_PIM_END);
    if (RTN_Valid(gen_pim_end))
    {
        RTN_Open(gen_pim_end);
        // Instrument dummy_gen_pim_end() to set end flag
        RTN_InsertCall(gen_pim_end, IPOINT_BEFORE, 
                        (AFUNPTR)set_gen_pim_end_flag,
                        IARG_END);

        RTN_Close(gen_pim_end);
    }

    //  Find the dummy_main_begin() function.
    RTN main_begin = RTN_FindByName(img, DUMMY_MAIN_BEGIN);
    if (RTN_Valid(main_begin))
    {
        RTN_Open(main_begin);
        // Instrument dummy_gen_pim_begin() to set begin flag
        RTN_InsertCall(main_begin, IPOINT_AFTER,
                        (AFUNPTR)set_main_begin_flag,
                        IARG_END);
        RTN_Close(main_begin);
    }

    //  Find the dummy_main_end() function.
    RTN main_end = RTN_FindByName(img, DUMMY_MAIN_END);
    if (RTN_Valid(main_end))
    {
        RTN_Open(main_end);
        // Instrument dummy_gen_pim_begin() to set begin flag
        RTN_InsertCall(main_end, IPOINT_AFTER,
                        (AFUNPTR)set_main_end_flag,
                        IARG_END);
        RTN_Close(main_end);
    }
}

/* ===================================================================== */

VOID Instruction(INS ins, VOID *v)
{   
    //prog_pim
    UINT32 genMemOperands = INS_MemoryOperandCount(ins);
    // Iterate over each memory operand of the instruction.
    for (UINT32 genMemOp = 0; genMemOp < genMemOperands; genMemOp++)
    {
        // Note that in some architectures a single memory operand can be 
        // both read and written (for instance incl (%eax) on IA-32)
        // In that case we instrument it once for read and once for write.
        if (INS_MemoryOperandIsRead(ins, genMemOp) || INS_MemoryOperandIsWritten(ins, genMemOp))
        {   
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)count_gen_mem_ins,
                                     IARG_INST_PTR,
                                     IARG_MEMORYOP_EA, genMemOp,
                                     IARG_END);
        }
    }
    
    if(genMemOperands == 0)
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_gen_com_ins, IARG_END);
    
    // fix_pim
    UINT32 fixedMemOperands = INS_MemoryOperandCount(ins);  
    // Iterate over each memory operand of the instruction.
    for (UINT32 fixMemOp = 0; fixMemOp < fixedMemOperands; fixMemOp++)
    {
        // Note that in some architectures a single memory operand can be 
        // both read and written (for instance incl (%eax) on IA-32)
        // In that case we instrument it once for read and once for write.
        if (INS_MemoryOperandIsRead(ins, fixMemOp) || INS_MemoryOperandIsWritten(ins, fixMemOp))
        {   
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)count_fixed_mem_ins,
                                     IARG_INST_PTR,
                                     IARG_MEMORYOP_EA, fixMemOp,
                                     IARG_END);
        }
    }
    
    if(fixedMemOperands == 0)
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_fixed_com_ins, IARG_END);

}

/* ===================================================================== */

VOID Fini(INT32 code, VOID *v)
{
    LogFile << "==========Total instruction=========" << endl;
    LogFile << "fix_K_" << fix_IterationTime << " = " << fixed_pim_count << endl;
    LogFile << "fix_M_" << fix_IterationTime << " = " << fixed_pim_mem_count << endl;  
    LogFile << "prog_K_" << gen_IterationTime << " = " << gen_pim_count << endl;
    LogFile << "prog_M_" << gen_IterationTime << " = " << gen_pim_mem_count << endl;
    LogFile << "K = " << com_count << endl;
    LogFile << "M = " << mem_count << endl;
    LogFile << "====================================";
    LogFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */
   
INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instruction executed." << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    // Initialize pin & symbol manager
    PIN_InitSymbols();
    if (PIN_Init(argc,argv))
    {
        return Usage();
    }
    
    // Write to a file since cout and cerr maybe closed by the application
    LogFile.open(KnobOutputFile.Value().c_str());
    LogFile.setf(ios::showbase);
    
    // Register Image to be called to instrument functions.
    IMG_AddInstrumentFunction(Image, 0);
    
    // Register Instruction to be called to instrument instruction
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();
    
    return 0;
}