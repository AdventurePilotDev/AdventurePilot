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
void err_fun(double *nom_x, double *delta_x, double *out_8460591430937244758) {
   out_8460591430937244758[0] = delta_x[0] + nom_x[0];
   out_8460591430937244758[1] = delta_x[1] + nom_x[1];
   out_8460591430937244758[2] = delta_x[2] + nom_x[2];
   out_8460591430937244758[3] = delta_x[3] + nom_x[3];
   out_8460591430937244758[4] = delta_x[4] + nom_x[4];
   out_8460591430937244758[5] = delta_x[5] + nom_x[5];
   out_8460591430937244758[6] = delta_x[6] + nom_x[6];
   out_8460591430937244758[7] = delta_x[7] + nom_x[7];
   out_8460591430937244758[8] = delta_x[8] + nom_x[8];
   out_8460591430937244758[9] = delta_x[9] + nom_x[9];
   out_8460591430937244758[10] = delta_x[10] + nom_x[10];
   out_8460591430937244758[11] = delta_x[11] + nom_x[11];
   out_8460591430937244758[12] = delta_x[12] + nom_x[12];
   out_8460591430937244758[13] = delta_x[13] + nom_x[13];
   out_8460591430937244758[14] = delta_x[14] + nom_x[14];
   out_8460591430937244758[15] = delta_x[15] + nom_x[15];
   out_8460591430937244758[16] = delta_x[16] + nom_x[16];
   out_8460591430937244758[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2176108056044053025) {
   out_2176108056044053025[0] = -nom_x[0] + true_x[0];
   out_2176108056044053025[1] = -nom_x[1] + true_x[1];
   out_2176108056044053025[2] = -nom_x[2] + true_x[2];
   out_2176108056044053025[3] = -nom_x[3] + true_x[3];
   out_2176108056044053025[4] = -nom_x[4] + true_x[4];
   out_2176108056044053025[5] = -nom_x[5] + true_x[5];
   out_2176108056044053025[6] = -nom_x[6] + true_x[6];
   out_2176108056044053025[7] = -nom_x[7] + true_x[7];
   out_2176108056044053025[8] = -nom_x[8] + true_x[8];
   out_2176108056044053025[9] = -nom_x[9] + true_x[9];
   out_2176108056044053025[10] = -nom_x[10] + true_x[10];
   out_2176108056044053025[11] = -nom_x[11] + true_x[11];
   out_2176108056044053025[12] = -nom_x[12] + true_x[12];
   out_2176108056044053025[13] = -nom_x[13] + true_x[13];
   out_2176108056044053025[14] = -nom_x[14] + true_x[14];
   out_2176108056044053025[15] = -nom_x[15] + true_x[15];
   out_2176108056044053025[16] = -nom_x[16] + true_x[16];
   out_2176108056044053025[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_7992819708489120391) {
   out_7992819708489120391[0] = 1.0;
   out_7992819708489120391[1] = 0.0;
   out_7992819708489120391[2] = 0.0;
   out_7992819708489120391[3] = 0.0;
   out_7992819708489120391[4] = 0.0;
   out_7992819708489120391[5] = 0.0;
   out_7992819708489120391[6] = 0.0;
   out_7992819708489120391[7] = 0.0;
   out_7992819708489120391[8] = 0.0;
   out_7992819708489120391[9] = 0.0;
   out_7992819708489120391[10] = 0.0;
   out_7992819708489120391[11] = 0.0;
   out_7992819708489120391[12] = 0.0;
   out_7992819708489120391[13] = 0.0;
   out_7992819708489120391[14] = 0.0;
   out_7992819708489120391[15] = 0.0;
   out_7992819708489120391[16] = 0.0;
   out_7992819708489120391[17] = 0.0;
   out_7992819708489120391[18] = 0.0;
   out_7992819708489120391[19] = 1.0;
   out_7992819708489120391[20] = 0.0;
   out_7992819708489120391[21] = 0.0;
   out_7992819708489120391[22] = 0.0;
   out_7992819708489120391[23] = 0.0;
   out_7992819708489120391[24] = 0.0;
   out_7992819708489120391[25] = 0.0;
   out_7992819708489120391[26] = 0.0;
   out_7992819708489120391[27] = 0.0;
   out_7992819708489120391[28] = 0.0;
   out_7992819708489120391[29] = 0.0;
   out_7992819708489120391[30] = 0.0;
   out_7992819708489120391[31] = 0.0;
   out_7992819708489120391[32] = 0.0;
   out_7992819708489120391[33] = 0.0;
   out_7992819708489120391[34] = 0.0;
   out_7992819708489120391[35] = 0.0;
   out_7992819708489120391[36] = 0.0;
   out_7992819708489120391[37] = 0.0;
   out_7992819708489120391[38] = 1.0;
   out_7992819708489120391[39] = 0.0;
   out_7992819708489120391[40] = 0.0;
   out_7992819708489120391[41] = 0.0;
   out_7992819708489120391[42] = 0.0;
   out_7992819708489120391[43] = 0.0;
   out_7992819708489120391[44] = 0.0;
   out_7992819708489120391[45] = 0.0;
   out_7992819708489120391[46] = 0.0;
   out_7992819708489120391[47] = 0.0;
   out_7992819708489120391[48] = 0.0;
   out_7992819708489120391[49] = 0.0;
   out_7992819708489120391[50] = 0.0;
   out_7992819708489120391[51] = 0.0;
   out_7992819708489120391[52] = 0.0;
   out_7992819708489120391[53] = 0.0;
   out_7992819708489120391[54] = 0.0;
   out_7992819708489120391[55] = 0.0;
   out_7992819708489120391[56] = 0.0;
   out_7992819708489120391[57] = 1.0;
   out_7992819708489120391[58] = 0.0;
   out_7992819708489120391[59] = 0.0;
   out_7992819708489120391[60] = 0.0;
   out_7992819708489120391[61] = 0.0;
   out_7992819708489120391[62] = 0.0;
   out_7992819708489120391[63] = 0.0;
   out_7992819708489120391[64] = 0.0;
   out_7992819708489120391[65] = 0.0;
   out_7992819708489120391[66] = 0.0;
   out_7992819708489120391[67] = 0.0;
   out_7992819708489120391[68] = 0.0;
   out_7992819708489120391[69] = 0.0;
   out_7992819708489120391[70] = 0.0;
   out_7992819708489120391[71] = 0.0;
   out_7992819708489120391[72] = 0.0;
   out_7992819708489120391[73] = 0.0;
   out_7992819708489120391[74] = 0.0;
   out_7992819708489120391[75] = 0.0;
   out_7992819708489120391[76] = 1.0;
   out_7992819708489120391[77] = 0.0;
   out_7992819708489120391[78] = 0.0;
   out_7992819708489120391[79] = 0.0;
   out_7992819708489120391[80] = 0.0;
   out_7992819708489120391[81] = 0.0;
   out_7992819708489120391[82] = 0.0;
   out_7992819708489120391[83] = 0.0;
   out_7992819708489120391[84] = 0.0;
   out_7992819708489120391[85] = 0.0;
   out_7992819708489120391[86] = 0.0;
   out_7992819708489120391[87] = 0.0;
   out_7992819708489120391[88] = 0.0;
   out_7992819708489120391[89] = 0.0;
   out_7992819708489120391[90] = 0.0;
   out_7992819708489120391[91] = 0.0;
   out_7992819708489120391[92] = 0.0;
   out_7992819708489120391[93] = 0.0;
   out_7992819708489120391[94] = 0.0;
   out_7992819708489120391[95] = 1.0;
   out_7992819708489120391[96] = 0.0;
   out_7992819708489120391[97] = 0.0;
   out_7992819708489120391[98] = 0.0;
   out_7992819708489120391[99] = 0.0;
   out_7992819708489120391[100] = 0.0;
   out_7992819708489120391[101] = 0.0;
   out_7992819708489120391[102] = 0.0;
   out_7992819708489120391[103] = 0.0;
   out_7992819708489120391[104] = 0.0;
   out_7992819708489120391[105] = 0.0;
   out_7992819708489120391[106] = 0.0;
   out_7992819708489120391[107] = 0.0;
   out_7992819708489120391[108] = 0.0;
   out_7992819708489120391[109] = 0.0;
   out_7992819708489120391[110] = 0.0;
   out_7992819708489120391[111] = 0.0;
   out_7992819708489120391[112] = 0.0;
   out_7992819708489120391[113] = 0.0;
   out_7992819708489120391[114] = 1.0;
   out_7992819708489120391[115] = 0.0;
   out_7992819708489120391[116] = 0.0;
   out_7992819708489120391[117] = 0.0;
   out_7992819708489120391[118] = 0.0;
   out_7992819708489120391[119] = 0.0;
   out_7992819708489120391[120] = 0.0;
   out_7992819708489120391[121] = 0.0;
   out_7992819708489120391[122] = 0.0;
   out_7992819708489120391[123] = 0.0;
   out_7992819708489120391[124] = 0.0;
   out_7992819708489120391[125] = 0.0;
   out_7992819708489120391[126] = 0.0;
   out_7992819708489120391[127] = 0.0;
   out_7992819708489120391[128] = 0.0;
   out_7992819708489120391[129] = 0.0;
   out_7992819708489120391[130] = 0.0;
   out_7992819708489120391[131] = 0.0;
   out_7992819708489120391[132] = 0.0;
   out_7992819708489120391[133] = 1.0;
   out_7992819708489120391[134] = 0.0;
   out_7992819708489120391[135] = 0.0;
   out_7992819708489120391[136] = 0.0;
   out_7992819708489120391[137] = 0.0;
   out_7992819708489120391[138] = 0.0;
   out_7992819708489120391[139] = 0.0;
   out_7992819708489120391[140] = 0.0;
   out_7992819708489120391[141] = 0.0;
   out_7992819708489120391[142] = 0.0;
   out_7992819708489120391[143] = 0.0;
   out_7992819708489120391[144] = 0.0;
   out_7992819708489120391[145] = 0.0;
   out_7992819708489120391[146] = 0.0;
   out_7992819708489120391[147] = 0.0;
   out_7992819708489120391[148] = 0.0;
   out_7992819708489120391[149] = 0.0;
   out_7992819708489120391[150] = 0.0;
   out_7992819708489120391[151] = 0.0;
   out_7992819708489120391[152] = 1.0;
   out_7992819708489120391[153] = 0.0;
   out_7992819708489120391[154] = 0.0;
   out_7992819708489120391[155] = 0.0;
   out_7992819708489120391[156] = 0.0;
   out_7992819708489120391[157] = 0.0;
   out_7992819708489120391[158] = 0.0;
   out_7992819708489120391[159] = 0.0;
   out_7992819708489120391[160] = 0.0;
   out_7992819708489120391[161] = 0.0;
   out_7992819708489120391[162] = 0.0;
   out_7992819708489120391[163] = 0.0;
   out_7992819708489120391[164] = 0.0;
   out_7992819708489120391[165] = 0.0;
   out_7992819708489120391[166] = 0.0;
   out_7992819708489120391[167] = 0.0;
   out_7992819708489120391[168] = 0.0;
   out_7992819708489120391[169] = 0.0;
   out_7992819708489120391[170] = 0.0;
   out_7992819708489120391[171] = 1.0;
   out_7992819708489120391[172] = 0.0;
   out_7992819708489120391[173] = 0.0;
   out_7992819708489120391[174] = 0.0;
   out_7992819708489120391[175] = 0.0;
   out_7992819708489120391[176] = 0.0;
   out_7992819708489120391[177] = 0.0;
   out_7992819708489120391[178] = 0.0;
   out_7992819708489120391[179] = 0.0;
   out_7992819708489120391[180] = 0.0;
   out_7992819708489120391[181] = 0.0;
   out_7992819708489120391[182] = 0.0;
   out_7992819708489120391[183] = 0.0;
   out_7992819708489120391[184] = 0.0;
   out_7992819708489120391[185] = 0.0;
   out_7992819708489120391[186] = 0.0;
   out_7992819708489120391[187] = 0.0;
   out_7992819708489120391[188] = 0.0;
   out_7992819708489120391[189] = 0.0;
   out_7992819708489120391[190] = 1.0;
   out_7992819708489120391[191] = 0.0;
   out_7992819708489120391[192] = 0.0;
   out_7992819708489120391[193] = 0.0;
   out_7992819708489120391[194] = 0.0;
   out_7992819708489120391[195] = 0.0;
   out_7992819708489120391[196] = 0.0;
   out_7992819708489120391[197] = 0.0;
   out_7992819708489120391[198] = 0.0;
   out_7992819708489120391[199] = 0.0;
   out_7992819708489120391[200] = 0.0;
   out_7992819708489120391[201] = 0.0;
   out_7992819708489120391[202] = 0.0;
   out_7992819708489120391[203] = 0.0;
   out_7992819708489120391[204] = 0.0;
   out_7992819708489120391[205] = 0.0;
   out_7992819708489120391[206] = 0.0;
   out_7992819708489120391[207] = 0.0;
   out_7992819708489120391[208] = 0.0;
   out_7992819708489120391[209] = 1.0;
   out_7992819708489120391[210] = 0.0;
   out_7992819708489120391[211] = 0.0;
   out_7992819708489120391[212] = 0.0;
   out_7992819708489120391[213] = 0.0;
   out_7992819708489120391[214] = 0.0;
   out_7992819708489120391[215] = 0.0;
   out_7992819708489120391[216] = 0.0;
   out_7992819708489120391[217] = 0.0;
   out_7992819708489120391[218] = 0.0;
   out_7992819708489120391[219] = 0.0;
   out_7992819708489120391[220] = 0.0;
   out_7992819708489120391[221] = 0.0;
   out_7992819708489120391[222] = 0.0;
   out_7992819708489120391[223] = 0.0;
   out_7992819708489120391[224] = 0.0;
   out_7992819708489120391[225] = 0.0;
   out_7992819708489120391[226] = 0.0;
   out_7992819708489120391[227] = 0.0;
   out_7992819708489120391[228] = 1.0;
   out_7992819708489120391[229] = 0.0;
   out_7992819708489120391[230] = 0.0;
   out_7992819708489120391[231] = 0.0;
   out_7992819708489120391[232] = 0.0;
   out_7992819708489120391[233] = 0.0;
   out_7992819708489120391[234] = 0.0;
   out_7992819708489120391[235] = 0.0;
   out_7992819708489120391[236] = 0.0;
   out_7992819708489120391[237] = 0.0;
   out_7992819708489120391[238] = 0.0;
   out_7992819708489120391[239] = 0.0;
   out_7992819708489120391[240] = 0.0;
   out_7992819708489120391[241] = 0.0;
   out_7992819708489120391[242] = 0.0;
   out_7992819708489120391[243] = 0.0;
   out_7992819708489120391[244] = 0.0;
   out_7992819708489120391[245] = 0.0;
   out_7992819708489120391[246] = 0.0;
   out_7992819708489120391[247] = 1.0;
   out_7992819708489120391[248] = 0.0;
   out_7992819708489120391[249] = 0.0;
   out_7992819708489120391[250] = 0.0;
   out_7992819708489120391[251] = 0.0;
   out_7992819708489120391[252] = 0.0;
   out_7992819708489120391[253] = 0.0;
   out_7992819708489120391[254] = 0.0;
   out_7992819708489120391[255] = 0.0;
   out_7992819708489120391[256] = 0.0;
   out_7992819708489120391[257] = 0.0;
   out_7992819708489120391[258] = 0.0;
   out_7992819708489120391[259] = 0.0;
   out_7992819708489120391[260] = 0.0;
   out_7992819708489120391[261] = 0.0;
   out_7992819708489120391[262] = 0.0;
   out_7992819708489120391[263] = 0.0;
   out_7992819708489120391[264] = 0.0;
   out_7992819708489120391[265] = 0.0;
   out_7992819708489120391[266] = 1.0;
   out_7992819708489120391[267] = 0.0;
   out_7992819708489120391[268] = 0.0;
   out_7992819708489120391[269] = 0.0;
   out_7992819708489120391[270] = 0.0;
   out_7992819708489120391[271] = 0.0;
   out_7992819708489120391[272] = 0.0;
   out_7992819708489120391[273] = 0.0;
   out_7992819708489120391[274] = 0.0;
   out_7992819708489120391[275] = 0.0;
   out_7992819708489120391[276] = 0.0;
   out_7992819708489120391[277] = 0.0;
   out_7992819708489120391[278] = 0.0;
   out_7992819708489120391[279] = 0.0;
   out_7992819708489120391[280] = 0.0;
   out_7992819708489120391[281] = 0.0;
   out_7992819708489120391[282] = 0.0;
   out_7992819708489120391[283] = 0.0;
   out_7992819708489120391[284] = 0.0;
   out_7992819708489120391[285] = 1.0;
   out_7992819708489120391[286] = 0.0;
   out_7992819708489120391[287] = 0.0;
   out_7992819708489120391[288] = 0.0;
   out_7992819708489120391[289] = 0.0;
   out_7992819708489120391[290] = 0.0;
   out_7992819708489120391[291] = 0.0;
   out_7992819708489120391[292] = 0.0;
   out_7992819708489120391[293] = 0.0;
   out_7992819708489120391[294] = 0.0;
   out_7992819708489120391[295] = 0.0;
   out_7992819708489120391[296] = 0.0;
   out_7992819708489120391[297] = 0.0;
   out_7992819708489120391[298] = 0.0;
   out_7992819708489120391[299] = 0.0;
   out_7992819708489120391[300] = 0.0;
   out_7992819708489120391[301] = 0.0;
   out_7992819708489120391[302] = 0.0;
   out_7992819708489120391[303] = 0.0;
   out_7992819708489120391[304] = 1.0;
   out_7992819708489120391[305] = 0.0;
   out_7992819708489120391[306] = 0.0;
   out_7992819708489120391[307] = 0.0;
   out_7992819708489120391[308] = 0.0;
   out_7992819708489120391[309] = 0.0;
   out_7992819708489120391[310] = 0.0;
   out_7992819708489120391[311] = 0.0;
   out_7992819708489120391[312] = 0.0;
   out_7992819708489120391[313] = 0.0;
   out_7992819708489120391[314] = 0.0;
   out_7992819708489120391[315] = 0.0;
   out_7992819708489120391[316] = 0.0;
   out_7992819708489120391[317] = 0.0;
   out_7992819708489120391[318] = 0.0;
   out_7992819708489120391[319] = 0.0;
   out_7992819708489120391[320] = 0.0;
   out_7992819708489120391[321] = 0.0;
   out_7992819708489120391[322] = 0.0;
   out_7992819708489120391[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_4870646846854820424) {
   out_4870646846854820424[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_4870646846854820424[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_4870646846854820424[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_4870646846854820424[3] = dt*state[12] + state[3];
   out_4870646846854820424[4] = dt*state[13] + state[4];
   out_4870646846854820424[5] = dt*state[14] + state[5];
   out_4870646846854820424[6] = state[6];
   out_4870646846854820424[7] = state[7];
   out_4870646846854820424[8] = state[8];
   out_4870646846854820424[9] = state[9];
   out_4870646846854820424[10] = state[10];
   out_4870646846854820424[11] = state[11];
   out_4870646846854820424[12] = state[12];
   out_4870646846854820424[13] = state[13];
   out_4870646846854820424[14] = state[14];
   out_4870646846854820424[15] = state[15];
   out_4870646846854820424[16] = state[16];
   out_4870646846854820424[17] = state[17];
}
void F_fun(double *state, double dt, double *out_8049488768110552288) {
   out_8049488768110552288[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8049488768110552288[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8049488768110552288[2] = 0;
   out_8049488768110552288[3] = 0;
   out_8049488768110552288[4] = 0;
   out_8049488768110552288[5] = 0;
   out_8049488768110552288[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8049488768110552288[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8049488768110552288[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8049488768110552288[9] = 0;
   out_8049488768110552288[10] = 0;
   out_8049488768110552288[11] = 0;
   out_8049488768110552288[12] = 0;
   out_8049488768110552288[13] = 0;
   out_8049488768110552288[14] = 0;
   out_8049488768110552288[15] = 0;
   out_8049488768110552288[16] = 0;
   out_8049488768110552288[17] = 0;
   out_8049488768110552288[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8049488768110552288[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8049488768110552288[20] = 0;
   out_8049488768110552288[21] = 0;
   out_8049488768110552288[22] = 0;
   out_8049488768110552288[23] = 0;
   out_8049488768110552288[24] = 0;
   out_8049488768110552288[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8049488768110552288[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8049488768110552288[27] = 0;
   out_8049488768110552288[28] = 0;
   out_8049488768110552288[29] = 0;
   out_8049488768110552288[30] = 0;
   out_8049488768110552288[31] = 0;
   out_8049488768110552288[32] = 0;
   out_8049488768110552288[33] = 0;
   out_8049488768110552288[34] = 0;
   out_8049488768110552288[35] = 0;
   out_8049488768110552288[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8049488768110552288[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8049488768110552288[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8049488768110552288[39] = 0;
   out_8049488768110552288[40] = 0;
   out_8049488768110552288[41] = 0;
   out_8049488768110552288[42] = 0;
   out_8049488768110552288[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8049488768110552288[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8049488768110552288[45] = 0;
   out_8049488768110552288[46] = 0;
   out_8049488768110552288[47] = 0;
   out_8049488768110552288[48] = 0;
   out_8049488768110552288[49] = 0;
   out_8049488768110552288[50] = 0;
   out_8049488768110552288[51] = 0;
   out_8049488768110552288[52] = 0;
   out_8049488768110552288[53] = 0;
   out_8049488768110552288[54] = 0;
   out_8049488768110552288[55] = 0;
   out_8049488768110552288[56] = 0;
   out_8049488768110552288[57] = 1;
   out_8049488768110552288[58] = 0;
   out_8049488768110552288[59] = 0;
   out_8049488768110552288[60] = 0;
   out_8049488768110552288[61] = 0;
   out_8049488768110552288[62] = 0;
   out_8049488768110552288[63] = 0;
   out_8049488768110552288[64] = 0;
   out_8049488768110552288[65] = 0;
   out_8049488768110552288[66] = dt;
   out_8049488768110552288[67] = 0;
   out_8049488768110552288[68] = 0;
   out_8049488768110552288[69] = 0;
   out_8049488768110552288[70] = 0;
   out_8049488768110552288[71] = 0;
   out_8049488768110552288[72] = 0;
   out_8049488768110552288[73] = 0;
   out_8049488768110552288[74] = 0;
   out_8049488768110552288[75] = 0;
   out_8049488768110552288[76] = 1;
   out_8049488768110552288[77] = 0;
   out_8049488768110552288[78] = 0;
   out_8049488768110552288[79] = 0;
   out_8049488768110552288[80] = 0;
   out_8049488768110552288[81] = 0;
   out_8049488768110552288[82] = 0;
   out_8049488768110552288[83] = 0;
   out_8049488768110552288[84] = 0;
   out_8049488768110552288[85] = dt;
   out_8049488768110552288[86] = 0;
   out_8049488768110552288[87] = 0;
   out_8049488768110552288[88] = 0;
   out_8049488768110552288[89] = 0;
   out_8049488768110552288[90] = 0;
   out_8049488768110552288[91] = 0;
   out_8049488768110552288[92] = 0;
   out_8049488768110552288[93] = 0;
   out_8049488768110552288[94] = 0;
   out_8049488768110552288[95] = 1;
   out_8049488768110552288[96] = 0;
   out_8049488768110552288[97] = 0;
   out_8049488768110552288[98] = 0;
   out_8049488768110552288[99] = 0;
   out_8049488768110552288[100] = 0;
   out_8049488768110552288[101] = 0;
   out_8049488768110552288[102] = 0;
   out_8049488768110552288[103] = 0;
   out_8049488768110552288[104] = dt;
   out_8049488768110552288[105] = 0;
   out_8049488768110552288[106] = 0;
   out_8049488768110552288[107] = 0;
   out_8049488768110552288[108] = 0;
   out_8049488768110552288[109] = 0;
   out_8049488768110552288[110] = 0;
   out_8049488768110552288[111] = 0;
   out_8049488768110552288[112] = 0;
   out_8049488768110552288[113] = 0;
   out_8049488768110552288[114] = 1;
   out_8049488768110552288[115] = 0;
   out_8049488768110552288[116] = 0;
   out_8049488768110552288[117] = 0;
   out_8049488768110552288[118] = 0;
   out_8049488768110552288[119] = 0;
   out_8049488768110552288[120] = 0;
   out_8049488768110552288[121] = 0;
   out_8049488768110552288[122] = 0;
   out_8049488768110552288[123] = 0;
   out_8049488768110552288[124] = 0;
   out_8049488768110552288[125] = 0;
   out_8049488768110552288[126] = 0;
   out_8049488768110552288[127] = 0;
   out_8049488768110552288[128] = 0;
   out_8049488768110552288[129] = 0;
   out_8049488768110552288[130] = 0;
   out_8049488768110552288[131] = 0;
   out_8049488768110552288[132] = 0;
   out_8049488768110552288[133] = 1;
   out_8049488768110552288[134] = 0;
   out_8049488768110552288[135] = 0;
   out_8049488768110552288[136] = 0;
   out_8049488768110552288[137] = 0;
   out_8049488768110552288[138] = 0;
   out_8049488768110552288[139] = 0;
   out_8049488768110552288[140] = 0;
   out_8049488768110552288[141] = 0;
   out_8049488768110552288[142] = 0;
   out_8049488768110552288[143] = 0;
   out_8049488768110552288[144] = 0;
   out_8049488768110552288[145] = 0;
   out_8049488768110552288[146] = 0;
   out_8049488768110552288[147] = 0;
   out_8049488768110552288[148] = 0;
   out_8049488768110552288[149] = 0;
   out_8049488768110552288[150] = 0;
   out_8049488768110552288[151] = 0;
   out_8049488768110552288[152] = 1;
   out_8049488768110552288[153] = 0;
   out_8049488768110552288[154] = 0;
   out_8049488768110552288[155] = 0;
   out_8049488768110552288[156] = 0;
   out_8049488768110552288[157] = 0;
   out_8049488768110552288[158] = 0;
   out_8049488768110552288[159] = 0;
   out_8049488768110552288[160] = 0;
   out_8049488768110552288[161] = 0;
   out_8049488768110552288[162] = 0;
   out_8049488768110552288[163] = 0;
   out_8049488768110552288[164] = 0;
   out_8049488768110552288[165] = 0;
   out_8049488768110552288[166] = 0;
   out_8049488768110552288[167] = 0;
   out_8049488768110552288[168] = 0;
   out_8049488768110552288[169] = 0;
   out_8049488768110552288[170] = 0;
   out_8049488768110552288[171] = 1;
   out_8049488768110552288[172] = 0;
   out_8049488768110552288[173] = 0;
   out_8049488768110552288[174] = 0;
   out_8049488768110552288[175] = 0;
   out_8049488768110552288[176] = 0;
   out_8049488768110552288[177] = 0;
   out_8049488768110552288[178] = 0;
   out_8049488768110552288[179] = 0;
   out_8049488768110552288[180] = 0;
   out_8049488768110552288[181] = 0;
   out_8049488768110552288[182] = 0;
   out_8049488768110552288[183] = 0;
   out_8049488768110552288[184] = 0;
   out_8049488768110552288[185] = 0;
   out_8049488768110552288[186] = 0;
   out_8049488768110552288[187] = 0;
   out_8049488768110552288[188] = 0;
   out_8049488768110552288[189] = 0;
   out_8049488768110552288[190] = 1;
   out_8049488768110552288[191] = 0;
   out_8049488768110552288[192] = 0;
   out_8049488768110552288[193] = 0;
   out_8049488768110552288[194] = 0;
   out_8049488768110552288[195] = 0;
   out_8049488768110552288[196] = 0;
   out_8049488768110552288[197] = 0;
   out_8049488768110552288[198] = 0;
   out_8049488768110552288[199] = 0;
   out_8049488768110552288[200] = 0;
   out_8049488768110552288[201] = 0;
   out_8049488768110552288[202] = 0;
   out_8049488768110552288[203] = 0;
   out_8049488768110552288[204] = 0;
   out_8049488768110552288[205] = 0;
   out_8049488768110552288[206] = 0;
   out_8049488768110552288[207] = 0;
   out_8049488768110552288[208] = 0;
   out_8049488768110552288[209] = 1;
   out_8049488768110552288[210] = 0;
   out_8049488768110552288[211] = 0;
   out_8049488768110552288[212] = 0;
   out_8049488768110552288[213] = 0;
   out_8049488768110552288[214] = 0;
   out_8049488768110552288[215] = 0;
   out_8049488768110552288[216] = 0;
   out_8049488768110552288[217] = 0;
   out_8049488768110552288[218] = 0;
   out_8049488768110552288[219] = 0;
   out_8049488768110552288[220] = 0;
   out_8049488768110552288[221] = 0;
   out_8049488768110552288[222] = 0;
   out_8049488768110552288[223] = 0;
   out_8049488768110552288[224] = 0;
   out_8049488768110552288[225] = 0;
   out_8049488768110552288[226] = 0;
   out_8049488768110552288[227] = 0;
   out_8049488768110552288[228] = 1;
   out_8049488768110552288[229] = 0;
   out_8049488768110552288[230] = 0;
   out_8049488768110552288[231] = 0;
   out_8049488768110552288[232] = 0;
   out_8049488768110552288[233] = 0;
   out_8049488768110552288[234] = 0;
   out_8049488768110552288[235] = 0;
   out_8049488768110552288[236] = 0;
   out_8049488768110552288[237] = 0;
   out_8049488768110552288[238] = 0;
   out_8049488768110552288[239] = 0;
   out_8049488768110552288[240] = 0;
   out_8049488768110552288[241] = 0;
   out_8049488768110552288[242] = 0;
   out_8049488768110552288[243] = 0;
   out_8049488768110552288[244] = 0;
   out_8049488768110552288[245] = 0;
   out_8049488768110552288[246] = 0;
   out_8049488768110552288[247] = 1;
   out_8049488768110552288[248] = 0;
   out_8049488768110552288[249] = 0;
   out_8049488768110552288[250] = 0;
   out_8049488768110552288[251] = 0;
   out_8049488768110552288[252] = 0;
   out_8049488768110552288[253] = 0;
   out_8049488768110552288[254] = 0;
   out_8049488768110552288[255] = 0;
   out_8049488768110552288[256] = 0;
   out_8049488768110552288[257] = 0;
   out_8049488768110552288[258] = 0;
   out_8049488768110552288[259] = 0;
   out_8049488768110552288[260] = 0;
   out_8049488768110552288[261] = 0;
   out_8049488768110552288[262] = 0;
   out_8049488768110552288[263] = 0;
   out_8049488768110552288[264] = 0;
   out_8049488768110552288[265] = 0;
   out_8049488768110552288[266] = 1;
   out_8049488768110552288[267] = 0;
   out_8049488768110552288[268] = 0;
   out_8049488768110552288[269] = 0;
   out_8049488768110552288[270] = 0;
   out_8049488768110552288[271] = 0;
   out_8049488768110552288[272] = 0;
   out_8049488768110552288[273] = 0;
   out_8049488768110552288[274] = 0;
   out_8049488768110552288[275] = 0;
   out_8049488768110552288[276] = 0;
   out_8049488768110552288[277] = 0;
   out_8049488768110552288[278] = 0;
   out_8049488768110552288[279] = 0;
   out_8049488768110552288[280] = 0;
   out_8049488768110552288[281] = 0;
   out_8049488768110552288[282] = 0;
   out_8049488768110552288[283] = 0;
   out_8049488768110552288[284] = 0;
   out_8049488768110552288[285] = 1;
   out_8049488768110552288[286] = 0;
   out_8049488768110552288[287] = 0;
   out_8049488768110552288[288] = 0;
   out_8049488768110552288[289] = 0;
   out_8049488768110552288[290] = 0;
   out_8049488768110552288[291] = 0;
   out_8049488768110552288[292] = 0;
   out_8049488768110552288[293] = 0;
   out_8049488768110552288[294] = 0;
   out_8049488768110552288[295] = 0;
   out_8049488768110552288[296] = 0;
   out_8049488768110552288[297] = 0;
   out_8049488768110552288[298] = 0;
   out_8049488768110552288[299] = 0;
   out_8049488768110552288[300] = 0;
   out_8049488768110552288[301] = 0;
   out_8049488768110552288[302] = 0;
   out_8049488768110552288[303] = 0;
   out_8049488768110552288[304] = 1;
   out_8049488768110552288[305] = 0;
   out_8049488768110552288[306] = 0;
   out_8049488768110552288[307] = 0;
   out_8049488768110552288[308] = 0;
   out_8049488768110552288[309] = 0;
   out_8049488768110552288[310] = 0;
   out_8049488768110552288[311] = 0;
   out_8049488768110552288[312] = 0;
   out_8049488768110552288[313] = 0;
   out_8049488768110552288[314] = 0;
   out_8049488768110552288[315] = 0;
   out_8049488768110552288[316] = 0;
   out_8049488768110552288[317] = 0;
   out_8049488768110552288[318] = 0;
   out_8049488768110552288[319] = 0;
   out_8049488768110552288[320] = 0;
   out_8049488768110552288[321] = 0;
   out_8049488768110552288[322] = 0;
   out_8049488768110552288[323] = 1;
}
void h_4(double *state, double *unused, double *out_4098665418884332867) {
   out_4098665418884332867[0] = state[6] + state[9];
   out_4098665418884332867[1] = state[7] + state[10];
   out_4098665418884332867[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_951541957568700951) {
   out_951541957568700951[0] = 0;
   out_951541957568700951[1] = 0;
   out_951541957568700951[2] = 0;
   out_951541957568700951[3] = 0;
   out_951541957568700951[4] = 0;
   out_951541957568700951[5] = 0;
   out_951541957568700951[6] = 1;
   out_951541957568700951[7] = 0;
   out_951541957568700951[8] = 0;
   out_951541957568700951[9] = 1;
   out_951541957568700951[10] = 0;
   out_951541957568700951[11] = 0;
   out_951541957568700951[12] = 0;
   out_951541957568700951[13] = 0;
   out_951541957568700951[14] = 0;
   out_951541957568700951[15] = 0;
   out_951541957568700951[16] = 0;
   out_951541957568700951[17] = 0;
   out_951541957568700951[18] = 0;
   out_951541957568700951[19] = 0;
   out_951541957568700951[20] = 0;
   out_951541957568700951[21] = 0;
   out_951541957568700951[22] = 0;
   out_951541957568700951[23] = 0;
   out_951541957568700951[24] = 0;
   out_951541957568700951[25] = 1;
   out_951541957568700951[26] = 0;
   out_951541957568700951[27] = 0;
   out_951541957568700951[28] = 1;
   out_951541957568700951[29] = 0;
   out_951541957568700951[30] = 0;
   out_951541957568700951[31] = 0;
   out_951541957568700951[32] = 0;
   out_951541957568700951[33] = 0;
   out_951541957568700951[34] = 0;
   out_951541957568700951[35] = 0;
   out_951541957568700951[36] = 0;
   out_951541957568700951[37] = 0;
   out_951541957568700951[38] = 0;
   out_951541957568700951[39] = 0;
   out_951541957568700951[40] = 0;
   out_951541957568700951[41] = 0;
   out_951541957568700951[42] = 0;
   out_951541957568700951[43] = 0;
   out_951541957568700951[44] = 1;
   out_951541957568700951[45] = 0;
   out_951541957568700951[46] = 0;
   out_951541957568700951[47] = 1;
   out_951541957568700951[48] = 0;
   out_951541957568700951[49] = 0;
   out_951541957568700951[50] = 0;
   out_951541957568700951[51] = 0;
   out_951541957568700951[52] = 0;
   out_951541957568700951[53] = 0;
}
void h_10(double *state, double *unused, double *out_7546292780274681562) {
   out_7546292780274681562[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7546292780274681562[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7546292780274681562[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5588343581066124789) {
   out_5588343581066124789[0] = 0;
   out_5588343581066124789[1] = 9.8100000000000005*cos(state[1]);
   out_5588343581066124789[2] = 0;
   out_5588343581066124789[3] = 0;
   out_5588343581066124789[4] = -state[8];
   out_5588343581066124789[5] = state[7];
   out_5588343581066124789[6] = 0;
   out_5588343581066124789[7] = state[5];
   out_5588343581066124789[8] = -state[4];
   out_5588343581066124789[9] = 0;
   out_5588343581066124789[10] = 0;
   out_5588343581066124789[11] = 0;
   out_5588343581066124789[12] = 1;
   out_5588343581066124789[13] = 0;
   out_5588343581066124789[14] = 0;
   out_5588343581066124789[15] = 1;
   out_5588343581066124789[16] = 0;
   out_5588343581066124789[17] = 0;
   out_5588343581066124789[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5588343581066124789[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5588343581066124789[20] = 0;
   out_5588343581066124789[21] = state[8];
   out_5588343581066124789[22] = 0;
   out_5588343581066124789[23] = -state[6];
   out_5588343581066124789[24] = -state[5];
   out_5588343581066124789[25] = 0;
   out_5588343581066124789[26] = state[3];
   out_5588343581066124789[27] = 0;
   out_5588343581066124789[28] = 0;
   out_5588343581066124789[29] = 0;
   out_5588343581066124789[30] = 0;
   out_5588343581066124789[31] = 1;
   out_5588343581066124789[32] = 0;
   out_5588343581066124789[33] = 0;
   out_5588343581066124789[34] = 1;
   out_5588343581066124789[35] = 0;
   out_5588343581066124789[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5588343581066124789[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5588343581066124789[38] = 0;
   out_5588343581066124789[39] = -state[7];
   out_5588343581066124789[40] = state[6];
   out_5588343581066124789[41] = 0;
   out_5588343581066124789[42] = state[4];
   out_5588343581066124789[43] = -state[3];
   out_5588343581066124789[44] = 0;
   out_5588343581066124789[45] = 0;
   out_5588343581066124789[46] = 0;
   out_5588343581066124789[47] = 0;
   out_5588343581066124789[48] = 0;
   out_5588343581066124789[49] = 0;
   out_5588343581066124789[50] = 1;
   out_5588343581066124789[51] = 0;
   out_5588343581066124789[52] = 0;
   out_5588343581066124789[53] = 1;
}
void h_13(double *state, double *unused, double *out_6823517049288757610) {
   out_6823517049288757610[0] = state[3];
   out_6823517049288757610[1] = state[4];
   out_6823517049288757610[2] = state[5];
}
void H_13(double *state, double *unused, double *out_2260731867763631850) {
   out_2260731867763631850[0] = 0;
   out_2260731867763631850[1] = 0;
   out_2260731867763631850[2] = 0;
   out_2260731867763631850[3] = 1;
   out_2260731867763631850[4] = 0;
   out_2260731867763631850[5] = 0;
   out_2260731867763631850[6] = 0;
   out_2260731867763631850[7] = 0;
   out_2260731867763631850[8] = 0;
   out_2260731867763631850[9] = 0;
   out_2260731867763631850[10] = 0;
   out_2260731867763631850[11] = 0;
   out_2260731867763631850[12] = 0;
   out_2260731867763631850[13] = 0;
   out_2260731867763631850[14] = 0;
   out_2260731867763631850[15] = 0;
   out_2260731867763631850[16] = 0;
   out_2260731867763631850[17] = 0;
   out_2260731867763631850[18] = 0;
   out_2260731867763631850[19] = 0;
   out_2260731867763631850[20] = 0;
   out_2260731867763631850[21] = 0;
   out_2260731867763631850[22] = 1;
   out_2260731867763631850[23] = 0;
   out_2260731867763631850[24] = 0;
   out_2260731867763631850[25] = 0;
   out_2260731867763631850[26] = 0;
   out_2260731867763631850[27] = 0;
   out_2260731867763631850[28] = 0;
   out_2260731867763631850[29] = 0;
   out_2260731867763631850[30] = 0;
   out_2260731867763631850[31] = 0;
   out_2260731867763631850[32] = 0;
   out_2260731867763631850[33] = 0;
   out_2260731867763631850[34] = 0;
   out_2260731867763631850[35] = 0;
   out_2260731867763631850[36] = 0;
   out_2260731867763631850[37] = 0;
   out_2260731867763631850[38] = 0;
   out_2260731867763631850[39] = 0;
   out_2260731867763631850[40] = 0;
   out_2260731867763631850[41] = 1;
   out_2260731867763631850[42] = 0;
   out_2260731867763631850[43] = 0;
   out_2260731867763631850[44] = 0;
   out_2260731867763631850[45] = 0;
   out_2260731867763631850[46] = 0;
   out_2260731867763631850[47] = 0;
   out_2260731867763631850[48] = 0;
   out_2260731867763631850[49] = 0;
   out_2260731867763631850[50] = 0;
   out_2260731867763631850[51] = 0;
   out_2260731867763631850[52] = 0;
   out_2260731867763631850[53] = 0;
}
void h_14(double *state, double *unused, double *out_4599078664097125310) {
   out_4599078664097125310[0] = state[6];
   out_4599078664097125310[1] = state[7];
   out_4599078664097125310[2] = state[8];
}
void H_14(double *state, double *unused, double *out_1386658484213584550) {
   out_1386658484213584550[0] = 0;
   out_1386658484213584550[1] = 0;
   out_1386658484213584550[2] = 0;
   out_1386658484213584550[3] = 0;
   out_1386658484213584550[4] = 0;
   out_1386658484213584550[5] = 0;
   out_1386658484213584550[6] = 1;
   out_1386658484213584550[7] = 0;
   out_1386658484213584550[8] = 0;
   out_1386658484213584550[9] = 0;
   out_1386658484213584550[10] = 0;
   out_1386658484213584550[11] = 0;
   out_1386658484213584550[12] = 0;
   out_1386658484213584550[13] = 0;
   out_1386658484213584550[14] = 0;
   out_1386658484213584550[15] = 0;
   out_1386658484213584550[16] = 0;
   out_1386658484213584550[17] = 0;
   out_1386658484213584550[18] = 0;
   out_1386658484213584550[19] = 0;
   out_1386658484213584550[20] = 0;
   out_1386658484213584550[21] = 0;
   out_1386658484213584550[22] = 0;
   out_1386658484213584550[23] = 0;
   out_1386658484213584550[24] = 0;
   out_1386658484213584550[25] = 1;
   out_1386658484213584550[26] = 0;
   out_1386658484213584550[27] = 0;
   out_1386658484213584550[28] = 0;
   out_1386658484213584550[29] = 0;
   out_1386658484213584550[30] = 0;
   out_1386658484213584550[31] = 0;
   out_1386658484213584550[32] = 0;
   out_1386658484213584550[33] = 0;
   out_1386658484213584550[34] = 0;
   out_1386658484213584550[35] = 0;
   out_1386658484213584550[36] = 0;
   out_1386658484213584550[37] = 0;
   out_1386658484213584550[38] = 0;
   out_1386658484213584550[39] = 0;
   out_1386658484213584550[40] = 0;
   out_1386658484213584550[41] = 0;
   out_1386658484213584550[42] = 0;
   out_1386658484213584550[43] = 0;
   out_1386658484213584550[44] = 1;
   out_1386658484213584550[45] = 0;
   out_1386658484213584550[46] = 0;
   out_1386658484213584550[47] = 0;
   out_1386658484213584550[48] = 0;
   out_1386658484213584550[49] = 0;
   out_1386658484213584550[50] = 0;
   out_1386658484213584550[51] = 0;
   out_1386658484213584550[52] = 0;
   out_1386658484213584550[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_8460591430937244758) {
  err_fun(nom_x, delta_x, out_8460591430937244758);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2176108056044053025) {
  inv_err_fun(nom_x, true_x, out_2176108056044053025);
}
void pose_H_mod_fun(double *state, double *out_7992819708489120391) {
  H_mod_fun(state, out_7992819708489120391);
}
void pose_f_fun(double *state, double dt, double *out_4870646846854820424) {
  f_fun(state,  dt, out_4870646846854820424);
}
void pose_F_fun(double *state, double dt, double *out_8049488768110552288) {
  F_fun(state,  dt, out_8049488768110552288);
}
void pose_h_4(double *state, double *unused, double *out_4098665418884332867) {
  h_4(state, unused, out_4098665418884332867);
}
void pose_H_4(double *state, double *unused, double *out_951541957568700951) {
  H_4(state, unused, out_951541957568700951);
}
void pose_h_10(double *state, double *unused, double *out_7546292780274681562) {
  h_10(state, unused, out_7546292780274681562);
}
void pose_H_10(double *state, double *unused, double *out_5588343581066124789) {
  H_10(state, unused, out_5588343581066124789);
}
void pose_h_13(double *state, double *unused, double *out_6823517049288757610) {
  h_13(state, unused, out_6823517049288757610);
}
void pose_H_13(double *state, double *unused, double *out_2260731867763631850) {
  H_13(state, unused, out_2260731867763631850);
}
void pose_h_14(double *state, double *unused, double *out_4599078664097125310) {
  h_14(state, unused, out_4599078664097125310);
}
void pose_H_14(double *state, double *unused, double *out_1386658484213584550) {
  H_14(state, unused, out_1386658484213584550);
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
