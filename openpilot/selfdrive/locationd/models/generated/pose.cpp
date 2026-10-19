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
void err_fun(double *nom_x, double *delta_x, double *out_3502113927026605761) {
   out_3502113927026605761[0] = delta_x[0] + nom_x[0];
   out_3502113927026605761[1] = delta_x[1] + nom_x[1];
   out_3502113927026605761[2] = delta_x[2] + nom_x[2];
   out_3502113927026605761[3] = delta_x[3] + nom_x[3];
   out_3502113927026605761[4] = delta_x[4] + nom_x[4];
   out_3502113927026605761[5] = delta_x[5] + nom_x[5];
   out_3502113927026605761[6] = delta_x[6] + nom_x[6];
   out_3502113927026605761[7] = delta_x[7] + nom_x[7];
   out_3502113927026605761[8] = delta_x[8] + nom_x[8];
   out_3502113927026605761[9] = delta_x[9] + nom_x[9];
   out_3502113927026605761[10] = delta_x[10] + nom_x[10];
   out_3502113927026605761[11] = delta_x[11] + nom_x[11];
   out_3502113927026605761[12] = delta_x[12] + nom_x[12];
   out_3502113927026605761[13] = delta_x[13] + nom_x[13];
   out_3502113927026605761[14] = delta_x[14] + nom_x[14];
   out_3502113927026605761[15] = delta_x[15] + nom_x[15];
   out_3502113927026605761[16] = delta_x[16] + nom_x[16];
   out_3502113927026605761[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4448786092888764286) {
   out_4448786092888764286[0] = -nom_x[0] + true_x[0];
   out_4448786092888764286[1] = -nom_x[1] + true_x[1];
   out_4448786092888764286[2] = -nom_x[2] + true_x[2];
   out_4448786092888764286[3] = -nom_x[3] + true_x[3];
   out_4448786092888764286[4] = -nom_x[4] + true_x[4];
   out_4448786092888764286[5] = -nom_x[5] + true_x[5];
   out_4448786092888764286[6] = -nom_x[6] + true_x[6];
   out_4448786092888764286[7] = -nom_x[7] + true_x[7];
   out_4448786092888764286[8] = -nom_x[8] + true_x[8];
   out_4448786092888764286[9] = -nom_x[9] + true_x[9];
   out_4448786092888764286[10] = -nom_x[10] + true_x[10];
   out_4448786092888764286[11] = -nom_x[11] + true_x[11];
   out_4448786092888764286[12] = -nom_x[12] + true_x[12];
   out_4448786092888764286[13] = -nom_x[13] + true_x[13];
   out_4448786092888764286[14] = -nom_x[14] + true_x[14];
   out_4448786092888764286[15] = -nom_x[15] + true_x[15];
   out_4448786092888764286[16] = -nom_x[16] + true_x[16];
   out_4448786092888764286[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_3056675913014331784) {
   out_3056675913014331784[0] = 1.0;
   out_3056675913014331784[1] = 0.0;
   out_3056675913014331784[2] = 0.0;
   out_3056675913014331784[3] = 0.0;
   out_3056675913014331784[4] = 0.0;
   out_3056675913014331784[5] = 0.0;
   out_3056675913014331784[6] = 0.0;
   out_3056675913014331784[7] = 0.0;
   out_3056675913014331784[8] = 0.0;
   out_3056675913014331784[9] = 0.0;
   out_3056675913014331784[10] = 0.0;
   out_3056675913014331784[11] = 0.0;
   out_3056675913014331784[12] = 0.0;
   out_3056675913014331784[13] = 0.0;
   out_3056675913014331784[14] = 0.0;
   out_3056675913014331784[15] = 0.0;
   out_3056675913014331784[16] = 0.0;
   out_3056675913014331784[17] = 0.0;
   out_3056675913014331784[18] = 0.0;
   out_3056675913014331784[19] = 1.0;
   out_3056675913014331784[20] = 0.0;
   out_3056675913014331784[21] = 0.0;
   out_3056675913014331784[22] = 0.0;
   out_3056675913014331784[23] = 0.0;
   out_3056675913014331784[24] = 0.0;
   out_3056675913014331784[25] = 0.0;
   out_3056675913014331784[26] = 0.0;
   out_3056675913014331784[27] = 0.0;
   out_3056675913014331784[28] = 0.0;
   out_3056675913014331784[29] = 0.0;
   out_3056675913014331784[30] = 0.0;
   out_3056675913014331784[31] = 0.0;
   out_3056675913014331784[32] = 0.0;
   out_3056675913014331784[33] = 0.0;
   out_3056675913014331784[34] = 0.0;
   out_3056675913014331784[35] = 0.0;
   out_3056675913014331784[36] = 0.0;
   out_3056675913014331784[37] = 0.0;
   out_3056675913014331784[38] = 1.0;
   out_3056675913014331784[39] = 0.0;
   out_3056675913014331784[40] = 0.0;
   out_3056675913014331784[41] = 0.0;
   out_3056675913014331784[42] = 0.0;
   out_3056675913014331784[43] = 0.0;
   out_3056675913014331784[44] = 0.0;
   out_3056675913014331784[45] = 0.0;
   out_3056675913014331784[46] = 0.0;
   out_3056675913014331784[47] = 0.0;
   out_3056675913014331784[48] = 0.0;
   out_3056675913014331784[49] = 0.0;
   out_3056675913014331784[50] = 0.0;
   out_3056675913014331784[51] = 0.0;
   out_3056675913014331784[52] = 0.0;
   out_3056675913014331784[53] = 0.0;
   out_3056675913014331784[54] = 0.0;
   out_3056675913014331784[55] = 0.0;
   out_3056675913014331784[56] = 0.0;
   out_3056675913014331784[57] = 1.0;
   out_3056675913014331784[58] = 0.0;
   out_3056675913014331784[59] = 0.0;
   out_3056675913014331784[60] = 0.0;
   out_3056675913014331784[61] = 0.0;
   out_3056675913014331784[62] = 0.0;
   out_3056675913014331784[63] = 0.0;
   out_3056675913014331784[64] = 0.0;
   out_3056675913014331784[65] = 0.0;
   out_3056675913014331784[66] = 0.0;
   out_3056675913014331784[67] = 0.0;
   out_3056675913014331784[68] = 0.0;
   out_3056675913014331784[69] = 0.0;
   out_3056675913014331784[70] = 0.0;
   out_3056675913014331784[71] = 0.0;
   out_3056675913014331784[72] = 0.0;
   out_3056675913014331784[73] = 0.0;
   out_3056675913014331784[74] = 0.0;
   out_3056675913014331784[75] = 0.0;
   out_3056675913014331784[76] = 1.0;
   out_3056675913014331784[77] = 0.0;
   out_3056675913014331784[78] = 0.0;
   out_3056675913014331784[79] = 0.0;
   out_3056675913014331784[80] = 0.0;
   out_3056675913014331784[81] = 0.0;
   out_3056675913014331784[82] = 0.0;
   out_3056675913014331784[83] = 0.0;
   out_3056675913014331784[84] = 0.0;
   out_3056675913014331784[85] = 0.0;
   out_3056675913014331784[86] = 0.0;
   out_3056675913014331784[87] = 0.0;
   out_3056675913014331784[88] = 0.0;
   out_3056675913014331784[89] = 0.0;
   out_3056675913014331784[90] = 0.0;
   out_3056675913014331784[91] = 0.0;
   out_3056675913014331784[92] = 0.0;
   out_3056675913014331784[93] = 0.0;
   out_3056675913014331784[94] = 0.0;
   out_3056675913014331784[95] = 1.0;
   out_3056675913014331784[96] = 0.0;
   out_3056675913014331784[97] = 0.0;
   out_3056675913014331784[98] = 0.0;
   out_3056675913014331784[99] = 0.0;
   out_3056675913014331784[100] = 0.0;
   out_3056675913014331784[101] = 0.0;
   out_3056675913014331784[102] = 0.0;
   out_3056675913014331784[103] = 0.0;
   out_3056675913014331784[104] = 0.0;
   out_3056675913014331784[105] = 0.0;
   out_3056675913014331784[106] = 0.0;
   out_3056675913014331784[107] = 0.0;
   out_3056675913014331784[108] = 0.0;
   out_3056675913014331784[109] = 0.0;
   out_3056675913014331784[110] = 0.0;
   out_3056675913014331784[111] = 0.0;
   out_3056675913014331784[112] = 0.0;
   out_3056675913014331784[113] = 0.0;
   out_3056675913014331784[114] = 1.0;
   out_3056675913014331784[115] = 0.0;
   out_3056675913014331784[116] = 0.0;
   out_3056675913014331784[117] = 0.0;
   out_3056675913014331784[118] = 0.0;
   out_3056675913014331784[119] = 0.0;
   out_3056675913014331784[120] = 0.0;
   out_3056675913014331784[121] = 0.0;
   out_3056675913014331784[122] = 0.0;
   out_3056675913014331784[123] = 0.0;
   out_3056675913014331784[124] = 0.0;
   out_3056675913014331784[125] = 0.0;
   out_3056675913014331784[126] = 0.0;
   out_3056675913014331784[127] = 0.0;
   out_3056675913014331784[128] = 0.0;
   out_3056675913014331784[129] = 0.0;
   out_3056675913014331784[130] = 0.0;
   out_3056675913014331784[131] = 0.0;
   out_3056675913014331784[132] = 0.0;
   out_3056675913014331784[133] = 1.0;
   out_3056675913014331784[134] = 0.0;
   out_3056675913014331784[135] = 0.0;
   out_3056675913014331784[136] = 0.0;
   out_3056675913014331784[137] = 0.0;
   out_3056675913014331784[138] = 0.0;
   out_3056675913014331784[139] = 0.0;
   out_3056675913014331784[140] = 0.0;
   out_3056675913014331784[141] = 0.0;
   out_3056675913014331784[142] = 0.0;
   out_3056675913014331784[143] = 0.0;
   out_3056675913014331784[144] = 0.0;
   out_3056675913014331784[145] = 0.0;
   out_3056675913014331784[146] = 0.0;
   out_3056675913014331784[147] = 0.0;
   out_3056675913014331784[148] = 0.0;
   out_3056675913014331784[149] = 0.0;
   out_3056675913014331784[150] = 0.0;
   out_3056675913014331784[151] = 0.0;
   out_3056675913014331784[152] = 1.0;
   out_3056675913014331784[153] = 0.0;
   out_3056675913014331784[154] = 0.0;
   out_3056675913014331784[155] = 0.0;
   out_3056675913014331784[156] = 0.0;
   out_3056675913014331784[157] = 0.0;
   out_3056675913014331784[158] = 0.0;
   out_3056675913014331784[159] = 0.0;
   out_3056675913014331784[160] = 0.0;
   out_3056675913014331784[161] = 0.0;
   out_3056675913014331784[162] = 0.0;
   out_3056675913014331784[163] = 0.0;
   out_3056675913014331784[164] = 0.0;
   out_3056675913014331784[165] = 0.0;
   out_3056675913014331784[166] = 0.0;
   out_3056675913014331784[167] = 0.0;
   out_3056675913014331784[168] = 0.0;
   out_3056675913014331784[169] = 0.0;
   out_3056675913014331784[170] = 0.0;
   out_3056675913014331784[171] = 1.0;
   out_3056675913014331784[172] = 0.0;
   out_3056675913014331784[173] = 0.0;
   out_3056675913014331784[174] = 0.0;
   out_3056675913014331784[175] = 0.0;
   out_3056675913014331784[176] = 0.0;
   out_3056675913014331784[177] = 0.0;
   out_3056675913014331784[178] = 0.0;
   out_3056675913014331784[179] = 0.0;
   out_3056675913014331784[180] = 0.0;
   out_3056675913014331784[181] = 0.0;
   out_3056675913014331784[182] = 0.0;
   out_3056675913014331784[183] = 0.0;
   out_3056675913014331784[184] = 0.0;
   out_3056675913014331784[185] = 0.0;
   out_3056675913014331784[186] = 0.0;
   out_3056675913014331784[187] = 0.0;
   out_3056675913014331784[188] = 0.0;
   out_3056675913014331784[189] = 0.0;
   out_3056675913014331784[190] = 1.0;
   out_3056675913014331784[191] = 0.0;
   out_3056675913014331784[192] = 0.0;
   out_3056675913014331784[193] = 0.0;
   out_3056675913014331784[194] = 0.0;
   out_3056675913014331784[195] = 0.0;
   out_3056675913014331784[196] = 0.0;
   out_3056675913014331784[197] = 0.0;
   out_3056675913014331784[198] = 0.0;
   out_3056675913014331784[199] = 0.0;
   out_3056675913014331784[200] = 0.0;
   out_3056675913014331784[201] = 0.0;
   out_3056675913014331784[202] = 0.0;
   out_3056675913014331784[203] = 0.0;
   out_3056675913014331784[204] = 0.0;
   out_3056675913014331784[205] = 0.0;
   out_3056675913014331784[206] = 0.0;
   out_3056675913014331784[207] = 0.0;
   out_3056675913014331784[208] = 0.0;
   out_3056675913014331784[209] = 1.0;
   out_3056675913014331784[210] = 0.0;
   out_3056675913014331784[211] = 0.0;
   out_3056675913014331784[212] = 0.0;
   out_3056675913014331784[213] = 0.0;
   out_3056675913014331784[214] = 0.0;
   out_3056675913014331784[215] = 0.0;
   out_3056675913014331784[216] = 0.0;
   out_3056675913014331784[217] = 0.0;
   out_3056675913014331784[218] = 0.0;
   out_3056675913014331784[219] = 0.0;
   out_3056675913014331784[220] = 0.0;
   out_3056675913014331784[221] = 0.0;
   out_3056675913014331784[222] = 0.0;
   out_3056675913014331784[223] = 0.0;
   out_3056675913014331784[224] = 0.0;
   out_3056675913014331784[225] = 0.0;
   out_3056675913014331784[226] = 0.0;
   out_3056675913014331784[227] = 0.0;
   out_3056675913014331784[228] = 1.0;
   out_3056675913014331784[229] = 0.0;
   out_3056675913014331784[230] = 0.0;
   out_3056675913014331784[231] = 0.0;
   out_3056675913014331784[232] = 0.0;
   out_3056675913014331784[233] = 0.0;
   out_3056675913014331784[234] = 0.0;
   out_3056675913014331784[235] = 0.0;
   out_3056675913014331784[236] = 0.0;
   out_3056675913014331784[237] = 0.0;
   out_3056675913014331784[238] = 0.0;
   out_3056675913014331784[239] = 0.0;
   out_3056675913014331784[240] = 0.0;
   out_3056675913014331784[241] = 0.0;
   out_3056675913014331784[242] = 0.0;
   out_3056675913014331784[243] = 0.0;
   out_3056675913014331784[244] = 0.0;
   out_3056675913014331784[245] = 0.0;
   out_3056675913014331784[246] = 0.0;
   out_3056675913014331784[247] = 1.0;
   out_3056675913014331784[248] = 0.0;
   out_3056675913014331784[249] = 0.0;
   out_3056675913014331784[250] = 0.0;
   out_3056675913014331784[251] = 0.0;
   out_3056675913014331784[252] = 0.0;
   out_3056675913014331784[253] = 0.0;
   out_3056675913014331784[254] = 0.0;
   out_3056675913014331784[255] = 0.0;
   out_3056675913014331784[256] = 0.0;
   out_3056675913014331784[257] = 0.0;
   out_3056675913014331784[258] = 0.0;
   out_3056675913014331784[259] = 0.0;
   out_3056675913014331784[260] = 0.0;
   out_3056675913014331784[261] = 0.0;
   out_3056675913014331784[262] = 0.0;
   out_3056675913014331784[263] = 0.0;
   out_3056675913014331784[264] = 0.0;
   out_3056675913014331784[265] = 0.0;
   out_3056675913014331784[266] = 1.0;
   out_3056675913014331784[267] = 0.0;
   out_3056675913014331784[268] = 0.0;
   out_3056675913014331784[269] = 0.0;
   out_3056675913014331784[270] = 0.0;
   out_3056675913014331784[271] = 0.0;
   out_3056675913014331784[272] = 0.0;
   out_3056675913014331784[273] = 0.0;
   out_3056675913014331784[274] = 0.0;
   out_3056675913014331784[275] = 0.0;
   out_3056675913014331784[276] = 0.0;
   out_3056675913014331784[277] = 0.0;
   out_3056675913014331784[278] = 0.0;
   out_3056675913014331784[279] = 0.0;
   out_3056675913014331784[280] = 0.0;
   out_3056675913014331784[281] = 0.0;
   out_3056675913014331784[282] = 0.0;
   out_3056675913014331784[283] = 0.0;
   out_3056675913014331784[284] = 0.0;
   out_3056675913014331784[285] = 1.0;
   out_3056675913014331784[286] = 0.0;
   out_3056675913014331784[287] = 0.0;
   out_3056675913014331784[288] = 0.0;
   out_3056675913014331784[289] = 0.0;
   out_3056675913014331784[290] = 0.0;
   out_3056675913014331784[291] = 0.0;
   out_3056675913014331784[292] = 0.0;
   out_3056675913014331784[293] = 0.0;
   out_3056675913014331784[294] = 0.0;
   out_3056675913014331784[295] = 0.0;
   out_3056675913014331784[296] = 0.0;
   out_3056675913014331784[297] = 0.0;
   out_3056675913014331784[298] = 0.0;
   out_3056675913014331784[299] = 0.0;
   out_3056675913014331784[300] = 0.0;
   out_3056675913014331784[301] = 0.0;
   out_3056675913014331784[302] = 0.0;
   out_3056675913014331784[303] = 0.0;
   out_3056675913014331784[304] = 1.0;
   out_3056675913014331784[305] = 0.0;
   out_3056675913014331784[306] = 0.0;
   out_3056675913014331784[307] = 0.0;
   out_3056675913014331784[308] = 0.0;
   out_3056675913014331784[309] = 0.0;
   out_3056675913014331784[310] = 0.0;
   out_3056675913014331784[311] = 0.0;
   out_3056675913014331784[312] = 0.0;
   out_3056675913014331784[313] = 0.0;
   out_3056675913014331784[314] = 0.0;
   out_3056675913014331784[315] = 0.0;
   out_3056675913014331784[316] = 0.0;
   out_3056675913014331784[317] = 0.0;
   out_3056675913014331784[318] = 0.0;
   out_3056675913014331784[319] = 0.0;
   out_3056675913014331784[320] = 0.0;
   out_3056675913014331784[321] = 0.0;
   out_3056675913014331784[322] = 0.0;
   out_3056675913014331784[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_1826304795513048853) {
   out_1826304795513048853[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_1826304795513048853[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_1826304795513048853[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_1826304795513048853[3] = dt*state[12] + state[3];
   out_1826304795513048853[4] = dt*state[13] + state[4];
   out_1826304795513048853[5] = dt*state[14] + state[5];
   out_1826304795513048853[6] = state[6];
   out_1826304795513048853[7] = state[7];
   out_1826304795513048853[8] = state[8];
   out_1826304795513048853[9] = state[9];
   out_1826304795513048853[10] = state[10];
   out_1826304795513048853[11] = state[11];
   out_1826304795513048853[12] = state[12];
   out_1826304795513048853[13] = state[13];
   out_1826304795513048853[14] = state[14];
   out_1826304795513048853[15] = state[15];
   out_1826304795513048853[16] = state[16];
   out_1826304795513048853[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2679369155394544549) {
   out_2679369155394544549[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2679369155394544549[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2679369155394544549[2] = 0;
   out_2679369155394544549[3] = 0;
   out_2679369155394544549[4] = 0;
   out_2679369155394544549[5] = 0;
   out_2679369155394544549[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2679369155394544549[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2679369155394544549[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2679369155394544549[9] = 0;
   out_2679369155394544549[10] = 0;
   out_2679369155394544549[11] = 0;
   out_2679369155394544549[12] = 0;
   out_2679369155394544549[13] = 0;
   out_2679369155394544549[14] = 0;
   out_2679369155394544549[15] = 0;
   out_2679369155394544549[16] = 0;
   out_2679369155394544549[17] = 0;
   out_2679369155394544549[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2679369155394544549[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2679369155394544549[20] = 0;
   out_2679369155394544549[21] = 0;
   out_2679369155394544549[22] = 0;
   out_2679369155394544549[23] = 0;
   out_2679369155394544549[24] = 0;
   out_2679369155394544549[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2679369155394544549[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2679369155394544549[27] = 0;
   out_2679369155394544549[28] = 0;
   out_2679369155394544549[29] = 0;
   out_2679369155394544549[30] = 0;
   out_2679369155394544549[31] = 0;
   out_2679369155394544549[32] = 0;
   out_2679369155394544549[33] = 0;
   out_2679369155394544549[34] = 0;
   out_2679369155394544549[35] = 0;
   out_2679369155394544549[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2679369155394544549[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2679369155394544549[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2679369155394544549[39] = 0;
   out_2679369155394544549[40] = 0;
   out_2679369155394544549[41] = 0;
   out_2679369155394544549[42] = 0;
   out_2679369155394544549[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2679369155394544549[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2679369155394544549[45] = 0;
   out_2679369155394544549[46] = 0;
   out_2679369155394544549[47] = 0;
   out_2679369155394544549[48] = 0;
   out_2679369155394544549[49] = 0;
   out_2679369155394544549[50] = 0;
   out_2679369155394544549[51] = 0;
   out_2679369155394544549[52] = 0;
   out_2679369155394544549[53] = 0;
   out_2679369155394544549[54] = 0;
   out_2679369155394544549[55] = 0;
   out_2679369155394544549[56] = 0;
   out_2679369155394544549[57] = 1;
   out_2679369155394544549[58] = 0;
   out_2679369155394544549[59] = 0;
   out_2679369155394544549[60] = 0;
   out_2679369155394544549[61] = 0;
   out_2679369155394544549[62] = 0;
   out_2679369155394544549[63] = 0;
   out_2679369155394544549[64] = 0;
   out_2679369155394544549[65] = 0;
   out_2679369155394544549[66] = dt;
   out_2679369155394544549[67] = 0;
   out_2679369155394544549[68] = 0;
   out_2679369155394544549[69] = 0;
   out_2679369155394544549[70] = 0;
   out_2679369155394544549[71] = 0;
   out_2679369155394544549[72] = 0;
   out_2679369155394544549[73] = 0;
   out_2679369155394544549[74] = 0;
   out_2679369155394544549[75] = 0;
   out_2679369155394544549[76] = 1;
   out_2679369155394544549[77] = 0;
   out_2679369155394544549[78] = 0;
   out_2679369155394544549[79] = 0;
   out_2679369155394544549[80] = 0;
   out_2679369155394544549[81] = 0;
   out_2679369155394544549[82] = 0;
   out_2679369155394544549[83] = 0;
   out_2679369155394544549[84] = 0;
   out_2679369155394544549[85] = dt;
   out_2679369155394544549[86] = 0;
   out_2679369155394544549[87] = 0;
   out_2679369155394544549[88] = 0;
   out_2679369155394544549[89] = 0;
   out_2679369155394544549[90] = 0;
   out_2679369155394544549[91] = 0;
   out_2679369155394544549[92] = 0;
   out_2679369155394544549[93] = 0;
   out_2679369155394544549[94] = 0;
   out_2679369155394544549[95] = 1;
   out_2679369155394544549[96] = 0;
   out_2679369155394544549[97] = 0;
   out_2679369155394544549[98] = 0;
   out_2679369155394544549[99] = 0;
   out_2679369155394544549[100] = 0;
   out_2679369155394544549[101] = 0;
   out_2679369155394544549[102] = 0;
   out_2679369155394544549[103] = 0;
   out_2679369155394544549[104] = dt;
   out_2679369155394544549[105] = 0;
   out_2679369155394544549[106] = 0;
   out_2679369155394544549[107] = 0;
   out_2679369155394544549[108] = 0;
   out_2679369155394544549[109] = 0;
   out_2679369155394544549[110] = 0;
   out_2679369155394544549[111] = 0;
   out_2679369155394544549[112] = 0;
   out_2679369155394544549[113] = 0;
   out_2679369155394544549[114] = 1;
   out_2679369155394544549[115] = 0;
   out_2679369155394544549[116] = 0;
   out_2679369155394544549[117] = 0;
   out_2679369155394544549[118] = 0;
   out_2679369155394544549[119] = 0;
   out_2679369155394544549[120] = 0;
   out_2679369155394544549[121] = 0;
   out_2679369155394544549[122] = 0;
   out_2679369155394544549[123] = 0;
   out_2679369155394544549[124] = 0;
   out_2679369155394544549[125] = 0;
   out_2679369155394544549[126] = 0;
   out_2679369155394544549[127] = 0;
   out_2679369155394544549[128] = 0;
   out_2679369155394544549[129] = 0;
   out_2679369155394544549[130] = 0;
   out_2679369155394544549[131] = 0;
   out_2679369155394544549[132] = 0;
   out_2679369155394544549[133] = 1;
   out_2679369155394544549[134] = 0;
   out_2679369155394544549[135] = 0;
   out_2679369155394544549[136] = 0;
   out_2679369155394544549[137] = 0;
   out_2679369155394544549[138] = 0;
   out_2679369155394544549[139] = 0;
   out_2679369155394544549[140] = 0;
   out_2679369155394544549[141] = 0;
   out_2679369155394544549[142] = 0;
   out_2679369155394544549[143] = 0;
   out_2679369155394544549[144] = 0;
   out_2679369155394544549[145] = 0;
   out_2679369155394544549[146] = 0;
   out_2679369155394544549[147] = 0;
   out_2679369155394544549[148] = 0;
   out_2679369155394544549[149] = 0;
   out_2679369155394544549[150] = 0;
   out_2679369155394544549[151] = 0;
   out_2679369155394544549[152] = 1;
   out_2679369155394544549[153] = 0;
   out_2679369155394544549[154] = 0;
   out_2679369155394544549[155] = 0;
   out_2679369155394544549[156] = 0;
   out_2679369155394544549[157] = 0;
   out_2679369155394544549[158] = 0;
   out_2679369155394544549[159] = 0;
   out_2679369155394544549[160] = 0;
   out_2679369155394544549[161] = 0;
   out_2679369155394544549[162] = 0;
   out_2679369155394544549[163] = 0;
   out_2679369155394544549[164] = 0;
   out_2679369155394544549[165] = 0;
   out_2679369155394544549[166] = 0;
   out_2679369155394544549[167] = 0;
   out_2679369155394544549[168] = 0;
   out_2679369155394544549[169] = 0;
   out_2679369155394544549[170] = 0;
   out_2679369155394544549[171] = 1;
   out_2679369155394544549[172] = 0;
   out_2679369155394544549[173] = 0;
   out_2679369155394544549[174] = 0;
   out_2679369155394544549[175] = 0;
   out_2679369155394544549[176] = 0;
   out_2679369155394544549[177] = 0;
   out_2679369155394544549[178] = 0;
   out_2679369155394544549[179] = 0;
   out_2679369155394544549[180] = 0;
   out_2679369155394544549[181] = 0;
   out_2679369155394544549[182] = 0;
   out_2679369155394544549[183] = 0;
   out_2679369155394544549[184] = 0;
   out_2679369155394544549[185] = 0;
   out_2679369155394544549[186] = 0;
   out_2679369155394544549[187] = 0;
   out_2679369155394544549[188] = 0;
   out_2679369155394544549[189] = 0;
   out_2679369155394544549[190] = 1;
   out_2679369155394544549[191] = 0;
   out_2679369155394544549[192] = 0;
   out_2679369155394544549[193] = 0;
   out_2679369155394544549[194] = 0;
   out_2679369155394544549[195] = 0;
   out_2679369155394544549[196] = 0;
   out_2679369155394544549[197] = 0;
   out_2679369155394544549[198] = 0;
   out_2679369155394544549[199] = 0;
   out_2679369155394544549[200] = 0;
   out_2679369155394544549[201] = 0;
   out_2679369155394544549[202] = 0;
   out_2679369155394544549[203] = 0;
   out_2679369155394544549[204] = 0;
   out_2679369155394544549[205] = 0;
   out_2679369155394544549[206] = 0;
   out_2679369155394544549[207] = 0;
   out_2679369155394544549[208] = 0;
   out_2679369155394544549[209] = 1;
   out_2679369155394544549[210] = 0;
   out_2679369155394544549[211] = 0;
   out_2679369155394544549[212] = 0;
   out_2679369155394544549[213] = 0;
   out_2679369155394544549[214] = 0;
   out_2679369155394544549[215] = 0;
   out_2679369155394544549[216] = 0;
   out_2679369155394544549[217] = 0;
   out_2679369155394544549[218] = 0;
   out_2679369155394544549[219] = 0;
   out_2679369155394544549[220] = 0;
   out_2679369155394544549[221] = 0;
   out_2679369155394544549[222] = 0;
   out_2679369155394544549[223] = 0;
   out_2679369155394544549[224] = 0;
   out_2679369155394544549[225] = 0;
   out_2679369155394544549[226] = 0;
   out_2679369155394544549[227] = 0;
   out_2679369155394544549[228] = 1;
   out_2679369155394544549[229] = 0;
   out_2679369155394544549[230] = 0;
   out_2679369155394544549[231] = 0;
   out_2679369155394544549[232] = 0;
   out_2679369155394544549[233] = 0;
   out_2679369155394544549[234] = 0;
   out_2679369155394544549[235] = 0;
   out_2679369155394544549[236] = 0;
   out_2679369155394544549[237] = 0;
   out_2679369155394544549[238] = 0;
   out_2679369155394544549[239] = 0;
   out_2679369155394544549[240] = 0;
   out_2679369155394544549[241] = 0;
   out_2679369155394544549[242] = 0;
   out_2679369155394544549[243] = 0;
   out_2679369155394544549[244] = 0;
   out_2679369155394544549[245] = 0;
   out_2679369155394544549[246] = 0;
   out_2679369155394544549[247] = 1;
   out_2679369155394544549[248] = 0;
   out_2679369155394544549[249] = 0;
   out_2679369155394544549[250] = 0;
   out_2679369155394544549[251] = 0;
   out_2679369155394544549[252] = 0;
   out_2679369155394544549[253] = 0;
   out_2679369155394544549[254] = 0;
   out_2679369155394544549[255] = 0;
   out_2679369155394544549[256] = 0;
   out_2679369155394544549[257] = 0;
   out_2679369155394544549[258] = 0;
   out_2679369155394544549[259] = 0;
   out_2679369155394544549[260] = 0;
   out_2679369155394544549[261] = 0;
   out_2679369155394544549[262] = 0;
   out_2679369155394544549[263] = 0;
   out_2679369155394544549[264] = 0;
   out_2679369155394544549[265] = 0;
   out_2679369155394544549[266] = 1;
   out_2679369155394544549[267] = 0;
   out_2679369155394544549[268] = 0;
   out_2679369155394544549[269] = 0;
   out_2679369155394544549[270] = 0;
   out_2679369155394544549[271] = 0;
   out_2679369155394544549[272] = 0;
   out_2679369155394544549[273] = 0;
   out_2679369155394544549[274] = 0;
   out_2679369155394544549[275] = 0;
   out_2679369155394544549[276] = 0;
   out_2679369155394544549[277] = 0;
   out_2679369155394544549[278] = 0;
   out_2679369155394544549[279] = 0;
   out_2679369155394544549[280] = 0;
   out_2679369155394544549[281] = 0;
   out_2679369155394544549[282] = 0;
   out_2679369155394544549[283] = 0;
   out_2679369155394544549[284] = 0;
   out_2679369155394544549[285] = 1;
   out_2679369155394544549[286] = 0;
   out_2679369155394544549[287] = 0;
   out_2679369155394544549[288] = 0;
   out_2679369155394544549[289] = 0;
   out_2679369155394544549[290] = 0;
   out_2679369155394544549[291] = 0;
   out_2679369155394544549[292] = 0;
   out_2679369155394544549[293] = 0;
   out_2679369155394544549[294] = 0;
   out_2679369155394544549[295] = 0;
   out_2679369155394544549[296] = 0;
   out_2679369155394544549[297] = 0;
   out_2679369155394544549[298] = 0;
   out_2679369155394544549[299] = 0;
   out_2679369155394544549[300] = 0;
   out_2679369155394544549[301] = 0;
   out_2679369155394544549[302] = 0;
   out_2679369155394544549[303] = 0;
   out_2679369155394544549[304] = 1;
   out_2679369155394544549[305] = 0;
   out_2679369155394544549[306] = 0;
   out_2679369155394544549[307] = 0;
   out_2679369155394544549[308] = 0;
   out_2679369155394544549[309] = 0;
   out_2679369155394544549[310] = 0;
   out_2679369155394544549[311] = 0;
   out_2679369155394544549[312] = 0;
   out_2679369155394544549[313] = 0;
   out_2679369155394544549[314] = 0;
   out_2679369155394544549[315] = 0;
   out_2679369155394544549[316] = 0;
   out_2679369155394544549[317] = 0;
   out_2679369155394544549[318] = 0;
   out_2679369155394544549[319] = 0;
   out_2679369155394544549[320] = 0;
   out_2679369155394544549[321] = 0;
   out_2679369155394544549[322] = 0;
   out_2679369155394544549[323] = 1;
}
void h_4(double *state, double *unused, double *out_4714243789752105753) {
   out_4714243789752105753[0] = state[6] + state[9];
   out_4714243789752105753[1] = state[7] + state[10];
   out_4714243789752105753[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_5444406278175342083) {
   out_5444406278175342083[0] = 0;
   out_5444406278175342083[1] = 0;
   out_5444406278175342083[2] = 0;
   out_5444406278175342083[3] = 0;
   out_5444406278175342083[4] = 0;
   out_5444406278175342083[5] = 0;
   out_5444406278175342083[6] = 1;
   out_5444406278175342083[7] = 0;
   out_5444406278175342083[8] = 0;
   out_5444406278175342083[9] = 1;
   out_5444406278175342083[10] = 0;
   out_5444406278175342083[11] = 0;
   out_5444406278175342083[12] = 0;
   out_5444406278175342083[13] = 0;
   out_5444406278175342083[14] = 0;
   out_5444406278175342083[15] = 0;
   out_5444406278175342083[16] = 0;
   out_5444406278175342083[17] = 0;
   out_5444406278175342083[18] = 0;
   out_5444406278175342083[19] = 0;
   out_5444406278175342083[20] = 0;
   out_5444406278175342083[21] = 0;
   out_5444406278175342083[22] = 0;
   out_5444406278175342083[23] = 0;
   out_5444406278175342083[24] = 0;
   out_5444406278175342083[25] = 1;
   out_5444406278175342083[26] = 0;
   out_5444406278175342083[27] = 0;
   out_5444406278175342083[28] = 1;
   out_5444406278175342083[29] = 0;
   out_5444406278175342083[30] = 0;
   out_5444406278175342083[31] = 0;
   out_5444406278175342083[32] = 0;
   out_5444406278175342083[33] = 0;
   out_5444406278175342083[34] = 0;
   out_5444406278175342083[35] = 0;
   out_5444406278175342083[36] = 0;
   out_5444406278175342083[37] = 0;
   out_5444406278175342083[38] = 0;
   out_5444406278175342083[39] = 0;
   out_5444406278175342083[40] = 0;
   out_5444406278175342083[41] = 0;
   out_5444406278175342083[42] = 0;
   out_5444406278175342083[43] = 0;
   out_5444406278175342083[44] = 1;
   out_5444406278175342083[45] = 0;
   out_5444406278175342083[46] = 0;
   out_5444406278175342083[47] = 1;
   out_5444406278175342083[48] = 0;
   out_5444406278175342083[49] = 0;
   out_5444406278175342083[50] = 0;
   out_5444406278175342083[51] = 0;
   out_5444406278175342083[52] = 0;
   out_5444406278175342083[53] = 0;
}
void h_10(double *state, double *unused, double *out_6341543449718086925) {
   out_6341543449718086925[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_6341543449718086925[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_6341543449718086925[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_7615405668400164669) {
   out_7615405668400164669[0] = 0;
   out_7615405668400164669[1] = 9.8100000000000005*cos(state[1]);
   out_7615405668400164669[2] = 0;
   out_7615405668400164669[3] = 0;
   out_7615405668400164669[4] = -state[8];
   out_7615405668400164669[5] = state[7];
   out_7615405668400164669[6] = 0;
   out_7615405668400164669[7] = state[5];
   out_7615405668400164669[8] = -state[4];
   out_7615405668400164669[9] = 0;
   out_7615405668400164669[10] = 0;
   out_7615405668400164669[11] = 0;
   out_7615405668400164669[12] = 1;
   out_7615405668400164669[13] = 0;
   out_7615405668400164669[14] = 0;
   out_7615405668400164669[15] = 1;
   out_7615405668400164669[16] = 0;
   out_7615405668400164669[17] = 0;
   out_7615405668400164669[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_7615405668400164669[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_7615405668400164669[20] = 0;
   out_7615405668400164669[21] = state[8];
   out_7615405668400164669[22] = 0;
   out_7615405668400164669[23] = -state[6];
   out_7615405668400164669[24] = -state[5];
   out_7615405668400164669[25] = 0;
   out_7615405668400164669[26] = state[3];
   out_7615405668400164669[27] = 0;
   out_7615405668400164669[28] = 0;
   out_7615405668400164669[29] = 0;
   out_7615405668400164669[30] = 0;
   out_7615405668400164669[31] = 1;
   out_7615405668400164669[32] = 0;
   out_7615405668400164669[33] = 0;
   out_7615405668400164669[34] = 1;
   out_7615405668400164669[35] = 0;
   out_7615405668400164669[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_7615405668400164669[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_7615405668400164669[38] = 0;
   out_7615405668400164669[39] = -state[7];
   out_7615405668400164669[40] = state[6];
   out_7615405668400164669[41] = 0;
   out_7615405668400164669[42] = state[4];
   out_7615405668400164669[43] = -state[3];
   out_7615405668400164669[44] = 0;
   out_7615405668400164669[45] = 0;
   out_7615405668400164669[46] = 0;
   out_7615405668400164669[47] = 0;
   out_7615405668400164669[48] = 0;
   out_7615405668400164669[49] = 0;
   out_7615405668400164669[50] = 1;
   out_7615405668400164669[51] = 0;
   out_7615405668400164669[52] = 0;
   out_7615405668400164669[53] = 1;
}
void h_13(double *state, double *unused, double *out_4272667523865022176) {
   out_4272667523865022176[0] = state[3];
   out_4272667523865022176[1] = state[4];
   out_4272667523865022176[2] = state[5];
}
void H_13(double *state, double *unused, double *out_9168582332231685509) {
   out_9168582332231685509[0] = 0;
   out_9168582332231685509[1] = 0;
   out_9168582332231685509[2] = 0;
   out_9168582332231685509[3] = 1;
   out_9168582332231685509[4] = 0;
   out_9168582332231685509[5] = 0;
   out_9168582332231685509[6] = 0;
   out_9168582332231685509[7] = 0;
   out_9168582332231685509[8] = 0;
   out_9168582332231685509[9] = 0;
   out_9168582332231685509[10] = 0;
   out_9168582332231685509[11] = 0;
   out_9168582332231685509[12] = 0;
   out_9168582332231685509[13] = 0;
   out_9168582332231685509[14] = 0;
   out_9168582332231685509[15] = 0;
   out_9168582332231685509[16] = 0;
   out_9168582332231685509[17] = 0;
   out_9168582332231685509[18] = 0;
   out_9168582332231685509[19] = 0;
   out_9168582332231685509[20] = 0;
   out_9168582332231685509[21] = 0;
   out_9168582332231685509[22] = 1;
   out_9168582332231685509[23] = 0;
   out_9168582332231685509[24] = 0;
   out_9168582332231685509[25] = 0;
   out_9168582332231685509[26] = 0;
   out_9168582332231685509[27] = 0;
   out_9168582332231685509[28] = 0;
   out_9168582332231685509[29] = 0;
   out_9168582332231685509[30] = 0;
   out_9168582332231685509[31] = 0;
   out_9168582332231685509[32] = 0;
   out_9168582332231685509[33] = 0;
   out_9168582332231685509[34] = 0;
   out_9168582332231685509[35] = 0;
   out_9168582332231685509[36] = 0;
   out_9168582332231685509[37] = 0;
   out_9168582332231685509[38] = 0;
   out_9168582332231685509[39] = 0;
   out_9168582332231685509[40] = 0;
   out_9168582332231685509[41] = 1;
   out_9168582332231685509[42] = 0;
   out_9168582332231685509[43] = 0;
   out_9168582332231685509[44] = 0;
   out_9168582332231685509[45] = 0;
   out_9168582332231685509[46] = 0;
   out_9168582332231685509[47] = 0;
   out_9168582332231685509[48] = 0;
   out_9168582332231685509[49] = 0;
   out_9168582332231685509[50] = 0;
   out_9168582332231685509[51] = 0;
   out_9168582332231685509[52] = 0;
   out_9168582332231685509[53] = 0;
}
void h_14(double *state, double *unused, double *out_4516626462834453539) {
   out_4516626462834453539[0] = state[6];
   out_4516626462834453539[1] = state[7];
   out_4516626462834453539[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8527194710470714379) {
   out_8527194710470714379[0] = 0;
   out_8527194710470714379[1] = 0;
   out_8527194710470714379[2] = 0;
   out_8527194710470714379[3] = 0;
   out_8527194710470714379[4] = 0;
   out_8527194710470714379[5] = 0;
   out_8527194710470714379[6] = 1;
   out_8527194710470714379[7] = 0;
   out_8527194710470714379[8] = 0;
   out_8527194710470714379[9] = 0;
   out_8527194710470714379[10] = 0;
   out_8527194710470714379[11] = 0;
   out_8527194710470714379[12] = 0;
   out_8527194710470714379[13] = 0;
   out_8527194710470714379[14] = 0;
   out_8527194710470714379[15] = 0;
   out_8527194710470714379[16] = 0;
   out_8527194710470714379[17] = 0;
   out_8527194710470714379[18] = 0;
   out_8527194710470714379[19] = 0;
   out_8527194710470714379[20] = 0;
   out_8527194710470714379[21] = 0;
   out_8527194710470714379[22] = 0;
   out_8527194710470714379[23] = 0;
   out_8527194710470714379[24] = 0;
   out_8527194710470714379[25] = 1;
   out_8527194710470714379[26] = 0;
   out_8527194710470714379[27] = 0;
   out_8527194710470714379[28] = 0;
   out_8527194710470714379[29] = 0;
   out_8527194710470714379[30] = 0;
   out_8527194710470714379[31] = 0;
   out_8527194710470714379[32] = 0;
   out_8527194710470714379[33] = 0;
   out_8527194710470714379[34] = 0;
   out_8527194710470714379[35] = 0;
   out_8527194710470714379[36] = 0;
   out_8527194710470714379[37] = 0;
   out_8527194710470714379[38] = 0;
   out_8527194710470714379[39] = 0;
   out_8527194710470714379[40] = 0;
   out_8527194710470714379[41] = 0;
   out_8527194710470714379[42] = 0;
   out_8527194710470714379[43] = 0;
   out_8527194710470714379[44] = 1;
   out_8527194710470714379[45] = 0;
   out_8527194710470714379[46] = 0;
   out_8527194710470714379[47] = 0;
   out_8527194710470714379[48] = 0;
   out_8527194710470714379[49] = 0;
   out_8527194710470714379[50] = 0;
   out_8527194710470714379[51] = 0;
   out_8527194710470714379[52] = 0;
   out_8527194710470714379[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_3502113927026605761) {
  err_fun(nom_x, delta_x, out_3502113927026605761);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4448786092888764286) {
  inv_err_fun(nom_x, true_x, out_4448786092888764286);
}
void pose_H_mod_fun(double *state, double *out_3056675913014331784) {
  H_mod_fun(state, out_3056675913014331784);
}
void pose_f_fun(double *state, double dt, double *out_1826304795513048853) {
  f_fun(state,  dt, out_1826304795513048853);
}
void pose_F_fun(double *state, double dt, double *out_2679369155394544549) {
  F_fun(state,  dt, out_2679369155394544549);
}
void pose_h_4(double *state, double *unused, double *out_4714243789752105753) {
  h_4(state, unused, out_4714243789752105753);
}
void pose_H_4(double *state, double *unused, double *out_5444406278175342083) {
  H_4(state, unused, out_5444406278175342083);
}
void pose_h_10(double *state, double *unused, double *out_6341543449718086925) {
  h_10(state, unused, out_6341543449718086925);
}
void pose_H_10(double *state, double *unused, double *out_7615405668400164669) {
  H_10(state, unused, out_7615405668400164669);
}
void pose_h_13(double *state, double *unused, double *out_4272667523865022176) {
  h_13(state, unused, out_4272667523865022176);
}
void pose_H_13(double *state, double *unused, double *out_9168582332231685509) {
  H_13(state, unused, out_9168582332231685509);
}
void pose_h_14(double *state, double *unused, double *out_4516626462834453539) {
  h_14(state, unused, out_4516626462834453539);
}
void pose_H_14(double *state, double *unused, double *out_8527194710470714379) {
  H_14(state, unused, out_8527194710470714379);
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
