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
void err_fun(double *nom_x, double *delta_x, double *out_5042717407786303213) {
   out_5042717407786303213[0] = delta_x[0] + nom_x[0];
   out_5042717407786303213[1] = delta_x[1] + nom_x[1];
   out_5042717407786303213[2] = delta_x[2] + nom_x[2];
   out_5042717407786303213[3] = delta_x[3] + nom_x[3];
   out_5042717407786303213[4] = delta_x[4] + nom_x[4];
   out_5042717407786303213[5] = delta_x[5] + nom_x[5];
   out_5042717407786303213[6] = delta_x[6] + nom_x[6];
   out_5042717407786303213[7] = delta_x[7] + nom_x[7];
   out_5042717407786303213[8] = delta_x[8] + nom_x[8];
   out_5042717407786303213[9] = delta_x[9] + nom_x[9];
   out_5042717407786303213[10] = delta_x[10] + nom_x[10];
   out_5042717407786303213[11] = delta_x[11] + nom_x[11];
   out_5042717407786303213[12] = delta_x[12] + nom_x[12];
   out_5042717407786303213[13] = delta_x[13] + nom_x[13];
   out_5042717407786303213[14] = delta_x[14] + nom_x[14];
   out_5042717407786303213[15] = delta_x[15] + nom_x[15];
   out_5042717407786303213[16] = delta_x[16] + nom_x[16];
   out_5042717407786303213[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1689930166948831822) {
   out_1689930166948831822[0] = -nom_x[0] + true_x[0];
   out_1689930166948831822[1] = -nom_x[1] + true_x[1];
   out_1689930166948831822[2] = -nom_x[2] + true_x[2];
   out_1689930166948831822[3] = -nom_x[3] + true_x[3];
   out_1689930166948831822[4] = -nom_x[4] + true_x[4];
   out_1689930166948831822[5] = -nom_x[5] + true_x[5];
   out_1689930166948831822[6] = -nom_x[6] + true_x[6];
   out_1689930166948831822[7] = -nom_x[7] + true_x[7];
   out_1689930166948831822[8] = -nom_x[8] + true_x[8];
   out_1689930166948831822[9] = -nom_x[9] + true_x[9];
   out_1689930166948831822[10] = -nom_x[10] + true_x[10];
   out_1689930166948831822[11] = -nom_x[11] + true_x[11];
   out_1689930166948831822[12] = -nom_x[12] + true_x[12];
   out_1689930166948831822[13] = -nom_x[13] + true_x[13];
   out_1689930166948831822[14] = -nom_x[14] + true_x[14];
   out_1689930166948831822[15] = -nom_x[15] + true_x[15];
   out_1689930166948831822[16] = -nom_x[16] + true_x[16];
   out_1689930166948831822[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_3910683721715691775) {
   out_3910683721715691775[0] = 1.0;
   out_3910683721715691775[1] = 0.0;
   out_3910683721715691775[2] = 0.0;
   out_3910683721715691775[3] = 0.0;
   out_3910683721715691775[4] = 0.0;
   out_3910683721715691775[5] = 0.0;
   out_3910683721715691775[6] = 0.0;
   out_3910683721715691775[7] = 0.0;
   out_3910683721715691775[8] = 0.0;
   out_3910683721715691775[9] = 0.0;
   out_3910683721715691775[10] = 0.0;
   out_3910683721715691775[11] = 0.0;
   out_3910683721715691775[12] = 0.0;
   out_3910683721715691775[13] = 0.0;
   out_3910683721715691775[14] = 0.0;
   out_3910683721715691775[15] = 0.0;
   out_3910683721715691775[16] = 0.0;
   out_3910683721715691775[17] = 0.0;
   out_3910683721715691775[18] = 0.0;
   out_3910683721715691775[19] = 1.0;
   out_3910683721715691775[20] = 0.0;
   out_3910683721715691775[21] = 0.0;
   out_3910683721715691775[22] = 0.0;
   out_3910683721715691775[23] = 0.0;
   out_3910683721715691775[24] = 0.0;
   out_3910683721715691775[25] = 0.0;
   out_3910683721715691775[26] = 0.0;
   out_3910683721715691775[27] = 0.0;
   out_3910683721715691775[28] = 0.0;
   out_3910683721715691775[29] = 0.0;
   out_3910683721715691775[30] = 0.0;
   out_3910683721715691775[31] = 0.0;
   out_3910683721715691775[32] = 0.0;
   out_3910683721715691775[33] = 0.0;
   out_3910683721715691775[34] = 0.0;
   out_3910683721715691775[35] = 0.0;
   out_3910683721715691775[36] = 0.0;
   out_3910683721715691775[37] = 0.0;
   out_3910683721715691775[38] = 1.0;
   out_3910683721715691775[39] = 0.0;
   out_3910683721715691775[40] = 0.0;
   out_3910683721715691775[41] = 0.0;
   out_3910683721715691775[42] = 0.0;
   out_3910683721715691775[43] = 0.0;
   out_3910683721715691775[44] = 0.0;
   out_3910683721715691775[45] = 0.0;
   out_3910683721715691775[46] = 0.0;
   out_3910683721715691775[47] = 0.0;
   out_3910683721715691775[48] = 0.0;
   out_3910683721715691775[49] = 0.0;
   out_3910683721715691775[50] = 0.0;
   out_3910683721715691775[51] = 0.0;
   out_3910683721715691775[52] = 0.0;
   out_3910683721715691775[53] = 0.0;
   out_3910683721715691775[54] = 0.0;
   out_3910683721715691775[55] = 0.0;
   out_3910683721715691775[56] = 0.0;
   out_3910683721715691775[57] = 1.0;
   out_3910683721715691775[58] = 0.0;
   out_3910683721715691775[59] = 0.0;
   out_3910683721715691775[60] = 0.0;
   out_3910683721715691775[61] = 0.0;
   out_3910683721715691775[62] = 0.0;
   out_3910683721715691775[63] = 0.0;
   out_3910683721715691775[64] = 0.0;
   out_3910683721715691775[65] = 0.0;
   out_3910683721715691775[66] = 0.0;
   out_3910683721715691775[67] = 0.0;
   out_3910683721715691775[68] = 0.0;
   out_3910683721715691775[69] = 0.0;
   out_3910683721715691775[70] = 0.0;
   out_3910683721715691775[71] = 0.0;
   out_3910683721715691775[72] = 0.0;
   out_3910683721715691775[73] = 0.0;
   out_3910683721715691775[74] = 0.0;
   out_3910683721715691775[75] = 0.0;
   out_3910683721715691775[76] = 1.0;
   out_3910683721715691775[77] = 0.0;
   out_3910683721715691775[78] = 0.0;
   out_3910683721715691775[79] = 0.0;
   out_3910683721715691775[80] = 0.0;
   out_3910683721715691775[81] = 0.0;
   out_3910683721715691775[82] = 0.0;
   out_3910683721715691775[83] = 0.0;
   out_3910683721715691775[84] = 0.0;
   out_3910683721715691775[85] = 0.0;
   out_3910683721715691775[86] = 0.0;
   out_3910683721715691775[87] = 0.0;
   out_3910683721715691775[88] = 0.0;
   out_3910683721715691775[89] = 0.0;
   out_3910683721715691775[90] = 0.0;
   out_3910683721715691775[91] = 0.0;
   out_3910683721715691775[92] = 0.0;
   out_3910683721715691775[93] = 0.0;
   out_3910683721715691775[94] = 0.0;
   out_3910683721715691775[95] = 1.0;
   out_3910683721715691775[96] = 0.0;
   out_3910683721715691775[97] = 0.0;
   out_3910683721715691775[98] = 0.0;
   out_3910683721715691775[99] = 0.0;
   out_3910683721715691775[100] = 0.0;
   out_3910683721715691775[101] = 0.0;
   out_3910683721715691775[102] = 0.0;
   out_3910683721715691775[103] = 0.0;
   out_3910683721715691775[104] = 0.0;
   out_3910683721715691775[105] = 0.0;
   out_3910683721715691775[106] = 0.0;
   out_3910683721715691775[107] = 0.0;
   out_3910683721715691775[108] = 0.0;
   out_3910683721715691775[109] = 0.0;
   out_3910683721715691775[110] = 0.0;
   out_3910683721715691775[111] = 0.0;
   out_3910683721715691775[112] = 0.0;
   out_3910683721715691775[113] = 0.0;
   out_3910683721715691775[114] = 1.0;
   out_3910683721715691775[115] = 0.0;
   out_3910683721715691775[116] = 0.0;
   out_3910683721715691775[117] = 0.0;
   out_3910683721715691775[118] = 0.0;
   out_3910683721715691775[119] = 0.0;
   out_3910683721715691775[120] = 0.0;
   out_3910683721715691775[121] = 0.0;
   out_3910683721715691775[122] = 0.0;
   out_3910683721715691775[123] = 0.0;
   out_3910683721715691775[124] = 0.0;
   out_3910683721715691775[125] = 0.0;
   out_3910683721715691775[126] = 0.0;
   out_3910683721715691775[127] = 0.0;
   out_3910683721715691775[128] = 0.0;
   out_3910683721715691775[129] = 0.0;
   out_3910683721715691775[130] = 0.0;
   out_3910683721715691775[131] = 0.0;
   out_3910683721715691775[132] = 0.0;
   out_3910683721715691775[133] = 1.0;
   out_3910683721715691775[134] = 0.0;
   out_3910683721715691775[135] = 0.0;
   out_3910683721715691775[136] = 0.0;
   out_3910683721715691775[137] = 0.0;
   out_3910683721715691775[138] = 0.0;
   out_3910683721715691775[139] = 0.0;
   out_3910683721715691775[140] = 0.0;
   out_3910683721715691775[141] = 0.0;
   out_3910683721715691775[142] = 0.0;
   out_3910683721715691775[143] = 0.0;
   out_3910683721715691775[144] = 0.0;
   out_3910683721715691775[145] = 0.0;
   out_3910683721715691775[146] = 0.0;
   out_3910683721715691775[147] = 0.0;
   out_3910683721715691775[148] = 0.0;
   out_3910683721715691775[149] = 0.0;
   out_3910683721715691775[150] = 0.0;
   out_3910683721715691775[151] = 0.0;
   out_3910683721715691775[152] = 1.0;
   out_3910683721715691775[153] = 0.0;
   out_3910683721715691775[154] = 0.0;
   out_3910683721715691775[155] = 0.0;
   out_3910683721715691775[156] = 0.0;
   out_3910683721715691775[157] = 0.0;
   out_3910683721715691775[158] = 0.0;
   out_3910683721715691775[159] = 0.0;
   out_3910683721715691775[160] = 0.0;
   out_3910683721715691775[161] = 0.0;
   out_3910683721715691775[162] = 0.0;
   out_3910683721715691775[163] = 0.0;
   out_3910683721715691775[164] = 0.0;
   out_3910683721715691775[165] = 0.0;
   out_3910683721715691775[166] = 0.0;
   out_3910683721715691775[167] = 0.0;
   out_3910683721715691775[168] = 0.0;
   out_3910683721715691775[169] = 0.0;
   out_3910683721715691775[170] = 0.0;
   out_3910683721715691775[171] = 1.0;
   out_3910683721715691775[172] = 0.0;
   out_3910683721715691775[173] = 0.0;
   out_3910683721715691775[174] = 0.0;
   out_3910683721715691775[175] = 0.0;
   out_3910683721715691775[176] = 0.0;
   out_3910683721715691775[177] = 0.0;
   out_3910683721715691775[178] = 0.0;
   out_3910683721715691775[179] = 0.0;
   out_3910683721715691775[180] = 0.0;
   out_3910683721715691775[181] = 0.0;
   out_3910683721715691775[182] = 0.0;
   out_3910683721715691775[183] = 0.0;
   out_3910683721715691775[184] = 0.0;
   out_3910683721715691775[185] = 0.0;
   out_3910683721715691775[186] = 0.0;
   out_3910683721715691775[187] = 0.0;
   out_3910683721715691775[188] = 0.0;
   out_3910683721715691775[189] = 0.0;
   out_3910683721715691775[190] = 1.0;
   out_3910683721715691775[191] = 0.0;
   out_3910683721715691775[192] = 0.0;
   out_3910683721715691775[193] = 0.0;
   out_3910683721715691775[194] = 0.0;
   out_3910683721715691775[195] = 0.0;
   out_3910683721715691775[196] = 0.0;
   out_3910683721715691775[197] = 0.0;
   out_3910683721715691775[198] = 0.0;
   out_3910683721715691775[199] = 0.0;
   out_3910683721715691775[200] = 0.0;
   out_3910683721715691775[201] = 0.0;
   out_3910683721715691775[202] = 0.0;
   out_3910683721715691775[203] = 0.0;
   out_3910683721715691775[204] = 0.0;
   out_3910683721715691775[205] = 0.0;
   out_3910683721715691775[206] = 0.0;
   out_3910683721715691775[207] = 0.0;
   out_3910683721715691775[208] = 0.0;
   out_3910683721715691775[209] = 1.0;
   out_3910683721715691775[210] = 0.0;
   out_3910683721715691775[211] = 0.0;
   out_3910683721715691775[212] = 0.0;
   out_3910683721715691775[213] = 0.0;
   out_3910683721715691775[214] = 0.0;
   out_3910683721715691775[215] = 0.0;
   out_3910683721715691775[216] = 0.0;
   out_3910683721715691775[217] = 0.0;
   out_3910683721715691775[218] = 0.0;
   out_3910683721715691775[219] = 0.0;
   out_3910683721715691775[220] = 0.0;
   out_3910683721715691775[221] = 0.0;
   out_3910683721715691775[222] = 0.0;
   out_3910683721715691775[223] = 0.0;
   out_3910683721715691775[224] = 0.0;
   out_3910683721715691775[225] = 0.0;
   out_3910683721715691775[226] = 0.0;
   out_3910683721715691775[227] = 0.0;
   out_3910683721715691775[228] = 1.0;
   out_3910683721715691775[229] = 0.0;
   out_3910683721715691775[230] = 0.0;
   out_3910683721715691775[231] = 0.0;
   out_3910683721715691775[232] = 0.0;
   out_3910683721715691775[233] = 0.0;
   out_3910683721715691775[234] = 0.0;
   out_3910683721715691775[235] = 0.0;
   out_3910683721715691775[236] = 0.0;
   out_3910683721715691775[237] = 0.0;
   out_3910683721715691775[238] = 0.0;
   out_3910683721715691775[239] = 0.0;
   out_3910683721715691775[240] = 0.0;
   out_3910683721715691775[241] = 0.0;
   out_3910683721715691775[242] = 0.0;
   out_3910683721715691775[243] = 0.0;
   out_3910683721715691775[244] = 0.0;
   out_3910683721715691775[245] = 0.0;
   out_3910683721715691775[246] = 0.0;
   out_3910683721715691775[247] = 1.0;
   out_3910683721715691775[248] = 0.0;
   out_3910683721715691775[249] = 0.0;
   out_3910683721715691775[250] = 0.0;
   out_3910683721715691775[251] = 0.0;
   out_3910683721715691775[252] = 0.0;
   out_3910683721715691775[253] = 0.0;
   out_3910683721715691775[254] = 0.0;
   out_3910683721715691775[255] = 0.0;
   out_3910683721715691775[256] = 0.0;
   out_3910683721715691775[257] = 0.0;
   out_3910683721715691775[258] = 0.0;
   out_3910683721715691775[259] = 0.0;
   out_3910683721715691775[260] = 0.0;
   out_3910683721715691775[261] = 0.0;
   out_3910683721715691775[262] = 0.0;
   out_3910683721715691775[263] = 0.0;
   out_3910683721715691775[264] = 0.0;
   out_3910683721715691775[265] = 0.0;
   out_3910683721715691775[266] = 1.0;
   out_3910683721715691775[267] = 0.0;
   out_3910683721715691775[268] = 0.0;
   out_3910683721715691775[269] = 0.0;
   out_3910683721715691775[270] = 0.0;
   out_3910683721715691775[271] = 0.0;
   out_3910683721715691775[272] = 0.0;
   out_3910683721715691775[273] = 0.0;
   out_3910683721715691775[274] = 0.0;
   out_3910683721715691775[275] = 0.0;
   out_3910683721715691775[276] = 0.0;
   out_3910683721715691775[277] = 0.0;
   out_3910683721715691775[278] = 0.0;
   out_3910683721715691775[279] = 0.0;
   out_3910683721715691775[280] = 0.0;
   out_3910683721715691775[281] = 0.0;
   out_3910683721715691775[282] = 0.0;
   out_3910683721715691775[283] = 0.0;
   out_3910683721715691775[284] = 0.0;
   out_3910683721715691775[285] = 1.0;
   out_3910683721715691775[286] = 0.0;
   out_3910683721715691775[287] = 0.0;
   out_3910683721715691775[288] = 0.0;
   out_3910683721715691775[289] = 0.0;
   out_3910683721715691775[290] = 0.0;
   out_3910683721715691775[291] = 0.0;
   out_3910683721715691775[292] = 0.0;
   out_3910683721715691775[293] = 0.0;
   out_3910683721715691775[294] = 0.0;
   out_3910683721715691775[295] = 0.0;
   out_3910683721715691775[296] = 0.0;
   out_3910683721715691775[297] = 0.0;
   out_3910683721715691775[298] = 0.0;
   out_3910683721715691775[299] = 0.0;
   out_3910683721715691775[300] = 0.0;
   out_3910683721715691775[301] = 0.0;
   out_3910683721715691775[302] = 0.0;
   out_3910683721715691775[303] = 0.0;
   out_3910683721715691775[304] = 1.0;
   out_3910683721715691775[305] = 0.0;
   out_3910683721715691775[306] = 0.0;
   out_3910683721715691775[307] = 0.0;
   out_3910683721715691775[308] = 0.0;
   out_3910683721715691775[309] = 0.0;
   out_3910683721715691775[310] = 0.0;
   out_3910683721715691775[311] = 0.0;
   out_3910683721715691775[312] = 0.0;
   out_3910683721715691775[313] = 0.0;
   out_3910683721715691775[314] = 0.0;
   out_3910683721715691775[315] = 0.0;
   out_3910683721715691775[316] = 0.0;
   out_3910683721715691775[317] = 0.0;
   out_3910683721715691775[318] = 0.0;
   out_3910683721715691775[319] = 0.0;
   out_3910683721715691775[320] = 0.0;
   out_3910683721715691775[321] = 0.0;
   out_3910683721715691775[322] = 0.0;
   out_3910683721715691775[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_6246311212408209668) {
   out_6246311212408209668[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_6246311212408209668[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_6246311212408209668[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_6246311212408209668[3] = dt*state[12] + state[3];
   out_6246311212408209668[4] = dt*state[13] + state[4];
   out_6246311212408209668[5] = dt*state[14] + state[5];
   out_6246311212408209668[6] = state[6];
   out_6246311212408209668[7] = state[7];
   out_6246311212408209668[8] = state[8];
   out_6246311212408209668[9] = state[9];
   out_6246311212408209668[10] = state[10];
   out_6246311212408209668[11] = state[11];
   out_6246311212408209668[12] = state[12];
   out_6246311212408209668[13] = state[13];
   out_6246311212408209668[14] = state[14];
   out_6246311212408209668[15] = state[15];
   out_6246311212408209668[16] = state[16];
   out_6246311212408209668[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3355224291126807009) {
   out_3355224291126807009[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3355224291126807009[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3355224291126807009[2] = 0;
   out_3355224291126807009[3] = 0;
   out_3355224291126807009[4] = 0;
   out_3355224291126807009[5] = 0;
   out_3355224291126807009[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3355224291126807009[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3355224291126807009[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3355224291126807009[9] = 0;
   out_3355224291126807009[10] = 0;
   out_3355224291126807009[11] = 0;
   out_3355224291126807009[12] = 0;
   out_3355224291126807009[13] = 0;
   out_3355224291126807009[14] = 0;
   out_3355224291126807009[15] = 0;
   out_3355224291126807009[16] = 0;
   out_3355224291126807009[17] = 0;
   out_3355224291126807009[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3355224291126807009[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3355224291126807009[20] = 0;
   out_3355224291126807009[21] = 0;
   out_3355224291126807009[22] = 0;
   out_3355224291126807009[23] = 0;
   out_3355224291126807009[24] = 0;
   out_3355224291126807009[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3355224291126807009[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3355224291126807009[27] = 0;
   out_3355224291126807009[28] = 0;
   out_3355224291126807009[29] = 0;
   out_3355224291126807009[30] = 0;
   out_3355224291126807009[31] = 0;
   out_3355224291126807009[32] = 0;
   out_3355224291126807009[33] = 0;
   out_3355224291126807009[34] = 0;
   out_3355224291126807009[35] = 0;
   out_3355224291126807009[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3355224291126807009[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3355224291126807009[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3355224291126807009[39] = 0;
   out_3355224291126807009[40] = 0;
   out_3355224291126807009[41] = 0;
   out_3355224291126807009[42] = 0;
   out_3355224291126807009[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3355224291126807009[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3355224291126807009[45] = 0;
   out_3355224291126807009[46] = 0;
   out_3355224291126807009[47] = 0;
   out_3355224291126807009[48] = 0;
   out_3355224291126807009[49] = 0;
   out_3355224291126807009[50] = 0;
   out_3355224291126807009[51] = 0;
   out_3355224291126807009[52] = 0;
   out_3355224291126807009[53] = 0;
   out_3355224291126807009[54] = 0;
   out_3355224291126807009[55] = 0;
   out_3355224291126807009[56] = 0;
   out_3355224291126807009[57] = 1;
   out_3355224291126807009[58] = 0;
   out_3355224291126807009[59] = 0;
   out_3355224291126807009[60] = 0;
   out_3355224291126807009[61] = 0;
   out_3355224291126807009[62] = 0;
   out_3355224291126807009[63] = 0;
   out_3355224291126807009[64] = 0;
   out_3355224291126807009[65] = 0;
   out_3355224291126807009[66] = dt;
   out_3355224291126807009[67] = 0;
   out_3355224291126807009[68] = 0;
   out_3355224291126807009[69] = 0;
   out_3355224291126807009[70] = 0;
   out_3355224291126807009[71] = 0;
   out_3355224291126807009[72] = 0;
   out_3355224291126807009[73] = 0;
   out_3355224291126807009[74] = 0;
   out_3355224291126807009[75] = 0;
   out_3355224291126807009[76] = 1;
   out_3355224291126807009[77] = 0;
   out_3355224291126807009[78] = 0;
   out_3355224291126807009[79] = 0;
   out_3355224291126807009[80] = 0;
   out_3355224291126807009[81] = 0;
   out_3355224291126807009[82] = 0;
   out_3355224291126807009[83] = 0;
   out_3355224291126807009[84] = 0;
   out_3355224291126807009[85] = dt;
   out_3355224291126807009[86] = 0;
   out_3355224291126807009[87] = 0;
   out_3355224291126807009[88] = 0;
   out_3355224291126807009[89] = 0;
   out_3355224291126807009[90] = 0;
   out_3355224291126807009[91] = 0;
   out_3355224291126807009[92] = 0;
   out_3355224291126807009[93] = 0;
   out_3355224291126807009[94] = 0;
   out_3355224291126807009[95] = 1;
   out_3355224291126807009[96] = 0;
   out_3355224291126807009[97] = 0;
   out_3355224291126807009[98] = 0;
   out_3355224291126807009[99] = 0;
   out_3355224291126807009[100] = 0;
   out_3355224291126807009[101] = 0;
   out_3355224291126807009[102] = 0;
   out_3355224291126807009[103] = 0;
   out_3355224291126807009[104] = dt;
   out_3355224291126807009[105] = 0;
   out_3355224291126807009[106] = 0;
   out_3355224291126807009[107] = 0;
   out_3355224291126807009[108] = 0;
   out_3355224291126807009[109] = 0;
   out_3355224291126807009[110] = 0;
   out_3355224291126807009[111] = 0;
   out_3355224291126807009[112] = 0;
   out_3355224291126807009[113] = 0;
   out_3355224291126807009[114] = 1;
   out_3355224291126807009[115] = 0;
   out_3355224291126807009[116] = 0;
   out_3355224291126807009[117] = 0;
   out_3355224291126807009[118] = 0;
   out_3355224291126807009[119] = 0;
   out_3355224291126807009[120] = 0;
   out_3355224291126807009[121] = 0;
   out_3355224291126807009[122] = 0;
   out_3355224291126807009[123] = 0;
   out_3355224291126807009[124] = 0;
   out_3355224291126807009[125] = 0;
   out_3355224291126807009[126] = 0;
   out_3355224291126807009[127] = 0;
   out_3355224291126807009[128] = 0;
   out_3355224291126807009[129] = 0;
   out_3355224291126807009[130] = 0;
   out_3355224291126807009[131] = 0;
   out_3355224291126807009[132] = 0;
   out_3355224291126807009[133] = 1;
   out_3355224291126807009[134] = 0;
   out_3355224291126807009[135] = 0;
   out_3355224291126807009[136] = 0;
   out_3355224291126807009[137] = 0;
   out_3355224291126807009[138] = 0;
   out_3355224291126807009[139] = 0;
   out_3355224291126807009[140] = 0;
   out_3355224291126807009[141] = 0;
   out_3355224291126807009[142] = 0;
   out_3355224291126807009[143] = 0;
   out_3355224291126807009[144] = 0;
   out_3355224291126807009[145] = 0;
   out_3355224291126807009[146] = 0;
   out_3355224291126807009[147] = 0;
   out_3355224291126807009[148] = 0;
   out_3355224291126807009[149] = 0;
   out_3355224291126807009[150] = 0;
   out_3355224291126807009[151] = 0;
   out_3355224291126807009[152] = 1;
   out_3355224291126807009[153] = 0;
   out_3355224291126807009[154] = 0;
   out_3355224291126807009[155] = 0;
   out_3355224291126807009[156] = 0;
   out_3355224291126807009[157] = 0;
   out_3355224291126807009[158] = 0;
   out_3355224291126807009[159] = 0;
   out_3355224291126807009[160] = 0;
   out_3355224291126807009[161] = 0;
   out_3355224291126807009[162] = 0;
   out_3355224291126807009[163] = 0;
   out_3355224291126807009[164] = 0;
   out_3355224291126807009[165] = 0;
   out_3355224291126807009[166] = 0;
   out_3355224291126807009[167] = 0;
   out_3355224291126807009[168] = 0;
   out_3355224291126807009[169] = 0;
   out_3355224291126807009[170] = 0;
   out_3355224291126807009[171] = 1;
   out_3355224291126807009[172] = 0;
   out_3355224291126807009[173] = 0;
   out_3355224291126807009[174] = 0;
   out_3355224291126807009[175] = 0;
   out_3355224291126807009[176] = 0;
   out_3355224291126807009[177] = 0;
   out_3355224291126807009[178] = 0;
   out_3355224291126807009[179] = 0;
   out_3355224291126807009[180] = 0;
   out_3355224291126807009[181] = 0;
   out_3355224291126807009[182] = 0;
   out_3355224291126807009[183] = 0;
   out_3355224291126807009[184] = 0;
   out_3355224291126807009[185] = 0;
   out_3355224291126807009[186] = 0;
   out_3355224291126807009[187] = 0;
   out_3355224291126807009[188] = 0;
   out_3355224291126807009[189] = 0;
   out_3355224291126807009[190] = 1;
   out_3355224291126807009[191] = 0;
   out_3355224291126807009[192] = 0;
   out_3355224291126807009[193] = 0;
   out_3355224291126807009[194] = 0;
   out_3355224291126807009[195] = 0;
   out_3355224291126807009[196] = 0;
   out_3355224291126807009[197] = 0;
   out_3355224291126807009[198] = 0;
   out_3355224291126807009[199] = 0;
   out_3355224291126807009[200] = 0;
   out_3355224291126807009[201] = 0;
   out_3355224291126807009[202] = 0;
   out_3355224291126807009[203] = 0;
   out_3355224291126807009[204] = 0;
   out_3355224291126807009[205] = 0;
   out_3355224291126807009[206] = 0;
   out_3355224291126807009[207] = 0;
   out_3355224291126807009[208] = 0;
   out_3355224291126807009[209] = 1;
   out_3355224291126807009[210] = 0;
   out_3355224291126807009[211] = 0;
   out_3355224291126807009[212] = 0;
   out_3355224291126807009[213] = 0;
   out_3355224291126807009[214] = 0;
   out_3355224291126807009[215] = 0;
   out_3355224291126807009[216] = 0;
   out_3355224291126807009[217] = 0;
   out_3355224291126807009[218] = 0;
   out_3355224291126807009[219] = 0;
   out_3355224291126807009[220] = 0;
   out_3355224291126807009[221] = 0;
   out_3355224291126807009[222] = 0;
   out_3355224291126807009[223] = 0;
   out_3355224291126807009[224] = 0;
   out_3355224291126807009[225] = 0;
   out_3355224291126807009[226] = 0;
   out_3355224291126807009[227] = 0;
   out_3355224291126807009[228] = 1;
   out_3355224291126807009[229] = 0;
   out_3355224291126807009[230] = 0;
   out_3355224291126807009[231] = 0;
   out_3355224291126807009[232] = 0;
   out_3355224291126807009[233] = 0;
   out_3355224291126807009[234] = 0;
   out_3355224291126807009[235] = 0;
   out_3355224291126807009[236] = 0;
   out_3355224291126807009[237] = 0;
   out_3355224291126807009[238] = 0;
   out_3355224291126807009[239] = 0;
   out_3355224291126807009[240] = 0;
   out_3355224291126807009[241] = 0;
   out_3355224291126807009[242] = 0;
   out_3355224291126807009[243] = 0;
   out_3355224291126807009[244] = 0;
   out_3355224291126807009[245] = 0;
   out_3355224291126807009[246] = 0;
   out_3355224291126807009[247] = 1;
   out_3355224291126807009[248] = 0;
   out_3355224291126807009[249] = 0;
   out_3355224291126807009[250] = 0;
   out_3355224291126807009[251] = 0;
   out_3355224291126807009[252] = 0;
   out_3355224291126807009[253] = 0;
   out_3355224291126807009[254] = 0;
   out_3355224291126807009[255] = 0;
   out_3355224291126807009[256] = 0;
   out_3355224291126807009[257] = 0;
   out_3355224291126807009[258] = 0;
   out_3355224291126807009[259] = 0;
   out_3355224291126807009[260] = 0;
   out_3355224291126807009[261] = 0;
   out_3355224291126807009[262] = 0;
   out_3355224291126807009[263] = 0;
   out_3355224291126807009[264] = 0;
   out_3355224291126807009[265] = 0;
   out_3355224291126807009[266] = 1;
   out_3355224291126807009[267] = 0;
   out_3355224291126807009[268] = 0;
   out_3355224291126807009[269] = 0;
   out_3355224291126807009[270] = 0;
   out_3355224291126807009[271] = 0;
   out_3355224291126807009[272] = 0;
   out_3355224291126807009[273] = 0;
   out_3355224291126807009[274] = 0;
   out_3355224291126807009[275] = 0;
   out_3355224291126807009[276] = 0;
   out_3355224291126807009[277] = 0;
   out_3355224291126807009[278] = 0;
   out_3355224291126807009[279] = 0;
   out_3355224291126807009[280] = 0;
   out_3355224291126807009[281] = 0;
   out_3355224291126807009[282] = 0;
   out_3355224291126807009[283] = 0;
   out_3355224291126807009[284] = 0;
   out_3355224291126807009[285] = 1;
   out_3355224291126807009[286] = 0;
   out_3355224291126807009[287] = 0;
   out_3355224291126807009[288] = 0;
   out_3355224291126807009[289] = 0;
   out_3355224291126807009[290] = 0;
   out_3355224291126807009[291] = 0;
   out_3355224291126807009[292] = 0;
   out_3355224291126807009[293] = 0;
   out_3355224291126807009[294] = 0;
   out_3355224291126807009[295] = 0;
   out_3355224291126807009[296] = 0;
   out_3355224291126807009[297] = 0;
   out_3355224291126807009[298] = 0;
   out_3355224291126807009[299] = 0;
   out_3355224291126807009[300] = 0;
   out_3355224291126807009[301] = 0;
   out_3355224291126807009[302] = 0;
   out_3355224291126807009[303] = 0;
   out_3355224291126807009[304] = 1;
   out_3355224291126807009[305] = 0;
   out_3355224291126807009[306] = 0;
   out_3355224291126807009[307] = 0;
   out_3355224291126807009[308] = 0;
   out_3355224291126807009[309] = 0;
   out_3355224291126807009[310] = 0;
   out_3355224291126807009[311] = 0;
   out_3355224291126807009[312] = 0;
   out_3355224291126807009[313] = 0;
   out_3355224291126807009[314] = 0;
   out_3355224291126807009[315] = 0;
   out_3355224291126807009[316] = 0;
   out_3355224291126807009[317] = 0;
   out_3355224291126807009[318] = 0;
   out_3355224291126807009[319] = 0;
   out_3355224291126807009[320] = 0;
   out_3355224291126807009[321] = 0;
   out_3355224291126807009[322] = 0;
   out_3355224291126807009[323] = 1;
}
void h_4(double *state, double *unused, double *out_3011476903633263124) {
   out_3011476903633263124[0] = state[6] + state[9];
   out_3011476903633263124[1] = state[7] + state[10];
   out_3011476903633263124[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_4955044292825461911) {
   out_4955044292825461911[0] = 0;
   out_4955044292825461911[1] = 0;
   out_4955044292825461911[2] = 0;
   out_4955044292825461911[3] = 0;
   out_4955044292825461911[4] = 0;
   out_4955044292825461911[5] = 0;
   out_4955044292825461911[6] = 1;
   out_4955044292825461911[7] = 0;
   out_4955044292825461911[8] = 0;
   out_4955044292825461911[9] = 1;
   out_4955044292825461911[10] = 0;
   out_4955044292825461911[11] = 0;
   out_4955044292825461911[12] = 0;
   out_4955044292825461911[13] = 0;
   out_4955044292825461911[14] = 0;
   out_4955044292825461911[15] = 0;
   out_4955044292825461911[16] = 0;
   out_4955044292825461911[17] = 0;
   out_4955044292825461911[18] = 0;
   out_4955044292825461911[19] = 0;
   out_4955044292825461911[20] = 0;
   out_4955044292825461911[21] = 0;
   out_4955044292825461911[22] = 0;
   out_4955044292825461911[23] = 0;
   out_4955044292825461911[24] = 0;
   out_4955044292825461911[25] = 1;
   out_4955044292825461911[26] = 0;
   out_4955044292825461911[27] = 0;
   out_4955044292825461911[28] = 1;
   out_4955044292825461911[29] = 0;
   out_4955044292825461911[30] = 0;
   out_4955044292825461911[31] = 0;
   out_4955044292825461911[32] = 0;
   out_4955044292825461911[33] = 0;
   out_4955044292825461911[34] = 0;
   out_4955044292825461911[35] = 0;
   out_4955044292825461911[36] = 0;
   out_4955044292825461911[37] = 0;
   out_4955044292825461911[38] = 0;
   out_4955044292825461911[39] = 0;
   out_4955044292825461911[40] = 0;
   out_4955044292825461911[41] = 0;
   out_4955044292825461911[42] = 0;
   out_4955044292825461911[43] = 0;
   out_4955044292825461911[44] = 1;
   out_4955044292825461911[45] = 0;
   out_4955044292825461911[46] = 0;
   out_4955044292825461911[47] = 1;
   out_4955044292825461911[48] = 0;
   out_4955044292825461911[49] = 0;
   out_4955044292825461911[50] = 0;
   out_4955044292825461911[51] = 0;
   out_4955044292825461911[52] = 0;
   out_4955044292825461911[53] = 0;
}
void h_10(double *state, double *unused, double *out_4355309041220408349) {
   out_4355309041220408349[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_4355309041220408349[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_4355309041220408349[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_3909620274325923014) {
   out_3909620274325923014[0] = 0;
   out_3909620274325923014[1] = 9.8100000000000005*cos(state[1]);
   out_3909620274325923014[2] = 0;
   out_3909620274325923014[3] = 0;
   out_3909620274325923014[4] = -state[8];
   out_3909620274325923014[5] = state[7];
   out_3909620274325923014[6] = 0;
   out_3909620274325923014[7] = state[5];
   out_3909620274325923014[8] = -state[4];
   out_3909620274325923014[9] = 0;
   out_3909620274325923014[10] = 0;
   out_3909620274325923014[11] = 0;
   out_3909620274325923014[12] = 1;
   out_3909620274325923014[13] = 0;
   out_3909620274325923014[14] = 0;
   out_3909620274325923014[15] = 1;
   out_3909620274325923014[16] = 0;
   out_3909620274325923014[17] = 0;
   out_3909620274325923014[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_3909620274325923014[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_3909620274325923014[20] = 0;
   out_3909620274325923014[21] = state[8];
   out_3909620274325923014[22] = 0;
   out_3909620274325923014[23] = -state[6];
   out_3909620274325923014[24] = -state[5];
   out_3909620274325923014[25] = 0;
   out_3909620274325923014[26] = state[3];
   out_3909620274325923014[27] = 0;
   out_3909620274325923014[28] = 0;
   out_3909620274325923014[29] = 0;
   out_3909620274325923014[30] = 0;
   out_3909620274325923014[31] = 1;
   out_3909620274325923014[32] = 0;
   out_3909620274325923014[33] = 0;
   out_3909620274325923014[34] = 1;
   out_3909620274325923014[35] = 0;
   out_3909620274325923014[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_3909620274325923014[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_3909620274325923014[38] = 0;
   out_3909620274325923014[39] = -state[7];
   out_3909620274325923014[40] = state[6];
   out_3909620274325923014[41] = 0;
   out_3909620274325923014[42] = state[4];
   out_3909620274325923014[43] = -state[3];
   out_3909620274325923014[44] = 0;
   out_3909620274325923014[45] = 0;
   out_3909620274325923014[46] = 0;
   out_3909620274325923014[47] = 0;
   out_3909620274325923014[48] = 0;
   out_3909620274325923014[49] = 0;
   out_3909620274325923014[50] = 1;
   out_3909620274325923014[51] = 0;
   out_3909620274325923014[52] = 0;
   out_3909620274325923014[53] = 1;
}
void h_13(double *state, double *unused, double *out_6750839247096495864) {
   out_6750839247096495864[0] = state[3];
   out_6750839247096495864[1] = state[4];
   out_6750839247096495864[2] = state[5];
}
void H_13(double *state, double *unused, double *out_2655586915491239018) {
   out_2655586915491239018[0] = 0;
   out_2655586915491239018[1] = 0;
   out_2655586915491239018[2] = 0;
   out_2655586915491239018[3] = 1;
   out_2655586915491239018[4] = 0;
   out_2655586915491239018[5] = 0;
   out_2655586915491239018[6] = 0;
   out_2655586915491239018[7] = 0;
   out_2655586915491239018[8] = 0;
   out_2655586915491239018[9] = 0;
   out_2655586915491239018[10] = 0;
   out_2655586915491239018[11] = 0;
   out_2655586915491239018[12] = 0;
   out_2655586915491239018[13] = 0;
   out_2655586915491239018[14] = 0;
   out_2655586915491239018[15] = 0;
   out_2655586915491239018[16] = 0;
   out_2655586915491239018[17] = 0;
   out_2655586915491239018[18] = 0;
   out_2655586915491239018[19] = 0;
   out_2655586915491239018[20] = 0;
   out_2655586915491239018[21] = 0;
   out_2655586915491239018[22] = 1;
   out_2655586915491239018[23] = 0;
   out_2655586915491239018[24] = 0;
   out_2655586915491239018[25] = 0;
   out_2655586915491239018[26] = 0;
   out_2655586915491239018[27] = 0;
   out_2655586915491239018[28] = 0;
   out_2655586915491239018[29] = 0;
   out_2655586915491239018[30] = 0;
   out_2655586915491239018[31] = 0;
   out_2655586915491239018[32] = 0;
   out_2655586915491239018[33] = 0;
   out_2655586915491239018[34] = 0;
   out_2655586915491239018[35] = 0;
   out_2655586915491239018[36] = 0;
   out_2655586915491239018[37] = 0;
   out_2655586915491239018[38] = 0;
   out_2655586915491239018[39] = 0;
   out_2655586915491239018[40] = 0;
   out_2655586915491239018[41] = 1;
   out_2655586915491239018[42] = 0;
   out_2655586915491239018[43] = 0;
   out_2655586915491239018[44] = 0;
   out_2655586915491239018[45] = 0;
   out_2655586915491239018[46] = 0;
   out_2655586915491239018[47] = 0;
   out_2655586915491239018[48] = 0;
   out_2655586915491239018[49] = 0;
   out_2655586915491239018[50] = 0;
   out_2655586915491239018[51] = 0;
   out_2655586915491239018[52] = 0;
   out_2655586915491239018[53] = 0;
}
void h_14(double *state, double *unused, double *out_1463673518145024401) {
   out_1463673518145024401[0] = state[6];
   out_1463673518145024401[1] = state[7];
   out_1463673518145024401[2] = state[8];
}
void H_14(double *state, double *unused, double *out_991803436485977382) {
   out_991803436485977382[0] = 0;
   out_991803436485977382[1] = 0;
   out_991803436485977382[2] = 0;
   out_991803436485977382[3] = 0;
   out_991803436485977382[4] = 0;
   out_991803436485977382[5] = 0;
   out_991803436485977382[6] = 1;
   out_991803436485977382[7] = 0;
   out_991803436485977382[8] = 0;
   out_991803436485977382[9] = 0;
   out_991803436485977382[10] = 0;
   out_991803436485977382[11] = 0;
   out_991803436485977382[12] = 0;
   out_991803436485977382[13] = 0;
   out_991803436485977382[14] = 0;
   out_991803436485977382[15] = 0;
   out_991803436485977382[16] = 0;
   out_991803436485977382[17] = 0;
   out_991803436485977382[18] = 0;
   out_991803436485977382[19] = 0;
   out_991803436485977382[20] = 0;
   out_991803436485977382[21] = 0;
   out_991803436485977382[22] = 0;
   out_991803436485977382[23] = 0;
   out_991803436485977382[24] = 0;
   out_991803436485977382[25] = 1;
   out_991803436485977382[26] = 0;
   out_991803436485977382[27] = 0;
   out_991803436485977382[28] = 0;
   out_991803436485977382[29] = 0;
   out_991803436485977382[30] = 0;
   out_991803436485977382[31] = 0;
   out_991803436485977382[32] = 0;
   out_991803436485977382[33] = 0;
   out_991803436485977382[34] = 0;
   out_991803436485977382[35] = 0;
   out_991803436485977382[36] = 0;
   out_991803436485977382[37] = 0;
   out_991803436485977382[38] = 0;
   out_991803436485977382[39] = 0;
   out_991803436485977382[40] = 0;
   out_991803436485977382[41] = 0;
   out_991803436485977382[42] = 0;
   out_991803436485977382[43] = 0;
   out_991803436485977382[44] = 1;
   out_991803436485977382[45] = 0;
   out_991803436485977382[46] = 0;
   out_991803436485977382[47] = 0;
   out_991803436485977382[48] = 0;
   out_991803436485977382[49] = 0;
   out_991803436485977382[50] = 0;
   out_991803436485977382[51] = 0;
   out_991803436485977382[52] = 0;
   out_991803436485977382[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_5042717407786303213) {
  err_fun(nom_x, delta_x, out_5042717407786303213);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1689930166948831822) {
  inv_err_fun(nom_x, true_x, out_1689930166948831822);
}
void pose_H_mod_fun(double *state, double *out_3910683721715691775) {
  H_mod_fun(state, out_3910683721715691775);
}
void pose_f_fun(double *state, double dt, double *out_6246311212408209668) {
  f_fun(state,  dt, out_6246311212408209668);
}
void pose_F_fun(double *state, double dt, double *out_3355224291126807009) {
  F_fun(state,  dt, out_3355224291126807009);
}
void pose_h_4(double *state, double *unused, double *out_3011476903633263124) {
  h_4(state, unused, out_3011476903633263124);
}
void pose_H_4(double *state, double *unused, double *out_4955044292825461911) {
  H_4(state, unused, out_4955044292825461911);
}
void pose_h_10(double *state, double *unused, double *out_4355309041220408349) {
  h_10(state, unused, out_4355309041220408349);
}
void pose_H_10(double *state, double *unused, double *out_3909620274325923014) {
  H_10(state, unused, out_3909620274325923014);
}
void pose_h_13(double *state, double *unused, double *out_6750839247096495864) {
  h_13(state, unused, out_6750839247096495864);
}
void pose_H_13(double *state, double *unused, double *out_2655586915491239018) {
  H_13(state, unused, out_2655586915491239018);
}
void pose_h_14(double *state, double *unused, double *out_1463673518145024401) {
  h_14(state, unused, out_1463673518145024401);
}
void pose_H_14(double *state, double *unused, double *out_991803436485977382) {
  H_14(state, unused, out_991803436485977382);
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
