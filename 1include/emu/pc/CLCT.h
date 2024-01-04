#ifndef CLCT_H
#define CLCT_H
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string>
//#include "TaoFunc.h"
#include "CSCConstants.h"
/// TODO
//		--.pat output function
//		--Add Overlapping Hit Checking

// #define COMPILE_TYPE 0xc
// move them to constant.h
//#define LAYERS 6
//define NUM_CFEB 7
//#define STRIPS_CFEB 32			// number halfstrips in cfeb
#define RAMPAGE_SIZE 4*1024
#define LAYER_SET 1
//#define GEM_FIBERS 4


namespace cw {


	//	Structures & Classes
	struct Hit
	{
		public:
		// Data Members
		int bx;
		int hs;
		int lay;
		int COMPILE_TYPE;

		// Constructors
		Hit(void);
		Hit(int Bx, int Hs, int Layer, int Type);
		void SetOTMBCompileType(int x) { COMPILE_TYPE = x;}
    friend std::ostream& operator << (std::ostream& os, const Hit& hit);
		friend std::istream& operator >> (std::istream& is, Hit& hit);
	};

	struct Response{
		int Nhit;
		int Key;
		int PiD;
		int ResEight;
		int CC_code;
	};
	//  CSC Macro Object
	class CLCT
	{
		public:
		// Data Members
		int bx;
		int hs;
		int pat;
		int nhits;
		int COMPILE_TYPE;
		std::vector<Hit> hits;

		// Constructors
		CLCT(void);
		CLCT(int, int, int, int, int);
		CLCT(int, int, int, std::vector<Hit>&, int);
		CLCT(const CLCT& clct);

		void RelativeSpaceTime(void);		// Sets bx, hs params of <hits> relative (+-) this->bx
		void NormalSpaceTime(void);			// 	undo the above operation
		void RegenHits(void);

		// Accessing Functions
		int GetParam(int);
		void SetParam(int,int);
		void SetOTMBCompileType(int x) { COMPILE_TYPE = x;}

		// Operators
		//CLCT& operator = (const CLCT&);
		friend std::ostream& operator << (std::ostream&, const CLCT&);
		friend std::istream& operator >> (std::istream&, CLCT&);
	};
	//   CSC object for writing (.pat)
	class Group
	{
		public:
		std::vector<unsigned char> hexdata;

		Group(void);
		Group(std::vector<Hit>&, int);

		void addHit(Hit&, int);
		friend std::ostream& operator << (std::ostream&, const Group&);
	};
	//   GEM Macro object
	class Cluster
	{
		public:
		// Data Members	  Range
		int bx;		// 0-500
		int roll;	// 1-8
		int pad;	// 0-191, 192 is invalid
		int size;	// 1-8
		int layer;	// 1-2
    int realVfat;

		Cluster(void);
		Cluster(int, int, int, int, int);
		Cluster(const Cluster&);

		friend std::ostream& operator<<(std::ostream&, const Cluster&);
		friend std::istream& operator>>(std::istream&, Cluster&);

		unsigned int info(void);
	};
	//    GEM object for writing (.pat)
	struct gemPacket {
		int				num_clusters;
		std::vector<unsigned int>	raw_info;

		gemPacket(void);
		gemPacket(std::vector<Cluster>& iClu,	unsigned int sInd = 0);

		friend std::ostream& operator<<(std::ostream&, const gemPacket&);	// writes 8 bytes (i.e. 1 bx of gem data for a single channel)
	};



/////////////////////// CCLUT ////////////////////////////////////////////////////
class CCLUT
	{
	public:
		// Data Members
		int bx;
		int hs;
		int pat;
		int LUT_code;
		int COMPILE_TYPE;
		int eightres;
		int bending;
		int lr;
		std::vector<Hit> hits;

		// Constructors
		CCLUT(void);
		CCLUT(int, int, int, int, int);
		CCLUT(int, int, int, int, int, int, int, int);
		CCLUT(const CCLUT& cclut);

		void RelativeSpaceTime(void);		// Sets bx, hs params of <hits> relative (+-) this->bx
		void NormalSpaceTime(void);			// 	undo the above operation
		void RegenHits(bool input = true, int Bit11_true = 0);
		void Convert_LUT_to_bending(/* arguments */) ;

		// Accessing Functions
		int GetParam(int);
		void SetParam(int,int);
		void SetOTMBCompileType(int x) { COMPILE_TYPE = x;}


		// Operators
		//CCLUT& operator = (const CCLUT&);
		friend std::ostream& operator << (std::ostream&, const CCLUT&);
		friend std::istream& operator >> (std::istream&, CCLUT&);
	};

//////////////////////////////////////////////////////////////////////////////////


	// Positioning Functions
	int GetCFEB(int hs);		//	Out:	Cfeb given half strip
	int GetLocal(int hs);		//	Out:	Halfstrip relative to CFEB

	// Text File Parsers / Writers (human readable)
	int ReadTxt(std::string&, std::vector<CLCT>&);					// input : file prefix ONLY
	std::string ReadTxt(std::string&, std::vector<CLCT>&, std::vector<Cluster>&);	// input : (file prefix) + ".txt"	GEM Capable!!

	void WriteTxt(std::string&, std::vector<CLCT>&);

	// Writes Patterns (.pat) to be loaded to EmuBoard
	bool WritePat(std::string&, std::vector<CLCT>&);	// CSC
	bool WritePat(std::string&, std::vector<Hit>& );	// comparator hit
	bool WritePat(std::string&, std::vector<Cluster>&);	// GEM

	// For Sorting purposes
	bool compareHit(Hit, Hit);
	bool compareCLCT(CLCT, CLCT);

	void ExtractHits(std::vector<CLCT>& clcts, std::vector<Hit>& hits, int feb = -1);
	void CollectClusters(std::vector<Cluster>& pads, std::vector<Cluster>* in_pads, int layer, int gem_fiber = -1);

	// Print object data to console
	void DumpGroup(Group grp, int Bx);
	void DumpHit(Hit&);

	void PrintCLCT(CLCT&, std::ostream& oss = (std::cout), bool opt = true);
	void PrintCCLUT(CCLUT&, std::ostream& oss = (std::cout), bool opt = true);

/////////////////////// CCLUT - Helper Functions ////////////////////////////////////////////////////
// Text File Parsers / Writers (human readable)
  bool Hits_Generator_LUT(int Bx, int Key, int Pat, int LUT_code, std::vector<Hit>& hits, int COMPILE_TYPE, bool input, int Bit11_int);
	bool Check_Hits(CCLUT Send, CCLUT Read);
	bool Check_Hits_map(std::vector<std::vector<int> > VisualMap_input, std::vector<std::vector<int> > &VisualMap_output);
	void generate_response(CCLUT Send, CCLUT Read, CCLUT ExpectedTrigger, CCLUT Corr, int corr, CCLUT Accuracy, CCLUT Accuracy_1, int accu, std::vector<std::vector<std::string> > &response, int Bit11_int);
	void Update_Correlation(CCLUT Send, CCLUT Read, CCLUT &Corr, int accuracy = 0);
	void Plot_And_Compare_Hits(CCLUT Resp, std::vector<std::vector<int> > &VisualMap);
	CCLUT ExpectedTriggering(CCLUT input, int Bit11_int, int mute);
	std::vector<double> Get_Expexted_Key_Input(CCLUT Res);
  int EightResolution(int KeyLayer, int CCLUT_code, int PiD);
	int GetNHits(int CCLUT_code);

	int ReadTxt(std::string&, std::vector<CCLUT>&);					// input : file prefix ONLY
	std::string ReadTxt(std::string&, std::vector<CCLUT>&, std::vector<Cluster>&);	// input : (file prefix) + ".txt"	GEM Capable!!

	void WriteTxt(std::string&, std::vector<CCLUT>&);

	// Writes Patterns (.pat) to be loaded to EmuBoard
	bool WritePat(std::string&, std::vector<CCLUT>&);	// CSC

	void ExtractHits(std::vector<CCLUT>& ccluts, std::vector<Hit>& hits, int feb = -1);

	void TMB_Check(std::vector<CCLUT>&, std::string&);
///////////////////////////////////////////////////////////////////////////////////

}
#endif
