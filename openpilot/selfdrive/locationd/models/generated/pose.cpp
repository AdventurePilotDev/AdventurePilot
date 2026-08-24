#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_5594348787172829747) {
   out_5594348787172829747[0] = delta_x[0] + nom_x[0];
   out_5594348787172829747[1] = delta_x[1] + nom_x[1];
   out_5594348787172829747[2] = delta_x[2] + nom_x[2];
   out_5594348787172829747[3] = delta_x[3] + nom_x[3];
   out_5594348787172829747[4] = delta_x[4] + nom_x[4];
   out_5594348787172829747[5] = delta_x[5] + nom_x[5];
   out_5594348787172829747[6] = delta_x[6] + nom_x[6];
   out_5594348787172829747[7] = delta_x[7] + nom_x[7];
   out_5594348787172829747[8] = delta_x[8] + nom_x[8];
   out_5594348787172829747[9] = delta_x[9] + nom_x[9];
   out_5594348787172829747[10] = delta_x[10] + nom_x[10];
   out_5594348787172829747[11] = delta_x[11] + nom_x[11];
   out_5594348787172829747[12] = delta_x[12] + nom_x[12];
   out_5594348787172829747[13] = delta_x[13] + nom_x[13];
   out_5594348787172829747[14] = delta_x[14] + nom_x[14];
   out_5594348787172829747[15] = delta_x[15] + nom_x[15];
   out_5594348787172829747[16] = delta_x[16] + nom_x[16];
   out_5594348787172829747[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7955832377025227799) {
   out_7955832377025227799[0] = -nom_x[0] + true_x[0];
   out_7955832377025227799[1] = -nom_x[1] + true_x[1];
   out_7955832377025227799[2] = -nom_x[2] + true_x[2];
   out_7955832377025227799[3] = -nom_x[3] + true_x[3];
   out_7955832377025227799[4] = -nom_x[4] + true_x[4];
   out_7955832377025227799[5] = -nom_x[5] + true_x[5];
   out_7955832377025227799[6] = -nom_x[6] + true_x[6];
   out_7955832377025227799[7] = -nom_x[7] + true_x[7];
   out_7955832377025227799[8] = -nom_x[8] + true_x[8];
   out_7955832377025227799[9] = -nom_x[9] + true_x[9];
   out_7955832377025227799[10] = -nom_x[10] + true_x[10];
   out_7955832377025227799[11] = -nom_x[11] + true_x[11];
   out_7955832377025227799[12] = -nom_x[12] + true_x[12];
   out_7955832377025227799[13] = -nom_x[13] + true_x[13];
   out_7955832377025227799[14] = -nom_x[14] + true_x[14];
   out_7955832377025227799[15] = -nom_x[15] + true_x[15];
   out_7955832377025227799[16] = -nom_x[16] + true_x[16];
   out_7955832377025227799[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_738947463718714402) {
   out_738947463718714402[0] = 1.0;
   out_738947463718714402[1] = 0.0;
   out_738947463718714402[2] = 0.0;
   out_738947463718714402[3] = 0.0;
   out_738947463718714402[4] = 0.0;
   out_738947463718714402[5] = 0.0;
   out_738947463718714402[6] = 0.0;
   out_738947463718714402[7] = 0.0;
   out_738947463718714402[8] = 0.0;
   out_738947463718714402[9] = 0.0;
   out_738947463718714402[10] = 0.0;
   out_738947463718714402[11] = 0.0;
   out_738947463718714402[12] = 0.0;
   out_738947463718714402[13] = 0.0;
   out_738947463718714402[14] = 0.0;
   out_738947463718714402[15] = 0.0;
   out_738947463718714402[16] = 0.0;
   out_738947463718714402[17] = 0.0;
   out_738947463718714402[18] = 0.0;
   out_738947463718714402[19] = 1.0;
   out_738947463718714402[20] = 0.0;
   out_738947463718714402[21] = 0.0;
   out_738947463718714402[22] = 0.0;
   out_738947463718714402[23] = 0.0;
   out_738947463718714402[24] = 0.0;
   out_738947463718714402[25] = 0.0;
   out_738947463718714402[26] = 0.0;
   out_738947463718714402[27] = 0.0;
   out_738947463718714402[28] = 0.0;
   out_738947463718714402[29] = 0.0;
   out_738947463718714402[30] = 0.0;
   out_738947463718714402[31] = 0.0;
   out_738947463718714402[32] = 0.0;
   out_738947463718714402[33] = 0.0;
   out_738947463718714402[34] = 0.0;
   out_738947463718714402[35] = 0.0;
   out_738947463718714402[36] = 0.0;
   out_738947463718714402[37] = 0.0;
   out_738947463718714402[38] = 1.0;
   out_738947463718714402[39] = 0.0;
   out_738947463718714402[40] = 0.0;
   out_738947463718714402[41] = 0.0;
   out_738947463718714402[42] = 0.0;
   out_738947463718714402[43] = 0.0;
   out_738947463718714402[44] = 0.0;
   out_738947463718714402[45] = 0.0;
   out_738947463718714402[46] = 0.0;
   out_738947463718714402[47] = 0.0;
   out_738947463718714402[48] = 0.0;
   out_738947463718714402[49] = 0.0;
   out_738947463718714402[50] = 0.0;
   out_738947463718714402[51] = 0.0;
   out_738947463718714402[52] = 0.0;
   out_738947463718714402[53] = 0.0;
   out_738947463718714402[54] = 0.0;
   out_738947463718714402[55] = 0.0;
   out_738947463718714402[56] = 0.0;
   out_738947463718714402[57] = 1.0;
   out_738947463718714402[58] = 0.0;
   out_738947463718714402[59] = 0.0;
   out_738947463718714402[60] = 0.0;
   out_738947463718714402[61] = 0.0;
   out_738947463718714402[62] = 0.0;
   out_738947463718714402[63] = 0.0;
   out_738947463718714402[64] = 0.0;
   out_738947463718714402[65] = 0.0;
   out_738947463718714402[66] = 0.0;
   out_738947463718714402[67] = 0.0;
   out_738947463718714402[68] = 0.0;
   out_738947463718714402[69] = 0.0;
   out_738947463718714402[70] = 0.0;
   out_738947463718714402[71] = 0.0;
   out_738947463718714402[72] = 0.0;
   out_738947463718714402[73] = 0.0;
   out_738947463718714402[74] = 0.0;
   out_738947463718714402[75] = 0.0;
   out_738947463718714402[76] = 1.0;
   out_738947463718714402[77] = 0.0;
   out_738947463718714402[78] = 0.0;
   out_738947463718714402[79] = 0.0;
   out_738947463718714402[80] = 0.0;
   out_738947463718714402[81] = 0.0;
   out_738947463718714402[82] = 0.0;
   out_738947463718714402[83] = 0.0;
   out_738947463718714402[84] = 0.0;
   out_738947463718714402[85] = 0.0;
   out_738947463718714402[86] = 0.0;
   out_738947463718714402[87] = 0.0;
   out_738947463718714402[88] = 0.0;
   out_738947463718714402[89] = 0.0;
   out_738947463718714402[90] = 0.0;
   out_738947463718714402[91] = 0.0;
   out_738947463718714402[92] = 0.0;
   out_738947463718714402[93] = 0.0;
   out_738947463718714402[94] = 0.0;
   out_738947463718714402[95] = 1.0;
   out_738947463718714402[96] = 0.0;
   out_738947463718714402[97] = 0.0;
   out_738947463718714402[98] = 0.0;
   out_738947463718714402[99] = 0.0;
   out_738947463718714402[100] = 0.0;
   out_738947463718714402[101] = 0.0;
   out_738947463718714402[102] = 0.0;
   out_738947463718714402[103] = 0.0;
   out_738947463718714402[104] = 0.0;
   out_738947463718714402[105] = 0.0;
   out_738947463718714402[106] = 0.0;
   out_738947463718714402[107] = 0.0;
   out_738947463718714402[108] = 0.0;
   out_738947463718714402[109] = 0.0;
   out_738947463718714402[110] = 0.0;
   out_738947463718714402[111] = 0.0;
   out_738947463718714402[112] = 0.0;
   out_738947463718714402[113] = 0.0;
   out_738947463718714402[114] = 1.0;
   out_738947463718714402[115] = 0.0;
   out_738947463718714402[116] = 0.0;
   out_738947463718714402[117] = 0.0;
   out_738947463718714402[118] = 0.0;
   out_738947463718714402[119] = 0.0;
   out_738947463718714402[120] = 0.0;
   out_738947463718714402[121] = 0.0;
   out_738947463718714402[122] = 0.0;
   out_738947463718714402[123] = 0.0;
   out_738947463718714402[124] = 0.0;
   out_738947463718714402[125] = 0.0;
   out_738947463718714402[126] = 0.0;
   out_738947463718714402[127] = 0.0;
   out_738947463718714402[128] = 0.0;
   out_738947463718714402[129] = 0.0;
   out_738947463718714402[130] = 0.0;
   out_738947463718714402[131] = 0.0;
   out_738947463718714402[132] = 0.0;
   out_738947463718714402[133] = 1.0;
   out_738947463718714402[134] = 0.0;
   out_738947463718714402[135] = 0.0;
   out_738947463718714402[136] = 0.0;
   out_738947463718714402[137] = 0.0;
   out_738947463718714402[138] = 0.0;
   out_738947463718714402[139] = 0.0;
   out_738947463718714402[140] = 0.0;
   out_738947463718714402[141] = 0.0;
   out_738947463718714402[142] = 0.0;
   out_738947463718714402[143] = 0.0;
   out_738947463718714402[144] = 0.0;
   out_738947463718714402[145] = 0.0;
   out_738947463718714402[146] = 0.0;
   out_738947463718714402[147] = 0.0;
   out_738947463718714402[148] = 0.0;
   out_738947463718714402[149] = 0.0;
   out_738947463718714402[150] = 0.0;
   out_738947463718714402[151] = 0.0;
   out_738947463718714402[152] = 1.0;
   out_738947463718714402[153] = 0.0;
   out_738947463718714402[154] = 0.0;
   out_738947463718714402[155] = 0.0;
   out_738947463718714402[156] = 0.0;
   out_738947463718714402[157] = 0.0;
   out_738947463718714402[158] = 0.0;
   out_738947463718714402[159] = 0.0;
   out_738947463718714402[160] = 0.0;
   out_738947463718714402[161] = 0.0;
   out_738947463718714402[162] = 0.0;
   out_738947463718714402[163] = 0.0;
   out_738947463718714402[164] = 0.0;
   out_738947463718714402[165] = 0.0;
   out_738947463718714402[166] = 0.0;
   out_738947463718714402[167] = 0.0;
   out_738947463718714402[168] = 0.0;
   out_738947463718714402[169] = 0.0;
   out_738947463718714402[170] = 0.0;
   out_738947463718714402[171] = 1.0;
   out_738947463718714402[172] = 0.0;
   out_738947463718714402[173] = 0.0;
   out_738947463718714402[174] = 0.0;
   out_738947463718714402[175] = 0.0;
   out_738947463718714402[176] = 0.0;
   out_738947463718714402[177] = 0.0;
   out_738947463718714402[178] = 0.0;
   out_738947463718714402[179] = 0.0;
   out_738947463718714402[180] = 0.0;
   out_738947463718714402[181] = 0.0;
   out_738947463718714402[182] = 0.0;
   out_738947463718714402[183] = 0.0;
   out_738947463718714402[184] = 0.0;
   out_738947463718714402[185] = 0.0;
   out_738947463718714402[186] = 0.0;
   out_738947463718714402[187] = 0.0;
   out_738947463718714402[188] = 0.0;
   out_738947463718714402[189] = 0.0;
   out_738947463718714402[190] = 1.0;
   out_738947463718714402[191] = 0.0;
   out_738947463718714402[192] = 0.0;
   out_738947463718714402[193] = 0.0;
   out_738947463718714402[194] = 0.0;
   out_738947463718714402[195] = 0.0;
   out_738947463718714402[196] = 0.0;
   out_738947463718714402[197] = 0.0;
   out_738947463718714402[198] = 0.0;
   out_738947463718714402[199] = 0.0;
   out_738947463718714402[200] = 0.0;
   out_738947463718714402[201] = 0.0;
   out_738947463718714402[202] = 0.0;
   out_738947463718714402[203] = 0.0;
   out_738947463718714402[204] = 0.0;
   out_738947463718714402[205] = 0.0;
   out_738947463718714402[206] = 0.0;
   out_738947463718714402[207] = 0.0;
   out_738947463718714402[208] = 0.0;
   out_738947463718714402[209] = 1.0;
   out_738947463718714402[210] = 0.0;
   out_738947463718714402[211] = 0.0;
   out_738947463718714402[212] = 0.0;
   out_738947463718714402[213] = 0.0;
   out_738947463718714402[214] = 0.0;
   out_738947463718714402[215] = 0.0;
   out_738947463718714402[216] = 0.0;
   out_738947463718714402[217] = 0.0;
   out_738947463718714402[218] = 0.0;
   out_738947463718714402[219] = 0.0;
   out_738947463718714402[220] = 0.0;
   out_738947463718714402[221] = 0.0;
   out_738947463718714402[222] = 0.0;
   out_738947463718714402[223] = 0.0;
   out_738947463718714402[224] = 0.0;
   out_738947463718714402[225] = 0.0;
   out_738947463718714402[226] = 0.0;
   out_738947463718714402[227] = 0.0;
   out_738947463718714402[228] = 1.0;
   out_738947463718714402[229] = 0.0;
   out_738947463718714402[230] = 0.0;
   out_738947463718714402[231] = 0.0;
   out_738947463718714402[232] = 0.0;
   out_738947463718714402[233] = 0.0;
   out_738947463718714402[234] = 0.0;
   out_738947463718714402[235] = 0.0;
   out_738947463718714402[236] = 0.0;
   out_738947463718714402[237] = 0.0;
   out_738947463718714402[238] = 0.0;
   out_738947463718714402[239] = 0.0;
   out_738947463718714402[240] = 0.0;
   out_738947463718714402[241] = 0.0;
   out_738947463718714402[242] = 0.0;
   out_738947463718714402[243] = 0.0;
   out_738947463718714402[244] = 0.0;
   out_738947463718714402[245] = 0.0;
   out_738947463718714402[246] = 0.0;
   out_738947463718714402[247] = 1.0;
   out_738947463718714402[248] = 0.0;
   out_738947463718714402[249] = 0.0;
   out_738947463718714402[250] = 0.0;
   out_738947463718714402[251] = 0.0;
   out_738947463718714402[252] = 0.0;
   out_738947463718714402[253] = 0.0;
   out_738947463718714402[254] = 0.0;
   out_738947463718714402[255] = 0.0;
   out_738947463718714402[256] = 0.0;
   out_738947463718714402[257] = 0.0;
   out_738947463718714402[258] = 0.0;
   out_738947463718714402[259] = 0.0;
   out_738947463718714402[260] = 0.0;
   out_738947463718714402[261] = 0.0;
   out_738947463718714402[262] = 0.0;
   out_738947463718714402[263] = 0.0;
   out_738947463718714402[264] = 0.0;
   out_738947463718714402[265] = 0.0;
   out_738947463718714402[266] = 1.0;
   out_738947463718714402[267] = 0.0;
   out_738947463718714402[268] = 0.0;
   out_738947463718714402[269] = 0.0;
   out_738947463718714402[270] = 0.0;
   out_738947463718714402[271] = 0.0;
   out_738947463718714402[272] = 0.0;
   out_738947463718714402[273] = 0.0;
   out_738947463718714402[274] = 0.0;
   out_738947463718714402[275] = 0.0;
   out_738947463718714402[276] = 0.0;
   out_738947463718714402[277] = 0.0;
   out_738947463718714402[278] = 0.0;
   out_738947463718714402[279] = 0.0;
   out_738947463718714402[280] = 0.0;
   out_738947463718714402[281] = 0.0;
   out_738947463718714402[282] = 0.0;
   out_738947463718714402[283] = 0.0;
   out_738947463718714402[284] = 0.0;
   out_738947463718714402[285] = 1.0;
   out_738947463718714402[286] = 0.0;
   out_738947463718714402[287] = 0.0;
   out_738947463718714402[288] = 0.0;
   out_738947463718714402[289] = 0.0;
   out_738947463718714402[290] = 0.0;
   out_738947463718714402[291] = 0.0;
   out_738947463718714402[292] = 0.0;
   out_738947463718714402[293] = 0.0;
   out_738947463718714402[294] = 0.0;
   out_738947463718714402[295] = 0.0;
   out_738947463718714402[296] = 0.0;
   out_738947463718714402[297] = 0.0;
   out_738947463718714402[298] = 0.0;
   out_738947463718714402[299] = 0.0;
   out_738947463718714402[300] = 0.0;
   out_738947463718714402[301] = 0.0;
   out_738947463718714402[302] = 0.0;
   out_738947463718714402[303] = 0.0;
   out_738947463718714402[304] = 1.0;
   out_738947463718714402[305] = 0.0;
   out_738947463718714402[306] = 0.0;
   out_738947463718714402[307] = 0.0;
   out_738947463718714402[308] = 0.0;
   out_738947463718714402[309] = 0.0;
   out_738947463718714402[310] = 0.0;
   out_738947463718714402[311] = 0.0;
   out_738947463718714402[312] = 0.0;
   out_738947463718714402[313] = 0.0;
   out_738947463718714402[314] = 0.0;
   out_738947463718714402[315] = 0.0;
   out_738947463718714402[316] = 0.0;
   out_738947463718714402[317] = 0.0;
   out_738947463718714402[318] = 0.0;
   out_738947463718714402[319] = 0.0;
   out_738947463718714402[320] = 0.0;
   out_738947463718714402[321] = 0.0;
   out_738947463718714402[322] = 0.0;
   out_738947463718714402[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_7345501600501038484) {
   out_7345501600501038484[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_7345501600501038484[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_7345501600501038484[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_7345501600501038484[3] = dt*state[12] + state[3];
   out_7345501600501038484[4] = dt*state[13] + state[4];
   out_7345501600501038484[5] = dt*state[14] + state[5];
   out_7345501600501038484[6] = state[6];
   out_7345501600501038484[7] = state[7];
   out_7345501600501038484[8] = state[8];
   out_7345501600501038484[9] = state[9];
   out_7345501600501038484[10] = state[10];
   out_7345501600501038484[11] = state[11];
   out_7345501600501038484[12] = state[12];
   out_7345501600501038484[13] = state[13];
   out_7345501600501038484[14] = state[14];
   out_7345501600501038484[15] = state[15];
   out_7345501600501038484[16] = state[16];
   out_7345501600501038484[17] = state[17];
}
void F_fun(double *state, double dt, double *out_6198689461587572803) {
   out_6198689461587572803[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6198689461587572803[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6198689461587572803[2] = 0;
   out_6198689461587572803[3] = 0;
   out_6198689461587572803[4] = 0;
   out_6198689461587572803[5] = 0;
   out_6198689461587572803[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6198689461587572803[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6198689461587572803[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6198689461587572803[9] = 0;
   out_6198689461587572803[10] = 0;
   out_6198689461587572803[11] = 0;
   out_6198689461587572803[12] = 0;
   out_6198689461587572803[13] = 0;
   out_6198689461587572803[14] = 0;
   out_6198689461587572803[15] = 0;
   out_6198689461587572803[16] = 0;
   out_6198689461587572803[17] = 0;
   out_6198689461587572803[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6198689461587572803[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6198689461587572803[20] = 0;
   out_6198689461587572803[21] = 0;
   out_6198689461587572803[22] = 0;
   out_6198689461587572803[23] = 0;
   out_6198689461587572803[24] = 0;
   out_6198689461587572803[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6198689461587572803[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6198689461587572803[27] = 0;
   out_6198689461587572803[28] = 0;
   out_6198689461587572803[29] = 0;
   out_6198689461587572803[30] = 0;
   out_6198689461587572803[31] = 0;
   out_6198689461587572803[32] = 0;
   out_6198689461587572803[33] = 0;
   out_6198689461587572803[34] = 0;
   out_6198689461587572803[35] = 0;
   out_6198689461587572803[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6198689461587572803[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6198689461587572803[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6198689461587572803[39] = 0;
   out_6198689461587572803[40] = 0;
   out_6198689461587572803[41] = 0;
   out_6198689461587572803[42] = 0;
   out_6198689461587572803[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6198689461587572803[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6198689461587572803[45] = 0;
   out_6198689461587572803[46] = 0;
   out_6198689461587572803[47] = 0;
   out_6198689461587572803[48] = 0;
   out_6198689461587572803[49] = 0;
   out_6198689461587572803[50] = 0;
   out_6198689461587572803[51] = 0;
   out_6198689461587572803[52] = 0;
   out_6198689461587572803[53] = 0;
   out_6198689461587572803[54] = 0;
   out_6198689461587572803[55] = 0;
   out_6198689461587572803[56] = 0;
   out_6198689461587572803[57] = 1;
   out_6198689461587572803[58] = 0;
   out_6198689461587572803[59] = 0;
   out_6198689461587572803[60] = 0;
   out_6198689461587572803[61] = 0;
   out_6198689461587572803[62] = 0;
   out_6198689461587572803[63] = 0;
   out_6198689461587572803[64] = 0;
   out_6198689461587572803[65] = 0;
   out_6198689461587572803[66] = dt;
   out_6198689461587572803[67] = 0;
   out_6198689461587572803[68] = 0;
   out_6198689461587572803[69] = 0;
   out_6198689461587572803[70] = 0;
   out_6198689461587572803[71] = 0;
   out_6198689461587572803[72] = 0;
   out_6198689461587572803[73] = 0;
   out_6198689461587572803[74] = 0;
   out_6198689461587572803[75] = 0;
   out_6198689461587572803[76] = 1;
   out_6198689461587572803[77] = 0;
   out_6198689461587572803[78] = 0;
   out_6198689461587572803[79] = 0;
   out_6198689461587572803[80] = 0;
   out_6198689461587572803[81] = 0;
   out_6198689461587572803[82] = 0;
   out_6198689461587572803[83] = 0;
   out_6198689461587572803[84] = 0;
   out_6198689461587572803[85] = dt;
   out_6198689461587572803[86] = 0;
   out_6198689461587572803[87] = 0;
   out_6198689461587572803[88] = 0;
   out_6198689461587572803[89] = 0;
   out_6198689461587572803[90] = 0;
   out_6198689461587572803[91] = 0;
   out_6198689461587572803[92] = 0;
   out_6198689461587572803[93] = 0;
   out_6198689461587572803[94] = 0;
   out_6198689461587572803[95] = 1;
   out_6198689461587572803[96] = 0;
   out_6198689461587572803[97] = 0;
   out_6198689461587572803[98] = 0;
   out_6198689461587572803[99] = 0;
   out_6198689461587572803[100] = 0;
   out_6198689461587572803[101] = 0;
   out_6198689461587572803[102] = 0;
   out_6198689461587572803[103] = 0;
   out_6198689461587572803[104] = dt;
   out_6198689461587572803[105] = 0;
   out_6198689461587572803[106] = 0;
   out_6198689461587572803[107] = 0;
   out_6198689461587572803[108] = 0;
   out_6198689461587572803[109] = 0;
   out_6198689461587572803[110] = 0;
   out_6198689461587572803[111] = 0;
   out_6198689461587572803[112] = 0;
   out_6198689461587572803[113] = 0;
   out_6198689461587572803[114] = 1;
   out_6198689461587572803[115] = 0;
   out_6198689461587572803[116] = 0;
   out_6198689461587572803[117] = 0;
   out_6198689461587572803[118] = 0;
   out_6198689461587572803[119] = 0;
   out_6198689461587572803[120] = 0;
   out_6198689461587572803[121] = 0;
   out_6198689461587572803[122] = 0;
   out_6198689461587572803[123] = 0;
   out_6198689461587572803[124] = 0;
   out_6198689461587572803[125] = 0;
   out_6198689461587572803[126] = 0;
   out_6198689461587572803[127] = 0;
   out_6198689461587572803[128] = 0;
   out_6198689461587572803[129] = 0;
   out_6198689461587572803[130] = 0;
   out_6198689461587572803[131] = 0;
   out_6198689461587572803[132] = 0;
   out_6198689461587572803[133] = 1;
   out_6198689461587572803[134] = 0;
   out_6198689461587572803[135] = 0;
   out_6198689461587572803[136] = 0;
   out_6198689461587572803[137] = 0;
   out_6198689461587572803[138] = 0;
   out_6198689461587572803[139] = 0;
   out_6198689461587572803[140] = 0;
   out_6198689461587572803[141] = 0;
   out_6198689461587572803[142] = 0;
   out_6198689461587572803[143] = 0;
   out_6198689461587572803[144] = 0;
   out_6198689461587572803[145] = 0;
   out_6198689461587572803[146] = 0;
   out_6198689461587572803[147] = 0;
   out_6198689461587572803[148] = 0;
   out_6198689461587572803[149] = 0;
   out_6198689461587572803[150] = 0;
   out_6198689461587572803[151] = 0;
   out_6198689461587572803[152] = 1;
   out_6198689461587572803[153] = 0;
   out_6198689461587572803[154] = 0;
   out_6198689461587572803[155] = 0;
   out_6198689461587572803[156] = 0;
   out_6198689461587572803[157] = 0;
   out_6198689461587572803[158] = 0;
   out_6198689461587572803[159] = 0;
   out_6198689461587572803[160] = 0;
   out_6198689461587572803[161] = 0;
   out_6198689461587572803[162] = 0;
   out_6198689461587572803[163] = 0;
   out_6198689461587572803[164] = 0;
   out_6198689461587572803[165] = 0;
   out_6198689461587572803[166] = 0;
   out_6198689461587572803[167] = 0;
   out_6198689461587572803[168] = 0;
   out_6198689461587572803[169] = 0;
   out_6198689461587572803[170] = 0;
   out_6198689461587572803[171] = 1;
   out_6198689461587572803[172] = 0;
   out_6198689461587572803[173] = 0;
   out_6198689461587572803[174] = 0;
   out_6198689461587572803[175] = 0;
   out_6198689461587572803[176] = 0;
   out_6198689461587572803[177] = 0;
   out_6198689461587572803[178] = 0;
   out_6198689461587572803[179] = 0;
   out_6198689461587572803[180] = 0;
   out_6198689461587572803[181] = 0;
   out_6198689461587572803[182] = 0;
   out_6198689461587572803[183] = 0;
   out_6198689461587572803[184] = 0;
   out_6198689461587572803[185] = 0;
   out_6198689461587572803[186] = 0;
   out_6198689461587572803[187] = 0;
   out_6198689461587572803[188] = 0;
   out_6198689461587572803[189] = 0;
   out_6198689461587572803[190] = 1;
   out_6198689461587572803[191] = 0;
   out_6198689461587572803[192] = 0;
   out_6198689461587572803[193] = 0;
   out_6198689461587572803[194] = 0;
   out_6198689461587572803[195] = 0;
   out_6198689461587572803[196] = 0;
   out_6198689461587572803[197] = 0;
   out_6198689461587572803[198] = 0;
   out_6198689461587572803[199] = 0;
   out_6198689461587572803[200] = 0;
   out_6198689461587572803[201] = 0;
   out_6198689461587572803[202] = 0;
   out_6198689461587572803[203] = 0;
   out_6198689461587572803[204] = 0;
   out_6198689461587572803[205] = 0;
   out_6198689461587572803[206] = 0;
   out_6198689461587572803[207] = 0;
   out_6198689461587572803[208] = 0;
   out_6198689461587572803[209] = 1;
   out_6198689461587572803[210] = 0;
   out_6198689461587572803[211] = 0;
   out_6198689461587572803[212] = 0;
   out_6198689461587572803[213] = 0;
   out_6198689461587572803[214] = 0;
   out_6198689461587572803[215] = 0;
   out_6198689461587572803[216] = 0;
   out_6198689461587572803[217] = 0;
   out_6198689461587572803[218] = 0;
   out_6198689461587572803[219] = 0;
   out_6198689461587572803[220] = 0;
   out_6198689461587572803[221] = 0;
   out_6198689461587572803[222] = 0;
   out_6198689461587572803[223] = 0;
   out_6198689461587572803[224] = 0;
   out_6198689461587572803[225] = 0;
   out_6198689461587572803[226] = 0;
   out_6198689461587572803[227] = 0;
   out_6198689461587572803[228] = 1;
   out_6198689461587572803[229] = 0;
   out_6198689461587572803[230] = 0;
   out_6198689461587572803[231] = 0;
   out_6198689461587572803[232] = 0;
   out_6198689461587572803[233] = 0;
   out_6198689461587572803[234] = 0;
   out_6198689461587572803[235] = 0;
   out_6198689461587572803[236] = 0;
   out_6198689461587572803[237] = 0;
   out_6198689461587572803[238] = 0;
   out_6198689461587572803[239] = 0;
   out_6198689461587572803[240] = 0;
   out_6198689461587572803[241] = 0;
   out_6198689461587572803[242] = 0;
   out_6198689461587572803[243] = 0;
   out_6198689461587572803[244] = 0;
   out_6198689461587572803[245] = 0;
   out_6198689461587572803[246] = 0;
   out_6198689461587572803[247] = 1;
   out_6198689461587572803[248] = 0;
   out_6198689461587572803[249] = 0;
   out_6198689461587572803[250] = 0;
   out_6198689461587572803[251] = 0;
   out_6198689461587572803[252] = 0;
   out_6198689461587572803[253] = 0;
   out_6198689461587572803[254] = 0;
   out_6198689461587572803[255] = 0;
   out_6198689461587572803[256] = 0;
   out_6198689461587572803[257] = 0;
   out_6198689461587572803[258] = 0;
   out_6198689461587572803[259] = 0;
   out_6198689461587572803[260] = 0;
   out_6198689461587572803[261] = 0;
   out_6198689461587572803[262] = 0;
   out_6198689461587572803[263] = 0;
   out_6198689461587572803[264] = 0;
   out_6198689461587572803[265] = 0;
   out_6198689461587572803[266] = 1;
   out_6198689461587572803[267] = 0;
   out_6198689461587572803[268] = 0;
   out_6198689461587572803[269] = 0;
   out_6198689461587572803[270] = 0;
   out_6198689461587572803[271] = 0;
   out_6198689461587572803[272] = 0;
   out_6198689461587572803[273] = 0;
   out_6198689461587572803[274] = 0;
   out_6198689461587572803[275] = 0;
   out_6198689461587572803[276] = 0;
   out_6198689461587572803[277] = 0;
   out_6198689461587572803[278] = 0;
   out_6198689461587572803[279] = 0;
   out_6198689461587572803[280] = 0;
   out_6198689461587572803[281] = 0;
   out_6198689461587572803[282] = 0;
   out_6198689461587572803[283] = 0;
   out_6198689461587572803[284] = 0;
   out_6198689461587572803[285] = 1;
   out_6198689461587572803[286] = 0;
   out_6198689461587572803[287] = 0;
   out_6198689461587572803[288] = 0;
   out_6198689461587572803[289] = 0;
   out_6198689461587572803[290] = 0;
   out_6198689461587572803[291] = 0;
   out_6198689461587572803[292] = 0;
   out_6198689461587572803[293] = 0;
   out_6198689461587572803[294] = 0;
   out_6198689461587572803[295] = 0;
   out_6198689461587572803[296] = 0;
   out_6198689461587572803[297] = 0;
   out_6198689461587572803[298] = 0;
   out_6198689461587572803[299] = 0;
   out_6198689461587572803[300] = 0;
   out_6198689461587572803[301] = 0;
   out_6198689461587572803[302] = 0;
   out_6198689461587572803[303] = 0;
   out_6198689461587572803[304] = 1;
   out_6198689461587572803[305] = 0;
   out_6198689461587572803[306] = 0;
   out_6198689461587572803[307] = 0;
   out_6198689461587572803[308] = 0;
   out_6198689461587572803[309] = 0;
   out_6198689461587572803[310] = 0;
   out_6198689461587572803[311] = 0;
   out_6198689461587572803[312] = 0;
   out_6198689461587572803[313] = 0;
   out_6198689461587572803[314] = 0;
   out_6198689461587572803[315] = 0;
   out_6198689461587572803[316] = 0;
   out_6198689461587572803[317] = 0;
   out_6198689461587572803[318] = 0;
   out_6198689461587572803[319] = 0;
   out_6198689461587572803[320] = 0;
   out_6198689461587572803[321] = 0;
   out_6198689461587572803[322] = 0;
   out_6198689461587572803[323] = 1;
}
void h_4(double *state, double *unused, double *out_4572986314476450084) {
   out_4572986314476450084[0] = state[6] + state[9];
   out_4572986314476450084[1] = state[7] + state[10];
   out_4572986314476450084[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_1791217832987638276) {
   out_1791217832987638276[0] = 0;
   out_1791217832987638276[1] = 0;
   out_1791217832987638276[2] = 0;
   out_1791217832987638276[3] = 0;
   out_1791217832987638276[4] = 0;
   out_1791217832987638276[5] = 0;
   out_1791217832987638276[6] = 1;
   out_1791217832987638276[7] = 0;
   out_1791217832987638276[8] = 0;
   out_1791217832987638276[9] = 1;
   out_1791217832987638276[10] = 0;
   out_1791217832987638276[11] = 0;
   out_1791217832987638276[12] = 0;
   out_1791217832987638276[13] = 0;
   out_1791217832987638276[14] = 0;
   out_1791217832987638276[15] = 0;
   out_1791217832987638276[16] = 0;
   out_1791217832987638276[17] = 0;
   out_1791217832987638276[18] = 0;
   out_1791217832987638276[19] = 0;
   out_1791217832987638276[20] = 0;
   out_1791217832987638276[21] = 0;
   out_1791217832987638276[22] = 0;
   out_1791217832987638276[23] = 0;
   out_1791217832987638276[24] = 0;
   out_1791217832987638276[25] = 1;
   out_1791217832987638276[26] = 0;
   out_1791217832987638276[27] = 0;
   out_1791217832987638276[28] = 1;
   out_1791217832987638276[29] = 0;
   out_1791217832987638276[30] = 0;
   out_1791217832987638276[31] = 0;
   out_1791217832987638276[32] = 0;
   out_1791217832987638276[33] = 0;
   out_1791217832987638276[34] = 0;
   out_1791217832987638276[35] = 0;
   out_1791217832987638276[36] = 0;
   out_1791217832987638276[37] = 0;
   out_1791217832987638276[38] = 0;
   out_1791217832987638276[39] = 0;
   out_1791217832987638276[40] = 0;
   out_1791217832987638276[41] = 0;
   out_1791217832987638276[42] = 0;
   out_1791217832987638276[43] = 0;
   out_1791217832987638276[44] = 1;
   out_1791217832987638276[45] = 0;
   out_1791217832987638276[46] = 0;
   out_1791217832987638276[47] = 1;
   out_1791217832987638276[48] = 0;
   out_1791217832987638276[49] = 0;
   out_1791217832987638276[50] = 0;
   out_1791217832987638276[51] = 0;
   out_1791217832987638276[52] = 0;
   out_1791217832987638276[53] = 0;
}
void h_10(double *state, double *unused, double *out_6841094257975008708) {
   out_6841094257975008708[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_6841094257975008708[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_6841094257975008708[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5018362184474345094) {
   out_5018362184474345094[0] = 0;
   out_5018362184474345094[1] = 9.8100000000000005*cos(state[1]);
   out_5018362184474345094[2] = 0;
   out_5018362184474345094[3] = 0;
   out_5018362184474345094[4] = -state[8];
   out_5018362184474345094[5] = state[7];
   out_5018362184474345094[6] = 0;
   out_5018362184474345094[7] = state[5];
   out_5018362184474345094[8] = -state[4];
   out_5018362184474345094[9] = 0;
   out_5018362184474345094[10] = 0;
   out_5018362184474345094[11] = 0;
   out_5018362184474345094[12] = 1;
   out_5018362184474345094[13] = 0;
   out_5018362184474345094[14] = 0;
   out_5018362184474345094[15] = 1;
   out_5018362184474345094[16] = 0;
   out_5018362184474345094[17] = 0;
   out_5018362184474345094[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5018362184474345094[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5018362184474345094[20] = 0;
   out_5018362184474345094[21] = state[8];
   out_5018362184474345094[22] = 0;
   out_5018362184474345094[23] = -state[6];
   out_5018362184474345094[24] = -state[5];
   out_5018362184474345094[25] = 0;
   out_5018362184474345094[26] = state[3];
   out_5018362184474345094[27] = 0;
   out_5018362184474345094[28] = 0;
   out_5018362184474345094[29] = 0;
   out_5018362184474345094[30] = 0;
   out_5018362184474345094[31] = 1;
   out_5018362184474345094[32] = 0;
   out_5018362184474345094[33] = 0;
   out_5018362184474345094[34] = 1;
   out_5018362184474345094[35] = 0;
   out_5018362184474345094[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5018362184474345094[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5018362184474345094[38] = 0;
   out_5018362184474345094[39] = -state[7];
   out_5018362184474345094[40] = state[6];
   out_5018362184474345094[41] = 0;
   out_5018362184474345094[42] = state[4];
   out_5018362184474345094[43] = -state[3];
   out_5018362184474345094[44] = 0;
   out_5018362184474345094[45] = 0;
   out_5018362184474345094[46] = 0;
   out_5018362184474345094[47] = 0;
   out_5018362184474345094[48] = 0;
   out_5018362184474345094[49] = 0;
   out_5018362184474345094[50] = 1;
   out_5018362184474345094[51] = 0;
   out_5018362184474345094[52] = 0;
   out_5018362184474345094[53] = 1;
}
void h_13(double *state, double *unused, double *out_5707469078074511089) {
   out_5707469078074511089[0] = state[3];
   out_5707469078074511089[1] = state[4];
   out_5707469078074511089[2] = state[5];
}
void H_13(double *state, double *unused, double *out_9044895032405212411) {
   out_9044895032405212411[0] = 0;
   out_9044895032405212411[1] = 0;
   out_9044895032405212411[2] = 0;
   out_9044895032405212411[3] = 1;
   out_9044895032405212411[4] = 0;
   out_9044895032405212411[5] = 0;
   out_9044895032405212411[6] = 0;
   out_9044895032405212411[7] = 0;
   out_9044895032405212411[8] = 0;
   out_9044895032405212411[9] = 0;
   out_9044895032405212411[10] = 0;
   out_9044895032405212411[11] = 0;
   out_9044895032405212411[12] = 0;
   out_9044895032405212411[13] = 0;
   out_9044895032405212411[14] = 0;
   out_9044895032405212411[15] = 0;
   out_9044895032405212411[16] = 0;
   out_9044895032405212411[17] = 0;
   out_9044895032405212411[18] = 0;
   out_9044895032405212411[19] = 0;
   out_9044895032405212411[20] = 0;
   out_9044895032405212411[21] = 0;
   out_9044895032405212411[22] = 1;
   out_9044895032405212411[23] = 0;
   out_9044895032405212411[24] = 0;
   out_9044895032405212411[25] = 0;
   out_9044895032405212411[26] = 0;
   out_9044895032405212411[27] = 0;
   out_9044895032405212411[28] = 0;
   out_9044895032405212411[29] = 0;
   out_9044895032405212411[30] = 0;
   out_9044895032405212411[31] = 0;
   out_9044895032405212411[32] = 0;
   out_9044895032405212411[33] = 0;
   out_9044895032405212411[34] = 0;
   out_9044895032405212411[35] = 0;
   out_9044895032405212411[36] = 0;
   out_9044895032405212411[37] = 0;
   out_9044895032405212411[38] = 0;
   out_9044895032405212411[39] = 0;
   out_9044895032405212411[40] = 0;
   out_9044895032405212411[41] = 1;
   out_9044895032405212411[42] = 0;
   out_9044895032405212411[43] = 0;
   out_9044895032405212411[44] = 0;
   out_9044895032405212411[45] = 0;
   out_9044895032405212411[46] = 0;
   out_9044895032405212411[47] = 0;
   out_9044895032405212411[48] = 0;
   out_9044895032405212411[49] = 0;
   out_9044895032405212411[50] = 0;
   out_9044895032405212411[51] = 0;
   out_9044895032405212411[52] = 0;
   out_9044895032405212411[53] = 0;
}
void h_14(double *state, double *unused, double *out_7196792041580805526) {
   out_7196792041580805526[0] = state[6];
   out_7196792041580805526[1] = state[7];
   out_7196792041580805526[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5754458689327122805) {
   out_5754458689327122805[0] = 0;
   out_5754458689327122805[1] = 0;
   out_5754458689327122805[2] = 0;
   out_5754458689327122805[3] = 0;
   out_5754458689327122805[4] = 0;
   out_5754458689327122805[5] = 0;
   out_5754458689327122805[6] = 1;
   out_5754458689327122805[7] = 0;
   out_5754458689327122805[8] = 0;
   out_5754458689327122805[9] = 0;
   out_5754458689327122805[10] = 0;
   out_5754458689327122805[11] = 0;
   out_5754458689327122805[12] = 0;
   out_5754458689327122805[13] = 0;
   out_5754458689327122805[14] = 0;
   out_5754458689327122805[15] = 0;
   out_5754458689327122805[16] = 0;
   out_5754458689327122805[17] = 0;
   out_5754458689327122805[18] = 0;
   out_5754458689327122805[19] = 0;
   out_5754458689327122805[20] = 0;
   out_5754458689327122805[21] = 0;
   out_5754458689327122805[22] = 0;
   out_5754458689327122805[23] = 0;
   out_5754458689327122805[24] = 0;
   out_5754458689327122805[25] = 1;
   out_5754458689327122805[26] = 0;
   out_5754458689327122805[27] = 0;
   out_5754458689327122805[28] = 0;
   out_5754458689327122805[29] = 0;
   out_5754458689327122805[30] = 0;
   out_5754458689327122805[31] = 0;
   out_5754458689327122805[32] = 0;
   out_5754458689327122805[33] = 0;
   out_5754458689327122805[34] = 0;
   out_5754458689327122805[35] = 0;
   out_5754458689327122805[36] = 0;
   out_5754458689327122805[37] = 0;
   out_5754458689327122805[38] = 0;
   out_5754458689327122805[39] = 0;
   out_5754458689327122805[40] = 0;
   out_5754458689327122805[41] = 0;
   out_5754458689327122805[42] = 0;
   out_5754458689327122805[43] = 0;
   out_5754458689327122805[44] = 1;
   out_5754458689327122805[45] = 0;
   out_5754458689327122805[46] = 0;
   out_5754458689327122805[47] = 0;
   out_5754458689327122805[48] = 0;
   out_5754458689327122805[49] = 0;
   out_5754458689327122805[50] = 0;
   out_5754458689327122805[51] = 0;
   out_5754458689327122805[52] = 0;
   out_5754458689327122805[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_5594348787172829747) {
  err_fun(nom_x, delta_x, out_5594348787172829747);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7955832377025227799) {
  inv_err_fun(nom_x, true_x, out_7955832377025227799);
}
void pose_H_mod_fun(double *state, double *out_738947463718714402) {
  H_mod_fun(state, out_738947463718714402);
}
void pose_f_fun(double *state, double dt, double *out_7345501600501038484) {
  f_fun(state,  dt, out_7345501600501038484);
}
void pose_F_fun(double *state, double dt, double *out_6198689461587572803) {
  F_fun(state,  dt, out_6198689461587572803);
}
void pose_h_4(double *state, double *unused, double *out_4572986314476450084) {
  h_4(state, unused, out_4572986314476450084);
}
void pose_H_4(double *state, double *unused, double *out_1791217832987638276) {
  H_4(state, unused, out_1791217832987638276);
}
void pose_h_10(double *state, double *unused, double *out_6841094257975008708) {
  h_10(state, unused, out_6841094257975008708);
}
void pose_H_10(double *state, double *unused, double *out_5018362184474345094) {
  H_10(state, unused, out_5018362184474345094);
}
void pose_h_13(double *state, double *unused, double *out_5707469078074511089) {
  h_13(state, unused, out_5707469078074511089);
}
void pose_H_13(double *state, double *unused, double *out_9044895032405212411) {
  H_13(state, unused, out_9044895032405212411);
}
void pose_h_14(double *state, double *unused, double *out_7196792041580805526) {
  h_14(state, unused, out_7196792041580805526);
}
void pose_H_14(double *state, double *unused, double *out_5754458689327122805) {
  H_14(state, unused, out_5754458689327122805);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
