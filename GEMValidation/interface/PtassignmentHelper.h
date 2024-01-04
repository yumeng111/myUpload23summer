
#ifndef GEMCode_GEMValidation_Ptassignment_h
#define GEMCode_GEMValidation_Ptassignment_h

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#define PI 3.14159265

//eta partitions:1.2-1.4,1.4-1.6,1.6-1.8, 1.8-2.0, 2.0-2.2, 2.2-2.4
enum {EtaPartitions=6, Parity=4};
const double PositionEpLUT[Parity][EtaPartitions][3] = {
    		//prop_factor, slope, intercept
		{{1.279, 0.04784, 0.1122},
		 {1.279, 0.65424, 0.09761},
    		 {0.648, 0.05527, 0.08944},
		 {0.648, 0.08295, 0.1279},
		 {0.648, 0.1660, 0.2158},
		 {0.648, 0.4952, 0.7103},
		},
		{{0.6357, 0.0827, 0.2021},
		 {0.6357, 0.0906, 0.1773},
		 {0.3542, 0.1067, 0.1957},
		 {0.3542, 0.1561, 0.2645},
		 {0.3542, 0.3156, 0.4514},
		 {0.3542, 0.8242, 1.0712},
		},
		{{1.001, 0.038, 0.008345},
		 {1.001, 0.04157, 0.0617},
		 {0.5636, 0.0562, 0.08417},
		 {0.5636, 0.0870, 0.1426},
		 {0.5636, 0.1676, 0.2198},
		 {0.5636, 0.4953, 0.7272},
		},
		{{0.5252, 0.0739, 0.1714},
		 {0.5252, 0.07838, 0.1307},
		 {0.3217, 0.1066, 0.2026},
		 {0.3217, 0.1435, 0.2118},
		 {0.3217, 0.2874, 0.4055},
		 {0.3217, 0.7625, 1.075},
		},
	};


/*
const double DirectionEpLUT[Parity][EtaPartitions][2]={
    	       {{2.907, 5.906},
		{2.600, 5.191},
		{4.530, 9.422},
		{5.788, 9.743},
		{8.367, 10.22},
		{11.02, 14.84},
	       },
	       {{2.409, 5.198},
		{2.467, 4.397},
		{4.779, 9.954},
		{6.273, 11.91},
		{9.315, 12.21},
		{10.34, 11.02},
	       },
	       {{2.301, 4.929},
		{2.230, 3.111},
		{7.677, 16.82},
		{7.726, 13.36},
		{9.621, 10.62},
		{11.23, 13.44},
	       },
	       {{2.401, 4.758},
		{2.383, 3.782},
		{7.720, 16.91},
		{8.643, 16.45},
		{10.02, 11.83},
		{11.83, 17.66},
	       },
	};
*/
//updated the LUT, by Tao, 20160724
const double DirectionEpLUT[Parity][EtaPartitions][2]={
    	       {{2.907, 5.906},
		{2.600, 5.191},
		{4.405, 8.809},
		{7.332, 14.478},
		{8.747, 12.265},
		//{11.02, 14.84},
	       },
	       {{2.409, 5.198},
		{2.467, 4.397},
		{4.411, 8.621},
		{7.333, 14.552},
		{8.955, 13.093},
		//{10.34, 11.02},
	       },
	       {{2.301, 4.929},
		{2.230, 3.111},
		{5.288, 10.987},
		{8.242, 16.698},
		{9.213, 13.111},
		//{11.23, 13.44},
	       },
	       {{2.401, 4.758},
		{2.383, 3.782},
		{5.222, 10.769},
		{8.292, 16.955},
		{9.074, 12.083},
		//{11.83, 17.66},
	       },
	};

// phi_momentum = phi_position+gem-csc_bending*slope
const double BendingAngleLUT[2][2] = {
	{28.71,12.86},//even,odd for ME11
	{39.11,18.31},

};

int GetEtaPartition(float eta );

float Ptassign_Position(float deltay12, float deltay23, float eta, int par);

float Ptassign_Position_gp(GlobalPoint gp1, GlobalPoint gp2, GlobalPoint gp3, float eta, int par);

float deltaYcalculation(GlobalPoint gp1, GlobalPoint gp2);

float Ptassign_Direction(float bending_12, float eta, int par);

float PhiMomentum(float dphi, float phi_position, int st, bool evenodd);
float PhiMomentum_Radius(float dphi, float phi_position, float radius_csc, float radius_gem);
float PhiMomentum_Xfactor(float dphi, float phi_position, float x);
float PhiMomentum_Xfactor_V2(float phi_CSC, float phi_GEM, float x);


void calculateAlphaBeta(const std::vector<float>& v, 
                        const std::vector<float>& w, 
                        const std::vector<float>& ev, 
                        const std::vector<float>& ew, 
                        const std::vector<float>& status,
                        float& alpha, float& beta);

float normalizePhi(float phi);

#endif
