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
void err_fun(double *nom_x, double *delta_x, double *out_8919129796448948883) {
   out_8919129796448948883[0] = delta_x[0] + nom_x[0];
   out_8919129796448948883[1] = delta_x[1] + nom_x[1];
   out_8919129796448948883[2] = delta_x[2] + nom_x[2];
   out_8919129796448948883[3] = delta_x[3] + nom_x[3];
   out_8919129796448948883[4] = delta_x[4] + nom_x[4];
   out_8919129796448948883[5] = delta_x[5] + nom_x[5];
   out_8919129796448948883[6] = delta_x[6] + nom_x[6];
   out_8919129796448948883[7] = delta_x[7] + nom_x[7];
   out_8919129796448948883[8] = delta_x[8] + nom_x[8];
   out_8919129796448948883[9] = delta_x[9] + nom_x[9];
   out_8919129796448948883[10] = delta_x[10] + nom_x[10];
   out_8919129796448948883[11] = delta_x[11] + nom_x[11];
   out_8919129796448948883[12] = delta_x[12] + nom_x[12];
   out_8919129796448948883[13] = delta_x[13] + nom_x[13];
   out_8919129796448948883[14] = delta_x[14] + nom_x[14];
   out_8919129796448948883[15] = delta_x[15] + nom_x[15];
   out_8919129796448948883[16] = delta_x[16] + nom_x[16];
   out_8919129796448948883[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5759862885670035392) {
   out_5759862885670035392[0] = -nom_x[0] + true_x[0];
   out_5759862885670035392[1] = -nom_x[1] + true_x[1];
   out_5759862885670035392[2] = -nom_x[2] + true_x[2];
   out_5759862885670035392[3] = -nom_x[3] + true_x[3];
   out_5759862885670035392[4] = -nom_x[4] + true_x[4];
   out_5759862885670035392[5] = -nom_x[5] + true_x[5];
   out_5759862885670035392[6] = -nom_x[6] + true_x[6];
   out_5759862885670035392[7] = -nom_x[7] + true_x[7];
   out_5759862885670035392[8] = -nom_x[8] + true_x[8];
   out_5759862885670035392[9] = -nom_x[9] + true_x[9];
   out_5759862885670035392[10] = -nom_x[10] + true_x[10];
   out_5759862885670035392[11] = -nom_x[11] + true_x[11];
   out_5759862885670035392[12] = -nom_x[12] + true_x[12];
   out_5759862885670035392[13] = -nom_x[13] + true_x[13];
   out_5759862885670035392[14] = -nom_x[14] + true_x[14];
   out_5759862885670035392[15] = -nom_x[15] + true_x[15];
   out_5759862885670035392[16] = -nom_x[16] + true_x[16];
   out_5759862885670035392[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_7065747119210693848) {
   out_7065747119210693848[0] = 1.0;
   out_7065747119210693848[1] = 0.0;
   out_7065747119210693848[2] = 0.0;
   out_7065747119210693848[3] = 0.0;
   out_7065747119210693848[4] = 0.0;
   out_7065747119210693848[5] = 0.0;
   out_7065747119210693848[6] = 0.0;
   out_7065747119210693848[7] = 0.0;
   out_7065747119210693848[8] = 0.0;
   out_7065747119210693848[9] = 0.0;
   out_7065747119210693848[10] = 0.0;
   out_7065747119210693848[11] = 0.0;
   out_7065747119210693848[12] = 0.0;
   out_7065747119210693848[13] = 0.0;
   out_7065747119210693848[14] = 0.0;
   out_7065747119210693848[15] = 0.0;
   out_7065747119210693848[16] = 0.0;
   out_7065747119210693848[17] = 0.0;
   out_7065747119210693848[18] = 0.0;
   out_7065747119210693848[19] = 1.0;
   out_7065747119210693848[20] = 0.0;
   out_7065747119210693848[21] = 0.0;
   out_7065747119210693848[22] = 0.0;
   out_7065747119210693848[23] = 0.0;
   out_7065747119210693848[24] = 0.0;
   out_7065747119210693848[25] = 0.0;
   out_7065747119210693848[26] = 0.0;
   out_7065747119210693848[27] = 0.0;
   out_7065747119210693848[28] = 0.0;
   out_7065747119210693848[29] = 0.0;
   out_7065747119210693848[30] = 0.0;
   out_7065747119210693848[31] = 0.0;
   out_7065747119210693848[32] = 0.0;
   out_7065747119210693848[33] = 0.0;
   out_7065747119210693848[34] = 0.0;
   out_7065747119210693848[35] = 0.0;
   out_7065747119210693848[36] = 0.0;
   out_7065747119210693848[37] = 0.0;
   out_7065747119210693848[38] = 1.0;
   out_7065747119210693848[39] = 0.0;
   out_7065747119210693848[40] = 0.0;
   out_7065747119210693848[41] = 0.0;
   out_7065747119210693848[42] = 0.0;
   out_7065747119210693848[43] = 0.0;
   out_7065747119210693848[44] = 0.0;
   out_7065747119210693848[45] = 0.0;
   out_7065747119210693848[46] = 0.0;
   out_7065747119210693848[47] = 0.0;
   out_7065747119210693848[48] = 0.0;
   out_7065747119210693848[49] = 0.0;
   out_7065747119210693848[50] = 0.0;
   out_7065747119210693848[51] = 0.0;
   out_7065747119210693848[52] = 0.0;
   out_7065747119210693848[53] = 0.0;
   out_7065747119210693848[54] = 0.0;
   out_7065747119210693848[55] = 0.0;
   out_7065747119210693848[56] = 0.0;
   out_7065747119210693848[57] = 1.0;
   out_7065747119210693848[58] = 0.0;
   out_7065747119210693848[59] = 0.0;
   out_7065747119210693848[60] = 0.0;
   out_7065747119210693848[61] = 0.0;
   out_7065747119210693848[62] = 0.0;
   out_7065747119210693848[63] = 0.0;
   out_7065747119210693848[64] = 0.0;
   out_7065747119210693848[65] = 0.0;
   out_7065747119210693848[66] = 0.0;
   out_7065747119210693848[67] = 0.0;
   out_7065747119210693848[68] = 0.0;
   out_7065747119210693848[69] = 0.0;
   out_7065747119210693848[70] = 0.0;
   out_7065747119210693848[71] = 0.0;
   out_7065747119210693848[72] = 0.0;
   out_7065747119210693848[73] = 0.0;
   out_7065747119210693848[74] = 0.0;
   out_7065747119210693848[75] = 0.0;
   out_7065747119210693848[76] = 1.0;
   out_7065747119210693848[77] = 0.0;
   out_7065747119210693848[78] = 0.0;
   out_7065747119210693848[79] = 0.0;
   out_7065747119210693848[80] = 0.0;
   out_7065747119210693848[81] = 0.0;
   out_7065747119210693848[82] = 0.0;
   out_7065747119210693848[83] = 0.0;
   out_7065747119210693848[84] = 0.0;
   out_7065747119210693848[85] = 0.0;
   out_7065747119210693848[86] = 0.0;
   out_7065747119210693848[87] = 0.0;
   out_7065747119210693848[88] = 0.0;
   out_7065747119210693848[89] = 0.0;
   out_7065747119210693848[90] = 0.0;
   out_7065747119210693848[91] = 0.0;
   out_7065747119210693848[92] = 0.0;
   out_7065747119210693848[93] = 0.0;
   out_7065747119210693848[94] = 0.0;
   out_7065747119210693848[95] = 1.0;
   out_7065747119210693848[96] = 0.0;
   out_7065747119210693848[97] = 0.0;
   out_7065747119210693848[98] = 0.0;
   out_7065747119210693848[99] = 0.0;
   out_7065747119210693848[100] = 0.0;
   out_7065747119210693848[101] = 0.0;
   out_7065747119210693848[102] = 0.0;
   out_7065747119210693848[103] = 0.0;
   out_7065747119210693848[104] = 0.0;
   out_7065747119210693848[105] = 0.0;
   out_7065747119210693848[106] = 0.0;
   out_7065747119210693848[107] = 0.0;
   out_7065747119210693848[108] = 0.0;
   out_7065747119210693848[109] = 0.0;
   out_7065747119210693848[110] = 0.0;
   out_7065747119210693848[111] = 0.0;
   out_7065747119210693848[112] = 0.0;
   out_7065747119210693848[113] = 0.0;
   out_7065747119210693848[114] = 1.0;
   out_7065747119210693848[115] = 0.0;
   out_7065747119210693848[116] = 0.0;
   out_7065747119210693848[117] = 0.0;
   out_7065747119210693848[118] = 0.0;
   out_7065747119210693848[119] = 0.0;
   out_7065747119210693848[120] = 0.0;
   out_7065747119210693848[121] = 0.0;
   out_7065747119210693848[122] = 0.0;
   out_7065747119210693848[123] = 0.0;
   out_7065747119210693848[124] = 0.0;
   out_7065747119210693848[125] = 0.0;
   out_7065747119210693848[126] = 0.0;
   out_7065747119210693848[127] = 0.0;
   out_7065747119210693848[128] = 0.0;
   out_7065747119210693848[129] = 0.0;
   out_7065747119210693848[130] = 0.0;
   out_7065747119210693848[131] = 0.0;
   out_7065747119210693848[132] = 0.0;
   out_7065747119210693848[133] = 1.0;
   out_7065747119210693848[134] = 0.0;
   out_7065747119210693848[135] = 0.0;
   out_7065747119210693848[136] = 0.0;
   out_7065747119210693848[137] = 0.0;
   out_7065747119210693848[138] = 0.0;
   out_7065747119210693848[139] = 0.0;
   out_7065747119210693848[140] = 0.0;
   out_7065747119210693848[141] = 0.0;
   out_7065747119210693848[142] = 0.0;
   out_7065747119210693848[143] = 0.0;
   out_7065747119210693848[144] = 0.0;
   out_7065747119210693848[145] = 0.0;
   out_7065747119210693848[146] = 0.0;
   out_7065747119210693848[147] = 0.0;
   out_7065747119210693848[148] = 0.0;
   out_7065747119210693848[149] = 0.0;
   out_7065747119210693848[150] = 0.0;
   out_7065747119210693848[151] = 0.0;
   out_7065747119210693848[152] = 1.0;
   out_7065747119210693848[153] = 0.0;
   out_7065747119210693848[154] = 0.0;
   out_7065747119210693848[155] = 0.0;
   out_7065747119210693848[156] = 0.0;
   out_7065747119210693848[157] = 0.0;
   out_7065747119210693848[158] = 0.0;
   out_7065747119210693848[159] = 0.0;
   out_7065747119210693848[160] = 0.0;
   out_7065747119210693848[161] = 0.0;
   out_7065747119210693848[162] = 0.0;
   out_7065747119210693848[163] = 0.0;
   out_7065747119210693848[164] = 0.0;
   out_7065747119210693848[165] = 0.0;
   out_7065747119210693848[166] = 0.0;
   out_7065747119210693848[167] = 0.0;
   out_7065747119210693848[168] = 0.0;
   out_7065747119210693848[169] = 0.0;
   out_7065747119210693848[170] = 0.0;
   out_7065747119210693848[171] = 1.0;
   out_7065747119210693848[172] = 0.0;
   out_7065747119210693848[173] = 0.0;
   out_7065747119210693848[174] = 0.0;
   out_7065747119210693848[175] = 0.0;
   out_7065747119210693848[176] = 0.0;
   out_7065747119210693848[177] = 0.0;
   out_7065747119210693848[178] = 0.0;
   out_7065747119210693848[179] = 0.0;
   out_7065747119210693848[180] = 0.0;
   out_7065747119210693848[181] = 0.0;
   out_7065747119210693848[182] = 0.0;
   out_7065747119210693848[183] = 0.0;
   out_7065747119210693848[184] = 0.0;
   out_7065747119210693848[185] = 0.0;
   out_7065747119210693848[186] = 0.0;
   out_7065747119210693848[187] = 0.0;
   out_7065747119210693848[188] = 0.0;
   out_7065747119210693848[189] = 0.0;
   out_7065747119210693848[190] = 1.0;
   out_7065747119210693848[191] = 0.0;
   out_7065747119210693848[192] = 0.0;
   out_7065747119210693848[193] = 0.0;
   out_7065747119210693848[194] = 0.0;
   out_7065747119210693848[195] = 0.0;
   out_7065747119210693848[196] = 0.0;
   out_7065747119210693848[197] = 0.0;
   out_7065747119210693848[198] = 0.0;
   out_7065747119210693848[199] = 0.0;
   out_7065747119210693848[200] = 0.0;
   out_7065747119210693848[201] = 0.0;
   out_7065747119210693848[202] = 0.0;
   out_7065747119210693848[203] = 0.0;
   out_7065747119210693848[204] = 0.0;
   out_7065747119210693848[205] = 0.0;
   out_7065747119210693848[206] = 0.0;
   out_7065747119210693848[207] = 0.0;
   out_7065747119210693848[208] = 0.0;
   out_7065747119210693848[209] = 1.0;
   out_7065747119210693848[210] = 0.0;
   out_7065747119210693848[211] = 0.0;
   out_7065747119210693848[212] = 0.0;
   out_7065747119210693848[213] = 0.0;
   out_7065747119210693848[214] = 0.0;
   out_7065747119210693848[215] = 0.0;
   out_7065747119210693848[216] = 0.0;
   out_7065747119210693848[217] = 0.0;
   out_7065747119210693848[218] = 0.0;
   out_7065747119210693848[219] = 0.0;
   out_7065747119210693848[220] = 0.0;
   out_7065747119210693848[221] = 0.0;
   out_7065747119210693848[222] = 0.0;
   out_7065747119210693848[223] = 0.0;
   out_7065747119210693848[224] = 0.0;
   out_7065747119210693848[225] = 0.0;
   out_7065747119210693848[226] = 0.0;
   out_7065747119210693848[227] = 0.0;
   out_7065747119210693848[228] = 1.0;
   out_7065747119210693848[229] = 0.0;
   out_7065747119210693848[230] = 0.0;
   out_7065747119210693848[231] = 0.0;
   out_7065747119210693848[232] = 0.0;
   out_7065747119210693848[233] = 0.0;
   out_7065747119210693848[234] = 0.0;
   out_7065747119210693848[235] = 0.0;
   out_7065747119210693848[236] = 0.0;
   out_7065747119210693848[237] = 0.0;
   out_7065747119210693848[238] = 0.0;
   out_7065747119210693848[239] = 0.0;
   out_7065747119210693848[240] = 0.0;
   out_7065747119210693848[241] = 0.0;
   out_7065747119210693848[242] = 0.0;
   out_7065747119210693848[243] = 0.0;
   out_7065747119210693848[244] = 0.0;
   out_7065747119210693848[245] = 0.0;
   out_7065747119210693848[246] = 0.0;
   out_7065747119210693848[247] = 1.0;
   out_7065747119210693848[248] = 0.0;
   out_7065747119210693848[249] = 0.0;
   out_7065747119210693848[250] = 0.0;
   out_7065747119210693848[251] = 0.0;
   out_7065747119210693848[252] = 0.0;
   out_7065747119210693848[253] = 0.0;
   out_7065747119210693848[254] = 0.0;
   out_7065747119210693848[255] = 0.0;
   out_7065747119210693848[256] = 0.0;
   out_7065747119210693848[257] = 0.0;
   out_7065747119210693848[258] = 0.0;
   out_7065747119210693848[259] = 0.0;
   out_7065747119210693848[260] = 0.0;
   out_7065747119210693848[261] = 0.0;
   out_7065747119210693848[262] = 0.0;
   out_7065747119210693848[263] = 0.0;
   out_7065747119210693848[264] = 0.0;
   out_7065747119210693848[265] = 0.0;
   out_7065747119210693848[266] = 1.0;
   out_7065747119210693848[267] = 0.0;
   out_7065747119210693848[268] = 0.0;
   out_7065747119210693848[269] = 0.0;
   out_7065747119210693848[270] = 0.0;
   out_7065747119210693848[271] = 0.0;
   out_7065747119210693848[272] = 0.0;
   out_7065747119210693848[273] = 0.0;
   out_7065747119210693848[274] = 0.0;
   out_7065747119210693848[275] = 0.0;
   out_7065747119210693848[276] = 0.0;
   out_7065747119210693848[277] = 0.0;
   out_7065747119210693848[278] = 0.0;
   out_7065747119210693848[279] = 0.0;
   out_7065747119210693848[280] = 0.0;
   out_7065747119210693848[281] = 0.0;
   out_7065747119210693848[282] = 0.0;
   out_7065747119210693848[283] = 0.0;
   out_7065747119210693848[284] = 0.0;
   out_7065747119210693848[285] = 1.0;
   out_7065747119210693848[286] = 0.0;
   out_7065747119210693848[287] = 0.0;
   out_7065747119210693848[288] = 0.0;
   out_7065747119210693848[289] = 0.0;
   out_7065747119210693848[290] = 0.0;
   out_7065747119210693848[291] = 0.0;
   out_7065747119210693848[292] = 0.0;
   out_7065747119210693848[293] = 0.0;
   out_7065747119210693848[294] = 0.0;
   out_7065747119210693848[295] = 0.0;
   out_7065747119210693848[296] = 0.0;
   out_7065747119210693848[297] = 0.0;
   out_7065747119210693848[298] = 0.0;
   out_7065747119210693848[299] = 0.0;
   out_7065747119210693848[300] = 0.0;
   out_7065747119210693848[301] = 0.0;
   out_7065747119210693848[302] = 0.0;
   out_7065747119210693848[303] = 0.0;
   out_7065747119210693848[304] = 1.0;
   out_7065747119210693848[305] = 0.0;
   out_7065747119210693848[306] = 0.0;
   out_7065747119210693848[307] = 0.0;
   out_7065747119210693848[308] = 0.0;
   out_7065747119210693848[309] = 0.0;
   out_7065747119210693848[310] = 0.0;
   out_7065747119210693848[311] = 0.0;
   out_7065747119210693848[312] = 0.0;
   out_7065747119210693848[313] = 0.0;
   out_7065747119210693848[314] = 0.0;
   out_7065747119210693848[315] = 0.0;
   out_7065747119210693848[316] = 0.0;
   out_7065747119210693848[317] = 0.0;
   out_7065747119210693848[318] = 0.0;
   out_7065747119210693848[319] = 0.0;
   out_7065747119210693848[320] = 0.0;
   out_7065747119210693848[321] = 0.0;
   out_7065747119210693848[322] = 0.0;
   out_7065747119210693848[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_558603448437855289) {
   out_558603448437855289[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_558603448437855289[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_558603448437855289[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_558603448437855289[3] = dt*state[12] + state[3];
   out_558603448437855289[4] = dt*state[13] + state[4];
   out_558603448437855289[5] = dt*state[14] + state[5];
   out_558603448437855289[6] = state[6];
   out_558603448437855289[7] = state[7];
   out_558603448437855289[8] = state[8];
   out_558603448437855289[9] = state[9];
   out_558603448437855289[10] = state[10];
   out_558603448437855289[11] = state[11];
   out_558603448437855289[12] = state[12];
   out_558603448437855289[13] = state[13];
   out_558603448437855289[14] = state[14];
   out_558603448437855289[15] = state[15];
   out_558603448437855289[16] = state[16];
   out_558603448437855289[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1495718385613388763) {
   out_1495718385613388763[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1495718385613388763[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1495718385613388763[2] = 0;
   out_1495718385613388763[3] = 0;
   out_1495718385613388763[4] = 0;
   out_1495718385613388763[5] = 0;
   out_1495718385613388763[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1495718385613388763[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1495718385613388763[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1495718385613388763[9] = 0;
   out_1495718385613388763[10] = 0;
   out_1495718385613388763[11] = 0;
   out_1495718385613388763[12] = 0;
   out_1495718385613388763[13] = 0;
   out_1495718385613388763[14] = 0;
   out_1495718385613388763[15] = 0;
   out_1495718385613388763[16] = 0;
   out_1495718385613388763[17] = 0;
   out_1495718385613388763[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1495718385613388763[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1495718385613388763[20] = 0;
   out_1495718385613388763[21] = 0;
   out_1495718385613388763[22] = 0;
   out_1495718385613388763[23] = 0;
   out_1495718385613388763[24] = 0;
   out_1495718385613388763[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1495718385613388763[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1495718385613388763[27] = 0;
   out_1495718385613388763[28] = 0;
   out_1495718385613388763[29] = 0;
   out_1495718385613388763[30] = 0;
   out_1495718385613388763[31] = 0;
   out_1495718385613388763[32] = 0;
   out_1495718385613388763[33] = 0;
   out_1495718385613388763[34] = 0;
   out_1495718385613388763[35] = 0;
   out_1495718385613388763[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1495718385613388763[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1495718385613388763[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1495718385613388763[39] = 0;
   out_1495718385613388763[40] = 0;
   out_1495718385613388763[41] = 0;
   out_1495718385613388763[42] = 0;
   out_1495718385613388763[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1495718385613388763[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1495718385613388763[45] = 0;
   out_1495718385613388763[46] = 0;
   out_1495718385613388763[47] = 0;
   out_1495718385613388763[48] = 0;
   out_1495718385613388763[49] = 0;
   out_1495718385613388763[50] = 0;
   out_1495718385613388763[51] = 0;
   out_1495718385613388763[52] = 0;
   out_1495718385613388763[53] = 0;
   out_1495718385613388763[54] = 0;
   out_1495718385613388763[55] = 0;
   out_1495718385613388763[56] = 0;
   out_1495718385613388763[57] = 1;
   out_1495718385613388763[58] = 0;
   out_1495718385613388763[59] = 0;
   out_1495718385613388763[60] = 0;
   out_1495718385613388763[61] = 0;
   out_1495718385613388763[62] = 0;
   out_1495718385613388763[63] = 0;
   out_1495718385613388763[64] = 0;
   out_1495718385613388763[65] = 0;
   out_1495718385613388763[66] = dt;
   out_1495718385613388763[67] = 0;
   out_1495718385613388763[68] = 0;
   out_1495718385613388763[69] = 0;
   out_1495718385613388763[70] = 0;
   out_1495718385613388763[71] = 0;
   out_1495718385613388763[72] = 0;
   out_1495718385613388763[73] = 0;
   out_1495718385613388763[74] = 0;
   out_1495718385613388763[75] = 0;
   out_1495718385613388763[76] = 1;
   out_1495718385613388763[77] = 0;
   out_1495718385613388763[78] = 0;
   out_1495718385613388763[79] = 0;
   out_1495718385613388763[80] = 0;
   out_1495718385613388763[81] = 0;
   out_1495718385613388763[82] = 0;
   out_1495718385613388763[83] = 0;
   out_1495718385613388763[84] = 0;
   out_1495718385613388763[85] = dt;
   out_1495718385613388763[86] = 0;
   out_1495718385613388763[87] = 0;
   out_1495718385613388763[88] = 0;
   out_1495718385613388763[89] = 0;
   out_1495718385613388763[90] = 0;
   out_1495718385613388763[91] = 0;
   out_1495718385613388763[92] = 0;
   out_1495718385613388763[93] = 0;
   out_1495718385613388763[94] = 0;
   out_1495718385613388763[95] = 1;
   out_1495718385613388763[96] = 0;
   out_1495718385613388763[97] = 0;
   out_1495718385613388763[98] = 0;
   out_1495718385613388763[99] = 0;
   out_1495718385613388763[100] = 0;
   out_1495718385613388763[101] = 0;
   out_1495718385613388763[102] = 0;
   out_1495718385613388763[103] = 0;
   out_1495718385613388763[104] = dt;
   out_1495718385613388763[105] = 0;
   out_1495718385613388763[106] = 0;
   out_1495718385613388763[107] = 0;
   out_1495718385613388763[108] = 0;
   out_1495718385613388763[109] = 0;
   out_1495718385613388763[110] = 0;
   out_1495718385613388763[111] = 0;
   out_1495718385613388763[112] = 0;
   out_1495718385613388763[113] = 0;
   out_1495718385613388763[114] = 1;
   out_1495718385613388763[115] = 0;
   out_1495718385613388763[116] = 0;
   out_1495718385613388763[117] = 0;
   out_1495718385613388763[118] = 0;
   out_1495718385613388763[119] = 0;
   out_1495718385613388763[120] = 0;
   out_1495718385613388763[121] = 0;
   out_1495718385613388763[122] = 0;
   out_1495718385613388763[123] = 0;
   out_1495718385613388763[124] = 0;
   out_1495718385613388763[125] = 0;
   out_1495718385613388763[126] = 0;
   out_1495718385613388763[127] = 0;
   out_1495718385613388763[128] = 0;
   out_1495718385613388763[129] = 0;
   out_1495718385613388763[130] = 0;
   out_1495718385613388763[131] = 0;
   out_1495718385613388763[132] = 0;
   out_1495718385613388763[133] = 1;
   out_1495718385613388763[134] = 0;
   out_1495718385613388763[135] = 0;
   out_1495718385613388763[136] = 0;
   out_1495718385613388763[137] = 0;
   out_1495718385613388763[138] = 0;
   out_1495718385613388763[139] = 0;
   out_1495718385613388763[140] = 0;
   out_1495718385613388763[141] = 0;
   out_1495718385613388763[142] = 0;
   out_1495718385613388763[143] = 0;
   out_1495718385613388763[144] = 0;
   out_1495718385613388763[145] = 0;
   out_1495718385613388763[146] = 0;
   out_1495718385613388763[147] = 0;
   out_1495718385613388763[148] = 0;
   out_1495718385613388763[149] = 0;
   out_1495718385613388763[150] = 0;
   out_1495718385613388763[151] = 0;
   out_1495718385613388763[152] = 1;
   out_1495718385613388763[153] = 0;
   out_1495718385613388763[154] = 0;
   out_1495718385613388763[155] = 0;
   out_1495718385613388763[156] = 0;
   out_1495718385613388763[157] = 0;
   out_1495718385613388763[158] = 0;
   out_1495718385613388763[159] = 0;
   out_1495718385613388763[160] = 0;
   out_1495718385613388763[161] = 0;
   out_1495718385613388763[162] = 0;
   out_1495718385613388763[163] = 0;
   out_1495718385613388763[164] = 0;
   out_1495718385613388763[165] = 0;
   out_1495718385613388763[166] = 0;
   out_1495718385613388763[167] = 0;
   out_1495718385613388763[168] = 0;
   out_1495718385613388763[169] = 0;
   out_1495718385613388763[170] = 0;
   out_1495718385613388763[171] = 1;
   out_1495718385613388763[172] = 0;
   out_1495718385613388763[173] = 0;
   out_1495718385613388763[174] = 0;
   out_1495718385613388763[175] = 0;
   out_1495718385613388763[176] = 0;
   out_1495718385613388763[177] = 0;
   out_1495718385613388763[178] = 0;
   out_1495718385613388763[179] = 0;
   out_1495718385613388763[180] = 0;
   out_1495718385613388763[181] = 0;
   out_1495718385613388763[182] = 0;
   out_1495718385613388763[183] = 0;
   out_1495718385613388763[184] = 0;
   out_1495718385613388763[185] = 0;
   out_1495718385613388763[186] = 0;
   out_1495718385613388763[187] = 0;
   out_1495718385613388763[188] = 0;
   out_1495718385613388763[189] = 0;
   out_1495718385613388763[190] = 1;
   out_1495718385613388763[191] = 0;
   out_1495718385613388763[192] = 0;
   out_1495718385613388763[193] = 0;
   out_1495718385613388763[194] = 0;
   out_1495718385613388763[195] = 0;
   out_1495718385613388763[196] = 0;
   out_1495718385613388763[197] = 0;
   out_1495718385613388763[198] = 0;
   out_1495718385613388763[199] = 0;
   out_1495718385613388763[200] = 0;
   out_1495718385613388763[201] = 0;
   out_1495718385613388763[202] = 0;
   out_1495718385613388763[203] = 0;
   out_1495718385613388763[204] = 0;
   out_1495718385613388763[205] = 0;
   out_1495718385613388763[206] = 0;
   out_1495718385613388763[207] = 0;
   out_1495718385613388763[208] = 0;
   out_1495718385613388763[209] = 1;
   out_1495718385613388763[210] = 0;
   out_1495718385613388763[211] = 0;
   out_1495718385613388763[212] = 0;
   out_1495718385613388763[213] = 0;
   out_1495718385613388763[214] = 0;
   out_1495718385613388763[215] = 0;
   out_1495718385613388763[216] = 0;
   out_1495718385613388763[217] = 0;
   out_1495718385613388763[218] = 0;
   out_1495718385613388763[219] = 0;
   out_1495718385613388763[220] = 0;
   out_1495718385613388763[221] = 0;
   out_1495718385613388763[222] = 0;
   out_1495718385613388763[223] = 0;
   out_1495718385613388763[224] = 0;
   out_1495718385613388763[225] = 0;
   out_1495718385613388763[226] = 0;
   out_1495718385613388763[227] = 0;
   out_1495718385613388763[228] = 1;
   out_1495718385613388763[229] = 0;
   out_1495718385613388763[230] = 0;
   out_1495718385613388763[231] = 0;
   out_1495718385613388763[232] = 0;
   out_1495718385613388763[233] = 0;
   out_1495718385613388763[234] = 0;
   out_1495718385613388763[235] = 0;
   out_1495718385613388763[236] = 0;
   out_1495718385613388763[237] = 0;
   out_1495718385613388763[238] = 0;
   out_1495718385613388763[239] = 0;
   out_1495718385613388763[240] = 0;
   out_1495718385613388763[241] = 0;
   out_1495718385613388763[242] = 0;
   out_1495718385613388763[243] = 0;
   out_1495718385613388763[244] = 0;
   out_1495718385613388763[245] = 0;
   out_1495718385613388763[246] = 0;
   out_1495718385613388763[247] = 1;
   out_1495718385613388763[248] = 0;
   out_1495718385613388763[249] = 0;
   out_1495718385613388763[250] = 0;
   out_1495718385613388763[251] = 0;
   out_1495718385613388763[252] = 0;
   out_1495718385613388763[253] = 0;
   out_1495718385613388763[254] = 0;
   out_1495718385613388763[255] = 0;
   out_1495718385613388763[256] = 0;
   out_1495718385613388763[257] = 0;
   out_1495718385613388763[258] = 0;
   out_1495718385613388763[259] = 0;
   out_1495718385613388763[260] = 0;
   out_1495718385613388763[261] = 0;
   out_1495718385613388763[262] = 0;
   out_1495718385613388763[263] = 0;
   out_1495718385613388763[264] = 0;
   out_1495718385613388763[265] = 0;
   out_1495718385613388763[266] = 1;
   out_1495718385613388763[267] = 0;
   out_1495718385613388763[268] = 0;
   out_1495718385613388763[269] = 0;
   out_1495718385613388763[270] = 0;
   out_1495718385613388763[271] = 0;
   out_1495718385613388763[272] = 0;
   out_1495718385613388763[273] = 0;
   out_1495718385613388763[274] = 0;
   out_1495718385613388763[275] = 0;
   out_1495718385613388763[276] = 0;
   out_1495718385613388763[277] = 0;
   out_1495718385613388763[278] = 0;
   out_1495718385613388763[279] = 0;
   out_1495718385613388763[280] = 0;
   out_1495718385613388763[281] = 0;
   out_1495718385613388763[282] = 0;
   out_1495718385613388763[283] = 0;
   out_1495718385613388763[284] = 0;
   out_1495718385613388763[285] = 1;
   out_1495718385613388763[286] = 0;
   out_1495718385613388763[287] = 0;
   out_1495718385613388763[288] = 0;
   out_1495718385613388763[289] = 0;
   out_1495718385613388763[290] = 0;
   out_1495718385613388763[291] = 0;
   out_1495718385613388763[292] = 0;
   out_1495718385613388763[293] = 0;
   out_1495718385613388763[294] = 0;
   out_1495718385613388763[295] = 0;
   out_1495718385613388763[296] = 0;
   out_1495718385613388763[297] = 0;
   out_1495718385613388763[298] = 0;
   out_1495718385613388763[299] = 0;
   out_1495718385613388763[300] = 0;
   out_1495718385613388763[301] = 0;
   out_1495718385613388763[302] = 0;
   out_1495718385613388763[303] = 0;
   out_1495718385613388763[304] = 1;
   out_1495718385613388763[305] = 0;
   out_1495718385613388763[306] = 0;
   out_1495718385613388763[307] = 0;
   out_1495718385613388763[308] = 0;
   out_1495718385613388763[309] = 0;
   out_1495718385613388763[310] = 0;
   out_1495718385613388763[311] = 0;
   out_1495718385613388763[312] = 0;
   out_1495718385613388763[313] = 0;
   out_1495718385613388763[314] = 0;
   out_1495718385613388763[315] = 0;
   out_1495718385613388763[316] = 0;
   out_1495718385613388763[317] = 0;
   out_1495718385613388763[318] = 0;
   out_1495718385613388763[319] = 0;
   out_1495718385613388763[320] = 0;
   out_1495718385613388763[321] = 0;
   out_1495718385613388763[322] = 0;
   out_1495718385613388763[323] = 1;
}
void h_4(double *state, double *unused, double *out_5955033610265769474) {
   out_5955033610265769474[0] = state[6] + state[9];
   out_5955033610265769474[1] = state[7] + state[10];
   out_5955033610265769474[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_1217665312458735127) {
   out_1217665312458735127[0] = 0;
   out_1217665312458735127[1] = 0;
   out_1217665312458735127[2] = 0;
   out_1217665312458735127[3] = 0;
   out_1217665312458735127[4] = 0;
   out_1217665312458735127[5] = 0;
   out_1217665312458735127[6] = 1;
   out_1217665312458735127[7] = 0;
   out_1217665312458735127[8] = 0;
   out_1217665312458735127[9] = 1;
   out_1217665312458735127[10] = 0;
   out_1217665312458735127[11] = 0;
   out_1217665312458735127[12] = 0;
   out_1217665312458735127[13] = 0;
   out_1217665312458735127[14] = 0;
   out_1217665312458735127[15] = 0;
   out_1217665312458735127[16] = 0;
   out_1217665312458735127[17] = 0;
   out_1217665312458735127[18] = 0;
   out_1217665312458735127[19] = 0;
   out_1217665312458735127[20] = 0;
   out_1217665312458735127[21] = 0;
   out_1217665312458735127[22] = 0;
   out_1217665312458735127[23] = 0;
   out_1217665312458735127[24] = 0;
   out_1217665312458735127[25] = 1;
   out_1217665312458735127[26] = 0;
   out_1217665312458735127[27] = 0;
   out_1217665312458735127[28] = 1;
   out_1217665312458735127[29] = 0;
   out_1217665312458735127[30] = 0;
   out_1217665312458735127[31] = 0;
   out_1217665312458735127[32] = 0;
   out_1217665312458735127[33] = 0;
   out_1217665312458735127[34] = 0;
   out_1217665312458735127[35] = 0;
   out_1217665312458735127[36] = 0;
   out_1217665312458735127[37] = 0;
   out_1217665312458735127[38] = 0;
   out_1217665312458735127[39] = 0;
   out_1217665312458735127[40] = 0;
   out_1217665312458735127[41] = 0;
   out_1217665312458735127[42] = 0;
   out_1217665312458735127[43] = 0;
   out_1217665312458735127[44] = 1;
   out_1217665312458735127[45] = 0;
   out_1217665312458735127[46] = 0;
   out_1217665312458735127[47] = 1;
   out_1217665312458735127[48] = 0;
   out_1217665312458735127[49] = 0;
   out_1217665312458735127[50] = 0;
   out_1217665312458735127[51] = 0;
   out_1217665312458735127[52] = 0;
   out_1217665312458735127[53] = 0;
}
void h_10(double *state, double *unused, double *out_6977900249410694712) {
   out_6977900249410694712[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_6977900249410694712[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_6977900249410694712[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4531985994016089052) {
   out_4531985994016089052[0] = 0;
   out_4531985994016089052[1] = 9.8100000000000005*cos(state[1]);
   out_4531985994016089052[2] = 0;
   out_4531985994016089052[3] = 0;
   out_4531985994016089052[4] = -state[8];
   out_4531985994016089052[5] = state[7];
   out_4531985994016089052[6] = 0;
   out_4531985994016089052[7] = state[5];
   out_4531985994016089052[8] = -state[4];
   out_4531985994016089052[9] = 0;
   out_4531985994016089052[10] = 0;
   out_4531985994016089052[11] = 0;
   out_4531985994016089052[12] = 1;
   out_4531985994016089052[13] = 0;
   out_4531985994016089052[14] = 0;
   out_4531985994016089052[15] = 1;
   out_4531985994016089052[16] = 0;
   out_4531985994016089052[17] = 0;
   out_4531985994016089052[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4531985994016089052[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4531985994016089052[20] = 0;
   out_4531985994016089052[21] = state[8];
   out_4531985994016089052[22] = 0;
   out_4531985994016089052[23] = -state[6];
   out_4531985994016089052[24] = -state[5];
   out_4531985994016089052[25] = 0;
   out_4531985994016089052[26] = state[3];
   out_4531985994016089052[27] = 0;
   out_4531985994016089052[28] = 0;
   out_4531985994016089052[29] = 0;
   out_4531985994016089052[30] = 0;
   out_4531985994016089052[31] = 1;
   out_4531985994016089052[32] = 0;
   out_4531985994016089052[33] = 0;
   out_4531985994016089052[34] = 1;
   out_4531985994016089052[35] = 0;
   out_4531985994016089052[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4531985994016089052[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4531985994016089052[38] = 0;
   out_4531985994016089052[39] = -state[7];
   out_4531985994016089052[40] = state[6];
   out_4531985994016089052[41] = 0;
   out_4531985994016089052[42] = state[4];
   out_4531985994016089052[43] = -state[3];
   out_4531985994016089052[44] = 0;
   out_4531985994016089052[45] = 0;
   out_4531985994016089052[46] = 0;
   out_4531985994016089052[47] = 0;
   out_4531985994016089052[48] = 0;
   out_4531985994016089052[49] = 0;
   out_4531985994016089052[50] = 1;
   out_4531985994016089052[51] = 0;
   out_4531985994016089052[52] = 0;
   out_4531985994016089052[53] = 1;
}
void h_13(double *state, double *unused, double *out_1830313317809173444) {
   out_1830313317809173444[0] = state[3];
   out_1830313317809173444[1] = state[4];
   out_1830313317809173444[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4429939137791067928) {
   out_4429939137791067928[0] = 0;
   out_4429939137791067928[1] = 0;
   out_4429939137791067928[2] = 0;
   out_4429939137791067928[3] = 1;
   out_4429939137791067928[4] = 0;
   out_4429939137791067928[5] = 0;
   out_4429939137791067928[6] = 0;
   out_4429939137791067928[7] = 0;
   out_4429939137791067928[8] = 0;
   out_4429939137791067928[9] = 0;
   out_4429939137791067928[10] = 0;
   out_4429939137791067928[11] = 0;
   out_4429939137791067928[12] = 0;
   out_4429939137791067928[13] = 0;
   out_4429939137791067928[14] = 0;
   out_4429939137791067928[15] = 0;
   out_4429939137791067928[16] = 0;
   out_4429939137791067928[17] = 0;
   out_4429939137791067928[18] = 0;
   out_4429939137791067928[19] = 0;
   out_4429939137791067928[20] = 0;
   out_4429939137791067928[21] = 0;
   out_4429939137791067928[22] = 1;
   out_4429939137791067928[23] = 0;
   out_4429939137791067928[24] = 0;
   out_4429939137791067928[25] = 0;
   out_4429939137791067928[26] = 0;
   out_4429939137791067928[27] = 0;
   out_4429939137791067928[28] = 0;
   out_4429939137791067928[29] = 0;
   out_4429939137791067928[30] = 0;
   out_4429939137791067928[31] = 0;
   out_4429939137791067928[32] = 0;
   out_4429939137791067928[33] = 0;
   out_4429939137791067928[34] = 0;
   out_4429939137791067928[35] = 0;
   out_4429939137791067928[36] = 0;
   out_4429939137791067928[37] = 0;
   out_4429939137791067928[38] = 0;
   out_4429939137791067928[39] = 0;
   out_4429939137791067928[40] = 0;
   out_4429939137791067928[41] = 1;
   out_4429939137791067928[42] = 0;
   out_4429939137791067928[43] = 0;
   out_4429939137791067928[44] = 0;
   out_4429939137791067928[45] = 0;
   out_4429939137791067928[46] = 0;
   out_4429939137791067928[47] = 0;
   out_4429939137791067928[48] = 0;
   out_4429939137791067928[49] = 0;
   out_4429939137791067928[50] = 0;
   out_4429939137791067928[51] = 0;
   out_4429939137791067928[52] = 0;
   out_4429939137791067928[53] = 0;
}
void h_14(double *state, double *unused, double *out_194477322244382774) {
   out_194477322244382774[0] = state[6];
   out_194477322244382774[1] = state[7];
   out_194477322244382774[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5180906168798219656) {
   out_5180906168798219656[0] = 0;
   out_5180906168798219656[1] = 0;
   out_5180906168798219656[2] = 0;
   out_5180906168798219656[3] = 0;
   out_5180906168798219656[4] = 0;
   out_5180906168798219656[5] = 0;
   out_5180906168798219656[6] = 1;
   out_5180906168798219656[7] = 0;
   out_5180906168798219656[8] = 0;
   out_5180906168798219656[9] = 0;
   out_5180906168798219656[10] = 0;
   out_5180906168798219656[11] = 0;
   out_5180906168798219656[12] = 0;
   out_5180906168798219656[13] = 0;
   out_5180906168798219656[14] = 0;
   out_5180906168798219656[15] = 0;
   out_5180906168798219656[16] = 0;
   out_5180906168798219656[17] = 0;
   out_5180906168798219656[18] = 0;
   out_5180906168798219656[19] = 0;
   out_5180906168798219656[20] = 0;
   out_5180906168798219656[21] = 0;
   out_5180906168798219656[22] = 0;
   out_5180906168798219656[23] = 0;
   out_5180906168798219656[24] = 0;
   out_5180906168798219656[25] = 1;
   out_5180906168798219656[26] = 0;
   out_5180906168798219656[27] = 0;
   out_5180906168798219656[28] = 0;
   out_5180906168798219656[29] = 0;
   out_5180906168798219656[30] = 0;
   out_5180906168798219656[31] = 0;
   out_5180906168798219656[32] = 0;
   out_5180906168798219656[33] = 0;
   out_5180906168798219656[34] = 0;
   out_5180906168798219656[35] = 0;
   out_5180906168798219656[36] = 0;
   out_5180906168798219656[37] = 0;
   out_5180906168798219656[38] = 0;
   out_5180906168798219656[39] = 0;
   out_5180906168798219656[40] = 0;
   out_5180906168798219656[41] = 0;
   out_5180906168798219656[42] = 0;
   out_5180906168798219656[43] = 0;
   out_5180906168798219656[44] = 1;
   out_5180906168798219656[45] = 0;
   out_5180906168798219656[46] = 0;
   out_5180906168798219656[47] = 0;
   out_5180906168798219656[48] = 0;
   out_5180906168798219656[49] = 0;
   out_5180906168798219656[50] = 0;
   out_5180906168798219656[51] = 0;
   out_5180906168798219656[52] = 0;
   out_5180906168798219656[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_8919129796448948883) {
  err_fun(nom_x, delta_x, out_8919129796448948883);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5759862885670035392) {
  inv_err_fun(nom_x, true_x, out_5759862885670035392);
}
void pose_H_mod_fun(double *state, double *out_7065747119210693848) {
  H_mod_fun(state, out_7065747119210693848);
}
void pose_f_fun(double *state, double dt, double *out_558603448437855289) {
  f_fun(state,  dt, out_558603448437855289);
}
void pose_F_fun(double *state, double dt, double *out_1495718385613388763) {
  F_fun(state,  dt, out_1495718385613388763);
}
void pose_h_4(double *state, double *unused, double *out_5955033610265769474) {
  h_4(state, unused, out_5955033610265769474);
}
void pose_H_4(double *state, double *unused, double *out_1217665312458735127) {
  H_4(state, unused, out_1217665312458735127);
}
void pose_h_10(double *state, double *unused, double *out_6977900249410694712) {
  h_10(state, unused, out_6977900249410694712);
}
void pose_H_10(double *state, double *unused, double *out_4531985994016089052) {
  H_10(state, unused, out_4531985994016089052);
}
void pose_h_13(double *state, double *unused, double *out_1830313317809173444) {
  h_13(state, unused, out_1830313317809173444);
}
void pose_H_13(double *state, double *unused, double *out_4429939137791067928) {
  H_13(state, unused, out_4429939137791067928);
}
void pose_h_14(double *state, double *unused, double *out_194477322244382774) {
  h_14(state, unused, out_194477322244382774);
}
void pose_H_14(double *state, double *unused, double *out_5180906168798219656) {
  H_14(state, unused, out_5180906168798219656);
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
