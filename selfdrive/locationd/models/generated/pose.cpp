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
void err_fun(double *nom_x, double *delta_x, double *out_4876828158992611850) {
   out_4876828158992611850[0] = delta_x[0] + nom_x[0];
   out_4876828158992611850[1] = delta_x[1] + nom_x[1];
   out_4876828158992611850[2] = delta_x[2] + nom_x[2];
   out_4876828158992611850[3] = delta_x[3] + nom_x[3];
   out_4876828158992611850[4] = delta_x[4] + nom_x[4];
   out_4876828158992611850[5] = delta_x[5] + nom_x[5];
   out_4876828158992611850[6] = delta_x[6] + nom_x[6];
   out_4876828158992611850[7] = delta_x[7] + nom_x[7];
   out_4876828158992611850[8] = delta_x[8] + nom_x[8];
   out_4876828158992611850[9] = delta_x[9] + nom_x[9];
   out_4876828158992611850[10] = delta_x[10] + nom_x[10];
   out_4876828158992611850[11] = delta_x[11] + nom_x[11];
   out_4876828158992611850[12] = delta_x[12] + nom_x[12];
   out_4876828158992611850[13] = delta_x[13] + nom_x[13];
   out_4876828158992611850[14] = delta_x[14] + nom_x[14];
   out_4876828158992611850[15] = delta_x[15] + nom_x[15];
   out_4876828158992611850[16] = delta_x[16] + nom_x[16];
   out_4876828158992611850[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4991901881103411206) {
   out_4991901881103411206[0] = -nom_x[0] + true_x[0];
   out_4991901881103411206[1] = -nom_x[1] + true_x[1];
   out_4991901881103411206[2] = -nom_x[2] + true_x[2];
   out_4991901881103411206[3] = -nom_x[3] + true_x[3];
   out_4991901881103411206[4] = -nom_x[4] + true_x[4];
   out_4991901881103411206[5] = -nom_x[5] + true_x[5];
   out_4991901881103411206[6] = -nom_x[6] + true_x[6];
   out_4991901881103411206[7] = -nom_x[7] + true_x[7];
   out_4991901881103411206[8] = -nom_x[8] + true_x[8];
   out_4991901881103411206[9] = -nom_x[9] + true_x[9];
   out_4991901881103411206[10] = -nom_x[10] + true_x[10];
   out_4991901881103411206[11] = -nom_x[11] + true_x[11];
   out_4991901881103411206[12] = -nom_x[12] + true_x[12];
   out_4991901881103411206[13] = -nom_x[13] + true_x[13];
   out_4991901881103411206[14] = -nom_x[14] + true_x[14];
   out_4991901881103411206[15] = -nom_x[15] + true_x[15];
   out_4991901881103411206[16] = -nom_x[16] + true_x[16];
   out_4991901881103411206[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8109048779199487053) {
   out_8109048779199487053[0] = 1.0;
   out_8109048779199487053[1] = 0.0;
   out_8109048779199487053[2] = 0.0;
   out_8109048779199487053[3] = 0.0;
   out_8109048779199487053[4] = 0.0;
   out_8109048779199487053[5] = 0.0;
   out_8109048779199487053[6] = 0.0;
   out_8109048779199487053[7] = 0.0;
   out_8109048779199487053[8] = 0.0;
   out_8109048779199487053[9] = 0.0;
   out_8109048779199487053[10] = 0.0;
   out_8109048779199487053[11] = 0.0;
   out_8109048779199487053[12] = 0.0;
   out_8109048779199487053[13] = 0.0;
   out_8109048779199487053[14] = 0.0;
   out_8109048779199487053[15] = 0.0;
   out_8109048779199487053[16] = 0.0;
   out_8109048779199487053[17] = 0.0;
   out_8109048779199487053[18] = 0.0;
   out_8109048779199487053[19] = 1.0;
   out_8109048779199487053[20] = 0.0;
   out_8109048779199487053[21] = 0.0;
   out_8109048779199487053[22] = 0.0;
   out_8109048779199487053[23] = 0.0;
   out_8109048779199487053[24] = 0.0;
   out_8109048779199487053[25] = 0.0;
   out_8109048779199487053[26] = 0.0;
   out_8109048779199487053[27] = 0.0;
   out_8109048779199487053[28] = 0.0;
   out_8109048779199487053[29] = 0.0;
   out_8109048779199487053[30] = 0.0;
   out_8109048779199487053[31] = 0.0;
   out_8109048779199487053[32] = 0.0;
   out_8109048779199487053[33] = 0.0;
   out_8109048779199487053[34] = 0.0;
   out_8109048779199487053[35] = 0.0;
   out_8109048779199487053[36] = 0.0;
   out_8109048779199487053[37] = 0.0;
   out_8109048779199487053[38] = 1.0;
   out_8109048779199487053[39] = 0.0;
   out_8109048779199487053[40] = 0.0;
   out_8109048779199487053[41] = 0.0;
   out_8109048779199487053[42] = 0.0;
   out_8109048779199487053[43] = 0.0;
   out_8109048779199487053[44] = 0.0;
   out_8109048779199487053[45] = 0.0;
   out_8109048779199487053[46] = 0.0;
   out_8109048779199487053[47] = 0.0;
   out_8109048779199487053[48] = 0.0;
   out_8109048779199487053[49] = 0.0;
   out_8109048779199487053[50] = 0.0;
   out_8109048779199487053[51] = 0.0;
   out_8109048779199487053[52] = 0.0;
   out_8109048779199487053[53] = 0.0;
   out_8109048779199487053[54] = 0.0;
   out_8109048779199487053[55] = 0.0;
   out_8109048779199487053[56] = 0.0;
   out_8109048779199487053[57] = 1.0;
   out_8109048779199487053[58] = 0.0;
   out_8109048779199487053[59] = 0.0;
   out_8109048779199487053[60] = 0.0;
   out_8109048779199487053[61] = 0.0;
   out_8109048779199487053[62] = 0.0;
   out_8109048779199487053[63] = 0.0;
   out_8109048779199487053[64] = 0.0;
   out_8109048779199487053[65] = 0.0;
   out_8109048779199487053[66] = 0.0;
   out_8109048779199487053[67] = 0.0;
   out_8109048779199487053[68] = 0.0;
   out_8109048779199487053[69] = 0.0;
   out_8109048779199487053[70] = 0.0;
   out_8109048779199487053[71] = 0.0;
   out_8109048779199487053[72] = 0.0;
   out_8109048779199487053[73] = 0.0;
   out_8109048779199487053[74] = 0.0;
   out_8109048779199487053[75] = 0.0;
   out_8109048779199487053[76] = 1.0;
   out_8109048779199487053[77] = 0.0;
   out_8109048779199487053[78] = 0.0;
   out_8109048779199487053[79] = 0.0;
   out_8109048779199487053[80] = 0.0;
   out_8109048779199487053[81] = 0.0;
   out_8109048779199487053[82] = 0.0;
   out_8109048779199487053[83] = 0.0;
   out_8109048779199487053[84] = 0.0;
   out_8109048779199487053[85] = 0.0;
   out_8109048779199487053[86] = 0.0;
   out_8109048779199487053[87] = 0.0;
   out_8109048779199487053[88] = 0.0;
   out_8109048779199487053[89] = 0.0;
   out_8109048779199487053[90] = 0.0;
   out_8109048779199487053[91] = 0.0;
   out_8109048779199487053[92] = 0.0;
   out_8109048779199487053[93] = 0.0;
   out_8109048779199487053[94] = 0.0;
   out_8109048779199487053[95] = 1.0;
   out_8109048779199487053[96] = 0.0;
   out_8109048779199487053[97] = 0.0;
   out_8109048779199487053[98] = 0.0;
   out_8109048779199487053[99] = 0.0;
   out_8109048779199487053[100] = 0.0;
   out_8109048779199487053[101] = 0.0;
   out_8109048779199487053[102] = 0.0;
   out_8109048779199487053[103] = 0.0;
   out_8109048779199487053[104] = 0.0;
   out_8109048779199487053[105] = 0.0;
   out_8109048779199487053[106] = 0.0;
   out_8109048779199487053[107] = 0.0;
   out_8109048779199487053[108] = 0.0;
   out_8109048779199487053[109] = 0.0;
   out_8109048779199487053[110] = 0.0;
   out_8109048779199487053[111] = 0.0;
   out_8109048779199487053[112] = 0.0;
   out_8109048779199487053[113] = 0.0;
   out_8109048779199487053[114] = 1.0;
   out_8109048779199487053[115] = 0.0;
   out_8109048779199487053[116] = 0.0;
   out_8109048779199487053[117] = 0.0;
   out_8109048779199487053[118] = 0.0;
   out_8109048779199487053[119] = 0.0;
   out_8109048779199487053[120] = 0.0;
   out_8109048779199487053[121] = 0.0;
   out_8109048779199487053[122] = 0.0;
   out_8109048779199487053[123] = 0.0;
   out_8109048779199487053[124] = 0.0;
   out_8109048779199487053[125] = 0.0;
   out_8109048779199487053[126] = 0.0;
   out_8109048779199487053[127] = 0.0;
   out_8109048779199487053[128] = 0.0;
   out_8109048779199487053[129] = 0.0;
   out_8109048779199487053[130] = 0.0;
   out_8109048779199487053[131] = 0.0;
   out_8109048779199487053[132] = 0.0;
   out_8109048779199487053[133] = 1.0;
   out_8109048779199487053[134] = 0.0;
   out_8109048779199487053[135] = 0.0;
   out_8109048779199487053[136] = 0.0;
   out_8109048779199487053[137] = 0.0;
   out_8109048779199487053[138] = 0.0;
   out_8109048779199487053[139] = 0.0;
   out_8109048779199487053[140] = 0.0;
   out_8109048779199487053[141] = 0.0;
   out_8109048779199487053[142] = 0.0;
   out_8109048779199487053[143] = 0.0;
   out_8109048779199487053[144] = 0.0;
   out_8109048779199487053[145] = 0.0;
   out_8109048779199487053[146] = 0.0;
   out_8109048779199487053[147] = 0.0;
   out_8109048779199487053[148] = 0.0;
   out_8109048779199487053[149] = 0.0;
   out_8109048779199487053[150] = 0.0;
   out_8109048779199487053[151] = 0.0;
   out_8109048779199487053[152] = 1.0;
   out_8109048779199487053[153] = 0.0;
   out_8109048779199487053[154] = 0.0;
   out_8109048779199487053[155] = 0.0;
   out_8109048779199487053[156] = 0.0;
   out_8109048779199487053[157] = 0.0;
   out_8109048779199487053[158] = 0.0;
   out_8109048779199487053[159] = 0.0;
   out_8109048779199487053[160] = 0.0;
   out_8109048779199487053[161] = 0.0;
   out_8109048779199487053[162] = 0.0;
   out_8109048779199487053[163] = 0.0;
   out_8109048779199487053[164] = 0.0;
   out_8109048779199487053[165] = 0.0;
   out_8109048779199487053[166] = 0.0;
   out_8109048779199487053[167] = 0.0;
   out_8109048779199487053[168] = 0.0;
   out_8109048779199487053[169] = 0.0;
   out_8109048779199487053[170] = 0.0;
   out_8109048779199487053[171] = 1.0;
   out_8109048779199487053[172] = 0.0;
   out_8109048779199487053[173] = 0.0;
   out_8109048779199487053[174] = 0.0;
   out_8109048779199487053[175] = 0.0;
   out_8109048779199487053[176] = 0.0;
   out_8109048779199487053[177] = 0.0;
   out_8109048779199487053[178] = 0.0;
   out_8109048779199487053[179] = 0.0;
   out_8109048779199487053[180] = 0.0;
   out_8109048779199487053[181] = 0.0;
   out_8109048779199487053[182] = 0.0;
   out_8109048779199487053[183] = 0.0;
   out_8109048779199487053[184] = 0.0;
   out_8109048779199487053[185] = 0.0;
   out_8109048779199487053[186] = 0.0;
   out_8109048779199487053[187] = 0.0;
   out_8109048779199487053[188] = 0.0;
   out_8109048779199487053[189] = 0.0;
   out_8109048779199487053[190] = 1.0;
   out_8109048779199487053[191] = 0.0;
   out_8109048779199487053[192] = 0.0;
   out_8109048779199487053[193] = 0.0;
   out_8109048779199487053[194] = 0.0;
   out_8109048779199487053[195] = 0.0;
   out_8109048779199487053[196] = 0.0;
   out_8109048779199487053[197] = 0.0;
   out_8109048779199487053[198] = 0.0;
   out_8109048779199487053[199] = 0.0;
   out_8109048779199487053[200] = 0.0;
   out_8109048779199487053[201] = 0.0;
   out_8109048779199487053[202] = 0.0;
   out_8109048779199487053[203] = 0.0;
   out_8109048779199487053[204] = 0.0;
   out_8109048779199487053[205] = 0.0;
   out_8109048779199487053[206] = 0.0;
   out_8109048779199487053[207] = 0.0;
   out_8109048779199487053[208] = 0.0;
   out_8109048779199487053[209] = 1.0;
   out_8109048779199487053[210] = 0.0;
   out_8109048779199487053[211] = 0.0;
   out_8109048779199487053[212] = 0.0;
   out_8109048779199487053[213] = 0.0;
   out_8109048779199487053[214] = 0.0;
   out_8109048779199487053[215] = 0.0;
   out_8109048779199487053[216] = 0.0;
   out_8109048779199487053[217] = 0.0;
   out_8109048779199487053[218] = 0.0;
   out_8109048779199487053[219] = 0.0;
   out_8109048779199487053[220] = 0.0;
   out_8109048779199487053[221] = 0.0;
   out_8109048779199487053[222] = 0.0;
   out_8109048779199487053[223] = 0.0;
   out_8109048779199487053[224] = 0.0;
   out_8109048779199487053[225] = 0.0;
   out_8109048779199487053[226] = 0.0;
   out_8109048779199487053[227] = 0.0;
   out_8109048779199487053[228] = 1.0;
   out_8109048779199487053[229] = 0.0;
   out_8109048779199487053[230] = 0.0;
   out_8109048779199487053[231] = 0.0;
   out_8109048779199487053[232] = 0.0;
   out_8109048779199487053[233] = 0.0;
   out_8109048779199487053[234] = 0.0;
   out_8109048779199487053[235] = 0.0;
   out_8109048779199487053[236] = 0.0;
   out_8109048779199487053[237] = 0.0;
   out_8109048779199487053[238] = 0.0;
   out_8109048779199487053[239] = 0.0;
   out_8109048779199487053[240] = 0.0;
   out_8109048779199487053[241] = 0.0;
   out_8109048779199487053[242] = 0.0;
   out_8109048779199487053[243] = 0.0;
   out_8109048779199487053[244] = 0.0;
   out_8109048779199487053[245] = 0.0;
   out_8109048779199487053[246] = 0.0;
   out_8109048779199487053[247] = 1.0;
   out_8109048779199487053[248] = 0.0;
   out_8109048779199487053[249] = 0.0;
   out_8109048779199487053[250] = 0.0;
   out_8109048779199487053[251] = 0.0;
   out_8109048779199487053[252] = 0.0;
   out_8109048779199487053[253] = 0.0;
   out_8109048779199487053[254] = 0.0;
   out_8109048779199487053[255] = 0.0;
   out_8109048779199487053[256] = 0.0;
   out_8109048779199487053[257] = 0.0;
   out_8109048779199487053[258] = 0.0;
   out_8109048779199487053[259] = 0.0;
   out_8109048779199487053[260] = 0.0;
   out_8109048779199487053[261] = 0.0;
   out_8109048779199487053[262] = 0.0;
   out_8109048779199487053[263] = 0.0;
   out_8109048779199487053[264] = 0.0;
   out_8109048779199487053[265] = 0.0;
   out_8109048779199487053[266] = 1.0;
   out_8109048779199487053[267] = 0.0;
   out_8109048779199487053[268] = 0.0;
   out_8109048779199487053[269] = 0.0;
   out_8109048779199487053[270] = 0.0;
   out_8109048779199487053[271] = 0.0;
   out_8109048779199487053[272] = 0.0;
   out_8109048779199487053[273] = 0.0;
   out_8109048779199487053[274] = 0.0;
   out_8109048779199487053[275] = 0.0;
   out_8109048779199487053[276] = 0.0;
   out_8109048779199487053[277] = 0.0;
   out_8109048779199487053[278] = 0.0;
   out_8109048779199487053[279] = 0.0;
   out_8109048779199487053[280] = 0.0;
   out_8109048779199487053[281] = 0.0;
   out_8109048779199487053[282] = 0.0;
   out_8109048779199487053[283] = 0.0;
   out_8109048779199487053[284] = 0.0;
   out_8109048779199487053[285] = 1.0;
   out_8109048779199487053[286] = 0.0;
   out_8109048779199487053[287] = 0.0;
   out_8109048779199487053[288] = 0.0;
   out_8109048779199487053[289] = 0.0;
   out_8109048779199487053[290] = 0.0;
   out_8109048779199487053[291] = 0.0;
   out_8109048779199487053[292] = 0.0;
   out_8109048779199487053[293] = 0.0;
   out_8109048779199487053[294] = 0.0;
   out_8109048779199487053[295] = 0.0;
   out_8109048779199487053[296] = 0.0;
   out_8109048779199487053[297] = 0.0;
   out_8109048779199487053[298] = 0.0;
   out_8109048779199487053[299] = 0.0;
   out_8109048779199487053[300] = 0.0;
   out_8109048779199487053[301] = 0.0;
   out_8109048779199487053[302] = 0.0;
   out_8109048779199487053[303] = 0.0;
   out_8109048779199487053[304] = 1.0;
   out_8109048779199487053[305] = 0.0;
   out_8109048779199487053[306] = 0.0;
   out_8109048779199487053[307] = 0.0;
   out_8109048779199487053[308] = 0.0;
   out_8109048779199487053[309] = 0.0;
   out_8109048779199487053[310] = 0.0;
   out_8109048779199487053[311] = 0.0;
   out_8109048779199487053[312] = 0.0;
   out_8109048779199487053[313] = 0.0;
   out_8109048779199487053[314] = 0.0;
   out_8109048779199487053[315] = 0.0;
   out_8109048779199487053[316] = 0.0;
   out_8109048779199487053[317] = 0.0;
   out_8109048779199487053[318] = 0.0;
   out_8109048779199487053[319] = 0.0;
   out_8109048779199487053[320] = 0.0;
   out_8109048779199487053[321] = 0.0;
   out_8109048779199487053[322] = 0.0;
   out_8109048779199487053[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_4885069504534895976) {
   out_4885069504534895976[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_4885069504534895976[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_4885069504534895976[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_4885069504534895976[3] = dt*state[12] + state[3];
   out_4885069504534895976[4] = dt*state[13] + state[4];
   out_4885069504534895976[5] = dt*state[14] + state[5];
   out_4885069504534895976[6] = state[6];
   out_4885069504534895976[7] = state[7];
   out_4885069504534895976[8] = state[8];
   out_4885069504534895976[9] = state[9];
   out_4885069504534895976[10] = state[10];
   out_4885069504534895976[11] = state[11];
   out_4885069504534895976[12] = state[12];
   out_4885069504534895976[13] = state[13];
   out_4885069504534895976[14] = state[14];
   out_4885069504534895976[15] = state[15];
   out_4885069504534895976[16] = state[16];
   out_4885069504534895976[17] = state[17];
}
void F_fun(double *state, double dt, double *out_283482483053796941) {
   out_283482483053796941[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_283482483053796941[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_283482483053796941[2] = 0;
   out_283482483053796941[3] = 0;
   out_283482483053796941[4] = 0;
   out_283482483053796941[5] = 0;
   out_283482483053796941[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_283482483053796941[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_283482483053796941[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_283482483053796941[9] = 0;
   out_283482483053796941[10] = 0;
   out_283482483053796941[11] = 0;
   out_283482483053796941[12] = 0;
   out_283482483053796941[13] = 0;
   out_283482483053796941[14] = 0;
   out_283482483053796941[15] = 0;
   out_283482483053796941[16] = 0;
   out_283482483053796941[17] = 0;
   out_283482483053796941[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_283482483053796941[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_283482483053796941[20] = 0;
   out_283482483053796941[21] = 0;
   out_283482483053796941[22] = 0;
   out_283482483053796941[23] = 0;
   out_283482483053796941[24] = 0;
   out_283482483053796941[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_283482483053796941[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_283482483053796941[27] = 0;
   out_283482483053796941[28] = 0;
   out_283482483053796941[29] = 0;
   out_283482483053796941[30] = 0;
   out_283482483053796941[31] = 0;
   out_283482483053796941[32] = 0;
   out_283482483053796941[33] = 0;
   out_283482483053796941[34] = 0;
   out_283482483053796941[35] = 0;
   out_283482483053796941[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_283482483053796941[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_283482483053796941[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_283482483053796941[39] = 0;
   out_283482483053796941[40] = 0;
   out_283482483053796941[41] = 0;
   out_283482483053796941[42] = 0;
   out_283482483053796941[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_283482483053796941[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_283482483053796941[45] = 0;
   out_283482483053796941[46] = 0;
   out_283482483053796941[47] = 0;
   out_283482483053796941[48] = 0;
   out_283482483053796941[49] = 0;
   out_283482483053796941[50] = 0;
   out_283482483053796941[51] = 0;
   out_283482483053796941[52] = 0;
   out_283482483053796941[53] = 0;
   out_283482483053796941[54] = 0;
   out_283482483053796941[55] = 0;
   out_283482483053796941[56] = 0;
   out_283482483053796941[57] = 1;
   out_283482483053796941[58] = 0;
   out_283482483053796941[59] = 0;
   out_283482483053796941[60] = 0;
   out_283482483053796941[61] = 0;
   out_283482483053796941[62] = 0;
   out_283482483053796941[63] = 0;
   out_283482483053796941[64] = 0;
   out_283482483053796941[65] = 0;
   out_283482483053796941[66] = dt;
   out_283482483053796941[67] = 0;
   out_283482483053796941[68] = 0;
   out_283482483053796941[69] = 0;
   out_283482483053796941[70] = 0;
   out_283482483053796941[71] = 0;
   out_283482483053796941[72] = 0;
   out_283482483053796941[73] = 0;
   out_283482483053796941[74] = 0;
   out_283482483053796941[75] = 0;
   out_283482483053796941[76] = 1;
   out_283482483053796941[77] = 0;
   out_283482483053796941[78] = 0;
   out_283482483053796941[79] = 0;
   out_283482483053796941[80] = 0;
   out_283482483053796941[81] = 0;
   out_283482483053796941[82] = 0;
   out_283482483053796941[83] = 0;
   out_283482483053796941[84] = 0;
   out_283482483053796941[85] = dt;
   out_283482483053796941[86] = 0;
   out_283482483053796941[87] = 0;
   out_283482483053796941[88] = 0;
   out_283482483053796941[89] = 0;
   out_283482483053796941[90] = 0;
   out_283482483053796941[91] = 0;
   out_283482483053796941[92] = 0;
   out_283482483053796941[93] = 0;
   out_283482483053796941[94] = 0;
   out_283482483053796941[95] = 1;
   out_283482483053796941[96] = 0;
   out_283482483053796941[97] = 0;
   out_283482483053796941[98] = 0;
   out_283482483053796941[99] = 0;
   out_283482483053796941[100] = 0;
   out_283482483053796941[101] = 0;
   out_283482483053796941[102] = 0;
   out_283482483053796941[103] = 0;
   out_283482483053796941[104] = dt;
   out_283482483053796941[105] = 0;
   out_283482483053796941[106] = 0;
   out_283482483053796941[107] = 0;
   out_283482483053796941[108] = 0;
   out_283482483053796941[109] = 0;
   out_283482483053796941[110] = 0;
   out_283482483053796941[111] = 0;
   out_283482483053796941[112] = 0;
   out_283482483053796941[113] = 0;
   out_283482483053796941[114] = 1;
   out_283482483053796941[115] = 0;
   out_283482483053796941[116] = 0;
   out_283482483053796941[117] = 0;
   out_283482483053796941[118] = 0;
   out_283482483053796941[119] = 0;
   out_283482483053796941[120] = 0;
   out_283482483053796941[121] = 0;
   out_283482483053796941[122] = 0;
   out_283482483053796941[123] = 0;
   out_283482483053796941[124] = 0;
   out_283482483053796941[125] = 0;
   out_283482483053796941[126] = 0;
   out_283482483053796941[127] = 0;
   out_283482483053796941[128] = 0;
   out_283482483053796941[129] = 0;
   out_283482483053796941[130] = 0;
   out_283482483053796941[131] = 0;
   out_283482483053796941[132] = 0;
   out_283482483053796941[133] = 1;
   out_283482483053796941[134] = 0;
   out_283482483053796941[135] = 0;
   out_283482483053796941[136] = 0;
   out_283482483053796941[137] = 0;
   out_283482483053796941[138] = 0;
   out_283482483053796941[139] = 0;
   out_283482483053796941[140] = 0;
   out_283482483053796941[141] = 0;
   out_283482483053796941[142] = 0;
   out_283482483053796941[143] = 0;
   out_283482483053796941[144] = 0;
   out_283482483053796941[145] = 0;
   out_283482483053796941[146] = 0;
   out_283482483053796941[147] = 0;
   out_283482483053796941[148] = 0;
   out_283482483053796941[149] = 0;
   out_283482483053796941[150] = 0;
   out_283482483053796941[151] = 0;
   out_283482483053796941[152] = 1;
   out_283482483053796941[153] = 0;
   out_283482483053796941[154] = 0;
   out_283482483053796941[155] = 0;
   out_283482483053796941[156] = 0;
   out_283482483053796941[157] = 0;
   out_283482483053796941[158] = 0;
   out_283482483053796941[159] = 0;
   out_283482483053796941[160] = 0;
   out_283482483053796941[161] = 0;
   out_283482483053796941[162] = 0;
   out_283482483053796941[163] = 0;
   out_283482483053796941[164] = 0;
   out_283482483053796941[165] = 0;
   out_283482483053796941[166] = 0;
   out_283482483053796941[167] = 0;
   out_283482483053796941[168] = 0;
   out_283482483053796941[169] = 0;
   out_283482483053796941[170] = 0;
   out_283482483053796941[171] = 1;
   out_283482483053796941[172] = 0;
   out_283482483053796941[173] = 0;
   out_283482483053796941[174] = 0;
   out_283482483053796941[175] = 0;
   out_283482483053796941[176] = 0;
   out_283482483053796941[177] = 0;
   out_283482483053796941[178] = 0;
   out_283482483053796941[179] = 0;
   out_283482483053796941[180] = 0;
   out_283482483053796941[181] = 0;
   out_283482483053796941[182] = 0;
   out_283482483053796941[183] = 0;
   out_283482483053796941[184] = 0;
   out_283482483053796941[185] = 0;
   out_283482483053796941[186] = 0;
   out_283482483053796941[187] = 0;
   out_283482483053796941[188] = 0;
   out_283482483053796941[189] = 0;
   out_283482483053796941[190] = 1;
   out_283482483053796941[191] = 0;
   out_283482483053796941[192] = 0;
   out_283482483053796941[193] = 0;
   out_283482483053796941[194] = 0;
   out_283482483053796941[195] = 0;
   out_283482483053796941[196] = 0;
   out_283482483053796941[197] = 0;
   out_283482483053796941[198] = 0;
   out_283482483053796941[199] = 0;
   out_283482483053796941[200] = 0;
   out_283482483053796941[201] = 0;
   out_283482483053796941[202] = 0;
   out_283482483053796941[203] = 0;
   out_283482483053796941[204] = 0;
   out_283482483053796941[205] = 0;
   out_283482483053796941[206] = 0;
   out_283482483053796941[207] = 0;
   out_283482483053796941[208] = 0;
   out_283482483053796941[209] = 1;
   out_283482483053796941[210] = 0;
   out_283482483053796941[211] = 0;
   out_283482483053796941[212] = 0;
   out_283482483053796941[213] = 0;
   out_283482483053796941[214] = 0;
   out_283482483053796941[215] = 0;
   out_283482483053796941[216] = 0;
   out_283482483053796941[217] = 0;
   out_283482483053796941[218] = 0;
   out_283482483053796941[219] = 0;
   out_283482483053796941[220] = 0;
   out_283482483053796941[221] = 0;
   out_283482483053796941[222] = 0;
   out_283482483053796941[223] = 0;
   out_283482483053796941[224] = 0;
   out_283482483053796941[225] = 0;
   out_283482483053796941[226] = 0;
   out_283482483053796941[227] = 0;
   out_283482483053796941[228] = 1;
   out_283482483053796941[229] = 0;
   out_283482483053796941[230] = 0;
   out_283482483053796941[231] = 0;
   out_283482483053796941[232] = 0;
   out_283482483053796941[233] = 0;
   out_283482483053796941[234] = 0;
   out_283482483053796941[235] = 0;
   out_283482483053796941[236] = 0;
   out_283482483053796941[237] = 0;
   out_283482483053796941[238] = 0;
   out_283482483053796941[239] = 0;
   out_283482483053796941[240] = 0;
   out_283482483053796941[241] = 0;
   out_283482483053796941[242] = 0;
   out_283482483053796941[243] = 0;
   out_283482483053796941[244] = 0;
   out_283482483053796941[245] = 0;
   out_283482483053796941[246] = 0;
   out_283482483053796941[247] = 1;
   out_283482483053796941[248] = 0;
   out_283482483053796941[249] = 0;
   out_283482483053796941[250] = 0;
   out_283482483053796941[251] = 0;
   out_283482483053796941[252] = 0;
   out_283482483053796941[253] = 0;
   out_283482483053796941[254] = 0;
   out_283482483053796941[255] = 0;
   out_283482483053796941[256] = 0;
   out_283482483053796941[257] = 0;
   out_283482483053796941[258] = 0;
   out_283482483053796941[259] = 0;
   out_283482483053796941[260] = 0;
   out_283482483053796941[261] = 0;
   out_283482483053796941[262] = 0;
   out_283482483053796941[263] = 0;
   out_283482483053796941[264] = 0;
   out_283482483053796941[265] = 0;
   out_283482483053796941[266] = 1;
   out_283482483053796941[267] = 0;
   out_283482483053796941[268] = 0;
   out_283482483053796941[269] = 0;
   out_283482483053796941[270] = 0;
   out_283482483053796941[271] = 0;
   out_283482483053796941[272] = 0;
   out_283482483053796941[273] = 0;
   out_283482483053796941[274] = 0;
   out_283482483053796941[275] = 0;
   out_283482483053796941[276] = 0;
   out_283482483053796941[277] = 0;
   out_283482483053796941[278] = 0;
   out_283482483053796941[279] = 0;
   out_283482483053796941[280] = 0;
   out_283482483053796941[281] = 0;
   out_283482483053796941[282] = 0;
   out_283482483053796941[283] = 0;
   out_283482483053796941[284] = 0;
   out_283482483053796941[285] = 1;
   out_283482483053796941[286] = 0;
   out_283482483053796941[287] = 0;
   out_283482483053796941[288] = 0;
   out_283482483053796941[289] = 0;
   out_283482483053796941[290] = 0;
   out_283482483053796941[291] = 0;
   out_283482483053796941[292] = 0;
   out_283482483053796941[293] = 0;
   out_283482483053796941[294] = 0;
   out_283482483053796941[295] = 0;
   out_283482483053796941[296] = 0;
   out_283482483053796941[297] = 0;
   out_283482483053796941[298] = 0;
   out_283482483053796941[299] = 0;
   out_283482483053796941[300] = 0;
   out_283482483053796941[301] = 0;
   out_283482483053796941[302] = 0;
   out_283482483053796941[303] = 0;
   out_283482483053796941[304] = 1;
   out_283482483053796941[305] = 0;
   out_283482483053796941[306] = 0;
   out_283482483053796941[307] = 0;
   out_283482483053796941[308] = 0;
   out_283482483053796941[309] = 0;
   out_283482483053796941[310] = 0;
   out_283482483053796941[311] = 0;
   out_283482483053796941[312] = 0;
   out_283482483053796941[313] = 0;
   out_283482483053796941[314] = 0;
   out_283482483053796941[315] = 0;
   out_283482483053796941[316] = 0;
   out_283482483053796941[317] = 0;
   out_283482483053796941[318] = 0;
   out_283482483053796941[319] = 0;
   out_283482483053796941[320] = 0;
   out_283482483053796941[321] = 0;
   out_283482483053796941[322] = 0;
   out_283482483053796941[323] = 1;
}
void h_4(double *state, double *unused, double *out_113153149952726241) {
   out_113153149952726241[0] = state[6] + state[9];
   out_113153149952726241[1] = state[7] + state[10];
   out_113153149952726241[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_9139747812627166459) {
   out_9139747812627166459[0] = 0;
   out_9139747812627166459[1] = 0;
   out_9139747812627166459[2] = 0;
   out_9139747812627166459[3] = 0;
   out_9139747812627166459[4] = 0;
   out_9139747812627166459[5] = 0;
   out_9139747812627166459[6] = 1;
   out_9139747812627166459[7] = 0;
   out_9139747812627166459[8] = 0;
   out_9139747812627166459[9] = 1;
   out_9139747812627166459[10] = 0;
   out_9139747812627166459[11] = 0;
   out_9139747812627166459[12] = 0;
   out_9139747812627166459[13] = 0;
   out_9139747812627166459[14] = 0;
   out_9139747812627166459[15] = 0;
   out_9139747812627166459[16] = 0;
   out_9139747812627166459[17] = 0;
   out_9139747812627166459[18] = 0;
   out_9139747812627166459[19] = 0;
   out_9139747812627166459[20] = 0;
   out_9139747812627166459[21] = 0;
   out_9139747812627166459[22] = 0;
   out_9139747812627166459[23] = 0;
   out_9139747812627166459[24] = 0;
   out_9139747812627166459[25] = 1;
   out_9139747812627166459[26] = 0;
   out_9139747812627166459[27] = 0;
   out_9139747812627166459[28] = 1;
   out_9139747812627166459[29] = 0;
   out_9139747812627166459[30] = 0;
   out_9139747812627166459[31] = 0;
   out_9139747812627166459[32] = 0;
   out_9139747812627166459[33] = 0;
   out_9139747812627166459[34] = 0;
   out_9139747812627166459[35] = 0;
   out_9139747812627166459[36] = 0;
   out_9139747812627166459[37] = 0;
   out_9139747812627166459[38] = 0;
   out_9139747812627166459[39] = 0;
   out_9139747812627166459[40] = 0;
   out_9139747812627166459[41] = 0;
   out_9139747812627166459[42] = 0;
   out_9139747812627166459[43] = 0;
   out_9139747812627166459[44] = 1;
   out_9139747812627166459[45] = 0;
   out_9139747812627166459[46] = 0;
   out_9139747812627166459[47] = 1;
   out_9139747812627166459[48] = 0;
   out_9139747812627166459[49] = 0;
   out_9139747812627166459[50] = 0;
   out_9139747812627166459[51] = 0;
   out_9139747812627166459[52] = 0;
   out_9139747812627166459[53] = 0;
}
void h_10(double *state, double *unused, double *out_8729829078688731131) {
   out_8729829078688731131[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8729829078688731131[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8729829078688731131[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_1254535187907276211) {
   out_1254535187907276211[0] = 0;
   out_1254535187907276211[1] = 9.8100000000000005*cos(state[1]);
   out_1254535187907276211[2] = 0;
   out_1254535187907276211[3] = 0;
   out_1254535187907276211[4] = -state[8];
   out_1254535187907276211[5] = state[7];
   out_1254535187907276211[6] = 0;
   out_1254535187907276211[7] = state[5];
   out_1254535187907276211[8] = -state[4];
   out_1254535187907276211[9] = 0;
   out_1254535187907276211[10] = 0;
   out_1254535187907276211[11] = 0;
   out_1254535187907276211[12] = 1;
   out_1254535187907276211[13] = 0;
   out_1254535187907276211[14] = 0;
   out_1254535187907276211[15] = 1;
   out_1254535187907276211[16] = 0;
   out_1254535187907276211[17] = 0;
   out_1254535187907276211[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_1254535187907276211[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_1254535187907276211[20] = 0;
   out_1254535187907276211[21] = state[8];
   out_1254535187907276211[22] = 0;
   out_1254535187907276211[23] = -state[6];
   out_1254535187907276211[24] = -state[5];
   out_1254535187907276211[25] = 0;
   out_1254535187907276211[26] = state[3];
   out_1254535187907276211[27] = 0;
   out_1254535187907276211[28] = 0;
   out_1254535187907276211[29] = 0;
   out_1254535187907276211[30] = 0;
   out_1254535187907276211[31] = 1;
   out_1254535187907276211[32] = 0;
   out_1254535187907276211[33] = 0;
   out_1254535187907276211[34] = 1;
   out_1254535187907276211[35] = 0;
   out_1254535187907276211[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_1254535187907276211[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_1254535187907276211[38] = 0;
   out_1254535187907276211[39] = -state[7];
   out_1254535187907276211[40] = state[6];
   out_1254535187907276211[41] = 0;
   out_1254535187907276211[42] = state[4];
   out_1254535187907276211[43] = -state[3];
   out_1254535187907276211[44] = 0;
   out_1254535187907276211[45] = 0;
   out_1254535187907276211[46] = 0;
   out_1254535187907276211[47] = 0;
   out_1254535187907276211[48] = 0;
   out_1254535187907276211[49] = 0;
   out_1254535187907276211[50] = 1;
   out_1254535187907276211[51] = 0;
   out_1254535187907276211[52] = 0;
   out_1254535187907276211[53] = 1;
}
void h_13(double *state, double *unused, double *out_9102617023895648854) {
   out_9102617023895648854[0] = state[3];
   out_9102617023895648854[1] = state[4];
   out_9102617023895648854[2] = state[5];
}
void H_13(double *state, double *unused, double *out_5473240797779861133) {
   out_5473240797779861133[0] = 0;
   out_5473240797779861133[1] = 0;
   out_5473240797779861133[2] = 0;
   out_5473240797779861133[3] = 1;
   out_5473240797779861133[4] = 0;
   out_5473240797779861133[5] = 0;
   out_5473240797779861133[6] = 0;
   out_5473240797779861133[7] = 0;
   out_5473240797779861133[8] = 0;
   out_5473240797779861133[9] = 0;
   out_5473240797779861133[10] = 0;
   out_5473240797779861133[11] = 0;
   out_5473240797779861133[12] = 0;
   out_5473240797779861133[13] = 0;
   out_5473240797779861133[14] = 0;
   out_5473240797779861133[15] = 0;
   out_5473240797779861133[16] = 0;
   out_5473240797779861133[17] = 0;
   out_5473240797779861133[18] = 0;
   out_5473240797779861133[19] = 0;
   out_5473240797779861133[20] = 0;
   out_5473240797779861133[21] = 0;
   out_5473240797779861133[22] = 1;
   out_5473240797779861133[23] = 0;
   out_5473240797779861133[24] = 0;
   out_5473240797779861133[25] = 0;
   out_5473240797779861133[26] = 0;
   out_5473240797779861133[27] = 0;
   out_5473240797779861133[28] = 0;
   out_5473240797779861133[29] = 0;
   out_5473240797779861133[30] = 0;
   out_5473240797779861133[31] = 0;
   out_5473240797779861133[32] = 0;
   out_5473240797779861133[33] = 0;
   out_5473240797779861133[34] = 0;
   out_5473240797779861133[35] = 0;
   out_5473240797779861133[36] = 0;
   out_5473240797779861133[37] = 0;
   out_5473240797779861133[38] = 0;
   out_5473240797779861133[39] = 0;
   out_5473240797779861133[40] = 0;
   out_5473240797779861133[41] = 1;
   out_5473240797779861133[42] = 0;
   out_5473240797779861133[43] = 0;
   out_5473240797779861133[44] = 0;
   out_5473240797779861133[45] = 0;
   out_5473240797779861133[46] = 0;
   out_5473240797779861133[47] = 0;
   out_5473240797779861133[48] = 0;
   out_5473240797779861133[49] = 0;
   out_5473240797779861133[50] = 0;
   out_5473240797779861133[51] = 0;
   out_5473240797779861133[52] = 0;
   out_5473240797779861133[53] = 0;
}
void h_14(double *state, double *unused, double *out_5520898165787292420) {
   out_5520898165787292420[0] = state[6];
   out_5520898165787292420[1] = state[7];
   out_5520898165787292420[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6224207828787012861) {
   out_6224207828787012861[0] = 0;
   out_6224207828787012861[1] = 0;
   out_6224207828787012861[2] = 0;
   out_6224207828787012861[3] = 0;
   out_6224207828787012861[4] = 0;
   out_6224207828787012861[5] = 0;
   out_6224207828787012861[6] = 1;
   out_6224207828787012861[7] = 0;
   out_6224207828787012861[8] = 0;
   out_6224207828787012861[9] = 0;
   out_6224207828787012861[10] = 0;
   out_6224207828787012861[11] = 0;
   out_6224207828787012861[12] = 0;
   out_6224207828787012861[13] = 0;
   out_6224207828787012861[14] = 0;
   out_6224207828787012861[15] = 0;
   out_6224207828787012861[16] = 0;
   out_6224207828787012861[17] = 0;
   out_6224207828787012861[18] = 0;
   out_6224207828787012861[19] = 0;
   out_6224207828787012861[20] = 0;
   out_6224207828787012861[21] = 0;
   out_6224207828787012861[22] = 0;
   out_6224207828787012861[23] = 0;
   out_6224207828787012861[24] = 0;
   out_6224207828787012861[25] = 1;
   out_6224207828787012861[26] = 0;
   out_6224207828787012861[27] = 0;
   out_6224207828787012861[28] = 0;
   out_6224207828787012861[29] = 0;
   out_6224207828787012861[30] = 0;
   out_6224207828787012861[31] = 0;
   out_6224207828787012861[32] = 0;
   out_6224207828787012861[33] = 0;
   out_6224207828787012861[34] = 0;
   out_6224207828787012861[35] = 0;
   out_6224207828787012861[36] = 0;
   out_6224207828787012861[37] = 0;
   out_6224207828787012861[38] = 0;
   out_6224207828787012861[39] = 0;
   out_6224207828787012861[40] = 0;
   out_6224207828787012861[41] = 0;
   out_6224207828787012861[42] = 0;
   out_6224207828787012861[43] = 0;
   out_6224207828787012861[44] = 1;
   out_6224207828787012861[45] = 0;
   out_6224207828787012861[46] = 0;
   out_6224207828787012861[47] = 0;
   out_6224207828787012861[48] = 0;
   out_6224207828787012861[49] = 0;
   out_6224207828787012861[50] = 0;
   out_6224207828787012861[51] = 0;
   out_6224207828787012861[52] = 0;
   out_6224207828787012861[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_4876828158992611850) {
  err_fun(nom_x, delta_x, out_4876828158992611850);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4991901881103411206) {
  inv_err_fun(nom_x, true_x, out_4991901881103411206);
}
void pose_H_mod_fun(double *state, double *out_8109048779199487053) {
  H_mod_fun(state, out_8109048779199487053);
}
void pose_f_fun(double *state, double dt, double *out_4885069504534895976) {
  f_fun(state,  dt, out_4885069504534895976);
}
void pose_F_fun(double *state, double dt, double *out_283482483053796941) {
  F_fun(state,  dt, out_283482483053796941);
}
void pose_h_4(double *state, double *unused, double *out_113153149952726241) {
  h_4(state, unused, out_113153149952726241);
}
void pose_H_4(double *state, double *unused, double *out_9139747812627166459) {
  H_4(state, unused, out_9139747812627166459);
}
void pose_h_10(double *state, double *unused, double *out_8729829078688731131) {
  h_10(state, unused, out_8729829078688731131);
}
void pose_H_10(double *state, double *unused, double *out_1254535187907276211) {
  H_10(state, unused, out_1254535187907276211);
}
void pose_h_13(double *state, double *unused, double *out_9102617023895648854) {
  h_13(state, unused, out_9102617023895648854);
}
void pose_H_13(double *state, double *unused, double *out_5473240797779861133) {
  H_13(state, unused, out_5473240797779861133);
}
void pose_h_14(double *state, double *unused, double *out_5520898165787292420) {
  h_14(state, unused, out_5520898165787292420);
}
void pose_H_14(double *state, double *unused, double *out_6224207828787012861) {
  H_14(state, unused, out_6224207828787012861);
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
