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
void err_fun(double *nom_x, double *delta_x, double *out_4955557050137928755) {
   out_4955557050137928755[0] = delta_x[0] + nom_x[0];
   out_4955557050137928755[1] = delta_x[1] + nom_x[1];
   out_4955557050137928755[2] = delta_x[2] + nom_x[2];
   out_4955557050137928755[3] = delta_x[3] + nom_x[3];
   out_4955557050137928755[4] = delta_x[4] + nom_x[4];
   out_4955557050137928755[5] = delta_x[5] + nom_x[5];
   out_4955557050137928755[6] = delta_x[6] + nom_x[6];
   out_4955557050137928755[7] = delta_x[7] + nom_x[7];
   out_4955557050137928755[8] = delta_x[8] + nom_x[8];
   out_4955557050137928755[9] = delta_x[9] + nom_x[9];
   out_4955557050137928755[10] = delta_x[10] + nom_x[10];
   out_4955557050137928755[11] = delta_x[11] + nom_x[11];
   out_4955557050137928755[12] = delta_x[12] + nom_x[12];
   out_4955557050137928755[13] = delta_x[13] + nom_x[13];
   out_4955557050137928755[14] = delta_x[14] + nom_x[14];
   out_4955557050137928755[15] = delta_x[15] + nom_x[15];
   out_4955557050137928755[16] = delta_x[16] + nom_x[16];
   out_4955557050137928755[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3781966990875519323) {
   out_3781966990875519323[0] = -nom_x[0] + true_x[0];
   out_3781966990875519323[1] = -nom_x[1] + true_x[1];
   out_3781966990875519323[2] = -nom_x[2] + true_x[2];
   out_3781966990875519323[3] = -nom_x[3] + true_x[3];
   out_3781966990875519323[4] = -nom_x[4] + true_x[4];
   out_3781966990875519323[5] = -nom_x[5] + true_x[5];
   out_3781966990875519323[6] = -nom_x[6] + true_x[6];
   out_3781966990875519323[7] = -nom_x[7] + true_x[7];
   out_3781966990875519323[8] = -nom_x[8] + true_x[8];
   out_3781966990875519323[9] = -nom_x[9] + true_x[9];
   out_3781966990875519323[10] = -nom_x[10] + true_x[10];
   out_3781966990875519323[11] = -nom_x[11] + true_x[11];
   out_3781966990875519323[12] = -nom_x[12] + true_x[12];
   out_3781966990875519323[13] = -nom_x[13] + true_x[13];
   out_3781966990875519323[14] = -nom_x[14] + true_x[14];
   out_3781966990875519323[15] = -nom_x[15] + true_x[15];
   out_3781966990875519323[16] = -nom_x[16] + true_x[16];
   out_3781966990875519323[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4552347542280292738) {
   out_4552347542280292738[0] = 1.0;
   out_4552347542280292738[1] = 0.0;
   out_4552347542280292738[2] = 0.0;
   out_4552347542280292738[3] = 0.0;
   out_4552347542280292738[4] = 0.0;
   out_4552347542280292738[5] = 0.0;
   out_4552347542280292738[6] = 0.0;
   out_4552347542280292738[7] = 0.0;
   out_4552347542280292738[8] = 0.0;
   out_4552347542280292738[9] = 0.0;
   out_4552347542280292738[10] = 0.0;
   out_4552347542280292738[11] = 0.0;
   out_4552347542280292738[12] = 0.0;
   out_4552347542280292738[13] = 0.0;
   out_4552347542280292738[14] = 0.0;
   out_4552347542280292738[15] = 0.0;
   out_4552347542280292738[16] = 0.0;
   out_4552347542280292738[17] = 0.0;
   out_4552347542280292738[18] = 0.0;
   out_4552347542280292738[19] = 1.0;
   out_4552347542280292738[20] = 0.0;
   out_4552347542280292738[21] = 0.0;
   out_4552347542280292738[22] = 0.0;
   out_4552347542280292738[23] = 0.0;
   out_4552347542280292738[24] = 0.0;
   out_4552347542280292738[25] = 0.0;
   out_4552347542280292738[26] = 0.0;
   out_4552347542280292738[27] = 0.0;
   out_4552347542280292738[28] = 0.0;
   out_4552347542280292738[29] = 0.0;
   out_4552347542280292738[30] = 0.0;
   out_4552347542280292738[31] = 0.0;
   out_4552347542280292738[32] = 0.0;
   out_4552347542280292738[33] = 0.0;
   out_4552347542280292738[34] = 0.0;
   out_4552347542280292738[35] = 0.0;
   out_4552347542280292738[36] = 0.0;
   out_4552347542280292738[37] = 0.0;
   out_4552347542280292738[38] = 1.0;
   out_4552347542280292738[39] = 0.0;
   out_4552347542280292738[40] = 0.0;
   out_4552347542280292738[41] = 0.0;
   out_4552347542280292738[42] = 0.0;
   out_4552347542280292738[43] = 0.0;
   out_4552347542280292738[44] = 0.0;
   out_4552347542280292738[45] = 0.0;
   out_4552347542280292738[46] = 0.0;
   out_4552347542280292738[47] = 0.0;
   out_4552347542280292738[48] = 0.0;
   out_4552347542280292738[49] = 0.0;
   out_4552347542280292738[50] = 0.0;
   out_4552347542280292738[51] = 0.0;
   out_4552347542280292738[52] = 0.0;
   out_4552347542280292738[53] = 0.0;
   out_4552347542280292738[54] = 0.0;
   out_4552347542280292738[55] = 0.0;
   out_4552347542280292738[56] = 0.0;
   out_4552347542280292738[57] = 1.0;
   out_4552347542280292738[58] = 0.0;
   out_4552347542280292738[59] = 0.0;
   out_4552347542280292738[60] = 0.0;
   out_4552347542280292738[61] = 0.0;
   out_4552347542280292738[62] = 0.0;
   out_4552347542280292738[63] = 0.0;
   out_4552347542280292738[64] = 0.0;
   out_4552347542280292738[65] = 0.0;
   out_4552347542280292738[66] = 0.0;
   out_4552347542280292738[67] = 0.0;
   out_4552347542280292738[68] = 0.0;
   out_4552347542280292738[69] = 0.0;
   out_4552347542280292738[70] = 0.0;
   out_4552347542280292738[71] = 0.0;
   out_4552347542280292738[72] = 0.0;
   out_4552347542280292738[73] = 0.0;
   out_4552347542280292738[74] = 0.0;
   out_4552347542280292738[75] = 0.0;
   out_4552347542280292738[76] = 1.0;
   out_4552347542280292738[77] = 0.0;
   out_4552347542280292738[78] = 0.0;
   out_4552347542280292738[79] = 0.0;
   out_4552347542280292738[80] = 0.0;
   out_4552347542280292738[81] = 0.0;
   out_4552347542280292738[82] = 0.0;
   out_4552347542280292738[83] = 0.0;
   out_4552347542280292738[84] = 0.0;
   out_4552347542280292738[85] = 0.0;
   out_4552347542280292738[86] = 0.0;
   out_4552347542280292738[87] = 0.0;
   out_4552347542280292738[88] = 0.0;
   out_4552347542280292738[89] = 0.0;
   out_4552347542280292738[90] = 0.0;
   out_4552347542280292738[91] = 0.0;
   out_4552347542280292738[92] = 0.0;
   out_4552347542280292738[93] = 0.0;
   out_4552347542280292738[94] = 0.0;
   out_4552347542280292738[95] = 1.0;
   out_4552347542280292738[96] = 0.0;
   out_4552347542280292738[97] = 0.0;
   out_4552347542280292738[98] = 0.0;
   out_4552347542280292738[99] = 0.0;
   out_4552347542280292738[100] = 0.0;
   out_4552347542280292738[101] = 0.0;
   out_4552347542280292738[102] = 0.0;
   out_4552347542280292738[103] = 0.0;
   out_4552347542280292738[104] = 0.0;
   out_4552347542280292738[105] = 0.0;
   out_4552347542280292738[106] = 0.0;
   out_4552347542280292738[107] = 0.0;
   out_4552347542280292738[108] = 0.0;
   out_4552347542280292738[109] = 0.0;
   out_4552347542280292738[110] = 0.0;
   out_4552347542280292738[111] = 0.0;
   out_4552347542280292738[112] = 0.0;
   out_4552347542280292738[113] = 0.0;
   out_4552347542280292738[114] = 1.0;
   out_4552347542280292738[115] = 0.0;
   out_4552347542280292738[116] = 0.0;
   out_4552347542280292738[117] = 0.0;
   out_4552347542280292738[118] = 0.0;
   out_4552347542280292738[119] = 0.0;
   out_4552347542280292738[120] = 0.0;
   out_4552347542280292738[121] = 0.0;
   out_4552347542280292738[122] = 0.0;
   out_4552347542280292738[123] = 0.0;
   out_4552347542280292738[124] = 0.0;
   out_4552347542280292738[125] = 0.0;
   out_4552347542280292738[126] = 0.0;
   out_4552347542280292738[127] = 0.0;
   out_4552347542280292738[128] = 0.0;
   out_4552347542280292738[129] = 0.0;
   out_4552347542280292738[130] = 0.0;
   out_4552347542280292738[131] = 0.0;
   out_4552347542280292738[132] = 0.0;
   out_4552347542280292738[133] = 1.0;
   out_4552347542280292738[134] = 0.0;
   out_4552347542280292738[135] = 0.0;
   out_4552347542280292738[136] = 0.0;
   out_4552347542280292738[137] = 0.0;
   out_4552347542280292738[138] = 0.0;
   out_4552347542280292738[139] = 0.0;
   out_4552347542280292738[140] = 0.0;
   out_4552347542280292738[141] = 0.0;
   out_4552347542280292738[142] = 0.0;
   out_4552347542280292738[143] = 0.0;
   out_4552347542280292738[144] = 0.0;
   out_4552347542280292738[145] = 0.0;
   out_4552347542280292738[146] = 0.0;
   out_4552347542280292738[147] = 0.0;
   out_4552347542280292738[148] = 0.0;
   out_4552347542280292738[149] = 0.0;
   out_4552347542280292738[150] = 0.0;
   out_4552347542280292738[151] = 0.0;
   out_4552347542280292738[152] = 1.0;
   out_4552347542280292738[153] = 0.0;
   out_4552347542280292738[154] = 0.0;
   out_4552347542280292738[155] = 0.0;
   out_4552347542280292738[156] = 0.0;
   out_4552347542280292738[157] = 0.0;
   out_4552347542280292738[158] = 0.0;
   out_4552347542280292738[159] = 0.0;
   out_4552347542280292738[160] = 0.0;
   out_4552347542280292738[161] = 0.0;
   out_4552347542280292738[162] = 0.0;
   out_4552347542280292738[163] = 0.0;
   out_4552347542280292738[164] = 0.0;
   out_4552347542280292738[165] = 0.0;
   out_4552347542280292738[166] = 0.0;
   out_4552347542280292738[167] = 0.0;
   out_4552347542280292738[168] = 0.0;
   out_4552347542280292738[169] = 0.0;
   out_4552347542280292738[170] = 0.0;
   out_4552347542280292738[171] = 1.0;
   out_4552347542280292738[172] = 0.0;
   out_4552347542280292738[173] = 0.0;
   out_4552347542280292738[174] = 0.0;
   out_4552347542280292738[175] = 0.0;
   out_4552347542280292738[176] = 0.0;
   out_4552347542280292738[177] = 0.0;
   out_4552347542280292738[178] = 0.0;
   out_4552347542280292738[179] = 0.0;
   out_4552347542280292738[180] = 0.0;
   out_4552347542280292738[181] = 0.0;
   out_4552347542280292738[182] = 0.0;
   out_4552347542280292738[183] = 0.0;
   out_4552347542280292738[184] = 0.0;
   out_4552347542280292738[185] = 0.0;
   out_4552347542280292738[186] = 0.0;
   out_4552347542280292738[187] = 0.0;
   out_4552347542280292738[188] = 0.0;
   out_4552347542280292738[189] = 0.0;
   out_4552347542280292738[190] = 1.0;
   out_4552347542280292738[191] = 0.0;
   out_4552347542280292738[192] = 0.0;
   out_4552347542280292738[193] = 0.0;
   out_4552347542280292738[194] = 0.0;
   out_4552347542280292738[195] = 0.0;
   out_4552347542280292738[196] = 0.0;
   out_4552347542280292738[197] = 0.0;
   out_4552347542280292738[198] = 0.0;
   out_4552347542280292738[199] = 0.0;
   out_4552347542280292738[200] = 0.0;
   out_4552347542280292738[201] = 0.0;
   out_4552347542280292738[202] = 0.0;
   out_4552347542280292738[203] = 0.0;
   out_4552347542280292738[204] = 0.0;
   out_4552347542280292738[205] = 0.0;
   out_4552347542280292738[206] = 0.0;
   out_4552347542280292738[207] = 0.0;
   out_4552347542280292738[208] = 0.0;
   out_4552347542280292738[209] = 1.0;
   out_4552347542280292738[210] = 0.0;
   out_4552347542280292738[211] = 0.0;
   out_4552347542280292738[212] = 0.0;
   out_4552347542280292738[213] = 0.0;
   out_4552347542280292738[214] = 0.0;
   out_4552347542280292738[215] = 0.0;
   out_4552347542280292738[216] = 0.0;
   out_4552347542280292738[217] = 0.0;
   out_4552347542280292738[218] = 0.0;
   out_4552347542280292738[219] = 0.0;
   out_4552347542280292738[220] = 0.0;
   out_4552347542280292738[221] = 0.0;
   out_4552347542280292738[222] = 0.0;
   out_4552347542280292738[223] = 0.0;
   out_4552347542280292738[224] = 0.0;
   out_4552347542280292738[225] = 0.0;
   out_4552347542280292738[226] = 0.0;
   out_4552347542280292738[227] = 0.0;
   out_4552347542280292738[228] = 1.0;
   out_4552347542280292738[229] = 0.0;
   out_4552347542280292738[230] = 0.0;
   out_4552347542280292738[231] = 0.0;
   out_4552347542280292738[232] = 0.0;
   out_4552347542280292738[233] = 0.0;
   out_4552347542280292738[234] = 0.0;
   out_4552347542280292738[235] = 0.0;
   out_4552347542280292738[236] = 0.0;
   out_4552347542280292738[237] = 0.0;
   out_4552347542280292738[238] = 0.0;
   out_4552347542280292738[239] = 0.0;
   out_4552347542280292738[240] = 0.0;
   out_4552347542280292738[241] = 0.0;
   out_4552347542280292738[242] = 0.0;
   out_4552347542280292738[243] = 0.0;
   out_4552347542280292738[244] = 0.0;
   out_4552347542280292738[245] = 0.0;
   out_4552347542280292738[246] = 0.0;
   out_4552347542280292738[247] = 1.0;
   out_4552347542280292738[248] = 0.0;
   out_4552347542280292738[249] = 0.0;
   out_4552347542280292738[250] = 0.0;
   out_4552347542280292738[251] = 0.0;
   out_4552347542280292738[252] = 0.0;
   out_4552347542280292738[253] = 0.0;
   out_4552347542280292738[254] = 0.0;
   out_4552347542280292738[255] = 0.0;
   out_4552347542280292738[256] = 0.0;
   out_4552347542280292738[257] = 0.0;
   out_4552347542280292738[258] = 0.0;
   out_4552347542280292738[259] = 0.0;
   out_4552347542280292738[260] = 0.0;
   out_4552347542280292738[261] = 0.0;
   out_4552347542280292738[262] = 0.0;
   out_4552347542280292738[263] = 0.0;
   out_4552347542280292738[264] = 0.0;
   out_4552347542280292738[265] = 0.0;
   out_4552347542280292738[266] = 1.0;
   out_4552347542280292738[267] = 0.0;
   out_4552347542280292738[268] = 0.0;
   out_4552347542280292738[269] = 0.0;
   out_4552347542280292738[270] = 0.0;
   out_4552347542280292738[271] = 0.0;
   out_4552347542280292738[272] = 0.0;
   out_4552347542280292738[273] = 0.0;
   out_4552347542280292738[274] = 0.0;
   out_4552347542280292738[275] = 0.0;
   out_4552347542280292738[276] = 0.0;
   out_4552347542280292738[277] = 0.0;
   out_4552347542280292738[278] = 0.0;
   out_4552347542280292738[279] = 0.0;
   out_4552347542280292738[280] = 0.0;
   out_4552347542280292738[281] = 0.0;
   out_4552347542280292738[282] = 0.0;
   out_4552347542280292738[283] = 0.0;
   out_4552347542280292738[284] = 0.0;
   out_4552347542280292738[285] = 1.0;
   out_4552347542280292738[286] = 0.0;
   out_4552347542280292738[287] = 0.0;
   out_4552347542280292738[288] = 0.0;
   out_4552347542280292738[289] = 0.0;
   out_4552347542280292738[290] = 0.0;
   out_4552347542280292738[291] = 0.0;
   out_4552347542280292738[292] = 0.0;
   out_4552347542280292738[293] = 0.0;
   out_4552347542280292738[294] = 0.0;
   out_4552347542280292738[295] = 0.0;
   out_4552347542280292738[296] = 0.0;
   out_4552347542280292738[297] = 0.0;
   out_4552347542280292738[298] = 0.0;
   out_4552347542280292738[299] = 0.0;
   out_4552347542280292738[300] = 0.0;
   out_4552347542280292738[301] = 0.0;
   out_4552347542280292738[302] = 0.0;
   out_4552347542280292738[303] = 0.0;
   out_4552347542280292738[304] = 1.0;
   out_4552347542280292738[305] = 0.0;
   out_4552347542280292738[306] = 0.0;
   out_4552347542280292738[307] = 0.0;
   out_4552347542280292738[308] = 0.0;
   out_4552347542280292738[309] = 0.0;
   out_4552347542280292738[310] = 0.0;
   out_4552347542280292738[311] = 0.0;
   out_4552347542280292738[312] = 0.0;
   out_4552347542280292738[313] = 0.0;
   out_4552347542280292738[314] = 0.0;
   out_4552347542280292738[315] = 0.0;
   out_4552347542280292738[316] = 0.0;
   out_4552347542280292738[317] = 0.0;
   out_4552347542280292738[318] = 0.0;
   out_4552347542280292738[319] = 0.0;
   out_4552347542280292738[320] = 0.0;
   out_4552347542280292738[321] = 0.0;
   out_4552347542280292738[322] = 0.0;
   out_4552347542280292738[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_2861912360858063342) {
   out_2861912360858063342[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_2861912360858063342[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_2861912360858063342[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_2861912360858063342[3] = dt*state[12] + state[3];
   out_2861912360858063342[4] = dt*state[13] + state[4];
   out_2861912360858063342[5] = dt*state[14] + state[5];
   out_2861912360858063342[6] = state[6];
   out_2861912360858063342[7] = state[7];
   out_2861912360858063342[8] = state[8];
   out_2861912360858063342[9] = state[9];
   out_2861912360858063342[10] = state[10];
   out_2861912360858063342[11] = state[11];
   out_2861912360858063342[12] = state[12];
   out_2861912360858063342[13] = state[13];
   out_2861912360858063342[14] = state[14];
   out_2861912360858063342[15] = state[15];
   out_2861912360858063342[16] = state[16];
   out_2861912360858063342[17] = state[17];
}
void F_fun(double *state, double dt, double *out_8378829832132664204) {
   out_8378829832132664204[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8378829832132664204[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8378829832132664204[2] = 0;
   out_8378829832132664204[3] = 0;
   out_8378829832132664204[4] = 0;
   out_8378829832132664204[5] = 0;
   out_8378829832132664204[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8378829832132664204[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8378829832132664204[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8378829832132664204[9] = 0;
   out_8378829832132664204[10] = 0;
   out_8378829832132664204[11] = 0;
   out_8378829832132664204[12] = 0;
   out_8378829832132664204[13] = 0;
   out_8378829832132664204[14] = 0;
   out_8378829832132664204[15] = 0;
   out_8378829832132664204[16] = 0;
   out_8378829832132664204[17] = 0;
   out_8378829832132664204[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8378829832132664204[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8378829832132664204[20] = 0;
   out_8378829832132664204[21] = 0;
   out_8378829832132664204[22] = 0;
   out_8378829832132664204[23] = 0;
   out_8378829832132664204[24] = 0;
   out_8378829832132664204[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8378829832132664204[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8378829832132664204[27] = 0;
   out_8378829832132664204[28] = 0;
   out_8378829832132664204[29] = 0;
   out_8378829832132664204[30] = 0;
   out_8378829832132664204[31] = 0;
   out_8378829832132664204[32] = 0;
   out_8378829832132664204[33] = 0;
   out_8378829832132664204[34] = 0;
   out_8378829832132664204[35] = 0;
   out_8378829832132664204[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8378829832132664204[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8378829832132664204[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8378829832132664204[39] = 0;
   out_8378829832132664204[40] = 0;
   out_8378829832132664204[41] = 0;
   out_8378829832132664204[42] = 0;
   out_8378829832132664204[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8378829832132664204[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8378829832132664204[45] = 0;
   out_8378829832132664204[46] = 0;
   out_8378829832132664204[47] = 0;
   out_8378829832132664204[48] = 0;
   out_8378829832132664204[49] = 0;
   out_8378829832132664204[50] = 0;
   out_8378829832132664204[51] = 0;
   out_8378829832132664204[52] = 0;
   out_8378829832132664204[53] = 0;
   out_8378829832132664204[54] = 0;
   out_8378829832132664204[55] = 0;
   out_8378829832132664204[56] = 0;
   out_8378829832132664204[57] = 1;
   out_8378829832132664204[58] = 0;
   out_8378829832132664204[59] = 0;
   out_8378829832132664204[60] = 0;
   out_8378829832132664204[61] = 0;
   out_8378829832132664204[62] = 0;
   out_8378829832132664204[63] = 0;
   out_8378829832132664204[64] = 0;
   out_8378829832132664204[65] = 0;
   out_8378829832132664204[66] = dt;
   out_8378829832132664204[67] = 0;
   out_8378829832132664204[68] = 0;
   out_8378829832132664204[69] = 0;
   out_8378829832132664204[70] = 0;
   out_8378829832132664204[71] = 0;
   out_8378829832132664204[72] = 0;
   out_8378829832132664204[73] = 0;
   out_8378829832132664204[74] = 0;
   out_8378829832132664204[75] = 0;
   out_8378829832132664204[76] = 1;
   out_8378829832132664204[77] = 0;
   out_8378829832132664204[78] = 0;
   out_8378829832132664204[79] = 0;
   out_8378829832132664204[80] = 0;
   out_8378829832132664204[81] = 0;
   out_8378829832132664204[82] = 0;
   out_8378829832132664204[83] = 0;
   out_8378829832132664204[84] = 0;
   out_8378829832132664204[85] = dt;
   out_8378829832132664204[86] = 0;
   out_8378829832132664204[87] = 0;
   out_8378829832132664204[88] = 0;
   out_8378829832132664204[89] = 0;
   out_8378829832132664204[90] = 0;
   out_8378829832132664204[91] = 0;
   out_8378829832132664204[92] = 0;
   out_8378829832132664204[93] = 0;
   out_8378829832132664204[94] = 0;
   out_8378829832132664204[95] = 1;
   out_8378829832132664204[96] = 0;
   out_8378829832132664204[97] = 0;
   out_8378829832132664204[98] = 0;
   out_8378829832132664204[99] = 0;
   out_8378829832132664204[100] = 0;
   out_8378829832132664204[101] = 0;
   out_8378829832132664204[102] = 0;
   out_8378829832132664204[103] = 0;
   out_8378829832132664204[104] = dt;
   out_8378829832132664204[105] = 0;
   out_8378829832132664204[106] = 0;
   out_8378829832132664204[107] = 0;
   out_8378829832132664204[108] = 0;
   out_8378829832132664204[109] = 0;
   out_8378829832132664204[110] = 0;
   out_8378829832132664204[111] = 0;
   out_8378829832132664204[112] = 0;
   out_8378829832132664204[113] = 0;
   out_8378829832132664204[114] = 1;
   out_8378829832132664204[115] = 0;
   out_8378829832132664204[116] = 0;
   out_8378829832132664204[117] = 0;
   out_8378829832132664204[118] = 0;
   out_8378829832132664204[119] = 0;
   out_8378829832132664204[120] = 0;
   out_8378829832132664204[121] = 0;
   out_8378829832132664204[122] = 0;
   out_8378829832132664204[123] = 0;
   out_8378829832132664204[124] = 0;
   out_8378829832132664204[125] = 0;
   out_8378829832132664204[126] = 0;
   out_8378829832132664204[127] = 0;
   out_8378829832132664204[128] = 0;
   out_8378829832132664204[129] = 0;
   out_8378829832132664204[130] = 0;
   out_8378829832132664204[131] = 0;
   out_8378829832132664204[132] = 0;
   out_8378829832132664204[133] = 1;
   out_8378829832132664204[134] = 0;
   out_8378829832132664204[135] = 0;
   out_8378829832132664204[136] = 0;
   out_8378829832132664204[137] = 0;
   out_8378829832132664204[138] = 0;
   out_8378829832132664204[139] = 0;
   out_8378829832132664204[140] = 0;
   out_8378829832132664204[141] = 0;
   out_8378829832132664204[142] = 0;
   out_8378829832132664204[143] = 0;
   out_8378829832132664204[144] = 0;
   out_8378829832132664204[145] = 0;
   out_8378829832132664204[146] = 0;
   out_8378829832132664204[147] = 0;
   out_8378829832132664204[148] = 0;
   out_8378829832132664204[149] = 0;
   out_8378829832132664204[150] = 0;
   out_8378829832132664204[151] = 0;
   out_8378829832132664204[152] = 1;
   out_8378829832132664204[153] = 0;
   out_8378829832132664204[154] = 0;
   out_8378829832132664204[155] = 0;
   out_8378829832132664204[156] = 0;
   out_8378829832132664204[157] = 0;
   out_8378829832132664204[158] = 0;
   out_8378829832132664204[159] = 0;
   out_8378829832132664204[160] = 0;
   out_8378829832132664204[161] = 0;
   out_8378829832132664204[162] = 0;
   out_8378829832132664204[163] = 0;
   out_8378829832132664204[164] = 0;
   out_8378829832132664204[165] = 0;
   out_8378829832132664204[166] = 0;
   out_8378829832132664204[167] = 0;
   out_8378829832132664204[168] = 0;
   out_8378829832132664204[169] = 0;
   out_8378829832132664204[170] = 0;
   out_8378829832132664204[171] = 1;
   out_8378829832132664204[172] = 0;
   out_8378829832132664204[173] = 0;
   out_8378829832132664204[174] = 0;
   out_8378829832132664204[175] = 0;
   out_8378829832132664204[176] = 0;
   out_8378829832132664204[177] = 0;
   out_8378829832132664204[178] = 0;
   out_8378829832132664204[179] = 0;
   out_8378829832132664204[180] = 0;
   out_8378829832132664204[181] = 0;
   out_8378829832132664204[182] = 0;
   out_8378829832132664204[183] = 0;
   out_8378829832132664204[184] = 0;
   out_8378829832132664204[185] = 0;
   out_8378829832132664204[186] = 0;
   out_8378829832132664204[187] = 0;
   out_8378829832132664204[188] = 0;
   out_8378829832132664204[189] = 0;
   out_8378829832132664204[190] = 1;
   out_8378829832132664204[191] = 0;
   out_8378829832132664204[192] = 0;
   out_8378829832132664204[193] = 0;
   out_8378829832132664204[194] = 0;
   out_8378829832132664204[195] = 0;
   out_8378829832132664204[196] = 0;
   out_8378829832132664204[197] = 0;
   out_8378829832132664204[198] = 0;
   out_8378829832132664204[199] = 0;
   out_8378829832132664204[200] = 0;
   out_8378829832132664204[201] = 0;
   out_8378829832132664204[202] = 0;
   out_8378829832132664204[203] = 0;
   out_8378829832132664204[204] = 0;
   out_8378829832132664204[205] = 0;
   out_8378829832132664204[206] = 0;
   out_8378829832132664204[207] = 0;
   out_8378829832132664204[208] = 0;
   out_8378829832132664204[209] = 1;
   out_8378829832132664204[210] = 0;
   out_8378829832132664204[211] = 0;
   out_8378829832132664204[212] = 0;
   out_8378829832132664204[213] = 0;
   out_8378829832132664204[214] = 0;
   out_8378829832132664204[215] = 0;
   out_8378829832132664204[216] = 0;
   out_8378829832132664204[217] = 0;
   out_8378829832132664204[218] = 0;
   out_8378829832132664204[219] = 0;
   out_8378829832132664204[220] = 0;
   out_8378829832132664204[221] = 0;
   out_8378829832132664204[222] = 0;
   out_8378829832132664204[223] = 0;
   out_8378829832132664204[224] = 0;
   out_8378829832132664204[225] = 0;
   out_8378829832132664204[226] = 0;
   out_8378829832132664204[227] = 0;
   out_8378829832132664204[228] = 1;
   out_8378829832132664204[229] = 0;
   out_8378829832132664204[230] = 0;
   out_8378829832132664204[231] = 0;
   out_8378829832132664204[232] = 0;
   out_8378829832132664204[233] = 0;
   out_8378829832132664204[234] = 0;
   out_8378829832132664204[235] = 0;
   out_8378829832132664204[236] = 0;
   out_8378829832132664204[237] = 0;
   out_8378829832132664204[238] = 0;
   out_8378829832132664204[239] = 0;
   out_8378829832132664204[240] = 0;
   out_8378829832132664204[241] = 0;
   out_8378829832132664204[242] = 0;
   out_8378829832132664204[243] = 0;
   out_8378829832132664204[244] = 0;
   out_8378829832132664204[245] = 0;
   out_8378829832132664204[246] = 0;
   out_8378829832132664204[247] = 1;
   out_8378829832132664204[248] = 0;
   out_8378829832132664204[249] = 0;
   out_8378829832132664204[250] = 0;
   out_8378829832132664204[251] = 0;
   out_8378829832132664204[252] = 0;
   out_8378829832132664204[253] = 0;
   out_8378829832132664204[254] = 0;
   out_8378829832132664204[255] = 0;
   out_8378829832132664204[256] = 0;
   out_8378829832132664204[257] = 0;
   out_8378829832132664204[258] = 0;
   out_8378829832132664204[259] = 0;
   out_8378829832132664204[260] = 0;
   out_8378829832132664204[261] = 0;
   out_8378829832132664204[262] = 0;
   out_8378829832132664204[263] = 0;
   out_8378829832132664204[264] = 0;
   out_8378829832132664204[265] = 0;
   out_8378829832132664204[266] = 1;
   out_8378829832132664204[267] = 0;
   out_8378829832132664204[268] = 0;
   out_8378829832132664204[269] = 0;
   out_8378829832132664204[270] = 0;
   out_8378829832132664204[271] = 0;
   out_8378829832132664204[272] = 0;
   out_8378829832132664204[273] = 0;
   out_8378829832132664204[274] = 0;
   out_8378829832132664204[275] = 0;
   out_8378829832132664204[276] = 0;
   out_8378829832132664204[277] = 0;
   out_8378829832132664204[278] = 0;
   out_8378829832132664204[279] = 0;
   out_8378829832132664204[280] = 0;
   out_8378829832132664204[281] = 0;
   out_8378829832132664204[282] = 0;
   out_8378829832132664204[283] = 0;
   out_8378829832132664204[284] = 0;
   out_8378829832132664204[285] = 1;
   out_8378829832132664204[286] = 0;
   out_8378829832132664204[287] = 0;
   out_8378829832132664204[288] = 0;
   out_8378829832132664204[289] = 0;
   out_8378829832132664204[290] = 0;
   out_8378829832132664204[291] = 0;
   out_8378829832132664204[292] = 0;
   out_8378829832132664204[293] = 0;
   out_8378829832132664204[294] = 0;
   out_8378829832132664204[295] = 0;
   out_8378829832132664204[296] = 0;
   out_8378829832132664204[297] = 0;
   out_8378829832132664204[298] = 0;
   out_8378829832132664204[299] = 0;
   out_8378829832132664204[300] = 0;
   out_8378829832132664204[301] = 0;
   out_8378829832132664204[302] = 0;
   out_8378829832132664204[303] = 0;
   out_8378829832132664204[304] = 1;
   out_8378829832132664204[305] = 0;
   out_8378829832132664204[306] = 0;
   out_8378829832132664204[307] = 0;
   out_8378829832132664204[308] = 0;
   out_8378829832132664204[309] = 0;
   out_8378829832132664204[310] = 0;
   out_8378829832132664204[311] = 0;
   out_8378829832132664204[312] = 0;
   out_8378829832132664204[313] = 0;
   out_8378829832132664204[314] = 0;
   out_8378829832132664204[315] = 0;
   out_8378829832132664204[316] = 0;
   out_8378829832132664204[317] = 0;
   out_8378829832132664204[318] = 0;
   out_8378829832132664204[319] = 0;
   out_8378829832132664204[320] = 0;
   out_8378829832132664204[321] = 0;
   out_8378829832132664204[322] = 0;
   out_8378829832132664204[323] = 1;
}
void h_4(double *state, double *unused, double *out_7327194707407096308) {
   out_7327194707407096308[0] = state[6] + state[9];
   out_7327194707407096308[1] = state[7] + state[10];
   out_7327194707407096308[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_4185848689802696400) {
   out_4185848689802696400[0] = 0;
   out_4185848689802696400[1] = 0;
   out_4185848689802696400[2] = 0;
   out_4185848689802696400[3] = 0;
   out_4185848689802696400[4] = 0;
   out_4185848689802696400[5] = 0;
   out_4185848689802696400[6] = 1;
   out_4185848689802696400[7] = 0;
   out_4185848689802696400[8] = 0;
   out_4185848689802696400[9] = 1;
   out_4185848689802696400[10] = 0;
   out_4185848689802696400[11] = 0;
   out_4185848689802696400[12] = 0;
   out_4185848689802696400[13] = 0;
   out_4185848689802696400[14] = 0;
   out_4185848689802696400[15] = 0;
   out_4185848689802696400[16] = 0;
   out_4185848689802696400[17] = 0;
   out_4185848689802696400[18] = 0;
   out_4185848689802696400[19] = 0;
   out_4185848689802696400[20] = 0;
   out_4185848689802696400[21] = 0;
   out_4185848689802696400[22] = 0;
   out_4185848689802696400[23] = 0;
   out_4185848689802696400[24] = 0;
   out_4185848689802696400[25] = 1;
   out_4185848689802696400[26] = 0;
   out_4185848689802696400[27] = 0;
   out_4185848689802696400[28] = 1;
   out_4185848689802696400[29] = 0;
   out_4185848689802696400[30] = 0;
   out_4185848689802696400[31] = 0;
   out_4185848689802696400[32] = 0;
   out_4185848689802696400[33] = 0;
   out_4185848689802696400[34] = 0;
   out_4185848689802696400[35] = 0;
   out_4185848689802696400[36] = 0;
   out_4185848689802696400[37] = 0;
   out_4185848689802696400[38] = 0;
   out_4185848689802696400[39] = 0;
   out_4185848689802696400[40] = 0;
   out_4185848689802696400[41] = 0;
   out_4185848689802696400[42] = 0;
   out_4185848689802696400[43] = 0;
   out_4185848689802696400[44] = 1;
   out_4185848689802696400[45] = 0;
   out_4185848689802696400[46] = 0;
   out_4185848689802696400[47] = 1;
   out_4185848689802696400[48] = 0;
   out_4185848689802696400[49] = 0;
   out_4185848689802696400[50] = 0;
   out_4185848689802696400[51] = 0;
   out_4185848689802696400[52] = 0;
   out_4185848689802696400[53] = 0;
}
void h_10(double *state, double *unused, double *out_8976729366753643307) {
   out_8976729366753643307[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8976729366753643307[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8976729366753643307[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_1279619994472514045) {
   out_1279619994472514045[0] = 0;
   out_1279619994472514045[1] = 9.8100000000000005*cos(state[1]);
   out_1279619994472514045[2] = 0;
   out_1279619994472514045[3] = 0;
   out_1279619994472514045[4] = -state[8];
   out_1279619994472514045[5] = state[7];
   out_1279619994472514045[6] = 0;
   out_1279619994472514045[7] = state[5];
   out_1279619994472514045[8] = -state[4];
   out_1279619994472514045[9] = 0;
   out_1279619994472514045[10] = 0;
   out_1279619994472514045[11] = 0;
   out_1279619994472514045[12] = 1;
   out_1279619994472514045[13] = 0;
   out_1279619994472514045[14] = 0;
   out_1279619994472514045[15] = 1;
   out_1279619994472514045[16] = 0;
   out_1279619994472514045[17] = 0;
   out_1279619994472514045[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_1279619994472514045[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_1279619994472514045[20] = 0;
   out_1279619994472514045[21] = state[8];
   out_1279619994472514045[22] = 0;
   out_1279619994472514045[23] = -state[6];
   out_1279619994472514045[24] = -state[5];
   out_1279619994472514045[25] = 0;
   out_1279619994472514045[26] = state[3];
   out_1279619994472514045[27] = 0;
   out_1279619994472514045[28] = 0;
   out_1279619994472514045[29] = 0;
   out_1279619994472514045[30] = 0;
   out_1279619994472514045[31] = 1;
   out_1279619994472514045[32] = 0;
   out_1279619994472514045[33] = 0;
   out_1279619994472514045[34] = 1;
   out_1279619994472514045[35] = 0;
   out_1279619994472514045[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_1279619994472514045[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_1279619994472514045[38] = 0;
   out_1279619994472514045[39] = -state[7];
   out_1279619994472514045[40] = state[6];
   out_1279619994472514045[41] = 0;
   out_1279619994472514045[42] = state[4];
   out_1279619994472514045[43] = -state[3];
   out_1279619994472514045[44] = 0;
   out_1279619994472514045[45] = 0;
   out_1279619994472514045[46] = 0;
   out_1279619994472514045[47] = 0;
   out_1279619994472514045[48] = 0;
   out_1279619994472514045[49] = 0;
   out_1279619994472514045[50] = 1;
   out_1279619994472514045[51] = 0;
   out_1279619994472514045[52] = 0;
   out_1279619994472514045[53] = 1;
}
void h_13(double *state, double *unused, double *out_5013469042968699666) {
   out_5013469042968699666[0] = state[3];
   out_5013469042968699666[1] = state[4];
   out_5013469042968699666[2] = state[5];
}
void H_13(double *state, double *unused, double *out_7398122515135029201) {
   out_7398122515135029201[0] = 0;
   out_7398122515135029201[1] = 0;
   out_7398122515135029201[2] = 0;
   out_7398122515135029201[3] = 1;
   out_7398122515135029201[4] = 0;
   out_7398122515135029201[5] = 0;
   out_7398122515135029201[6] = 0;
   out_7398122515135029201[7] = 0;
   out_7398122515135029201[8] = 0;
   out_7398122515135029201[9] = 0;
   out_7398122515135029201[10] = 0;
   out_7398122515135029201[11] = 0;
   out_7398122515135029201[12] = 0;
   out_7398122515135029201[13] = 0;
   out_7398122515135029201[14] = 0;
   out_7398122515135029201[15] = 0;
   out_7398122515135029201[16] = 0;
   out_7398122515135029201[17] = 0;
   out_7398122515135029201[18] = 0;
   out_7398122515135029201[19] = 0;
   out_7398122515135029201[20] = 0;
   out_7398122515135029201[21] = 0;
   out_7398122515135029201[22] = 1;
   out_7398122515135029201[23] = 0;
   out_7398122515135029201[24] = 0;
   out_7398122515135029201[25] = 0;
   out_7398122515135029201[26] = 0;
   out_7398122515135029201[27] = 0;
   out_7398122515135029201[28] = 0;
   out_7398122515135029201[29] = 0;
   out_7398122515135029201[30] = 0;
   out_7398122515135029201[31] = 0;
   out_7398122515135029201[32] = 0;
   out_7398122515135029201[33] = 0;
   out_7398122515135029201[34] = 0;
   out_7398122515135029201[35] = 0;
   out_7398122515135029201[36] = 0;
   out_7398122515135029201[37] = 0;
   out_7398122515135029201[38] = 0;
   out_7398122515135029201[39] = 0;
   out_7398122515135029201[40] = 0;
   out_7398122515135029201[41] = 1;
   out_7398122515135029201[42] = 0;
   out_7398122515135029201[43] = 0;
   out_7398122515135029201[44] = 0;
   out_7398122515135029201[45] = 0;
   out_7398122515135029201[46] = 0;
   out_7398122515135029201[47] = 0;
   out_7398122515135029201[48] = 0;
   out_7398122515135029201[49] = 0;
   out_7398122515135029201[50] = 0;
   out_7398122515135029201[51] = 0;
   out_7398122515135029201[52] = 0;
   out_7398122515135029201[53] = 0;
}
void h_14(double *state, double *unused, double *out_2265009778651912904) {
   out_2265009778651912904[0] = state[6];
   out_2265009778651912904[1] = state[7];
   out_2265009778651912904[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8149089546142180929) {
   out_8149089546142180929[0] = 0;
   out_8149089546142180929[1] = 0;
   out_8149089546142180929[2] = 0;
   out_8149089546142180929[3] = 0;
   out_8149089546142180929[4] = 0;
   out_8149089546142180929[5] = 0;
   out_8149089546142180929[6] = 1;
   out_8149089546142180929[7] = 0;
   out_8149089546142180929[8] = 0;
   out_8149089546142180929[9] = 0;
   out_8149089546142180929[10] = 0;
   out_8149089546142180929[11] = 0;
   out_8149089546142180929[12] = 0;
   out_8149089546142180929[13] = 0;
   out_8149089546142180929[14] = 0;
   out_8149089546142180929[15] = 0;
   out_8149089546142180929[16] = 0;
   out_8149089546142180929[17] = 0;
   out_8149089546142180929[18] = 0;
   out_8149089546142180929[19] = 0;
   out_8149089546142180929[20] = 0;
   out_8149089546142180929[21] = 0;
   out_8149089546142180929[22] = 0;
   out_8149089546142180929[23] = 0;
   out_8149089546142180929[24] = 0;
   out_8149089546142180929[25] = 1;
   out_8149089546142180929[26] = 0;
   out_8149089546142180929[27] = 0;
   out_8149089546142180929[28] = 0;
   out_8149089546142180929[29] = 0;
   out_8149089546142180929[30] = 0;
   out_8149089546142180929[31] = 0;
   out_8149089546142180929[32] = 0;
   out_8149089546142180929[33] = 0;
   out_8149089546142180929[34] = 0;
   out_8149089546142180929[35] = 0;
   out_8149089546142180929[36] = 0;
   out_8149089546142180929[37] = 0;
   out_8149089546142180929[38] = 0;
   out_8149089546142180929[39] = 0;
   out_8149089546142180929[40] = 0;
   out_8149089546142180929[41] = 0;
   out_8149089546142180929[42] = 0;
   out_8149089546142180929[43] = 0;
   out_8149089546142180929[44] = 1;
   out_8149089546142180929[45] = 0;
   out_8149089546142180929[46] = 0;
   out_8149089546142180929[47] = 0;
   out_8149089546142180929[48] = 0;
   out_8149089546142180929[49] = 0;
   out_8149089546142180929[50] = 0;
   out_8149089546142180929[51] = 0;
   out_8149089546142180929[52] = 0;
   out_8149089546142180929[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_4955557050137928755) {
  err_fun(nom_x, delta_x, out_4955557050137928755);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3781966990875519323) {
  inv_err_fun(nom_x, true_x, out_3781966990875519323);
}
void pose_H_mod_fun(double *state, double *out_4552347542280292738) {
  H_mod_fun(state, out_4552347542280292738);
}
void pose_f_fun(double *state, double dt, double *out_2861912360858063342) {
  f_fun(state,  dt, out_2861912360858063342);
}
void pose_F_fun(double *state, double dt, double *out_8378829832132664204) {
  F_fun(state,  dt, out_8378829832132664204);
}
void pose_h_4(double *state, double *unused, double *out_7327194707407096308) {
  h_4(state, unused, out_7327194707407096308);
}
void pose_H_4(double *state, double *unused, double *out_4185848689802696400) {
  H_4(state, unused, out_4185848689802696400);
}
void pose_h_10(double *state, double *unused, double *out_8976729366753643307) {
  h_10(state, unused, out_8976729366753643307);
}
void pose_H_10(double *state, double *unused, double *out_1279619994472514045) {
  H_10(state, unused, out_1279619994472514045);
}
void pose_h_13(double *state, double *unused, double *out_5013469042968699666) {
  h_13(state, unused, out_5013469042968699666);
}
void pose_H_13(double *state, double *unused, double *out_7398122515135029201) {
  H_13(state, unused, out_7398122515135029201);
}
void pose_h_14(double *state, double *unused, double *out_2265009778651912904) {
  h_14(state, unused, out_2265009778651912904);
}
void pose_H_14(double *state, double *unused, double *out_8149089546142180929) {
  H_14(state, unused, out_8149089546142180929);
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
