/*	This is for version control
 *
 *		~~Cameron (06/24/2019)
 *
 *
 * */

#ifndef _Emu_PC_CSCGEMTestApplication_h_
#define _Emu_PC_CSCGEMTestApplication_h_

#include "xdaq/WebApplication.h"
#include "emu/pc/TMB.h"
//#include "GEMPatternGen.h"
//#include "PatternGen.h"
//#include "CSCConstants.h"
//#include "pattern_convert.h"
//#include "Yuriy_CLCT_PatternGen.h"
#include "Set.h"
#include <unistd.h>

//
#include "emu/pc/EmuPeripheralCrateBase.h"
#include "emu/pc/ChamberUtilities.h"
#include "emu/pc/EmuPeripheralCrateConfig.h"




namespace emu { namespace pc {

/** \class CSCGEMTestApplication
 * main web GUI Application class for the CSC-GEM test stand
 */
class CSCGEMTestApplication: public EmuPeripheralCrateBase
{
protected:
  //parsing section
  xdata::String XML_or_DB_;
  xdata::String xmlFile_;
  int parsed;
  std::string Valid_config_ID;
  int xml_or_db;
  std::vector<Crate*> crateVector;
  unsigned int total_crates_;
  int this_crate_no_;
  xdata::String standalone;
  bool standalone_;
  std::vector<DAQMB*> dmbVector;
  std::ostringstream OutputDMBTests[10][30];
  int current_crate_;
  std::ostringstream OutputTMBTests[10][30];
  Crate *thisCrate;
  std::ostringstream ChamberTestsOutput[10][30];
  std::ostringstream OutputCCBTests[30];
  std::string ThisCrateID_;
  CCB* thisCCB ;
  MPC * thisMPC;
  std::vector<Chamber*> chamberVector;
  //std::vector<DDU*> dduVector;
  //std::vector<TMBTester>   tmbTestVector;
  ChamberUtilities MyTest[10][30];
  int able_to_load_alct[9];
  EmuPeripheralCrateConfig * EPCConfig;
  int TMB_, DMB_,RAT_;

  char GemtxtMounNchar[6];
  int  GemtxtMounNint;
  bool startgemtxt;

  char DirGen[200];
  char DirYuriyConvtxt[200];
  char DirYuriyCLCTConvPat[200];
  char DirGEMPat[200];
  char DirCfebPat[200];
  char DirYuriyCLCTConvtxt[200];
  char DirGEMtxt[200];
  char DirCfebtxt[200];
  char DirUp[200];
  char YuriyConvtxtFile[200];
  char YuriyCLCTConvtxtFile[200];
  char GEMtxtFile[200];
  char CfebtxtFile[200];
  //char YuriyConvPatFile[200];
  char CfebPatFile[200];
  char DCFEBN[10];
  char HalfStripN[10];
  char FiberN[10];
  char PatFile[200];
  char NumRuns[20];
  char testingarray[512][5];
  char TempCharArray[7][512][5];    ///this might be unnecessarialy large
  char FileNameFiberN[8][400];

  char Fiber0PatName[200];
  char Fiber1PatName[200];
  char Fiber2PatName[200];
  char Fiber3PatName[200];
  char Fiber4PatName[200];
  char Fiber5PatName[200];
  char Fiber6PatName[200];
  char Fiber7PatName[200];

  bool PatLoadError;

  std::vector<TMB*>   tmbVector;

  ///////////////////////////////////////////////////////////////////////

  cw::Set patternSet;

  char GetOTMBType[1] = {'c'};
  int Bit11_int = 0;  // 11 or 12 bit CC_CODE
  int Run3_int = 0;  // Run2 (default) or Run3 output style
  int CFEB0123_delay = 20;
  int CFEB456_delay  = 20;
  int CFEB0123_delay_read = -999;
  int CFEB456_delay_read  = -999;
  char mute_char[32];
  char run2_char[32];
  char bit11_char[32] = {'0'};
  char run3_char[32] = {'0'};
  char CFEB0123_delay_char[32] = {'20'};
  char CFEB456_delay_char[32] =  {'20'};

  // new Comparator hit Buffers
  char GetHit_muon1_layer0_char[6];
  char GetHit_muon1_layer1_char[6];
  char GetHit_muon1_layer2_char[6];
  char GetHit_muon1_layer3_char[6];
  char GetHit_muon1_layer4_char[6];
  char GetHit_muon1_layer5_char[6];
  char GetHit_muon1_bx_char[6];


  // new CLCT Input Buffers
  char GetCLCT_bx_char[6];
  char GetCLCT_key_char[6];
  char GetCLCT_pid_char[6];
  char GetCLCT_nhit_char[6];
  // new GEM Input Buffers
  char GetGEM_bx_char[6];
  char GetGEM_roll_char[6];
  char GetGEM_pad_char[6];
  char GetGEM_size_char[6];
  char GetGEM_layer_char[6];
  // new CLCT Input Data
  int GetCLCT_bx_int;
  int GetCLCT_key_int;
  int GetCLCT_pid_int;
  int GetCLCT_nhit_int;
  // new GEM Input Data
  int GetGEM_bx_int;
  int GetGEM_roll_int;
  int GetGEM_pad_int;
  int GetGEM_size_int;
  int GetGEM_layer_int;


  ///////// CCLUT ////////////
  // new CLCT Input Buffers
  char GetCCLUT_bx_char[6];
  char GetCCLUT_key_char[6];
  char GetCCLUT_pid_char[6];
  char GetCCLUT_ccode_char[6];

  //GUI/////////////

  char GetCCLUT_bx_char_GUI[6];
  char GetCCLUT_key_char_GUI[6];

  int GetCCLUT_bx_int;
  int GetCCLUT_key_int;
  int GetCCLUT_pid_int;
  int GetCCLUT_ccode_int;
  ///////////////////////////

  char GetSaveDir[200];
  char GetSetPrefix[200];

  bool FileLoadedToBoard;

  // Pattern Study Automation~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  char GetNtrials_char[200];
  int  GetNtrials_int;
  int  CLCT0_Counter;
  int  CLCT1_Counter;

  std::ostringstream OutputStringTMBStatus[10];

  // Vars for AUTO pattern Studies
  int NumMuonsInStudy;
  int NumFreeParams;
  int TrialsPerStep;
  char TrialsPerStep_char[200];
  char ParamScanOutFile[200];
  char ParamScanOutFileError[200];

  //added by Kyla
  char SimulatedFileName[200];
  std::ifstream SimulatedFile;

  // new CLCT Input Buffers	// ps == Parameter Scan
  char GetCLCT_bx_ps_char[32];
  char GetCLCT_key_ps_char[32];
  char GetCLCT_pid_ps_char[32];
  char GetCLCT_nhit_ps_char[32];
  std::vector<std::string> GetCLCT_ps_char;
  // new CLCT Input Data
  int GetCLCT_bx_ps_int;
  int GetCLCT_key_ps_int;
  int GetCLCT_pid_ps_int;
  int GetCLCT_nhit_ps_int;

  // new CCLUT Input Buffers	// ps == Parameter Scan
  char GetCCLUT_bx_ps_char[32];
  char GetCCLUT_key_ps_char[32];
  char GetCCLUT_pid_ps_char[32];
  char GetCCLUT_ccode_ps_char[32];
  char GetCCLUT_bx_ps_step_char[32];
  char GetCCLUT_key_ps_step_char[32];
  char GetCCLUT_pid_ps_step_char[32];
  char GetCCLUT_ccode_ps_step_char[32];

  std::vector<std::string> GetCCLUT_ps_char;
  std::vector<std::string> GetCCLUT_ps_step_char;
  // new CCLUT Input Data
  int GetCCLUT_bx_ps_int;
  int GetCCLUT_key_ps_int;
  int GetCCLUT_pid_ps_int;
  int GetCCLUT_ccode_ps_int;

  std::vector<cw::CLCT> 	CLCT_ps_vec;
  std::vector<cw::CCLUT> 	CCLUT_ps_vec;
  std::vector<std::vector<std::vector<int> > >parameters;
  std::vector<cw::RangeParam>	Free_params;
  std::vector<cw::RangeParamCCLUT>	Free_paramsCCLUT;



public:

  //XDAQ_INSTANTIATOR();
  static xdaq::Application* instantiate(xdaq::ApplicationStub* s) throw (xdaq::exception::Exception);

  /// constructor
  CSCGEMTestApplication(xdaq::ApplicationStub * s);

  /// home-page of the application
  void Default(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);


private:

  //the location of the emulator board connection to the computer
  char* EmuBoardLocation;

  //Functions needed for parsing
  void SetCurrentCrate(int crate);

  //Parser
  bool ParsingXML();

  // Generate DCFEB data
  void GenerateDCFEBData(xgi::Input * in, xgi::Output * out );

  // Upload generated DCFEB data on emulator board's RAMs
  void UploadDCFEBData(xgi::Input * in, xgi::Output * out );

  // Check generated DCFEB data on emulator board's RAMs
  void CheckDCFEBData(xgi::Input * in, xgi::Output * out );

  // Initiate data transmission from the emulator board to the OTMB
  void TransmitDCFEBData(xgi::Input * in, xgi::Output * out );

  // Readout trigger results from OTMB
  void ReadOutTriggerResults(xgi::Input * in, xgi::Output * out );

  // Readout trigger results from OTMB
  void F0(xgi::Input * in, xgi::Output * out );

  //gets the desired number of mouns to go into the gem text file,
  //and enables the input for the gem text file
  void GenGEMPattxtFileMounN(xgi::Input * in, xgi::Output * out );

  //Creates a Text File of the GEM Pattern
  void GenGEMPattxtFile(xgi::Input * in, xgi::Output * out );

  void YuriyPatConvert(xgi::Input * in, xgi::Output * out );

  //Converts Tao's GEM .txt files to GEM .pat files
  void GemPatternConverter(xgi::Input * in, xgi::Output * out );

  void Yuriy_CLCT_PatternConverter(xgi::Input * in, xgi::Output * out );

  void CfebPatternConverter(xgi::Input * in, xgi::Output * out );

  //Test button for sending the file from the emulator board
  void SendFromEmuBoard(xgi::Input * in, xgi::Output * out );

  void OutputCLCTInfo(xgi::Input * in, xgi::Output * out );

  void OutputMPCFifo(xgi::Input * in, xgi::Output * out );

  void CrateConfiguration(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);


  // Cameron's new GUI Functions (Jan. 9 2020)
  void Bit11 (xgi::Input * in, xgi::Output * out);  // Added by sasha to support 11 and 12 bit CC
  void Run3 (xgi::Input * in, xgi::Output * out);  // Added by sasha to support Run2 and Run3  output styles
  void SetCFEBDelay (xgi::Input * in, xgi::Output * out); // Added by Sashha to chabge CFEB delays on the fly. DONT HARD RESERT AFER YOU SET IT!!!
  void GetCFEBDelay (xgi::Input * in, xgi::Output * out); // Added by Sashha to get CFEB delays.
  void GetOTMBCompileType(xgi::Input * in, xgi::Output * out);

  // Cameron's new GUI Functions (Jan. 9 2020), Tao, Oct, 2020
  void AddComparatorHit(xgi::Input * in, xgi::Output * out);

  void AddCLCT(xgi::Input * in, xgi::Output * out);

  void AddCCLUT(xgi::Input * in, xgi::Output * out);

  void AddCCLUT_GUI(xgi::Input * in, xgi::Output * out);  //Added by Sasha

  void AddGEM(xgi::Input * in, xgi::Output * out);

  void LoadToEmuBoard(xgi::Input * in, xgi::Output * out );

  void SaveCurrentSet(xgi::Input * in, xgi::Output * out );	// To Be Removed

  void SaveAsPat(xgi::Input * in, xgi::Output * out);

  void SaveAsTxt(xgi::Input * in, xgi::Output * out);

  void ClearEmuBoard(xgi::Input * in, xgi::Output * out );

  void ClearSet(xgi::Input * in, xgi::Output * out );

  void RunStudy(xgi::Input * in, xgi::Output * out );

  void PrintTMBCounters(xgi::Input * in, xgi::Output * out);

  void RunParamScan(xgi::Input * in, xgi::Output * out);

  ////////////////////////////////////////////////////////////

  void RunStudyCCLUT(xgi::Input * in, xgi::Output * out );

  void AddCLCTParamScan(xgi::Input * in, xgi::Output * out);

  void AddCCLUTParamScan(xgi::Input * in, xgi::Output * out);

  void ClearCCLUTParamScan(xgi::Input * in, xgi::Output * out );

  void RunParamScanCCLUT(xgi::Input * in, xgi::Output * out);

  void OpenSimulatedFile(xgi::Input * in, xgi::Output * out);

  void ReadSimulatedFile(xgi::Input * in, xgi::Output * out);

  void LoadSimulatedFileEvent(xgi::Input * in, xgi::Output * out);

  void AutomaticCheckingFile(xgi::Input * in, xgi::Output * out);

  void DOESWORK(xgi::Input * in, xgi::Output * out);

  void HardReset(xgi::Input * in, xgi::Output * out );
  void PrepareForTriggering(xgi::Input * in, xgi::Output * out );
};

}} // namespaces

#endif
