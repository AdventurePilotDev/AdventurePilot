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
void err_fun(double *nom_x, double *delta_x, double *out_7182192695751697228) {
   out_7182192695751697228[0] = delta_x[0] + nom_x[0];
   out_7182192695751697228[1] = delta_x[1] + nom_x[1];
   out_7182192695751697228[2] = delta_x[2] + nom_x[2];
   out_7182192695751697228[3] = delta_x[3] + nom_x[3];
   out_7182192695751697228[4] = delta_x[4] + nom_x[4];
   out_7182192695751697228[5] = delta_x[5] + nom_x[5];
   out_7182192695751697228[6] = delta_x[6] + nom_x[6];
   out_7182192695751697228[7] = delta_x[7] + nom_x[7];
   out_7182192695751697228[8] = delta_x[8] + nom_x[8];
   out_7182192695751697228[9] = delta_x[9] + nom_x[9];
   out_7182192695751697228[10] = delta_x[10] + nom_x[10];
   out_7182192695751697228[11] = delta_x[11] + nom_x[11];
   out_7182192695751697228[12] = delta_x[12] + nom_x[12];
   out_7182192695751697228[13] = delta_x[13] + nom_x[13];
   out_7182192695751697228[14] = delta_x[14] + nom_x[14];
   out_7182192695751697228[15] = delta_x[15] + nom_x[15];
   out_7182192695751697228[16] = delta_x[16] + nom_x[16];
   out_7182192695751697228[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_800007061037745391) {
   out_800007061037745391[0] = -nom_x[0] + true_x[0];
   out_800007061037745391[1] = -nom_x[1] + true_x[1];
   out_800007061037745391[2] = -nom_x[2] + true_x[2];
   out_800007061037745391[3] = -nom_x[3] + true_x[3];
   out_800007061037745391[4] = -nom_x[4] + true_x[4];
   out_800007061037745391[5] = -nom_x[5] + true_x[5];
   out_800007061037745391[6] = -nom_x[6] + true_x[6];
   out_800007061037745391[7] = -nom_x[7] + true_x[7];
   out_800007061037745391[8] = -nom_x[8] + true_x[8];
   out_800007061037745391[9] = -nom_x[9] + true_x[9];
   out_800007061037745391[10] = -nom_x[10] + true_x[10];
   out_800007061037745391[11] = -nom_x[11] + true_x[11];
   out_800007061037745391[12] = -nom_x[12] + true_x[12];
   out_800007061037745391[13] = -nom_x[13] + true_x[13];
   out_800007061037745391[14] = -nom_x[14] + true_x[14];
   out_800007061037745391[15] = -nom_x[15] + true_x[15];
   out_800007061037745391[16] = -nom_x[16] + true_x[16];
   out_800007061037745391[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4496961729871771833) {
   out_4496961729871771833[0] = 1.0;
   out_4496961729871771833[1] = 0.0;
   out_4496961729871771833[2] = 0.0;
   out_4496961729871771833[3] = 0.0;
   out_4496961729871771833[4] = 0.0;
   out_4496961729871771833[5] = 0.0;
   out_4496961729871771833[6] = 0.0;
   out_4496961729871771833[7] = 0.0;
   out_4496961729871771833[8] = 0.0;
   out_4496961729871771833[9] = 0.0;
   out_4496961729871771833[10] = 0.0;
   out_4496961729871771833[11] = 0.0;
   out_4496961729871771833[12] = 0.0;
   out_4496961729871771833[13] = 0.0;
   out_4496961729871771833[14] = 0.0;
   out_4496961729871771833[15] = 0.0;
   out_4496961729871771833[16] = 0.0;
   out_4496961729871771833[17] = 0.0;
   out_4496961729871771833[18] = 0.0;
   out_4496961729871771833[19] = 1.0;
   out_4496961729871771833[20] = 0.0;
   out_4496961729871771833[21] = 0.0;
   out_4496961729871771833[22] = 0.0;
   out_4496961729871771833[23] = 0.0;
   out_4496961729871771833[24] = 0.0;
   out_4496961729871771833[25] = 0.0;
   out_4496961729871771833[26] = 0.0;
   out_4496961729871771833[27] = 0.0;
   out_4496961729871771833[28] = 0.0;
   out_4496961729871771833[29] = 0.0;
   out_4496961729871771833[30] = 0.0;
   out_4496961729871771833[31] = 0.0;
   out_4496961729871771833[32] = 0.0;
   out_4496961729871771833[33] = 0.0;
   out_4496961729871771833[34] = 0.0;
   out_4496961729871771833[35] = 0.0;
   out_4496961729871771833[36] = 0.0;
   out_4496961729871771833[37] = 0.0;
   out_4496961729871771833[38] = 1.0;
   out_4496961729871771833[39] = 0.0;
   out_4496961729871771833[40] = 0.0;
   out_4496961729871771833[41] = 0.0;
   out_4496961729871771833[42] = 0.0;
   out_4496961729871771833[43] = 0.0;
   out_4496961729871771833[44] = 0.0;
   out_4496961729871771833[45] = 0.0;
   out_4496961729871771833[46] = 0.0;
   out_4496961729871771833[47] = 0.0;
   out_4496961729871771833[48] = 0.0;
   out_4496961729871771833[49] = 0.0;
   out_4496961729871771833[50] = 0.0;
   out_4496961729871771833[51] = 0.0;
   out_4496961729871771833[52] = 0.0;
   out_4496961729871771833[53] = 0.0;
   out_4496961729871771833[54] = 0.0;
   out_4496961729871771833[55] = 0.0;
   out_4496961729871771833[56] = 0.0;
   out_4496961729871771833[57] = 1.0;
   out_4496961729871771833[58] = 0.0;
   out_4496961729871771833[59] = 0.0;
   out_4496961729871771833[60] = 0.0;
   out_4496961729871771833[61] = 0.0;
   out_4496961729871771833[62] = 0.0;
   out_4496961729871771833[63] = 0.0;
   out_4496961729871771833[64] = 0.0;
   out_4496961729871771833[65] = 0.0;
   out_4496961729871771833[66] = 0.0;
   out_4496961729871771833[67] = 0.0;
   out_4496961729871771833[68] = 0.0;
   out_4496961729871771833[69] = 0.0;
   out_4496961729871771833[70] = 0.0;
   out_4496961729871771833[71] = 0.0;
   out_4496961729871771833[72] = 0.0;
   out_4496961729871771833[73] = 0.0;
   out_4496961729871771833[74] = 0.0;
   out_4496961729871771833[75] = 0.0;
   out_4496961729871771833[76] = 1.0;
   out_4496961729871771833[77] = 0.0;
   out_4496961729871771833[78] = 0.0;
   out_4496961729871771833[79] = 0.0;
   out_4496961729871771833[80] = 0.0;
   out_4496961729871771833[81] = 0.0;
   out_4496961729871771833[82] = 0.0;
   out_4496961729871771833[83] = 0.0;
   out_4496961729871771833[84] = 0.0;
   out_4496961729871771833[85] = 0.0;
   out_4496961729871771833[86] = 0.0;
   out_4496961729871771833[87] = 0.0;
   out_4496961729871771833[88] = 0.0;
   out_4496961729871771833[89] = 0.0;
   out_4496961729871771833[90] = 0.0;
   out_4496961729871771833[91] = 0.0;
   out_4496961729871771833[92] = 0.0;
   out_4496961729871771833[93] = 0.0;
   out_4496961729871771833[94] = 0.0;
   out_4496961729871771833[95] = 1.0;
   out_4496961729871771833[96] = 0.0;
   out_4496961729871771833[97] = 0.0;
   out_4496961729871771833[98] = 0.0;
   out_4496961729871771833[99] = 0.0;
   out_4496961729871771833[100] = 0.0;
   out_4496961729871771833[101] = 0.0;
   out_4496961729871771833[102] = 0.0;
   out_4496961729871771833[103] = 0.0;
   out_4496961729871771833[104] = 0.0;
   out_4496961729871771833[105] = 0.0;
   out_4496961729871771833[106] = 0.0;
   out_4496961729871771833[107] = 0.0;
   out_4496961729871771833[108] = 0.0;
   out_4496961729871771833[109] = 0.0;
   out_4496961729871771833[110] = 0.0;
   out_4496961729871771833[111] = 0.0;
   out_4496961729871771833[112] = 0.0;
   out_4496961729871771833[113] = 0.0;
   out_4496961729871771833[114] = 1.0;
   out_4496961729871771833[115] = 0.0;
   out_4496961729871771833[116] = 0.0;
   out_4496961729871771833[117] = 0.0;
   out_4496961729871771833[118] = 0.0;
   out_4496961729871771833[119] = 0.0;
   out_4496961729871771833[120] = 0.0;
   out_4496961729871771833[121] = 0.0;
   out_4496961729871771833[122] = 0.0;
   out_4496961729871771833[123] = 0.0;
   out_4496961729871771833[124] = 0.0;
   out_4496961729871771833[125] = 0.0;
   out_4496961729871771833[126] = 0.0;
   out_4496961729871771833[127] = 0.0;
   out_4496961729871771833[128] = 0.0;
   out_4496961729871771833[129] = 0.0;
   out_4496961729871771833[130] = 0.0;
   out_4496961729871771833[131] = 0.0;
   out_4496961729871771833[132] = 0.0;
   out_4496961729871771833[133] = 1.0;
   out_4496961729871771833[134] = 0.0;
   out_4496961729871771833[135] = 0.0;
   out_4496961729871771833[136] = 0.0;
   out_4496961729871771833[137] = 0.0;
   out_4496961729871771833[138] = 0.0;
   out_4496961729871771833[139] = 0.0;
   out_4496961729871771833[140] = 0.0;
   out_4496961729871771833[141] = 0.0;
   out_4496961729871771833[142] = 0.0;
   out_4496961729871771833[143] = 0.0;
   out_4496961729871771833[144] = 0.0;
   out_4496961729871771833[145] = 0.0;
   out_4496961729871771833[146] = 0.0;
   out_4496961729871771833[147] = 0.0;
   out_4496961729871771833[148] = 0.0;
   out_4496961729871771833[149] = 0.0;
   out_4496961729871771833[150] = 0.0;
   out_4496961729871771833[151] = 0.0;
   out_4496961729871771833[152] = 1.0;
   out_4496961729871771833[153] = 0.0;
   out_4496961729871771833[154] = 0.0;
   out_4496961729871771833[155] = 0.0;
   out_4496961729871771833[156] = 0.0;
   out_4496961729871771833[157] = 0.0;
   out_4496961729871771833[158] = 0.0;
   out_4496961729871771833[159] = 0.0;
   out_4496961729871771833[160] = 0.0;
   out_4496961729871771833[161] = 0.0;
   out_4496961729871771833[162] = 0.0;
   out_4496961729871771833[163] = 0.0;
   out_4496961729871771833[164] = 0.0;
   out_4496961729871771833[165] = 0.0;
   out_4496961729871771833[166] = 0.0;
   out_4496961729871771833[167] = 0.0;
   out_4496961729871771833[168] = 0.0;
   out_4496961729871771833[169] = 0.0;
   out_4496961729871771833[170] = 0.0;
   out_4496961729871771833[171] = 1.0;
   out_4496961729871771833[172] = 0.0;
   out_4496961729871771833[173] = 0.0;
   out_4496961729871771833[174] = 0.0;
   out_4496961729871771833[175] = 0.0;
   out_4496961729871771833[176] = 0.0;
   out_4496961729871771833[177] = 0.0;
   out_4496961729871771833[178] = 0.0;
   out_4496961729871771833[179] = 0.0;
   out_4496961729871771833[180] = 0.0;
   out_4496961729871771833[181] = 0.0;
   out_4496961729871771833[182] = 0.0;
   out_4496961729871771833[183] = 0.0;
   out_4496961729871771833[184] = 0.0;
   out_4496961729871771833[185] = 0.0;
   out_4496961729871771833[186] = 0.0;
   out_4496961729871771833[187] = 0.0;
   out_4496961729871771833[188] = 0.0;
   out_4496961729871771833[189] = 0.0;
   out_4496961729871771833[190] = 1.0;
   out_4496961729871771833[191] = 0.0;
   out_4496961729871771833[192] = 0.0;
   out_4496961729871771833[193] = 0.0;
   out_4496961729871771833[194] = 0.0;
   out_4496961729871771833[195] = 0.0;
   out_4496961729871771833[196] = 0.0;
   out_4496961729871771833[197] = 0.0;
   out_4496961729871771833[198] = 0.0;
   out_4496961729871771833[199] = 0.0;
   out_4496961729871771833[200] = 0.0;
   out_4496961729871771833[201] = 0.0;
   out_4496961729871771833[202] = 0.0;
   out_4496961729871771833[203] = 0.0;
   out_4496961729871771833[204] = 0.0;
   out_4496961729871771833[205] = 0.0;
   out_4496961729871771833[206] = 0.0;
   out_4496961729871771833[207] = 0.0;
   out_4496961729871771833[208] = 0.0;
   out_4496961729871771833[209] = 1.0;
   out_4496961729871771833[210] = 0.0;
   out_4496961729871771833[211] = 0.0;
   out_4496961729871771833[212] = 0.0;
   out_4496961729871771833[213] = 0.0;
   out_4496961729871771833[214] = 0.0;
   out_4496961729871771833[215] = 0.0;
   out_4496961729871771833[216] = 0.0;
   out_4496961729871771833[217] = 0.0;
   out_4496961729871771833[218] = 0.0;
   out_4496961729871771833[219] = 0.0;
   out_4496961729871771833[220] = 0.0;
   out_4496961729871771833[221] = 0.0;
   out_4496961729871771833[222] = 0.0;
   out_4496961729871771833[223] = 0.0;
   out_4496961729871771833[224] = 0.0;
   out_4496961729871771833[225] = 0.0;
   out_4496961729871771833[226] = 0.0;
   out_4496961729871771833[227] = 0.0;
   out_4496961729871771833[228] = 1.0;
   out_4496961729871771833[229] = 0.0;
   out_4496961729871771833[230] = 0.0;
   out_4496961729871771833[231] = 0.0;
   out_4496961729871771833[232] = 0.0;
   out_4496961729871771833[233] = 0.0;
   out_4496961729871771833[234] = 0.0;
   out_4496961729871771833[235] = 0.0;
   out_4496961729871771833[236] = 0.0;
   out_4496961729871771833[237] = 0.0;
   out_4496961729871771833[238] = 0.0;
   out_4496961729871771833[239] = 0.0;
   out_4496961729871771833[240] = 0.0;
   out_4496961729871771833[241] = 0.0;
   out_4496961729871771833[242] = 0.0;
   out_4496961729871771833[243] = 0.0;
   out_4496961729871771833[244] = 0.0;
   out_4496961729871771833[245] = 0.0;
   out_4496961729871771833[246] = 0.0;
   out_4496961729871771833[247] = 1.0;
   out_4496961729871771833[248] = 0.0;
   out_4496961729871771833[249] = 0.0;
   out_4496961729871771833[250] = 0.0;
   out_4496961729871771833[251] = 0.0;
   out_4496961729871771833[252] = 0.0;
   out_4496961729871771833[253] = 0.0;
   out_4496961729871771833[254] = 0.0;
   out_4496961729871771833[255] = 0.0;
   out_4496961729871771833[256] = 0.0;
   out_4496961729871771833[257] = 0.0;
   out_4496961729871771833[258] = 0.0;
   out_4496961729871771833[259] = 0.0;
   out_4496961729871771833[260] = 0.0;
   out_4496961729871771833[261] = 0.0;
   out_4496961729871771833[262] = 0.0;
   out_4496961729871771833[263] = 0.0;
   out_4496961729871771833[264] = 0.0;
   out_4496961729871771833[265] = 0.0;
   out_4496961729871771833[266] = 1.0;
   out_4496961729871771833[267] = 0.0;
   out_4496961729871771833[268] = 0.0;
   out_4496961729871771833[269] = 0.0;
   out_4496961729871771833[270] = 0.0;
   out_4496961729871771833[271] = 0.0;
   out_4496961729871771833[272] = 0.0;
   out_4496961729871771833[273] = 0.0;
   out_4496961729871771833[274] = 0.0;
   out_4496961729871771833[275] = 0.0;
   out_4496961729871771833[276] = 0.0;
   out_4496961729871771833[277] = 0.0;
   out_4496961729871771833[278] = 0.0;
   out_4496961729871771833[279] = 0.0;
   out_4496961729871771833[280] = 0.0;
   out_4496961729871771833[281] = 0.0;
   out_4496961729871771833[282] = 0.0;
   out_4496961729871771833[283] = 0.0;
   out_4496961729871771833[284] = 0.0;
   out_4496961729871771833[285] = 1.0;
   out_4496961729871771833[286] = 0.0;
   out_4496961729871771833[287] = 0.0;
   out_4496961729871771833[288] = 0.0;
   out_4496961729871771833[289] = 0.0;
   out_4496961729871771833[290] = 0.0;
   out_4496961729871771833[291] = 0.0;
   out_4496961729871771833[292] = 0.0;
   out_4496961729871771833[293] = 0.0;
   out_4496961729871771833[294] = 0.0;
   out_4496961729871771833[295] = 0.0;
   out_4496961729871771833[296] = 0.0;
   out_4496961729871771833[297] = 0.0;
   out_4496961729871771833[298] = 0.0;
   out_4496961729871771833[299] = 0.0;
   out_4496961729871771833[300] = 0.0;
   out_4496961729871771833[301] = 0.0;
   out_4496961729871771833[302] = 0.0;
   out_4496961729871771833[303] = 0.0;
   out_4496961729871771833[304] = 1.0;
   out_4496961729871771833[305] = 0.0;
   out_4496961729871771833[306] = 0.0;
   out_4496961729871771833[307] = 0.0;
   out_4496961729871771833[308] = 0.0;
   out_4496961729871771833[309] = 0.0;
   out_4496961729871771833[310] = 0.0;
   out_4496961729871771833[311] = 0.0;
   out_4496961729871771833[312] = 0.0;
   out_4496961729871771833[313] = 0.0;
   out_4496961729871771833[314] = 0.0;
   out_4496961729871771833[315] = 0.0;
   out_4496961729871771833[316] = 0.0;
   out_4496961729871771833[317] = 0.0;
   out_4496961729871771833[318] = 0.0;
   out_4496961729871771833[319] = 0.0;
   out_4496961729871771833[320] = 0.0;
   out_4496961729871771833[321] = 0.0;
   out_4496961729871771833[322] = 0.0;
   out_4496961729871771833[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_7612049814342810855) {
   out_7612049814342810855[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_7612049814342810855[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_7612049814342810855[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_7612049814342810855[3] = dt*state[12] + state[3];
   out_7612049814342810855[4] = dt*state[13] + state[4];
   out_7612049814342810855[5] = dt*state[14] + state[5];
   out_7612049814342810855[6] = state[6];
   out_7612049814342810855[7] = state[7];
   out_7612049814342810855[8] = state[8];
   out_7612049814342810855[9] = state[9];
   out_7612049814342810855[10] = state[10];
   out_7612049814342810855[11] = state[11];
   out_7612049814342810855[12] = state[12];
   out_7612049814342810855[13] = state[13];
   out_7612049814342810855[14] = state[14];
   out_7612049814342810855[15] = state[15];
   out_7612049814342810855[16] = state[16];
   out_7612049814342810855[17] = state[17];
}
void F_fun(double *state, double dt, double *out_4578642212278889444) {
   out_4578642212278889444[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4578642212278889444[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4578642212278889444[2] = 0;
   out_4578642212278889444[3] = 0;
   out_4578642212278889444[4] = 0;
   out_4578642212278889444[5] = 0;
   out_4578642212278889444[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4578642212278889444[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4578642212278889444[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4578642212278889444[9] = 0;
   out_4578642212278889444[10] = 0;
   out_4578642212278889444[11] = 0;
   out_4578642212278889444[12] = 0;
   out_4578642212278889444[13] = 0;
   out_4578642212278889444[14] = 0;
   out_4578642212278889444[15] = 0;
   out_4578642212278889444[16] = 0;
   out_4578642212278889444[17] = 0;
   out_4578642212278889444[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4578642212278889444[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4578642212278889444[20] = 0;
   out_4578642212278889444[21] = 0;
   out_4578642212278889444[22] = 0;
   out_4578642212278889444[23] = 0;
   out_4578642212278889444[24] = 0;
   out_4578642212278889444[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4578642212278889444[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4578642212278889444[27] = 0;
   out_4578642212278889444[28] = 0;
   out_4578642212278889444[29] = 0;
   out_4578642212278889444[30] = 0;
   out_4578642212278889444[31] = 0;
   out_4578642212278889444[32] = 0;
   out_4578642212278889444[33] = 0;
   out_4578642212278889444[34] = 0;
   out_4578642212278889444[35] = 0;
   out_4578642212278889444[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4578642212278889444[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4578642212278889444[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4578642212278889444[39] = 0;
   out_4578642212278889444[40] = 0;
   out_4578642212278889444[41] = 0;
   out_4578642212278889444[42] = 0;
   out_4578642212278889444[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4578642212278889444[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4578642212278889444[45] = 0;
   out_4578642212278889444[46] = 0;
   out_4578642212278889444[47] = 0;
   out_4578642212278889444[48] = 0;
   out_4578642212278889444[49] = 0;
   out_4578642212278889444[50] = 0;
   out_4578642212278889444[51] = 0;
   out_4578642212278889444[52] = 0;
   out_4578642212278889444[53] = 0;
   out_4578642212278889444[54] = 0;
   out_4578642212278889444[55] = 0;
   out_4578642212278889444[56] = 0;
   out_4578642212278889444[57] = 1;
   out_4578642212278889444[58] = 0;
   out_4578642212278889444[59] = 0;
   out_4578642212278889444[60] = 0;
   out_4578642212278889444[61] = 0;
   out_4578642212278889444[62] = 0;
   out_4578642212278889444[63] = 0;
   out_4578642212278889444[64] = 0;
   out_4578642212278889444[65] = 0;
   out_4578642212278889444[66] = dt;
   out_4578642212278889444[67] = 0;
   out_4578642212278889444[68] = 0;
   out_4578642212278889444[69] = 0;
   out_4578642212278889444[70] = 0;
   out_4578642212278889444[71] = 0;
   out_4578642212278889444[72] = 0;
   out_4578642212278889444[73] = 0;
   out_4578642212278889444[74] = 0;
   out_4578642212278889444[75] = 0;
   out_4578642212278889444[76] = 1;
   out_4578642212278889444[77] = 0;
   out_4578642212278889444[78] = 0;
   out_4578642212278889444[79] = 0;
   out_4578642212278889444[80] = 0;
   out_4578642212278889444[81] = 0;
   out_4578642212278889444[82] = 0;
   out_4578642212278889444[83] = 0;
   out_4578642212278889444[84] = 0;
   out_4578642212278889444[85] = dt;
   out_4578642212278889444[86] = 0;
   out_4578642212278889444[87] = 0;
   out_4578642212278889444[88] = 0;
   out_4578642212278889444[89] = 0;
   out_4578642212278889444[90] = 0;
   out_4578642212278889444[91] = 0;
   out_4578642212278889444[92] = 0;
   out_4578642212278889444[93] = 0;
   out_4578642212278889444[94] = 0;
   out_4578642212278889444[95] = 1;
   out_4578642212278889444[96] = 0;
   out_4578642212278889444[97] = 0;
   out_4578642212278889444[98] = 0;
   out_4578642212278889444[99] = 0;
   out_4578642212278889444[100] = 0;
   out_4578642212278889444[101] = 0;
   out_4578642212278889444[102] = 0;
   out_4578642212278889444[103] = 0;
   out_4578642212278889444[104] = dt;
   out_4578642212278889444[105] = 0;
   out_4578642212278889444[106] = 0;
   out_4578642212278889444[107] = 0;
   out_4578642212278889444[108] = 0;
   out_4578642212278889444[109] = 0;
   out_4578642212278889444[110] = 0;
   out_4578642212278889444[111] = 0;
   out_4578642212278889444[112] = 0;
   out_4578642212278889444[113] = 0;
   out_4578642212278889444[114] = 1;
   out_4578642212278889444[115] = 0;
   out_4578642212278889444[116] = 0;
   out_4578642212278889444[117] = 0;
   out_4578642212278889444[118] = 0;
   out_4578642212278889444[119] = 0;
   out_4578642212278889444[120] = 0;
   out_4578642212278889444[121] = 0;
   out_4578642212278889444[122] = 0;
   out_4578642212278889444[123] = 0;
   out_4578642212278889444[124] = 0;
   out_4578642212278889444[125] = 0;
   out_4578642212278889444[126] = 0;
   out_4578642212278889444[127] = 0;
   out_4578642212278889444[128] = 0;
   out_4578642212278889444[129] = 0;
   out_4578642212278889444[130] = 0;
   out_4578642212278889444[131] = 0;
   out_4578642212278889444[132] = 0;
   out_4578642212278889444[133] = 1;
   out_4578642212278889444[134] = 0;
   out_4578642212278889444[135] = 0;
   out_4578642212278889444[136] = 0;
   out_4578642212278889444[137] = 0;
   out_4578642212278889444[138] = 0;
   out_4578642212278889444[139] = 0;
   out_4578642212278889444[140] = 0;
   out_4578642212278889444[141] = 0;
   out_4578642212278889444[142] = 0;
   out_4578642212278889444[143] = 0;
   out_4578642212278889444[144] = 0;
   out_4578642212278889444[145] = 0;
   out_4578642212278889444[146] = 0;
   out_4578642212278889444[147] = 0;
   out_4578642212278889444[148] = 0;
   out_4578642212278889444[149] = 0;
   out_4578642212278889444[150] = 0;
   out_4578642212278889444[151] = 0;
   out_4578642212278889444[152] = 1;
   out_4578642212278889444[153] = 0;
   out_4578642212278889444[154] = 0;
   out_4578642212278889444[155] = 0;
   out_4578642212278889444[156] = 0;
   out_4578642212278889444[157] = 0;
   out_4578642212278889444[158] = 0;
   out_4578642212278889444[159] = 0;
   out_4578642212278889444[160] = 0;
   out_4578642212278889444[161] = 0;
   out_4578642212278889444[162] = 0;
   out_4578642212278889444[163] = 0;
   out_4578642212278889444[164] = 0;
   out_4578642212278889444[165] = 0;
   out_4578642212278889444[166] = 0;
   out_4578642212278889444[167] = 0;
   out_4578642212278889444[168] = 0;
   out_4578642212278889444[169] = 0;
   out_4578642212278889444[170] = 0;
   out_4578642212278889444[171] = 1;
   out_4578642212278889444[172] = 0;
   out_4578642212278889444[173] = 0;
   out_4578642212278889444[174] = 0;
   out_4578642212278889444[175] = 0;
   out_4578642212278889444[176] = 0;
   out_4578642212278889444[177] = 0;
   out_4578642212278889444[178] = 0;
   out_4578642212278889444[179] = 0;
   out_4578642212278889444[180] = 0;
   out_4578642212278889444[181] = 0;
   out_4578642212278889444[182] = 0;
   out_4578642212278889444[183] = 0;
   out_4578642212278889444[184] = 0;
   out_4578642212278889444[185] = 0;
   out_4578642212278889444[186] = 0;
   out_4578642212278889444[187] = 0;
   out_4578642212278889444[188] = 0;
   out_4578642212278889444[189] = 0;
   out_4578642212278889444[190] = 1;
   out_4578642212278889444[191] = 0;
   out_4578642212278889444[192] = 0;
   out_4578642212278889444[193] = 0;
   out_4578642212278889444[194] = 0;
   out_4578642212278889444[195] = 0;
   out_4578642212278889444[196] = 0;
   out_4578642212278889444[197] = 0;
   out_4578642212278889444[198] = 0;
   out_4578642212278889444[199] = 0;
   out_4578642212278889444[200] = 0;
   out_4578642212278889444[201] = 0;
   out_4578642212278889444[202] = 0;
   out_4578642212278889444[203] = 0;
   out_4578642212278889444[204] = 0;
   out_4578642212278889444[205] = 0;
   out_4578642212278889444[206] = 0;
   out_4578642212278889444[207] = 0;
   out_4578642212278889444[208] = 0;
   out_4578642212278889444[209] = 1;
   out_4578642212278889444[210] = 0;
   out_4578642212278889444[211] = 0;
   out_4578642212278889444[212] = 0;
   out_4578642212278889444[213] = 0;
   out_4578642212278889444[214] = 0;
   out_4578642212278889444[215] = 0;
   out_4578642212278889444[216] = 0;
   out_4578642212278889444[217] = 0;
   out_4578642212278889444[218] = 0;
   out_4578642212278889444[219] = 0;
   out_4578642212278889444[220] = 0;
   out_4578642212278889444[221] = 0;
   out_4578642212278889444[222] = 0;
   out_4578642212278889444[223] = 0;
   out_4578642212278889444[224] = 0;
   out_4578642212278889444[225] = 0;
   out_4578642212278889444[226] = 0;
   out_4578642212278889444[227] = 0;
   out_4578642212278889444[228] = 1;
   out_4578642212278889444[229] = 0;
   out_4578642212278889444[230] = 0;
   out_4578642212278889444[231] = 0;
   out_4578642212278889444[232] = 0;
   out_4578642212278889444[233] = 0;
   out_4578642212278889444[234] = 0;
   out_4578642212278889444[235] = 0;
   out_4578642212278889444[236] = 0;
   out_4578642212278889444[237] = 0;
   out_4578642212278889444[238] = 0;
   out_4578642212278889444[239] = 0;
   out_4578642212278889444[240] = 0;
   out_4578642212278889444[241] = 0;
   out_4578642212278889444[242] = 0;
   out_4578642212278889444[243] = 0;
   out_4578642212278889444[244] = 0;
   out_4578642212278889444[245] = 0;
   out_4578642212278889444[246] = 0;
   out_4578642212278889444[247] = 1;
   out_4578642212278889444[248] = 0;
   out_4578642212278889444[249] = 0;
   out_4578642212278889444[250] = 0;
   out_4578642212278889444[251] = 0;
   out_4578642212278889444[252] = 0;
   out_4578642212278889444[253] = 0;
   out_4578642212278889444[254] = 0;
   out_4578642212278889444[255] = 0;
   out_4578642212278889444[256] = 0;
   out_4578642212278889444[257] = 0;
   out_4578642212278889444[258] = 0;
   out_4578642212278889444[259] = 0;
   out_4578642212278889444[260] = 0;
   out_4578642212278889444[261] = 0;
   out_4578642212278889444[262] = 0;
   out_4578642212278889444[263] = 0;
   out_4578642212278889444[264] = 0;
   out_4578642212278889444[265] = 0;
   out_4578642212278889444[266] = 1;
   out_4578642212278889444[267] = 0;
   out_4578642212278889444[268] = 0;
   out_4578642212278889444[269] = 0;
   out_4578642212278889444[270] = 0;
   out_4578642212278889444[271] = 0;
   out_4578642212278889444[272] = 0;
   out_4578642212278889444[273] = 0;
   out_4578642212278889444[274] = 0;
   out_4578642212278889444[275] = 0;
   out_4578642212278889444[276] = 0;
   out_4578642212278889444[277] = 0;
   out_4578642212278889444[278] = 0;
   out_4578642212278889444[279] = 0;
   out_4578642212278889444[280] = 0;
   out_4578642212278889444[281] = 0;
   out_4578642212278889444[282] = 0;
   out_4578642212278889444[283] = 0;
   out_4578642212278889444[284] = 0;
   out_4578642212278889444[285] = 1;
   out_4578642212278889444[286] = 0;
   out_4578642212278889444[287] = 0;
   out_4578642212278889444[288] = 0;
   out_4578642212278889444[289] = 0;
   out_4578642212278889444[290] = 0;
   out_4578642212278889444[291] = 0;
   out_4578642212278889444[292] = 0;
   out_4578642212278889444[293] = 0;
   out_4578642212278889444[294] = 0;
   out_4578642212278889444[295] = 0;
   out_4578642212278889444[296] = 0;
   out_4578642212278889444[297] = 0;
   out_4578642212278889444[298] = 0;
   out_4578642212278889444[299] = 0;
   out_4578642212278889444[300] = 0;
   out_4578642212278889444[301] = 0;
   out_4578642212278889444[302] = 0;
   out_4578642212278889444[303] = 0;
   out_4578642212278889444[304] = 1;
   out_4578642212278889444[305] = 0;
   out_4578642212278889444[306] = 0;
   out_4578642212278889444[307] = 0;
   out_4578642212278889444[308] = 0;
   out_4578642212278889444[309] = 0;
   out_4578642212278889444[310] = 0;
   out_4578642212278889444[311] = 0;
   out_4578642212278889444[312] = 0;
   out_4578642212278889444[313] = 0;
   out_4578642212278889444[314] = 0;
   out_4578642212278889444[315] = 0;
   out_4578642212278889444[316] = 0;
   out_4578642212278889444[317] = 0;
   out_4578642212278889444[318] = 0;
   out_4578642212278889444[319] = 0;
   out_4578642212278889444[320] = 0;
   out_4578642212278889444[321] = 0;
   out_4578642212278889444[322] = 0;
   out_4578642212278889444[323] = 1;
}
void h_4(double *state, double *unused, double *out_3436790236773195671) {
   out_3436790236773195671[0] = state[6] + state[9];
   out_3436790236773195671[1] = state[7] + state[10];
   out_3436790236773195671[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_5251122731926484440) {
   out_5251122731926484440[0] = 0;
   out_5251122731926484440[1] = 0;
   out_5251122731926484440[2] = 0;
   out_5251122731926484440[3] = 0;
   out_5251122731926484440[4] = 0;
   out_5251122731926484440[5] = 0;
   out_5251122731926484440[6] = 1;
   out_5251122731926484440[7] = 0;
   out_5251122731926484440[8] = 0;
   out_5251122731926484440[9] = 1;
   out_5251122731926484440[10] = 0;
   out_5251122731926484440[11] = 0;
   out_5251122731926484440[12] = 0;
   out_5251122731926484440[13] = 0;
   out_5251122731926484440[14] = 0;
   out_5251122731926484440[15] = 0;
   out_5251122731926484440[16] = 0;
   out_5251122731926484440[17] = 0;
   out_5251122731926484440[18] = 0;
   out_5251122731926484440[19] = 0;
   out_5251122731926484440[20] = 0;
   out_5251122731926484440[21] = 0;
   out_5251122731926484440[22] = 0;
   out_5251122731926484440[23] = 0;
   out_5251122731926484440[24] = 0;
   out_5251122731926484440[25] = 1;
   out_5251122731926484440[26] = 0;
   out_5251122731926484440[27] = 0;
   out_5251122731926484440[28] = 1;
   out_5251122731926484440[29] = 0;
   out_5251122731926484440[30] = 0;
   out_5251122731926484440[31] = 0;
   out_5251122731926484440[32] = 0;
   out_5251122731926484440[33] = 0;
   out_5251122731926484440[34] = 0;
   out_5251122731926484440[35] = 0;
   out_5251122731926484440[36] = 0;
   out_5251122731926484440[37] = 0;
   out_5251122731926484440[38] = 0;
   out_5251122731926484440[39] = 0;
   out_5251122731926484440[40] = 0;
   out_5251122731926484440[41] = 0;
   out_5251122731926484440[42] = 0;
   out_5251122731926484440[43] = 0;
   out_5251122731926484440[44] = 1;
   out_5251122731926484440[45] = 0;
   out_5251122731926484440[46] = 0;
   out_5251122731926484440[47] = 1;
   out_5251122731926484440[48] = 0;
   out_5251122731926484440[49] = 0;
   out_5251122731926484440[50] = 0;
   out_5251122731926484440[51] = 0;
   out_5251122731926484440[52] = 0;
   out_5251122731926484440[53] = 0;
}
void h_10(double *state, double *unused, double *out_7778470094055120042) {
   out_7778470094055120042[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7778470094055120042[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7778470094055120042[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4797712383092980417) {
   out_4797712383092980417[0] = 0;
   out_4797712383092980417[1] = 9.8100000000000005*cos(state[1]);
   out_4797712383092980417[2] = 0;
   out_4797712383092980417[3] = 0;
   out_4797712383092980417[4] = -state[8];
   out_4797712383092980417[5] = state[7];
   out_4797712383092980417[6] = 0;
   out_4797712383092980417[7] = state[5];
   out_4797712383092980417[8] = -state[4];
   out_4797712383092980417[9] = 0;
   out_4797712383092980417[10] = 0;
   out_4797712383092980417[11] = 0;
   out_4797712383092980417[12] = 1;
   out_4797712383092980417[13] = 0;
   out_4797712383092980417[14] = 0;
   out_4797712383092980417[15] = 1;
   out_4797712383092980417[16] = 0;
   out_4797712383092980417[17] = 0;
   out_4797712383092980417[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4797712383092980417[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4797712383092980417[20] = 0;
   out_4797712383092980417[21] = state[8];
   out_4797712383092980417[22] = 0;
   out_4797712383092980417[23] = -state[6];
   out_4797712383092980417[24] = -state[5];
   out_4797712383092980417[25] = 0;
   out_4797712383092980417[26] = state[3];
   out_4797712383092980417[27] = 0;
   out_4797712383092980417[28] = 0;
   out_4797712383092980417[29] = 0;
   out_4797712383092980417[30] = 0;
   out_4797712383092980417[31] = 1;
   out_4797712383092980417[32] = 0;
   out_4797712383092980417[33] = 0;
   out_4797712383092980417[34] = 1;
   out_4797712383092980417[35] = 0;
   out_4797712383092980417[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4797712383092980417[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4797712383092980417[38] = 0;
   out_4797712383092980417[39] = -state[7];
   out_4797712383092980417[40] = state[6];
   out_4797712383092980417[41] = 0;
   out_4797712383092980417[42] = state[4];
   out_4797712383092980417[43] = -state[3];
   out_4797712383092980417[44] = 0;
   out_4797712383092980417[45] = 0;
   out_4797712383092980417[46] = 0;
   out_4797712383092980417[47] = 0;
   out_4797712383092980417[48] = 0;
   out_4797712383092980417[49] = 0;
   out_4797712383092980417[50] = 1;
   out_4797712383092980417[51] = 0;
   out_4797712383092980417[52] = 0;
   out_4797712383092980417[53] = 1;
}
void h_13(double *state, double *unused, double *out_6994430400069285445) {
   out_6994430400069285445[0] = state[3];
   out_6994430400069285445[1] = state[4];
   out_6994430400069285445[2] = state[5];
}
void H_13(double *state, double *unused, double *out_5584990133466366247) {
   out_5584990133466366247[0] = 0;
   out_5584990133466366247[1] = 0;
   out_5584990133466366247[2] = 0;
   out_5584990133466366247[3] = 1;
   out_5584990133466366247[4] = 0;
   out_5584990133466366247[5] = 0;
   out_5584990133466366247[6] = 0;
   out_5584990133466366247[7] = 0;
   out_5584990133466366247[8] = 0;
   out_5584990133466366247[9] = 0;
   out_5584990133466366247[10] = 0;
   out_5584990133466366247[11] = 0;
   out_5584990133466366247[12] = 0;
   out_5584990133466366247[13] = 0;
   out_5584990133466366247[14] = 0;
   out_5584990133466366247[15] = 0;
   out_5584990133466366247[16] = 0;
   out_5584990133466366247[17] = 0;
   out_5584990133466366247[18] = 0;
   out_5584990133466366247[19] = 0;
   out_5584990133466366247[20] = 0;
   out_5584990133466366247[21] = 0;
   out_5584990133466366247[22] = 1;
   out_5584990133466366247[23] = 0;
   out_5584990133466366247[24] = 0;
   out_5584990133466366247[25] = 0;
   out_5584990133466366247[26] = 0;
   out_5584990133466366247[27] = 0;
   out_5584990133466366247[28] = 0;
   out_5584990133466366247[29] = 0;
   out_5584990133466366247[30] = 0;
   out_5584990133466366247[31] = 0;
   out_5584990133466366247[32] = 0;
   out_5584990133466366247[33] = 0;
   out_5584990133466366247[34] = 0;
   out_5584990133466366247[35] = 0;
   out_5584990133466366247[36] = 0;
   out_5584990133466366247[37] = 0;
   out_5584990133466366247[38] = 0;
   out_5584990133466366247[39] = 0;
   out_5584990133466366247[40] = 0;
   out_5584990133466366247[41] = 1;
   out_5584990133466366247[42] = 0;
   out_5584990133466366247[43] = 0;
   out_5584990133466366247[44] = 0;
   out_5584990133466366247[45] = 0;
   out_5584990133466366247[46] = 0;
   out_5584990133466366247[47] = 0;
   out_5584990133466366247[48] = 0;
   out_5584990133466366247[49] = 0;
   out_5584990133466366247[50] = 0;
   out_5584990133466366247[51] = 0;
   out_5584990133466366247[52] = 0;
   out_5584990133466366247[53] = 0;
}
void h_14(double *state, double *unused, double *out_3179950759880466578) {
   out_3179950759880466578[0] = state[6];
   out_3179950759880466578[1] = state[7];
   out_3179950759880466578[2] = state[8];
}
void H_14(double *state, double *unused, double *out_9214363588265968969) {
   out_9214363588265968969[0] = 0;
   out_9214363588265968969[1] = 0;
   out_9214363588265968969[2] = 0;
   out_9214363588265968969[3] = 0;
   out_9214363588265968969[4] = 0;
   out_9214363588265968969[5] = 0;
   out_9214363588265968969[6] = 1;
   out_9214363588265968969[7] = 0;
   out_9214363588265968969[8] = 0;
   out_9214363588265968969[9] = 0;
   out_9214363588265968969[10] = 0;
   out_9214363588265968969[11] = 0;
   out_9214363588265968969[12] = 0;
   out_9214363588265968969[13] = 0;
   out_9214363588265968969[14] = 0;
   out_9214363588265968969[15] = 0;
   out_9214363588265968969[16] = 0;
   out_9214363588265968969[17] = 0;
   out_9214363588265968969[18] = 0;
   out_9214363588265968969[19] = 0;
   out_9214363588265968969[20] = 0;
   out_9214363588265968969[21] = 0;
   out_9214363588265968969[22] = 0;
   out_9214363588265968969[23] = 0;
   out_9214363588265968969[24] = 0;
   out_9214363588265968969[25] = 1;
   out_9214363588265968969[26] = 0;
   out_9214363588265968969[27] = 0;
   out_9214363588265968969[28] = 0;
   out_9214363588265968969[29] = 0;
   out_9214363588265968969[30] = 0;
   out_9214363588265968969[31] = 0;
   out_9214363588265968969[32] = 0;
   out_9214363588265968969[33] = 0;
   out_9214363588265968969[34] = 0;
   out_9214363588265968969[35] = 0;
   out_9214363588265968969[36] = 0;
   out_9214363588265968969[37] = 0;
   out_9214363588265968969[38] = 0;
   out_9214363588265968969[39] = 0;
   out_9214363588265968969[40] = 0;
   out_9214363588265968969[41] = 0;
   out_9214363588265968969[42] = 0;
   out_9214363588265968969[43] = 0;
   out_9214363588265968969[44] = 1;
   out_9214363588265968969[45] = 0;
   out_9214363588265968969[46] = 0;
   out_9214363588265968969[47] = 0;
   out_9214363588265968969[48] = 0;
   out_9214363588265968969[49] = 0;
   out_9214363588265968969[50] = 0;
   out_9214363588265968969[51] = 0;
   out_9214363588265968969[52] = 0;
   out_9214363588265968969[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_7182192695751697228) {
  err_fun(nom_x, delta_x, out_7182192695751697228);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_800007061037745391) {
  inv_err_fun(nom_x, true_x, out_800007061037745391);
}
void pose_H_mod_fun(double *state, double *out_4496961729871771833) {
  H_mod_fun(state, out_4496961729871771833);
}
void pose_f_fun(double *state, double dt, double *out_7612049814342810855) {
  f_fun(state,  dt, out_7612049814342810855);
}
void pose_F_fun(double *state, double dt, double *out_4578642212278889444) {
  F_fun(state,  dt, out_4578642212278889444);
}
void pose_h_4(double *state, double *unused, double *out_3436790236773195671) {
  h_4(state, unused, out_3436790236773195671);
}
void pose_H_4(double *state, double *unused, double *out_5251122731926484440) {
  H_4(state, unused, out_5251122731926484440);
}
void pose_h_10(double *state, double *unused, double *out_7778470094055120042) {
  h_10(state, unused, out_7778470094055120042);
}
void pose_H_10(double *state, double *unused, double *out_4797712383092980417) {
  H_10(state, unused, out_4797712383092980417);
}
void pose_h_13(double *state, double *unused, double *out_6994430400069285445) {
  h_13(state, unused, out_6994430400069285445);
}
void pose_H_13(double *state, double *unused, double *out_5584990133466366247) {
  H_13(state, unused, out_5584990133466366247);
}
void pose_h_14(double *state, double *unused, double *out_3179950759880466578) {
  h_14(state, unused, out_3179950759880466578);
}
void pose_H_14(double *state, double *unused, double *out_9214363588265968969) {
  H_14(state, unused, out_9214363588265968969);
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
