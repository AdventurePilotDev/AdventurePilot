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
void err_fun(double *nom_x, double *delta_x, double *out_2812259937135269323) {
   out_2812259937135269323[0] = delta_x[0] + nom_x[0];
   out_2812259937135269323[1] = delta_x[1] + nom_x[1];
   out_2812259937135269323[2] = delta_x[2] + nom_x[2];
   out_2812259937135269323[3] = delta_x[3] + nom_x[3];
   out_2812259937135269323[4] = delta_x[4] + nom_x[4];
   out_2812259937135269323[5] = delta_x[5] + nom_x[5];
   out_2812259937135269323[6] = delta_x[6] + nom_x[6];
   out_2812259937135269323[7] = delta_x[7] + nom_x[7];
   out_2812259937135269323[8] = delta_x[8] + nom_x[8];
   out_2812259937135269323[9] = delta_x[9] + nom_x[9];
   out_2812259937135269323[10] = delta_x[10] + nom_x[10];
   out_2812259937135269323[11] = delta_x[11] + nom_x[11];
   out_2812259937135269323[12] = delta_x[12] + nom_x[12];
   out_2812259937135269323[13] = delta_x[13] + nom_x[13];
   out_2812259937135269323[14] = delta_x[14] + nom_x[14];
   out_2812259937135269323[15] = delta_x[15] + nom_x[15];
   out_2812259937135269323[16] = delta_x[16] + nom_x[16];
   out_2812259937135269323[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7702513841393801961) {
   out_7702513841393801961[0] = -nom_x[0] + true_x[0];
   out_7702513841393801961[1] = -nom_x[1] + true_x[1];
   out_7702513841393801961[2] = -nom_x[2] + true_x[2];
   out_7702513841393801961[3] = -nom_x[3] + true_x[3];
   out_7702513841393801961[4] = -nom_x[4] + true_x[4];
   out_7702513841393801961[5] = -nom_x[5] + true_x[5];
   out_7702513841393801961[6] = -nom_x[6] + true_x[6];
   out_7702513841393801961[7] = -nom_x[7] + true_x[7];
   out_7702513841393801961[8] = -nom_x[8] + true_x[8];
   out_7702513841393801961[9] = -nom_x[9] + true_x[9];
   out_7702513841393801961[10] = -nom_x[10] + true_x[10];
   out_7702513841393801961[11] = -nom_x[11] + true_x[11];
   out_7702513841393801961[12] = -nom_x[12] + true_x[12];
   out_7702513841393801961[13] = -nom_x[13] + true_x[13];
   out_7702513841393801961[14] = -nom_x[14] + true_x[14];
   out_7702513841393801961[15] = -nom_x[15] + true_x[15];
   out_7702513841393801961[16] = -nom_x[16] + true_x[16];
   out_7702513841393801961[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8578061460798452048) {
   out_8578061460798452048[0] = 1.0;
   out_8578061460798452048[1] = 0.0;
   out_8578061460798452048[2] = 0.0;
   out_8578061460798452048[3] = 0.0;
   out_8578061460798452048[4] = 0.0;
   out_8578061460798452048[5] = 0.0;
   out_8578061460798452048[6] = 0.0;
   out_8578061460798452048[7] = 0.0;
   out_8578061460798452048[8] = 0.0;
   out_8578061460798452048[9] = 0.0;
   out_8578061460798452048[10] = 0.0;
   out_8578061460798452048[11] = 0.0;
   out_8578061460798452048[12] = 0.0;
   out_8578061460798452048[13] = 0.0;
   out_8578061460798452048[14] = 0.0;
   out_8578061460798452048[15] = 0.0;
   out_8578061460798452048[16] = 0.0;
   out_8578061460798452048[17] = 0.0;
   out_8578061460798452048[18] = 0.0;
   out_8578061460798452048[19] = 1.0;
   out_8578061460798452048[20] = 0.0;
   out_8578061460798452048[21] = 0.0;
   out_8578061460798452048[22] = 0.0;
   out_8578061460798452048[23] = 0.0;
   out_8578061460798452048[24] = 0.0;
   out_8578061460798452048[25] = 0.0;
   out_8578061460798452048[26] = 0.0;
   out_8578061460798452048[27] = 0.0;
   out_8578061460798452048[28] = 0.0;
   out_8578061460798452048[29] = 0.0;
   out_8578061460798452048[30] = 0.0;
   out_8578061460798452048[31] = 0.0;
   out_8578061460798452048[32] = 0.0;
   out_8578061460798452048[33] = 0.0;
   out_8578061460798452048[34] = 0.0;
   out_8578061460798452048[35] = 0.0;
   out_8578061460798452048[36] = 0.0;
   out_8578061460798452048[37] = 0.0;
   out_8578061460798452048[38] = 1.0;
   out_8578061460798452048[39] = 0.0;
   out_8578061460798452048[40] = 0.0;
   out_8578061460798452048[41] = 0.0;
   out_8578061460798452048[42] = 0.0;
   out_8578061460798452048[43] = 0.0;
   out_8578061460798452048[44] = 0.0;
   out_8578061460798452048[45] = 0.0;
   out_8578061460798452048[46] = 0.0;
   out_8578061460798452048[47] = 0.0;
   out_8578061460798452048[48] = 0.0;
   out_8578061460798452048[49] = 0.0;
   out_8578061460798452048[50] = 0.0;
   out_8578061460798452048[51] = 0.0;
   out_8578061460798452048[52] = 0.0;
   out_8578061460798452048[53] = 0.0;
   out_8578061460798452048[54] = 0.0;
   out_8578061460798452048[55] = 0.0;
   out_8578061460798452048[56] = 0.0;
   out_8578061460798452048[57] = 1.0;
   out_8578061460798452048[58] = 0.0;
   out_8578061460798452048[59] = 0.0;
   out_8578061460798452048[60] = 0.0;
   out_8578061460798452048[61] = 0.0;
   out_8578061460798452048[62] = 0.0;
   out_8578061460798452048[63] = 0.0;
   out_8578061460798452048[64] = 0.0;
   out_8578061460798452048[65] = 0.0;
   out_8578061460798452048[66] = 0.0;
   out_8578061460798452048[67] = 0.0;
   out_8578061460798452048[68] = 0.0;
   out_8578061460798452048[69] = 0.0;
   out_8578061460798452048[70] = 0.0;
   out_8578061460798452048[71] = 0.0;
   out_8578061460798452048[72] = 0.0;
   out_8578061460798452048[73] = 0.0;
   out_8578061460798452048[74] = 0.0;
   out_8578061460798452048[75] = 0.0;
   out_8578061460798452048[76] = 1.0;
   out_8578061460798452048[77] = 0.0;
   out_8578061460798452048[78] = 0.0;
   out_8578061460798452048[79] = 0.0;
   out_8578061460798452048[80] = 0.0;
   out_8578061460798452048[81] = 0.0;
   out_8578061460798452048[82] = 0.0;
   out_8578061460798452048[83] = 0.0;
   out_8578061460798452048[84] = 0.0;
   out_8578061460798452048[85] = 0.0;
   out_8578061460798452048[86] = 0.0;
   out_8578061460798452048[87] = 0.0;
   out_8578061460798452048[88] = 0.0;
   out_8578061460798452048[89] = 0.0;
   out_8578061460798452048[90] = 0.0;
   out_8578061460798452048[91] = 0.0;
   out_8578061460798452048[92] = 0.0;
   out_8578061460798452048[93] = 0.0;
   out_8578061460798452048[94] = 0.0;
   out_8578061460798452048[95] = 1.0;
   out_8578061460798452048[96] = 0.0;
   out_8578061460798452048[97] = 0.0;
   out_8578061460798452048[98] = 0.0;
   out_8578061460798452048[99] = 0.0;
   out_8578061460798452048[100] = 0.0;
   out_8578061460798452048[101] = 0.0;
   out_8578061460798452048[102] = 0.0;
   out_8578061460798452048[103] = 0.0;
   out_8578061460798452048[104] = 0.0;
   out_8578061460798452048[105] = 0.0;
   out_8578061460798452048[106] = 0.0;
   out_8578061460798452048[107] = 0.0;
   out_8578061460798452048[108] = 0.0;
   out_8578061460798452048[109] = 0.0;
   out_8578061460798452048[110] = 0.0;
   out_8578061460798452048[111] = 0.0;
   out_8578061460798452048[112] = 0.0;
   out_8578061460798452048[113] = 0.0;
   out_8578061460798452048[114] = 1.0;
   out_8578061460798452048[115] = 0.0;
   out_8578061460798452048[116] = 0.0;
   out_8578061460798452048[117] = 0.0;
   out_8578061460798452048[118] = 0.0;
   out_8578061460798452048[119] = 0.0;
   out_8578061460798452048[120] = 0.0;
   out_8578061460798452048[121] = 0.0;
   out_8578061460798452048[122] = 0.0;
   out_8578061460798452048[123] = 0.0;
   out_8578061460798452048[124] = 0.0;
   out_8578061460798452048[125] = 0.0;
   out_8578061460798452048[126] = 0.0;
   out_8578061460798452048[127] = 0.0;
   out_8578061460798452048[128] = 0.0;
   out_8578061460798452048[129] = 0.0;
   out_8578061460798452048[130] = 0.0;
   out_8578061460798452048[131] = 0.0;
   out_8578061460798452048[132] = 0.0;
   out_8578061460798452048[133] = 1.0;
   out_8578061460798452048[134] = 0.0;
   out_8578061460798452048[135] = 0.0;
   out_8578061460798452048[136] = 0.0;
   out_8578061460798452048[137] = 0.0;
   out_8578061460798452048[138] = 0.0;
   out_8578061460798452048[139] = 0.0;
   out_8578061460798452048[140] = 0.0;
   out_8578061460798452048[141] = 0.0;
   out_8578061460798452048[142] = 0.0;
   out_8578061460798452048[143] = 0.0;
   out_8578061460798452048[144] = 0.0;
   out_8578061460798452048[145] = 0.0;
   out_8578061460798452048[146] = 0.0;
   out_8578061460798452048[147] = 0.0;
   out_8578061460798452048[148] = 0.0;
   out_8578061460798452048[149] = 0.0;
   out_8578061460798452048[150] = 0.0;
   out_8578061460798452048[151] = 0.0;
   out_8578061460798452048[152] = 1.0;
   out_8578061460798452048[153] = 0.0;
   out_8578061460798452048[154] = 0.0;
   out_8578061460798452048[155] = 0.0;
   out_8578061460798452048[156] = 0.0;
   out_8578061460798452048[157] = 0.0;
   out_8578061460798452048[158] = 0.0;
   out_8578061460798452048[159] = 0.0;
   out_8578061460798452048[160] = 0.0;
   out_8578061460798452048[161] = 0.0;
   out_8578061460798452048[162] = 0.0;
   out_8578061460798452048[163] = 0.0;
   out_8578061460798452048[164] = 0.0;
   out_8578061460798452048[165] = 0.0;
   out_8578061460798452048[166] = 0.0;
   out_8578061460798452048[167] = 0.0;
   out_8578061460798452048[168] = 0.0;
   out_8578061460798452048[169] = 0.0;
   out_8578061460798452048[170] = 0.0;
   out_8578061460798452048[171] = 1.0;
   out_8578061460798452048[172] = 0.0;
   out_8578061460798452048[173] = 0.0;
   out_8578061460798452048[174] = 0.0;
   out_8578061460798452048[175] = 0.0;
   out_8578061460798452048[176] = 0.0;
   out_8578061460798452048[177] = 0.0;
   out_8578061460798452048[178] = 0.0;
   out_8578061460798452048[179] = 0.0;
   out_8578061460798452048[180] = 0.0;
   out_8578061460798452048[181] = 0.0;
   out_8578061460798452048[182] = 0.0;
   out_8578061460798452048[183] = 0.0;
   out_8578061460798452048[184] = 0.0;
   out_8578061460798452048[185] = 0.0;
   out_8578061460798452048[186] = 0.0;
   out_8578061460798452048[187] = 0.0;
   out_8578061460798452048[188] = 0.0;
   out_8578061460798452048[189] = 0.0;
   out_8578061460798452048[190] = 1.0;
   out_8578061460798452048[191] = 0.0;
   out_8578061460798452048[192] = 0.0;
   out_8578061460798452048[193] = 0.0;
   out_8578061460798452048[194] = 0.0;
   out_8578061460798452048[195] = 0.0;
   out_8578061460798452048[196] = 0.0;
   out_8578061460798452048[197] = 0.0;
   out_8578061460798452048[198] = 0.0;
   out_8578061460798452048[199] = 0.0;
   out_8578061460798452048[200] = 0.0;
   out_8578061460798452048[201] = 0.0;
   out_8578061460798452048[202] = 0.0;
   out_8578061460798452048[203] = 0.0;
   out_8578061460798452048[204] = 0.0;
   out_8578061460798452048[205] = 0.0;
   out_8578061460798452048[206] = 0.0;
   out_8578061460798452048[207] = 0.0;
   out_8578061460798452048[208] = 0.0;
   out_8578061460798452048[209] = 1.0;
   out_8578061460798452048[210] = 0.0;
   out_8578061460798452048[211] = 0.0;
   out_8578061460798452048[212] = 0.0;
   out_8578061460798452048[213] = 0.0;
   out_8578061460798452048[214] = 0.0;
   out_8578061460798452048[215] = 0.0;
   out_8578061460798452048[216] = 0.0;
   out_8578061460798452048[217] = 0.0;
   out_8578061460798452048[218] = 0.0;
   out_8578061460798452048[219] = 0.0;
   out_8578061460798452048[220] = 0.0;
   out_8578061460798452048[221] = 0.0;
   out_8578061460798452048[222] = 0.0;
   out_8578061460798452048[223] = 0.0;
   out_8578061460798452048[224] = 0.0;
   out_8578061460798452048[225] = 0.0;
   out_8578061460798452048[226] = 0.0;
   out_8578061460798452048[227] = 0.0;
   out_8578061460798452048[228] = 1.0;
   out_8578061460798452048[229] = 0.0;
   out_8578061460798452048[230] = 0.0;
   out_8578061460798452048[231] = 0.0;
   out_8578061460798452048[232] = 0.0;
   out_8578061460798452048[233] = 0.0;
   out_8578061460798452048[234] = 0.0;
   out_8578061460798452048[235] = 0.0;
   out_8578061460798452048[236] = 0.0;
   out_8578061460798452048[237] = 0.0;
   out_8578061460798452048[238] = 0.0;
   out_8578061460798452048[239] = 0.0;
   out_8578061460798452048[240] = 0.0;
   out_8578061460798452048[241] = 0.0;
   out_8578061460798452048[242] = 0.0;
   out_8578061460798452048[243] = 0.0;
   out_8578061460798452048[244] = 0.0;
   out_8578061460798452048[245] = 0.0;
   out_8578061460798452048[246] = 0.0;
   out_8578061460798452048[247] = 1.0;
   out_8578061460798452048[248] = 0.0;
   out_8578061460798452048[249] = 0.0;
   out_8578061460798452048[250] = 0.0;
   out_8578061460798452048[251] = 0.0;
   out_8578061460798452048[252] = 0.0;
   out_8578061460798452048[253] = 0.0;
   out_8578061460798452048[254] = 0.0;
   out_8578061460798452048[255] = 0.0;
   out_8578061460798452048[256] = 0.0;
   out_8578061460798452048[257] = 0.0;
   out_8578061460798452048[258] = 0.0;
   out_8578061460798452048[259] = 0.0;
   out_8578061460798452048[260] = 0.0;
   out_8578061460798452048[261] = 0.0;
   out_8578061460798452048[262] = 0.0;
   out_8578061460798452048[263] = 0.0;
   out_8578061460798452048[264] = 0.0;
   out_8578061460798452048[265] = 0.0;
   out_8578061460798452048[266] = 1.0;
   out_8578061460798452048[267] = 0.0;
   out_8578061460798452048[268] = 0.0;
   out_8578061460798452048[269] = 0.0;
   out_8578061460798452048[270] = 0.0;
   out_8578061460798452048[271] = 0.0;
   out_8578061460798452048[272] = 0.0;
   out_8578061460798452048[273] = 0.0;
   out_8578061460798452048[274] = 0.0;
   out_8578061460798452048[275] = 0.0;
   out_8578061460798452048[276] = 0.0;
   out_8578061460798452048[277] = 0.0;
   out_8578061460798452048[278] = 0.0;
   out_8578061460798452048[279] = 0.0;
   out_8578061460798452048[280] = 0.0;
   out_8578061460798452048[281] = 0.0;
   out_8578061460798452048[282] = 0.0;
   out_8578061460798452048[283] = 0.0;
   out_8578061460798452048[284] = 0.0;
   out_8578061460798452048[285] = 1.0;
   out_8578061460798452048[286] = 0.0;
   out_8578061460798452048[287] = 0.0;
   out_8578061460798452048[288] = 0.0;
   out_8578061460798452048[289] = 0.0;
   out_8578061460798452048[290] = 0.0;
   out_8578061460798452048[291] = 0.0;
   out_8578061460798452048[292] = 0.0;
   out_8578061460798452048[293] = 0.0;
   out_8578061460798452048[294] = 0.0;
   out_8578061460798452048[295] = 0.0;
   out_8578061460798452048[296] = 0.0;
   out_8578061460798452048[297] = 0.0;
   out_8578061460798452048[298] = 0.0;
   out_8578061460798452048[299] = 0.0;
   out_8578061460798452048[300] = 0.0;
   out_8578061460798452048[301] = 0.0;
   out_8578061460798452048[302] = 0.0;
   out_8578061460798452048[303] = 0.0;
   out_8578061460798452048[304] = 1.0;
   out_8578061460798452048[305] = 0.0;
   out_8578061460798452048[306] = 0.0;
   out_8578061460798452048[307] = 0.0;
   out_8578061460798452048[308] = 0.0;
   out_8578061460798452048[309] = 0.0;
   out_8578061460798452048[310] = 0.0;
   out_8578061460798452048[311] = 0.0;
   out_8578061460798452048[312] = 0.0;
   out_8578061460798452048[313] = 0.0;
   out_8578061460798452048[314] = 0.0;
   out_8578061460798452048[315] = 0.0;
   out_8578061460798452048[316] = 0.0;
   out_8578061460798452048[317] = 0.0;
   out_8578061460798452048[318] = 0.0;
   out_8578061460798452048[319] = 0.0;
   out_8578061460798452048[320] = 0.0;
   out_8578061460798452048[321] = 0.0;
   out_8578061460798452048[322] = 0.0;
   out_8578061460798452048[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_6960018690742428862) {
   out_6960018690742428862[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_6960018690742428862[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_6960018690742428862[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_6960018690742428862[3] = dt*state[12] + state[3];
   out_6960018690742428862[4] = dt*state[13] + state[4];
   out_6960018690742428862[5] = dt*state[14] + state[5];
   out_6960018690742428862[6] = state[6];
   out_6960018690742428862[7] = state[7];
   out_6960018690742428862[8] = state[8];
   out_6960018690742428862[9] = state[9];
   out_6960018690742428862[10] = state[10];
   out_6960018690742428862[11] = state[11];
   out_6960018690742428862[12] = state[12];
   out_6960018690742428862[13] = state[13];
   out_6960018690742428862[14] = state[14];
   out_6960018690742428862[15] = state[15];
   out_6960018690742428862[16] = state[16];
   out_6960018690742428862[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1981298987176351576) {
   out_1981298987176351576[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1981298987176351576[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1981298987176351576[2] = 0;
   out_1981298987176351576[3] = 0;
   out_1981298987176351576[4] = 0;
   out_1981298987176351576[5] = 0;
   out_1981298987176351576[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1981298987176351576[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1981298987176351576[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1981298987176351576[9] = 0;
   out_1981298987176351576[10] = 0;
   out_1981298987176351576[11] = 0;
   out_1981298987176351576[12] = 0;
   out_1981298987176351576[13] = 0;
   out_1981298987176351576[14] = 0;
   out_1981298987176351576[15] = 0;
   out_1981298987176351576[16] = 0;
   out_1981298987176351576[17] = 0;
   out_1981298987176351576[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1981298987176351576[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1981298987176351576[20] = 0;
   out_1981298987176351576[21] = 0;
   out_1981298987176351576[22] = 0;
   out_1981298987176351576[23] = 0;
   out_1981298987176351576[24] = 0;
   out_1981298987176351576[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1981298987176351576[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1981298987176351576[27] = 0;
   out_1981298987176351576[28] = 0;
   out_1981298987176351576[29] = 0;
   out_1981298987176351576[30] = 0;
   out_1981298987176351576[31] = 0;
   out_1981298987176351576[32] = 0;
   out_1981298987176351576[33] = 0;
   out_1981298987176351576[34] = 0;
   out_1981298987176351576[35] = 0;
   out_1981298987176351576[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1981298987176351576[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1981298987176351576[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1981298987176351576[39] = 0;
   out_1981298987176351576[40] = 0;
   out_1981298987176351576[41] = 0;
   out_1981298987176351576[42] = 0;
   out_1981298987176351576[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1981298987176351576[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1981298987176351576[45] = 0;
   out_1981298987176351576[46] = 0;
   out_1981298987176351576[47] = 0;
   out_1981298987176351576[48] = 0;
   out_1981298987176351576[49] = 0;
   out_1981298987176351576[50] = 0;
   out_1981298987176351576[51] = 0;
   out_1981298987176351576[52] = 0;
   out_1981298987176351576[53] = 0;
   out_1981298987176351576[54] = 0;
   out_1981298987176351576[55] = 0;
   out_1981298987176351576[56] = 0;
   out_1981298987176351576[57] = 1;
   out_1981298987176351576[58] = 0;
   out_1981298987176351576[59] = 0;
   out_1981298987176351576[60] = 0;
   out_1981298987176351576[61] = 0;
   out_1981298987176351576[62] = 0;
   out_1981298987176351576[63] = 0;
   out_1981298987176351576[64] = 0;
   out_1981298987176351576[65] = 0;
   out_1981298987176351576[66] = dt;
   out_1981298987176351576[67] = 0;
   out_1981298987176351576[68] = 0;
   out_1981298987176351576[69] = 0;
   out_1981298987176351576[70] = 0;
   out_1981298987176351576[71] = 0;
   out_1981298987176351576[72] = 0;
   out_1981298987176351576[73] = 0;
   out_1981298987176351576[74] = 0;
   out_1981298987176351576[75] = 0;
   out_1981298987176351576[76] = 1;
   out_1981298987176351576[77] = 0;
   out_1981298987176351576[78] = 0;
   out_1981298987176351576[79] = 0;
   out_1981298987176351576[80] = 0;
   out_1981298987176351576[81] = 0;
   out_1981298987176351576[82] = 0;
   out_1981298987176351576[83] = 0;
   out_1981298987176351576[84] = 0;
   out_1981298987176351576[85] = dt;
   out_1981298987176351576[86] = 0;
   out_1981298987176351576[87] = 0;
   out_1981298987176351576[88] = 0;
   out_1981298987176351576[89] = 0;
   out_1981298987176351576[90] = 0;
   out_1981298987176351576[91] = 0;
   out_1981298987176351576[92] = 0;
   out_1981298987176351576[93] = 0;
   out_1981298987176351576[94] = 0;
   out_1981298987176351576[95] = 1;
   out_1981298987176351576[96] = 0;
   out_1981298987176351576[97] = 0;
   out_1981298987176351576[98] = 0;
   out_1981298987176351576[99] = 0;
   out_1981298987176351576[100] = 0;
   out_1981298987176351576[101] = 0;
   out_1981298987176351576[102] = 0;
   out_1981298987176351576[103] = 0;
   out_1981298987176351576[104] = dt;
   out_1981298987176351576[105] = 0;
   out_1981298987176351576[106] = 0;
   out_1981298987176351576[107] = 0;
   out_1981298987176351576[108] = 0;
   out_1981298987176351576[109] = 0;
   out_1981298987176351576[110] = 0;
   out_1981298987176351576[111] = 0;
   out_1981298987176351576[112] = 0;
   out_1981298987176351576[113] = 0;
   out_1981298987176351576[114] = 1;
   out_1981298987176351576[115] = 0;
   out_1981298987176351576[116] = 0;
   out_1981298987176351576[117] = 0;
   out_1981298987176351576[118] = 0;
   out_1981298987176351576[119] = 0;
   out_1981298987176351576[120] = 0;
   out_1981298987176351576[121] = 0;
   out_1981298987176351576[122] = 0;
   out_1981298987176351576[123] = 0;
   out_1981298987176351576[124] = 0;
   out_1981298987176351576[125] = 0;
   out_1981298987176351576[126] = 0;
   out_1981298987176351576[127] = 0;
   out_1981298987176351576[128] = 0;
   out_1981298987176351576[129] = 0;
   out_1981298987176351576[130] = 0;
   out_1981298987176351576[131] = 0;
   out_1981298987176351576[132] = 0;
   out_1981298987176351576[133] = 1;
   out_1981298987176351576[134] = 0;
   out_1981298987176351576[135] = 0;
   out_1981298987176351576[136] = 0;
   out_1981298987176351576[137] = 0;
   out_1981298987176351576[138] = 0;
   out_1981298987176351576[139] = 0;
   out_1981298987176351576[140] = 0;
   out_1981298987176351576[141] = 0;
   out_1981298987176351576[142] = 0;
   out_1981298987176351576[143] = 0;
   out_1981298987176351576[144] = 0;
   out_1981298987176351576[145] = 0;
   out_1981298987176351576[146] = 0;
   out_1981298987176351576[147] = 0;
   out_1981298987176351576[148] = 0;
   out_1981298987176351576[149] = 0;
   out_1981298987176351576[150] = 0;
   out_1981298987176351576[151] = 0;
   out_1981298987176351576[152] = 1;
   out_1981298987176351576[153] = 0;
   out_1981298987176351576[154] = 0;
   out_1981298987176351576[155] = 0;
   out_1981298987176351576[156] = 0;
   out_1981298987176351576[157] = 0;
   out_1981298987176351576[158] = 0;
   out_1981298987176351576[159] = 0;
   out_1981298987176351576[160] = 0;
   out_1981298987176351576[161] = 0;
   out_1981298987176351576[162] = 0;
   out_1981298987176351576[163] = 0;
   out_1981298987176351576[164] = 0;
   out_1981298987176351576[165] = 0;
   out_1981298987176351576[166] = 0;
   out_1981298987176351576[167] = 0;
   out_1981298987176351576[168] = 0;
   out_1981298987176351576[169] = 0;
   out_1981298987176351576[170] = 0;
   out_1981298987176351576[171] = 1;
   out_1981298987176351576[172] = 0;
   out_1981298987176351576[173] = 0;
   out_1981298987176351576[174] = 0;
   out_1981298987176351576[175] = 0;
   out_1981298987176351576[176] = 0;
   out_1981298987176351576[177] = 0;
   out_1981298987176351576[178] = 0;
   out_1981298987176351576[179] = 0;
   out_1981298987176351576[180] = 0;
   out_1981298987176351576[181] = 0;
   out_1981298987176351576[182] = 0;
   out_1981298987176351576[183] = 0;
   out_1981298987176351576[184] = 0;
   out_1981298987176351576[185] = 0;
   out_1981298987176351576[186] = 0;
   out_1981298987176351576[187] = 0;
   out_1981298987176351576[188] = 0;
   out_1981298987176351576[189] = 0;
   out_1981298987176351576[190] = 1;
   out_1981298987176351576[191] = 0;
   out_1981298987176351576[192] = 0;
   out_1981298987176351576[193] = 0;
   out_1981298987176351576[194] = 0;
   out_1981298987176351576[195] = 0;
   out_1981298987176351576[196] = 0;
   out_1981298987176351576[197] = 0;
   out_1981298987176351576[198] = 0;
   out_1981298987176351576[199] = 0;
   out_1981298987176351576[200] = 0;
   out_1981298987176351576[201] = 0;
   out_1981298987176351576[202] = 0;
   out_1981298987176351576[203] = 0;
   out_1981298987176351576[204] = 0;
   out_1981298987176351576[205] = 0;
   out_1981298987176351576[206] = 0;
   out_1981298987176351576[207] = 0;
   out_1981298987176351576[208] = 0;
   out_1981298987176351576[209] = 1;
   out_1981298987176351576[210] = 0;
   out_1981298987176351576[211] = 0;
   out_1981298987176351576[212] = 0;
   out_1981298987176351576[213] = 0;
   out_1981298987176351576[214] = 0;
   out_1981298987176351576[215] = 0;
   out_1981298987176351576[216] = 0;
   out_1981298987176351576[217] = 0;
   out_1981298987176351576[218] = 0;
   out_1981298987176351576[219] = 0;
   out_1981298987176351576[220] = 0;
   out_1981298987176351576[221] = 0;
   out_1981298987176351576[222] = 0;
   out_1981298987176351576[223] = 0;
   out_1981298987176351576[224] = 0;
   out_1981298987176351576[225] = 0;
   out_1981298987176351576[226] = 0;
   out_1981298987176351576[227] = 0;
   out_1981298987176351576[228] = 1;
   out_1981298987176351576[229] = 0;
   out_1981298987176351576[230] = 0;
   out_1981298987176351576[231] = 0;
   out_1981298987176351576[232] = 0;
   out_1981298987176351576[233] = 0;
   out_1981298987176351576[234] = 0;
   out_1981298987176351576[235] = 0;
   out_1981298987176351576[236] = 0;
   out_1981298987176351576[237] = 0;
   out_1981298987176351576[238] = 0;
   out_1981298987176351576[239] = 0;
   out_1981298987176351576[240] = 0;
   out_1981298987176351576[241] = 0;
   out_1981298987176351576[242] = 0;
   out_1981298987176351576[243] = 0;
   out_1981298987176351576[244] = 0;
   out_1981298987176351576[245] = 0;
   out_1981298987176351576[246] = 0;
   out_1981298987176351576[247] = 1;
   out_1981298987176351576[248] = 0;
   out_1981298987176351576[249] = 0;
   out_1981298987176351576[250] = 0;
   out_1981298987176351576[251] = 0;
   out_1981298987176351576[252] = 0;
   out_1981298987176351576[253] = 0;
   out_1981298987176351576[254] = 0;
   out_1981298987176351576[255] = 0;
   out_1981298987176351576[256] = 0;
   out_1981298987176351576[257] = 0;
   out_1981298987176351576[258] = 0;
   out_1981298987176351576[259] = 0;
   out_1981298987176351576[260] = 0;
   out_1981298987176351576[261] = 0;
   out_1981298987176351576[262] = 0;
   out_1981298987176351576[263] = 0;
   out_1981298987176351576[264] = 0;
   out_1981298987176351576[265] = 0;
   out_1981298987176351576[266] = 1;
   out_1981298987176351576[267] = 0;
   out_1981298987176351576[268] = 0;
   out_1981298987176351576[269] = 0;
   out_1981298987176351576[270] = 0;
   out_1981298987176351576[271] = 0;
   out_1981298987176351576[272] = 0;
   out_1981298987176351576[273] = 0;
   out_1981298987176351576[274] = 0;
   out_1981298987176351576[275] = 0;
   out_1981298987176351576[276] = 0;
   out_1981298987176351576[277] = 0;
   out_1981298987176351576[278] = 0;
   out_1981298987176351576[279] = 0;
   out_1981298987176351576[280] = 0;
   out_1981298987176351576[281] = 0;
   out_1981298987176351576[282] = 0;
   out_1981298987176351576[283] = 0;
   out_1981298987176351576[284] = 0;
   out_1981298987176351576[285] = 1;
   out_1981298987176351576[286] = 0;
   out_1981298987176351576[287] = 0;
   out_1981298987176351576[288] = 0;
   out_1981298987176351576[289] = 0;
   out_1981298987176351576[290] = 0;
   out_1981298987176351576[291] = 0;
   out_1981298987176351576[292] = 0;
   out_1981298987176351576[293] = 0;
   out_1981298987176351576[294] = 0;
   out_1981298987176351576[295] = 0;
   out_1981298987176351576[296] = 0;
   out_1981298987176351576[297] = 0;
   out_1981298987176351576[298] = 0;
   out_1981298987176351576[299] = 0;
   out_1981298987176351576[300] = 0;
   out_1981298987176351576[301] = 0;
   out_1981298987176351576[302] = 0;
   out_1981298987176351576[303] = 0;
   out_1981298987176351576[304] = 1;
   out_1981298987176351576[305] = 0;
   out_1981298987176351576[306] = 0;
   out_1981298987176351576[307] = 0;
   out_1981298987176351576[308] = 0;
   out_1981298987176351576[309] = 0;
   out_1981298987176351576[310] = 0;
   out_1981298987176351576[311] = 0;
   out_1981298987176351576[312] = 0;
   out_1981298987176351576[313] = 0;
   out_1981298987176351576[314] = 0;
   out_1981298987176351576[315] = 0;
   out_1981298987176351576[316] = 0;
   out_1981298987176351576[317] = 0;
   out_1981298987176351576[318] = 0;
   out_1981298987176351576[319] = 0;
   out_1981298987176351576[320] = 0;
   out_1981298987176351576[321] = 0;
   out_1981298987176351576[322] = 0;
   out_1981298987176351576[323] = 1;
}
void h_4(double *state, double *unused, double *out_8862304430070200672) {
   out_8862304430070200672[0] = state[6] + state[9];
   out_8862304430070200672[1] = state[7] + state[10];
   out_8862304430070200672[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_924320946853278226) {
   out_924320946853278226[0] = 0;
   out_924320946853278226[1] = 0;
   out_924320946853278226[2] = 0;
   out_924320946853278226[3] = 0;
   out_924320946853278226[4] = 0;
   out_924320946853278226[5] = 0;
   out_924320946853278226[6] = 1;
   out_924320946853278226[7] = 0;
   out_924320946853278226[8] = 0;
   out_924320946853278226[9] = 1;
   out_924320946853278226[10] = 0;
   out_924320946853278226[11] = 0;
   out_924320946853278226[12] = 0;
   out_924320946853278226[13] = 0;
   out_924320946853278226[14] = 0;
   out_924320946853278226[15] = 0;
   out_924320946853278226[16] = 0;
   out_924320946853278226[17] = 0;
   out_924320946853278226[18] = 0;
   out_924320946853278226[19] = 0;
   out_924320946853278226[20] = 0;
   out_924320946853278226[21] = 0;
   out_924320946853278226[22] = 0;
   out_924320946853278226[23] = 0;
   out_924320946853278226[24] = 0;
   out_924320946853278226[25] = 1;
   out_924320946853278226[26] = 0;
   out_924320946853278226[27] = 0;
   out_924320946853278226[28] = 1;
   out_924320946853278226[29] = 0;
   out_924320946853278226[30] = 0;
   out_924320946853278226[31] = 0;
   out_924320946853278226[32] = 0;
   out_924320946853278226[33] = 0;
   out_924320946853278226[34] = 0;
   out_924320946853278226[35] = 0;
   out_924320946853278226[36] = 0;
   out_924320946853278226[37] = 0;
   out_924320946853278226[38] = 0;
   out_924320946853278226[39] = 0;
   out_924320946853278226[40] = 0;
   out_924320946853278226[41] = 0;
   out_924320946853278226[42] = 0;
   out_924320946853278226[43] = 0;
   out_924320946853278226[44] = 1;
   out_924320946853278226[45] = 0;
   out_924320946853278226[46] = 0;
   out_924320946853278226[47] = 1;
   out_924320946853278226[48] = 0;
   out_924320946853278226[49] = 0;
   out_924320946853278226[50] = 0;
   out_924320946853278226[51] = 0;
   out_924320946853278226[52] = 0;
   out_924320946853278226[53] = 0;
}
void h_10(double *state, double *unused, double *out_2002575884586459867) {
   out_2002575884586459867[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2002575884586459867[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2002575884586459867[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_9217408551414011913) {
   out_9217408551414011913[0] = 0;
   out_9217408551414011913[1] = 9.8100000000000005*cos(state[1]);
   out_9217408551414011913[2] = 0;
   out_9217408551414011913[3] = 0;
   out_9217408551414011913[4] = -state[8];
   out_9217408551414011913[5] = state[7];
   out_9217408551414011913[6] = 0;
   out_9217408551414011913[7] = state[5];
   out_9217408551414011913[8] = -state[4];
   out_9217408551414011913[9] = 0;
   out_9217408551414011913[10] = 0;
   out_9217408551414011913[11] = 0;
   out_9217408551414011913[12] = 1;
   out_9217408551414011913[13] = 0;
   out_9217408551414011913[14] = 0;
   out_9217408551414011913[15] = 1;
   out_9217408551414011913[16] = 0;
   out_9217408551414011913[17] = 0;
   out_9217408551414011913[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_9217408551414011913[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_9217408551414011913[20] = 0;
   out_9217408551414011913[21] = state[8];
   out_9217408551414011913[22] = 0;
   out_9217408551414011913[23] = -state[6];
   out_9217408551414011913[24] = -state[5];
   out_9217408551414011913[25] = 0;
   out_9217408551414011913[26] = state[3];
   out_9217408551414011913[27] = 0;
   out_9217408551414011913[28] = 0;
   out_9217408551414011913[29] = 0;
   out_9217408551414011913[30] = 0;
   out_9217408551414011913[31] = 1;
   out_9217408551414011913[32] = 0;
   out_9217408551414011913[33] = 0;
   out_9217408551414011913[34] = 1;
   out_9217408551414011913[35] = 0;
   out_9217408551414011913[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_9217408551414011913[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_9217408551414011913[38] = 0;
   out_9217408551414011913[39] = -state[7];
   out_9217408551414011913[40] = state[6];
   out_9217408551414011913[41] = 0;
   out_9217408551414011913[42] = state[4];
   out_9217408551414011913[43] = -state[3];
   out_9217408551414011913[44] = 0;
   out_9217408551414011913[45] = 0;
   out_9217408551414011913[46] = 0;
   out_9217408551414011913[47] = 0;
   out_9217408551414011913[48] = 0;
   out_9217408551414011913[49] = 0;
   out_9217408551414011913[50] = 1;
   out_9217408551414011913[51] = 0;
   out_9217408551414011913[52] = 0;
   out_9217408551414011913[53] = 1;
}
void h_13(double *state, double *unused, double *out_942466340260082110) {
   out_942466340260082110[0] = state[3];
   out_942466340260082110[1] = state[4];
   out_942466340260082110[2] = state[5];
}
void H_13(double *state, double *unused, double *out_2909434516449245798) {
   out_2909434516449245798[0] = 0;
   out_2909434516449245798[1] = 0;
   out_2909434516449245798[2] = 0;
   out_2909434516449245798[3] = 1;
   out_2909434516449245798[4] = 0;
   out_2909434516449245798[5] = 0;
   out_2909434516449245798[6] = 0;
   out_2909434516449245798[7] = 0;
   out_2909434516449245798[8] = 0;
   out_2909434516449245798[9] = 0;
   out_2909434516449245798[10] = 0;
   out_2909434516449245798[11] = 0;
   out_2909434516449245798[12] = 0;
   out_2909434516449245798[13] = 0;
   out_2909434516449245798[14] = 0;
   out_2909434516449245798[15] = 0;
   out_2909434516449245798[16] = 0;
   out_2909434516449245798[17] = 0;
   out_2909434516449245798[18] = 0;
   out_2909434516449245798[19] = 0;
   out_2909434516449245798[20] = 0;
   out_2909434516449245798[21] = 0;
   out_2909434516449245798[22] = 1;
   out_2909434516449245798[23] = 0;
   out_2909434516449245798[24] = 0;
   out_2909434516449245798[25] = 0;
   out_2909434516449245798[26] = 0;
   out_2909434516449245798[27] = 0;
   out_2909434516449245798[28] = 0;
   out_2909434516449245798[29] = 0;
   out_2909434516449245798[30] = 0;
   out_2909434516449245798[31] = 0;
   out_2909434516449245798[32] = 0;
   out_2909434516449245798[33] = 0;
   out_2909434516449245798[34] = 0;
   out_2909434516449245798[35] = 0;
   out_2909434516449245798[36] = 0;
   out_2909434516449245798[37] = 0;
   out_2909434516449245798[38] = 0;
   out_2909434516449245798[39] = 0;
   out_2909434516449245798[40] = 0;
   out_2909434516449245798[41] = 1;
   out_2909434516449245798[42] = 0;
   out_2909434516449245798[43] = 0;
   out_2909434516449245798[44] = 0;
   out_2909434516449245798[45] = 0;
   out_2909434516449245798[46] = 0;
   out_2909434516449245798[47] = 0;
   out_2909434516449245798[48] = 0;
   out_2909434516449245798[49] = 0;
   out_2909434516449245798[50] = 0;
   out_2909434516449245798[51] = 0;
   out_2909434516449245798[52] = 0;
   out_2909434516449245798[53] = 0;
}
void h_14(double *state, double *unused, double *out_4372130999795494903) {
   out_4372130999795494903[0] = state[6];
   out_4372130999795494903[1] = state[7];
   out_4372130999795494903[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2158467485442094070) {
   out_2158467485442094070[0] = 0;
   out_2158467485442094070[1] = 0;
   out_2158467485442094070[2] = 0;
   out_2158467485442094070[3] = 0;
   out_2158467485442094070[4] = 0;
   out_2158467485442094070[5] = 0;
   out_2158467485442094070[6] = 1;
   out_2158467485442094070[7] = 0;
   out_2158467485442094070[8] = 0;
   out_2158467485442094070[9] = 0;
   out_2158467485442094070[10] = 0;
   out_2158467485442094070[11] = 0;
   out_2158467485442094070[12] = 0;
   out_2158467485442094070[13] = 0;
   out_2158467485442094070[14] = 0;
   out_2158467485442094070[15] = 0;
   out_2158467485442094070[16] = 0;
   out_2158467485442094070[17] = 0;
   out_2158467485442094070[18] = 0;
   out_2158467485442094070[19] = 0;
   out_2158467485442094070[20] = 0;
   out_2158467485442094070[21] = 0;
   out_2158467485442094070[22] = 0;
   out_2158467485442094070[23] = 0;
   out_2158467485442094070[24] = 0;
   out_2158467485442094070[25] = 1;
   out_2158467485442094070[26] = 0;
   out_2158467485442094070[27] = 0;
   out_2158467485442094070[28] = 0;
   out_2158467485442094070[29] = 0;
   out_2158467485442094070[30] = 0;
   out_2158467485442094070[31] = 0;
   out_2158467485442094070[32] = 0;
   out_2158467485442094070[33] = 0;
   out_2158467485442094070[34] = 0;
   out_2158467485442094070[35] = 0;
   out_2158467485442094070[36] = 0;
   out_2158467485442094070[37] = 0;
   out_2158467485442094070[38] = 0;
   out_2158467485442094070[39] = 0;
   out_2158467485442094070[40] = 0;
   out_2158467485442094070[41] = 0;
   out_2158467485442094070[42] = 0;
   out_2158467485442094070[43] = 0;
   out_2158467485442094070[44] = 1;
   out_2158467485442094070[45] = 0;
   out_2158467485442094070[46] = 0;
   out_2158467485442094070[47] = 0;
   out_2158467485442094070[48] = 0;
   out_2158467485442094070[49] = 0;
   out_2158467485442094070[50] = 0;
   out_2158467485442094070[51] = 0;
   out_2158467485442094070[52] = 0;
   out_2158467485442094070[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2812259937135269323) {
  err_fun(nom_x, delta_x, out_2812259937135269323);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7702513841393801961) {
  inv_err_fun(nom_x, true_x, out_7702513841393801961);
}
void pose_H_mod_fun(double *state, double *out_8578061460798452048) {
  H_mod_fun(state, out_8578061460798452048);
}
void pose_f_fun(double *state, double dt, double *out_6960018690742428862) {
  f_fun(state,  dt, out_6960018690742428862);
}
void pose_F_fun(double *state, double dt, double *out_1981298987176351576) {
  F_fun(state,  dt, out_1981298987176351576);
}
void pose_h_4(double *state, double *unused, double *out_8862304430070200672) {
  h_4(state, unused, out_8862304430070200672);
}
void pose_H_4(double *state, double *unused, double *out_924320946853278226) {
  H_4(state, unused, out_924320946853278226);
}
void pose_h_10(double *state, double *unused, double *out_2002575884586459867) {
  h_10(state, unused, out_2002575884586459867);
}
void pose_H_10(double *state, double *unused, double *out_9217408551414011913) {
  H_10(state, unused, out_9217408551414011913);
}
void pose_h_13(double *state, double *unused, double *out_942466340260082110) {
  h_13(state, unused, out_942466340260082110);
}
void pose_H_13(double *state, double *unused, double *out_2909434516449245798) {
  H_13(state, unused, out_2909434516449245798);
}
void pose_h_14(double *state, double *unused, double *out_4372130999795494903) {
  h_14(state, unused, out_4372130999795494903);
}
void pose_H_14(double *state, double *unused, double *out_2158467485442094070) {
  H_14(state, unused, out_2158467485442094070);
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
