#include "pin.H"
#include <iostream>
#include <fstream>

/* ===================================================================== */
/* Names of pim_begin and pim_end */
/* ===================================================================== */
#define DUMMY_FIXED_PIM_BEGIN "dummy_fixed_pim_begin"
#define DUMMY_FIXED_PIM_END "dummy_fixed_pim_end"

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */

std::ofstream LogFile;
bool fixed_pim_flag = false;
UINT64 fixed_pim_count = 0;

/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "pimLog.out", "specify trace file name");

/* ===================================================================== */


/* ===================================================================== */
/* Analysis routines                                                     */
/* ===================================================================== */
 
VOID set_fixed_pim_begin_flag()
{
    fixed_pim_flag = true;
}

VOID set_fixed_pim_end_flag()
{
    fixed_pim_flag = false;
}

VOID count_fixed_pim()
{
    fixed_pim_count += 1;
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
}

/* ===================================================================== */

VOID Instruction(INS ins, VOID *V)
{   
    //LogFile << fixed_pim_flag << endl;
    if (fixed_pim_flag)
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_fixed_pim, IARG_END);

}

/* ===================================================================== */

VOID Fini(INT32 code, VOID *v)
{
    LogFile << "==============================" << endl;
    LogFile << "fixed_pim_count= " << fixed_pim_count << endl;
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