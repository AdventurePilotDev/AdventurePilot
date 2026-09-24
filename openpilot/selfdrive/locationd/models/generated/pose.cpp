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
void err_fun(double *nom_x, double *delta_x, double *out_3982225953723843213) {
   out_3982225953723843213[0] = delta_x[0] + nom_x[0];
   out_3982225953723843213[1] = delta_x[1] + nom_x[1];
   out_3982225953723843213[2] = delta_x[2] + nom_x[2];
   out_3982225953723843213[3] = delta_x[3] + nom_x[3];
   out_3982225953723843213[4] = delta_x[4] + nom_x[4];
   out_3982225953723843213[5] = delta_x[5] + nom_x[5];
   out_3982225953723843213[6] = delta_x[6] + nom_x[6];
   out_3982225953723843213[7] = delta_x[7] + nom_x[7];
   out_3982225953723843213[8] = delta_x[8] + nom_x[8];
   out_3982225953723843213[9] = delta_x[9] + nom_x[9];
   out_3982225953723843213[10] = delta_x[10] + nom_x[10];
   out_3982225953723843213[11] = delta_x[11] + nom_x[11];
   out_3982225953723843213[12] = delta_x[12] + nom_x[12];
   out_3982225953723843213[13] = delta_x[13] + nom_x[13];
   out_3982225953723843213[14] = delta_x[14] + nom_x[14];
   out_3982225953723843213[15] = delta_x[15] + nom_x[15];
   out_3982225953723843213[16] = delta_x[16] + nom_x[16];
   out_3982225953723843213[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6906535223873576474) {
   out_6906535223873576474[0] = -nom_x[0] + true_x[0];
   out_6906535223873576474[1] = -nom_x[1] + true_x[1];
   out_6906535223873576474[2] = -nom_x[2] + true_x[2];
   out_6906535223873576474[3] = -nom_x[3] + true_x[3];
   out_6906535223873576474[4] = -nom_x[4] + true_x[4];
   out_6906535223873576474[5] = -nom_x[5] + true_x[5];
   out_6906535223873576474[6] = -nom_x[6] + true_x[6];
   out_6906535223873576474[7] = -nom_x[7] + true_x[7];
   out_6906535223873576474[8] = -nom_x[8] + true_x[8];
   out_6906535223873576474[9] = -nom_x[9] + true_x[9];
   out_6906535223873576474[10] = -nom_x[10] + true_x[10];
   out_6906535223873576474[11] = -nom_x[11] + true_x[11];
   out_6906535223873576474[12] = -nom_x[12] + true_x[12];
   out_6906535223873576474[13] = -nom_x[13] + true_x[13];
   out_6906535223873576474[14] = -nom_x[14] + true_x[14];
   out_6906535223873576474[15] = -nom_x[15] + true_x[15];
   out_6906535223873576474[16] = -nom_x[16] + true_x[16];
   out_6906535223873576474[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8356239334660173247) {
   out_8356239334660173247[0] = 1.0;
   out_8356239334660173247[1] = 0.0;
   out_8356239334660173247[2] = 0.0;
   out_8356239334660173247[3] = 0.0;
   out_8356239334660173247[4] = 0.0;
   out_8356239334660173247[5] = 0.0;
   out_8356239334660173247[6] = 0.0;
   out_8356239334660173247[7] = 0.0;
   out_8356239334660173247[8] = 0.0;
   out_8356239334660173247[9] = 0.0;
   out_8356239334660173247[10] = 0.0;
   out_8356239334660173247[11] = 0.0;
   out_8356239334660173247[12] = 0.0;
   out_8356239334660173247[13] = 0.0;
   out_8356239334660173247[14] = 0.0;
   out_8356239334660173247[15] = 0.0;
   out_8356239334660173247[16] = 0.0;
   out_8356239334660173247[17] = 0.0;
   out_8356239334660173247[18] = 0.0;
   out_8356239334660173247[19] = 1.0;
   out_8356239334660173247[20] = 0.0;
   out_8356239334660173247[21] = 0.0;
   out_8356239334660173247[22] = 0.0;
   out_8356239334660173247[23] = 0.0;
   out_8356239334660173247[24] = 0.0;
   out_8356239334660173247[25] = 0.0;
   out_8356239334660173247[26] = 0.0;
   out_8356239334660173247[27] = 0.0;
   out_8356239334660173247[28] = 0.0;
   out_8356239334660173247[29] = 0.0;
   out_8356239334660173247[30] = 0.0;
   out_8356239334660173247[31] = 0.0;
   out_8356239334660173247[32] = 0.0;
   out_8356239334660173247[33] = 0.0;
   out_8356239334660173247[34] = 0.0;
   out_8356239334660173247[35] = 0.0;
   out_8356239334660173247[36] = 0.0;
   out_8356239334660173247[37] = 0.0;
   out_8356239334660173247[38] = 1.0;
   out_8356239334660173247[39] = 0.0;
   out_8356239334660173247[40] = 0.0;
   out_8356239334660173247[41] = 0.0;
   out_8356239334660173247[42] = 0.0;
   out_8356239334660173247[43] = 0.0;
   out_8356239334660173247[44] = 0.0;
   out_8356239334660173247[45] = 0.0;
   out_8356239334660173247[46] = 0.0;
   out_8356239334660173247[47] = 0.0;
   out_8356239334660173247[48] = 0.0;
   out_8356239334660173247[49] = 0.0;
   out_8356239334660173247[50] = 0.0;
   out_8356239334660173247[51] = 0.0;
   out_8356239334660173247[52] = 0.0;
   out_8356239334660173247[53] = 0.0;
   out_8356239334660173247[54] = 0.0;
   out_8356239334660173247[55] = 0.0;
   out_8356239334660173247[56] = 0.0;
   out_8356239334660173247[57] = 1.0;
   out_8356239334660173247[58] = 0.0;
   out_8356239334660173247[59] = 0.0;
   out_8356239334660173247[60] = 0.0;
   out_8356239334660173247[61] = 0.0;
   out_8356239334660173247[62] = 0.0;
   out_8356239334660173247[63] = 0.0;
   out_8356239334660173247[64] = 0.0;
   out_8356239334660173247[65] = 0.0;
   out_8356239334660173247[66] = 0.0;
   out_8356239334660173247[67] = 0.0;
   out_8356239334660173247[68] = 0.0;
   out_8356239334660173247[69] = 0.0;
   out_8356239334660173247[70] = 0.0;
   out_8356239334660173247[71] = 0.0;
   out_8356239334660173247[72] = 0.0;
   out_8356239334660173247[73] = 0.0;
   out_8356239334660173247[74] = 0.0;
   out_8356239334660173247[75] = 0.0;
   out_8356239334660173247[76] = 1.0;
   out_8356239334660173247[77] = 0.0;
   out_8356239334660173247[78] = 0.0;
   out_8356239334660173247[79] = 0.0;
   out_8356239334660173247[80] = 0.0;
   out_8356239334660173247[81] = 0.0;
   out_8356239334660173247[82] = 0.0;
   out_8356239334660173247[83] = 0.0;
   out_8356239334660173247[84] = 0.0;
   out_8356239334660173247[85] = 0.0;
   out_8356239334660173247[86] = 0.0;
   out_8356239334660173247[87] = 0.0;
   out_8356239334660173247[88] = 0.0;
   out_8356239334660173247[89] = 0.0;
   out_8356239334660173247[90] = 0.0;
   out_8356239334660173247[91] = 0.0;
   out_8356239334660173247[92] = 0.0;
   out_8356239334660173247[93] = 0.0;
   out_8356239334660173247[94] = 0.0;
   out_8356239334660173247[95] = 1.0;
   out_8356239334660173247[96] = 0.0;
   out_8356239334660173247[97] = 0.0;
   out_8356239334660173247[98] = 0.0;
   out_8356239334660173247[99] = 0.0;
   out_8356239334660173247[100] = 0.0;
   out_8356239334660173247[101] = 0.0;
   out_8356239334660173247[102] = 0.0;
   out_8356239334660173247[103] = 0.0;
   out_8356239334660173247[104] = 0.0;
   out_8356239334660173247[105] = 0.0;
   out_8356239334660173247[106] = 0.0;
   out_8356239334660173247[107] = 0.0;
   out_8356239334660173247[108] = 0.0;
   out_8356239334660173247[109] = 0.0;
   out_8356239334660173247[110] = 0.0;
   out_8356239334660173247[111] = 0.0;
   out_8356239334660173247[112] = 0.0;
   out_8356239334660173247[113] = 0.0;
   out_8356239334660173247[114] = 1.0;
   out_8356239334660173247[115] = 0.0;
   out_8356239334660173247[116] = 0.0;
   out_8356239334660173247[117] = 0.0;
   out_8356239334660173247[118] = 0.0;
   out_8356239334660173247[119] = 0.0;
   out_8356239334660173247[120] = 0.0;
   out_8356239334660173247[121] = 0.0;
   out_8356239334660173247[122] = 0.0;
   out_8356239334660173247[123] = 0.0;
   out_8356239334660173247[124] = 0.0;
   out_8356239334660173247[125] = 0.0;
   out_8356239334660173247[126] = 0.0;
   out_8356239334660173247[127] = 0.0;
   out_8356239334660173247[128] = 0.0;
   out_8356239334660173247[129] = 0.0;
   out_8356239334660173247[130] = 0.0;
   out_8356239334660173247[131] = 0.0;
   out_8356239334660173247[132] = 0.0;
   out_8356239334660173247[133] = 1.0;
   out_8356239334660173247[134] = 0.0;
   out_8356239334660173247[135] = 0.0;
   out_8356239334660173247[136] = 0.0;
   out_8356239334660173247[137] = 0.0;
   out_8356239334660173247[138] = 0.0;
   out_8356239334660173247[139] = 0.0;
   out_8356239334660173247[140] = 0.0;
   out_8356239334660173247[141] = 0.0;
   out_8356239334660173247[142] = 0.0;
   out_8356239334660173247[143] = 0.0;
   out_8356239334660173247[144] = 0.0;
   out_8356239334660173247[145] = 0.0;
   out_8356239334660173247[146] = 0.0;
   out_8356239334660173247[147] = 0.0;
   out_8356239334660173247[148] = 0.0;
   out_8356239334660173247[149] = 0.0;
   out_8356239334660173247[150] = 0.0;
   out_8356239334660173247[151] = 0.0;
   out_8356239334660173247[152] = 1.0;
   out_8356239334660173247[153] = 0.0;
   out_8356239334660173247[154] = 0.0;
   out_8356239334660173247[155] = 0.0;
   out_8356239334660173247[156] = 0.0;
   out_8356239334660173247[157] = 0.0;
   out_8356239334660173247[158] = 0.0;
   out_8356239334660173247[159] = 0.0;
   out_8356239334660173247[160] = 0.0;
   out_8356239334660173247[161] = 0.0;
   out_8356239334660173247[162] = 0.0;
   out_8356239334660173247[163] = 0.0;
   out_8356239334660173247[164] = 0.0;
   out_8356239334660173247[165] = 0.0;
   out_8356239334660173247[166] = 0.0;
   out_8356239334660173247[167] = 0.0;
   out_8356239334660173247[168] = 0.0;
   out_8356239334660173247[169] = 0.0;
   out_8356239334660173247[170] = 0.0;
   out_8356239334660173247[171] = 1.0;
   out_8356239334660173247[172] = 0.0;
   out_8356239334660173247[173] = 0.0;
   out_8356239334660173247[174] = 0.0;
   out_8356239334660173247[175] = 0.0;
   out_8356239334660173247[176] = 0.0;
   out_8356239334660173247[177] = 0.0;
   out_8356239334660173247[178] = 0.0;
   out_8356239334660173247[179] = 0.0;
   out_8356239334660173247[180] = 0.0;
   out_8356239334660173247[181] = 0.0;
   out_8356239334660173247[182] = 0.0;
   out_8356239334660173247[183] = 0.0;
   out_8356239334660173247[184] = 0.0;
   out_8356239334660173247[185] = 0.0;
   out_8356239334660173247[186] = 0.0;
   out_8356239334660173247[187] = 0.0;
   out_8356239334660173247[188] = 0.0;
   out_8356239334660173247[189] = 0.0;
   out_8356239334660173247[190] = 1.0;
   out_8356239334660173247[191] = 0.0;
   out_8356239334660173247[192] = 0.0;
   out_8356239334660173247[193] = 0.0;
   out_8356239334660173247[194] = 0.0;
   out_8356239334660173247[195] = 0.0;
   out_8356239334660173247[196] = 0.0;
   out_8356239334660173247[197] = 0.0;
   out_8356239334660173247[198] = 0.0;
   out_8356239334660173247[199] = 0.0;
   out_8356239334660173247[200] = 0.0;
   out_8356239334660173247[201] = 0.0;
   out_8356239334660173247[202] = 0.0;
   out_8356239334660173247[203] = 0.0;
   out_8356239334660173247[204] = 0.0;
   out_8356239334660173247[205] = 0.0;
   out_8356239334660173247[206] = 0.0;
   out_8356239334660173247[207] = 0.0;
   out_8356239334660173247[208] = 0.0;
   out_8356239334660173247[209] = 1.0;
   out_8356239334660173247[210] = 0.0;
   out_8356239334660173247[211] = 0.0;
   out_8356239334660173247[212] = 0.0;
   out_8356239334660173247[213] = 0.0;
   out_8356239334660173247[214] = 0.0;
   out_8356239334660173247[215] = 0.0;
   out_8356239334660173247[216] = 0.0;
   out_8356239334660173247[217] = 0.0;
   out_8356239334660173247[218] = 0.0;
   out_8356239334660173247[219] = 0.0;
   out_8356239334660173247[220] = 0.0;
   out_8356239334660173247[221] = 0.0;
   out_8356239334660173247[222] = 0.0;
   out_8356239334660173247[223] = 0.0;
   out_8356239334660173247[224] = 0.0;
   out_8356239334660173247[225] = 0.0;
   out_8356239334660173247[226] = 0.0;
   out_8356239334660173247[227] = 0.0;
   out_8356239334660173247[228] = 1.0;
   out_8356239334660173247[229] = 0.0;
   out_8356239334660173247[230] = 0.0;
   out_8356239334660173247[231] = 0.0;
   out_8356239334660173247[232] = 0.0;
   out_8356239334660173247[233] = 0.0;
   out_8356239334660173247[234] = 0.0;
   out_8356239334660173247[235] = 0.0;
   out_8356239334660173247[236] = 0.0;
   out_8356239334660173247[237] = 0.0;
   out_8356239334660173247[238] = 0.0;
   out_8356239334660173247[239] = 0.0;
   out_8356239334660173247[240] = 0.0;
   out_8356239334660173247[241] = 0.0;
   out_8356239334660173247[242] = 0.0;
   out_8356239334660173247[243] = 0.0;
   out_8356239334660173247[244] = 0.0;
   out_8356239334660173247[245] = 0.0;
   out_8356239334660173247[246] = 0.0;
   out_8356239334660173247[247] = 1.0;
   out_8356239334660173247[248] = 0.0;
   out_8356239334660173247[249] = 0.0;
   out_8356239334660173247[250] = 0.0;
   out_8356239334660173247[251] = 0.0;
   out_8356239334660173247[252] = 0.0;
   out_8356239334660173247[253] = 0.0;
   out_8356239334660173247[254] = 0.0;
   out_8356239334660173247[255] = 0.0;
   out_8356239334660173247[256] = 0.0;
   out_8356239334660173247[257] = 0.0;
   out_8356239334660173247[258] = 0.0;
   out_8356239334660173247[259] = 0.0;
   out_8356239334660173247[260] = 0.0;
   out_8356239334660173247[261] = 0.0;
   out_8356239334660173247[262] = 0.0;
   out_8356239334660173247[263] = 0.0;
   out_8356239334660173247[264] = 0.0;
   out_8356239334660173247[265] = 0.0;
   out_8356239334660173247[266] = 1.0;
   out_8356239334660173247[267] = 0.0;
   out_8356239334660173247[268] = 0.0;
   out_8356239334660173247[269] = 0.0;
   out_8356239334660173247[270] = 0.0;
   out_8356239334660173247[271] = 0.0;
   out_8356239334660173247[272] = 0.0;
   out_8356239334660173247[273] = 0.0;
   out_8356239334660173247[274] = 0.0;
   out_8356239334660173247[275] = 0.0;
   out_8356239334660173247[276] = 0.0;
   out_8356239334660173247[277] = 0.0;
   out_8356239334660173247[278] = 0.0;
   out_8356239334660173247[279] = 0.0;
   out_8356239334660173247[280] = 0.0;
   out_8356239334660173247[281] = 0.0;
   out_8356239334660173247[282] = 0.0;
   out_8356239334660173247[283] = 0.0;
   out_8356239334660173247[284] = 0.0;
   out_8356239334660173247[285] = 1.0;
   out_8356239334660173247[286] = 0.0;
   out_8356239334660173247[287] = 0.0;
   out_8356239334660173247[288] = 0.0;
   out_8356239334660173247[289] = 0.0;
   out_8356239334660173247[290] = 0.0;
   out_8356239334660173247[291] = 0.0;
   out_8356239334660173247[292] = 0.0;
   out_8356239334660173247[293] = 0.0;
   out_8356239334660173247[294] = 0.0;
   out_8356239334660173247[295] = 0.0;
   out_8356239334660173247[296] = 0.0;
   out_8356239334660173247[297] = 0.0;
   out_8356239334660173247[298] = 0.0;
   out_8356239334660173247[299] = 0.0;
   out_8356239334660173247[300] = 0.0;
   out_8356239334660173247[301] = 0.0;
   out_8356239334660173247[302] = 0.0;
   out_8356239334660173247[303] = 0.0;
   out_8356239334660173247[304] = 1.0;
   out_8356239334660173247[305] = 0.0;
   out_8356239334660173247[306] = 0.0;
   out_8356239334660173247[307] = 0.0;
   out_8356239334660173247[308] = 0.0;
   out_8356239334660173247[309] = 0.0;
   out_8356239334660173247[310] = 0.0;
   out_8356239334660173247[311] = 0.0;
   out_8356239334660173247[312] = 0.0;
   out_8356239334660173247[313] = 0.0;
   out_8356239334660173247[314] = 0.0;
   out_8356239334660173247[315] = 0.0;
   out_8356239334660173247[316] = 0.0;
   out_8356239334660173247[317] = 0.0;
   out_8356239334660173247[318] = 0.0;
   out_8356239334660173247[319] = 0.0;
   out_8356239334660173247[320] = 0.0;
   out_8356239334660173247[321] = 0.0;
   out_8356239334660173247[322] = 0.0;
   out_8356239334660173247[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_94988861296200854) {
   out_94988861296200854[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_94988861296200854[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_94988861296200854[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_94988861296200854[3] = dt*state[12] + state[3];
   out_94988861296200854[4] = dt*state[13] + state[4];
   out_94988861296200854[5] = dt*state[14] + state[5];
   out_94988861296200854[6] = state[6];
   out_94988861296200854[7] = state[7];
   out_94988861296200854[8] = state[8];
   out_94988861296200854[9] = state[9];
   out_94988861296200854[10] = state[10];
   out_94988861296200854[11] = state[11];
   out_94988861296200854[12] = state[12];
   out_94988861296200854[13] = state[13];
   out_94988861296200854[14] = state[14];
   out_94988861296200854[15] = state[15];
   out_94988861296200854[16] = state[16];
   out_94988861296200854[17] = state[17];
}
void F_fun(double *state, double dt, double *out_9024536503228902294) {
   out_9024536503228902294[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_9024536503228902294[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_9024536503228902294[2] = 0;
   out_9024536503228902294[3] = 0;
   out_9024536503228902294[4] = 0;
   out_9024536503228902294[5] = 0;
   out_9024536503228902294[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_9024536503228902294[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_9024536503228902294[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_9024536503228902294[9] = 0;
   out_9024536503228902294[10] = 0;
   out_9024536503228902294[11] = 0;
   out_9024536503228902294[12] = 0;
   out_9024536503228902294[13] = 0;
   out_9024536503228902294[14] = 0;
   out_9024536503228902294[15] = 0;
   out_9024536503228902294[16] = 0;
   out_9024536503228902294[17] = 0;
   out_9024536503228902294[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_9024536503228902294[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_9024536503228902294[20] = 0;
   out_9024536503228902294[21] = 0;
   out_9024536503228902294[22] = 0;
   out_9024536503228902294[23] = 0;
   out_9024536503228902294[24] = 0;
   out_9024536503228902294[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_9024536503228902294[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_9024536503228902294[27] = 0;
   out_9024536503228902294[28] = 0;
   out_9024536503228902294[29] = 0;
   out_9024536503228902294[30] = 0;
   out_9024536503228902294[31] = 0;
   out_9024536503228902294[32] = 0;
   out_9024536503228902294[33] = 0;
   out_9024536503228902294[34] = 0;
   out_9024536503228902294[35] = 0;
   out_9024536503228902294[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_9024536503228902294[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_9024536503228902294[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_9024536503228902294[39] = 0;
   out_9024536503228902294[40] = 0;
   out_9024536503228902294[41] = 0;
   out_9024536503228902294[42] = 0;
   out_9024536503228902294[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_9024536503228902294[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_9024536503228902294[45] = 0;
   out_9024536503228902294[46] = 0;
   out_9024536503228902294[47] = 0;
   out_9024536503228902294[48] = 0;
   out_9024536503228902294[49] = 0;
   out_9024536503228902294[50] = 0;
   out_9024536503228902294[51] = 0;
   out_9024536503228902294[52] = 0;
   out_9024536503228902294[53] = 0;
   out_9024536503228902294[54] = 0;
   out_9024536503228902294[55] = 0;
   out_9024536503228902294[56] = 0;
   out_9024536503228902294[57] = 1;
   out_9024536503228902294[58] = 0;
   out_9024536503228902294[59] = 0;
   out_9024536503228902294[60] = 0;
   out_9024536503228902294[61] = 0;
   out_9024536503228902294[62] = 0;
   out_9024536503228902294[63] = 0;
   out_9024536503228902294[64] = 0;
   out_9024536503228902294[65] = 0;
   out_9024536503228902294[66] = dt;
   out_9024536503228902294[67] = 0;
   out_9024536503228902294[68] = 0;
   out_9024536503228902294[69] = 0;
   out_9024536503228902294[70] = 0;
   out_9024536503228902294[71] = 0;
   out_9024536503228902294[72] = 0;
   out_9024536503228902294[73] = 0;
   out_9024536503228902294[74] = 0;
   out_9024536503228902294[75] = 0;
   out_9024536503228902294[76] = 1;
   out_9024536503228902294[77] = 0;
   out_9024536503228902294[78] = 0;
   out_9024536503228902294[79] = 0;
   out_9024536503228902294[80] = 0;
   out_9024536503228902294[81] = 0;
   out_9024536503228902294[82] = 0;
   out_9024536503228902294[83] = 0;
   out_9024536503228902294[84] = 0;
   out_9024536503228902294[85] = dt;
   out_9024536503228902294[86] = 0;
   out_9024536503228902294[87] = 0;
   out_9024536503228902294[88] = 0;
   out_9024536503228902294[89] = 0;
   out_9024536503228902294[90] = 0;
   out_9024536503228902294[91] = 0;
   out_9024536503228902294[92] = 0;
   out_9024536503228902294[93] = 0;
   out_9024536503228902294[94] = 0;
   out_9024536503228902294[95] = 1;
   out_9024536503228902294[96] = 0;
   out_9024536503228902294[97] = 0;
   out_9024536503228902294[98] = 0;
   out_9024536503228902294[99] = 0;
   out_9024536503228902294[100] = 0;
   out_9024536503228902294[101] = 0;
   out_9024536503228902294[102] = 0;
   out_9024536503228902294[103] = 0;
   out_9024536503228902294[104] = dt;
   out_9024536503228902294[105] = 0;
   out_9024536503228902294[106] = 0;
   out_9024536503228902294[107] = 0;
   out_9024536503228902294[108] = 0;
   out_9024536503228902294[109] = 0;
   out_9024536503228902294[110] = 0;
   out_9024536503228902294[111] = 0;
   out_9024536503228902294[112] = 0;
   out_9024536503228902294[113] = 0;
   out_9024536503228902294[114] = 1;
   out_9024536503228902294[115] = 0;
   out_9024536503228902294[116] = 0;
   out_9024536503228902294[117] = 0;
   out_9024536503228902294[118] = 0;
   out_9024536503228902294[119] = 0;
   out_9024536503228902294[120] = 0;
   out_9024536503228902294[121] = 0;
   out_9024536503228902294[122] = 0;
   out_9024536503228902294[123] = 0;
   out_9024536503228902294[124] = 0;
   out_9024536503228902294[125] = 0;
   out_9024536503228902294[126] = 0;
   out_9024536503228902294[127] = 0;
   out_9024536503228902294[128] = 0;
   out_9024536503228902294[129] = 0;
   out_9024536503228902294[130] = 0;
   out_9024536503228902294[131] = 0;
   out_9024536503228902294[132] = 0;
   out_9024536503228902294[133] = 1;
   out_9024536503228902294[134] = 0;
   out_9024536503228902294[135] = 0;
   out_9024536503228902294[136] = 0;
   out_9024536503228902294[137] = 0;
   out_9024536503228902294[138] = 0;
   out_9024536503228902294[139] = 0;
   out_9024536503228902294[140] = 0;
   out_9024536503228902294[141] = 0;
   out_9024536503228902294[142] = 0;
   out_9024536503228902294[143] = 0;
   out_9024536503228902294[144] = 0;
   out_9024536503228902294[145] = 0;
   out_9024536503228902294[146] = 0;
   out_9024536503228902294[147] = 0;
   out_9024536503228902294[148] = 0;
   out_9024536503228902294[149] = 0;
   out_9024536503228902294[150] = 0;
   out_9024536503228902294[151] = 0;
   out_9024536503228902294[152] = 1;
   out_9024536503228902294[153] = 0;
   out_9024536503228902294[154] = 0;
   out_9024536503228902294[155] = 0;
   out_9024536503228902294[156] = 0;
   out_9024536503228902294[157] = 0;
   out_9024536503228902294[158] = 0;
   out_9024536503228902294[159] = 0;
   out_9024536503228902294[160] = 0;
   out_9024536503228902294[161] = 0;
   out_9024536503228902294[162] = 0;
   out_9024536503228902294[163] = 0;
   out_9024536503228902294[164] = 0;
   out_9024536503228902294[165] = 0;
   out_9024536503228902294[166] = 0;
   out_9024536503228902294[167] = 0;
   out_9024536503228902294[168] = 0;
   out_9024536503228902294[169] = 0;
   out_9024536503228902294[170] = 0;
   out_9024536503228902294[171] = 1;
   out_9024536503228902294[172] = 0;
   out_9024536503228902294[173] = 0;
   out_9024536503228902294[174] = 0;
   out_9024536503228902294[175] = 0;
   out_9024536503228902294[176] = 0;
   out_9024536503228902294[177] = 0;
   out_9024536503228902294[178] = 0;
   out_9024536503228902294[179] = 0;
   out_9024536503228902294[180] = 0;
   out_9024536503228902294[181] = 0;
   out_9024536503228902294[182] = 0;
   out_9024536503228902294[183] = 0;
   out_9024536503228902294[184] = 0;
   out_9024536503228902294[185] = 0;
   out_9024536503228902294[186] = 0;
   out_9024536503228902294[187] = 0;
   out_9024536503228902294[188] = 0;
   out_9024536503228902294[189] = 0;
   out_9024536503228902294[190] = 1;
   out_9024536503228902294[191] = 0;
   out_9024536503228902294[192] = 0;
   out_9024536503228902294[193] = 0;
   out_9024536503228902294[194] = 0;
   out_9024536503228902294[195] = 0;
   out_9024536503228902294[196] = 0;
   out_9024536503228902294[197] = 0;
   out_9024536503228902294[198] = 0;
   out_9024536503228902294[199] = 0;
   out_9024536503228902294[200] = 0;
   out_9024536503228902294[201] = 0;
   out_9024536503228902294[202] = 0;
   out_9024536503228902294[203] = 0;
   out_9024536503228902294[204] = 0;
   out_9024536503228902294[205] = 0;
   out_9024536503228902294[206] = 0;
   out_9024536503228902294[207] = 0;
   out_9024536503228902294[208] = 0;
   out_9024536503228902294[209] = 1;
   out_9024536503228902294[210] = 0;
   out_9024536503228902294[211] = 0;
   out_9024536503228902294[212] = 0;
   out_9024536503228902294[213] = 0;
   out_9024536503228902294[214] = 0;
   out_9024536503228902294[215] = 0;
   out_9024536503228902294[216] = 0;
   out_9024536503228902294[217] = 0;
   out_9024536503228902294[218] = 0;
   out_9024536503228902294[219] = 0;
   out_9024536503228902294[220] = 0;
   out_9024536503228902294[221] = 0;
   out_9024536503228902294[222] = 0;
   out_9024536503228902294[223] = 0;
   out_9024536503228902294[224] = 0;
   out_9024536503228902294[225] = 0;
   out_9024536503228902294[226] = 0;
   out_9024536503228902294[227] = 0;
   out_9024536503228902294[228] = 1;
   out_9024536503228902294[229] = 0;
   out_9024536503228902294[230] = 0;
   out_9024536503228902294[231] = 0;
   out_9024536503228902294[232] = 0;
   out_9024536503228902294[233] = 0;
   out_9024536503228902294[234] = 0;
   out_9024536503228902294[235] = 0;
   out_9024536503228902294[236] = 0;
   out_9024536503228902294[237] = 0;
   out_9024536503228902294[238] = 0;
   out_9024536503228902294[239] = 0;
   out_9024536503228902294[240] = 0;
   out_9024536503228902294[241] = 0;
   out_9024536503228902294[242] = 0;
   out_9024536503228902294[243] = 0;
   out_9024536503228902294[244] = 0;
   out_9024536503228902294[245] = 0;
   out_9024536503228902294[246] = 0;
   out_9024536503228902294[247] = 1;
   out_9024536503228902294[248] = 0;
   out_9024536503228902294[249] = 0;
   out_9024536503228902294[250] = 0;
   out_9024536503228902294[251] = 0;
   out_9024536503228902294[252] = 0;
   out_9024536503228902294[253] = 0;
   out_9024536503228902294[254] = 0;
   out_9024536503228902294[255] = 0;
   out_9024536503228902294[256] = 0;
   out_9024536503228902294[257] = 0;
   out_9024536503228902294[258] = 0;
   out_9024536503228902294[259] = 0;
   out_9024536503228902294[260] = 0;
   out_9024536503228902294[261] = 0;
   out_9024536503228902294[262] = 0;
   out_9024536503228902294[263] = 0;
   out_9024536503228902294[264] = 0;
   out_9024536503228902294[265] = 0;
   out_9024536503228902294[266] = 1;
   out_9024536503228902294[267] = 0;
   out_9024536503228902294[268] = 0;
   out_9024536503228902294[269] = 0;
   out_9024536503228902294[270] = 0;
   out_9024536503228902294[271] = 0;
   out_9024536503228902294[272] = 0;
   out_9024536503228902294[273] = 0;
   out_9024536503228902294[274] = 0;
   out_9024536503228902294[275] = 0;
   out_9024536503228902294[276] = 0;
   out_9024536503228902294[277] = 0;
   out_9024536503228902294[278] = 0;
   out_9024536503228902294[279] = 0;
   out_9024536503228902294[280] = 0;
   out_9024536503228902294[281] = 0;
   out_9024536503228902294[282] = 0;
   out_9024536503228902294[283] = 0;
   out_9024536503228902294[284] = 0;
   out_9024536503228902294[285] = 1;
   out_9024536503228902294[286] = 0;
   out_9024536503228902294[287] = 0;
   out_9024536503228902294[288] = 0;
   out_9024536503228902294[289] = 0;
   out_9024536503228902294[290] = 0;
   out_9024536503228902294[291] = 0;
   out_9024536503228902294[292] = 0;
   out_9024536503228902294[293] = 0;
   out_9024536503228902294[294] = 0;
   out_9024536503228902294[295] = 0;
   out_9024536503228902294[296] = 0;
   out_9024536503228902294[297] = 0;
   out_9024536503228902294[298] = 0;
   out_9024536503228902294[299] = 0;
   out_9024536503228902294[300] = 0;
   out_9024536503228902294[301] = 0;
   out_9024536503228902294[302] = 0;
   out_9024536503228902294[303] = 0;
   out_9024536503228902294[304] = 1;
   out_9024536503228902294[305] = 0;
   out_9024536503228902294[306] = 0;
   out_9024536503228902294[307] = 0;
   out_9024536503228902294[308] = 0;
   out_9024536503228902294[309] = 0;
   out_9024536503228902294[310] = 0;
   out_9024536503228902294[311] = 0;
   out_9024536503228902294[312] = 0;
   out_9024536503228902294[313] = 0;
   out_9024536503228902294[314] = 0;
   out_9024536503228902294[315] = 0;
   out_9024536503228902294[316] = 0;
   out_9024536503228902294[317] = 0;
   out_9024536503228902294[318] = 0;
   out_9024536503228902294[319] = 0;
   out_9024536503228902294[320] = 0;
   out_9024536503228902294[321] = 0;
   out_9024536503228902294[322] = 0;
   out_9024536503228902294[323] = 1;
}
void h_4(double *state, double *unused, double *out_5642475729519128019) {
   out_5642475729519128019[0] = state[6] + state[9];
   out_5642475729519128019[1] = state[7] + state[10];
   out_5642475729519128019[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_1146143072991557027) {
   out_1146143072991557027[0] = 0;
   out_1146143072991557027[1] = 0;
   out_1146143072991557027[2] = 0;
   out_1146143072991557027[3] = 0;
   out_1146143072991557027[4] = 0;
   out_1146143072991557027[5] = 0;
   out_1146143072991557027[6] = 1;
   out_1146143072991557027[7] = 0;
   out_1146143072991557027[8] = 0;
   out_1146143072991557027[9] = 1;
   out_1146143072991557027[10] = 0;
   out_1146143072991557027[11] = 0;
   out_1146143072991557027[12] = 0;
   out_1146143072991557027[13] = 0;
   out_1146143072991557027[14] = 0;
   out_1146143072991557027[15] = 0;
   out_1146143072991557027[16] = 0;
   out_1146143072991557027[17] = 0;
   out_1146143072991557027[18] = 0;
   out_1146143072991557027[19] = 0;
   out_1146143072991557027[20] = 0;
   out_1146143072991557027[21] = 0;
   out_1146143072991557027[22] = 0;
   out_1146143072991557027[23] = 0;
   out_1146143072991557027[24] = 0;
   out_1146143072991557027[25] = 1;
   out_1146143072991557027[26] = 0;
   out_1146143072991557027[27] = 0;
   out_1146143072991557027[28] = 1;
   out_1146143072991557027[29] = 0;
   out_1146143072991557027[30] = 0;
   out_1146143072991557027[31] = 0;
   out_1146143072991557027[32] = 0;
   out_1146143072991557027[33] = 0;
   out_1146143072991557027[34] = 0;
   out_1146143072991557027[35] = 0;
   out_1146143072991557027[36] = 0;
   out_1146143072991557027[37] = 0;
   out_1146143072991557027[38] = 0;
   out_1146143072991557027[39] = 0;
   out_1146143072991557027[40] = 0;
   out_1146143072991557027[41] = 0;
   out_1146143072991557027[42] = 0;
   out_1146143072991557027[43] = 0;
   out_1146143072991557027[44] = 1;
   out_1146143072991557027[45] = 0;
   out_1146143072991557027[46] = 0;
   out_1146143072991557027[47] = 1;
   out_1146143072991557027[48] = 0;
   out_1146143072991557027[49] = 0;
   out_1146143072991557027[50] = 0;
   out_1146143072991557027[51] = 0;
   out_1146143072991557027[52] = 0;
   out_1146143072991557027[53] = 0;
}
void h_10(double *state, double *unused, double *out_8808426522592935865) {
   out_8808426522592935865[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8808426522592935865[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8808426522592935865[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_1070655411699127393) {
   out_1070655411699127393[0] = 0;
   out_1070655411699127393[1] = 9.8100000000000005*cos(state[1]);
   out_1070655411699127393[2] = 0;
   out_1070655411699127393[3] = 0;
   out_1070655411699127393[4] = -state[8];
   out_1070655411699127393[5] = state[7];
   out_1070655411699127393[6] = 0;
   out_1070655411699127393[7] = state[5];
   out_1070655411699127393[8] = -state[4];
   out_1070655411699127393[9] = 0;
   out_1070655411699127393[10] = 0;
   out_1070655411699127393[11] = 0;
   out_1070655411699127393[12] = 1;
   out_1070655411699127393[13] = 0;
   out_1070655411699127393[14] = 0;
   out_1070655411699127393[15] = 1;
   out_1070655411699127393[16] = 0;
   out_1070655411699127393[17] = 0;
   out_1070655411699127393[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_1070655411699127393[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_1070655411699127393[20] = 0;
   out_1070655411699127393[21] = state[8];
   out_1070655411699127393[22] = 0;
   out_1070655411699127393[23] = -state[6];
   out_1070655411699127393[24] = -state[5];
   out_1070655411699127393[25] = 0;
   out_1070655411699127393[26] = state[3];
   out_1070655411699127393[27] = 0;
   out_1070655411699127393[28] = 0;
   out_1070655411699127393[29] = 0;
   out_1070655411699127393[30] = 0;
   out_1070655411699127393[31] = 1;
   out_1070655411699127393[32] = 0;
   out_1070655411699127393[33] = 0;
   out_1070655411699127393[34] = 1;
   out_1070655411699127393[35] = 0;
   out_1070655411699127393[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_1070655411699127393[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_1070655411699127393[38] = 0;
   out_1070655411699127393[39] = -state[7];
   out_1070655411699127393[40] = state[6];
   out_1070655411699127393[41] = 0;
   out_1070655411699127393[42] = state[4];
   out_1070655411699127393[43] = -state[3];
   out_1070655411699127393[44] = 0;
   out_1070655411699127393[45] = 0;
   out_1070655411699127393[46] = 0;
   out_1070655411699127393[47] = 0;
   out_1070655411699127393[48] = 0;
   out_1070655411699127393[49] = 0;
   out_1070655411699127393[50] = 1;
   out_1070655411699127393[51] = 0;
   out_1070655411699127393[52] = 0;
   out_1070655411699127393[53] = 1;
}
void h_13(double *state, double *unused, double *out_551055256458384175) {
   out_551055256458384175[0] = state[3];
   out_551055256458384175[1] = state[4];
   out_551055256458384175[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4358416898323889828) {
   out_4358416898323889828[0] = 0;
   out_4358416898323889828[1] = 0;
   out_4358416898323889828[2] = 0;
   out_4358416898323889828[3] = 1;
   out_4358416898323889828[4] = 0;
   out_4358416898323889828[5] = 0;
   out_4358416898323889828[6] = 0;
   out_4358416898323889828[7] = 0;
   out_4358416898323889828[8] = 0;
   out_4358416898323889828[9] = 0;
   out_4358416898323889828[10] = 0;
   out_4358416898323889828[11] = 0;
   out_4358416898323889828[12] = 0;
   out_4358416898323889828[13] = 0;
   out_4358416898323889828[14] = 0;
   out_4358416898323889828[15] = 0;
   out_4358416898323889828[16] = 0;
   out_4358416898323889828[17] = 0;
   out_4358416898323889828[18] = 0;
   out_4358416898323889828[19] = 0;
   out_4358416898323889828[20] = 0;
   out_4358416898323889828[21] = 0;
   out_4358416898323889828[22] = 1;
   out_4358416898323889828[23] = 0;
   out_4358416898323889828[24] = 0;
   out_4358416898323889828[25] = 0;
   out_4358416898323889828[26] = 0;
   out_4358416898323889828[27] = 0;
   out_4358416898323889828[28] = 0;
   out_4358416898323889828[29] = 0;
   out_4358416898323889828[30] = 0;
   out_4358416898323889828[31] = 0;
   out_4358416898323889828[32] = 0;
   out_4358416898323889828[33] = 0;
   out_4358416898323889828[34] = 0;
   out_4358416898323889828[35] = 0;
   out_4358416898323889828[36] = 0;
   out_4358416898323889828[37] = 0;
   out_4358416898323889828[38] = 0;
   out_4358416898323889828[39] = 0;
   out_4358416898323889828[40] = 0;
   out_4358416898323889828[41] = 1;
   out_4358416898323889828[42] = 0;
   out_4358416898323889828[43] = 0;
   out_4358416898323889828[44] = 0;
   out_4358416898323889828[45] = 0;
   out_4358416898323889828[46] = 0;
   out_4358416898323889828[47] = 0;
   out_4358416898323889828[48] = 0;
   out_4358416898323889828[49] = 0;
   out_4358416898323889828[50] = 0;
   out_4358416898323889828[51] = 0;
   out_4358416898323889828[52] = 0;
   out_4358416898323889828[53] = 0;
}
void h_14(double *state, double *unused, double *out_762241609475764552) {
   out_762241609475764552[0] = state[6];
   out_762241609475764552[1] = state[7];
   out_762241609475764552[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5109383929331041556) {
   out_5109383929331041556[0] = 0;
   out_5109383929331041556[1] = 0;
   out_5109383929331041556[2] = 0;
   out_5109383929331041556[3] = 0;
   out_5109383929331041556[4] = 0;
   out_5109383929331041556[5] = 0;
   out_5109383929331041556[6] = 1;
   out_5109383929331041556[7] = 0;
   out_5109383929331041556[8] = 0;
   out_5109383929331041556[9] = 0;
   out_5109383929331041556[10] = 0;
   out_5109383929331041556[11] = 0;
   out_5109383929331041556[12] = 0;
   out_5109383929331041556[13] = 0;
   out_5109383929331041556[14] = 0;
   out_5109383929331041556[15] = 0;
   out_5109383929331041556[16] = 0;
   out_5109383929331041556[17] = 0;
   out_5109383929331041556[18] = 0;
   out_5109383929331041556[19] = 0;
   out_5109383929331041556[20] = 0;
   out_5109383929331041556[21] = 0;
   out_5109383929331041556[22] = 0;
   out_5109383929331041556[23] = 0;
   out_5109383929331041556[24] = 0;
   out_5109383929331041556[25] = 1;
   out_5109383929331041556[26] = 0;
   out_5109383929331041556[27] = 0;
   out_5109383929331041556[28] = 0;
   out_5109383929331041556[29] = 0;
   out_5109383929331041556[30] = 0;
   out_5109383929331041556[31] = 0;
   out_5109383929331041556[32] = 0;
   out_5109383929331041556[33] = 0;
   out_5109383929331041556[34] = 0;
   out_5109383929331041556[35] = 0;
   out_5109383929331041556[36] = 0;
   out_5109383929331041556[37] = 0;
   out_5109383929331041556[38] = 0;
   out_5109383929331041556[39] = 0;
   out_5109383929331041556[40] = 0;
   out_5109383929331041556[41] = 0;
   out_5109383929331041556[42] = 0;
   out_5109383929331041556[43] = 0;
   out_5109383929331041556[44] = 1;
   out_5109383929331041556[45] = 0;
   out_5109383929331041556[46] = 0;
   out_5109383929331041556[47] = 0;
   out_5109383929331041556[48] = 0;
   out_5109383929331041556[49] = 0;
   out_5109383929331041556[50] = 0;
   out_5109383929331041556[51] = 0;
   out_5109383929331041556[52] = 0;
   out_5109383929331041556[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_3982225953723843213) {
  err_fun(nom_x, delta_x, out_3982225953723843213);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6906535223873576474) {
  inv_err_fun(nom_x, true_x, out_6906535223873576474);
}
void pose_H_mod_fun(double *state, double *out_8356239334660173247) {
  H_mod_fun(state, out_8356239334660173247);
}
void pose_f_fun(double *state, double dt, double *out_94988861296200854) {
  f_fun(state,  dt, out_94988861296200854);
}
void pose_F_fun(double *state, double dt, double *out_9024536503228902294) {
  F_fun(state,  dt, out_9024536503228902294);
}
void pose_h_4(double *state, double *unused, double *out_5642475729519128019) {
  h_4(state, unused, out_5642475729519128019);
}
void pose_H_4(double *state, double *unused, double *out_1146143072991557027) {
  H_4(state, unused, out_1146143072991557027);
}
void pose_h_10(double *state, double *unused, double *out_8808426522592935865) {
  h_10(state, unused, out_8808426522592935865);
}
void pose_H_10(double *state, double *unused, double *out_1070655411699127393) {
  H_10(state, unused, out_1070655411699127393);
}
void pose_h_13(double *state, double *unused, double *out_551055256458384175) {
  h_13(state, unused, out_551055256458384175);
}
void pose_H_13(double *state, double *unused, double *out_4358416898323889828) {
  H_13(state, unused, out_4358416898323889828);
}
void pose_h_14(double *state, double *unused, double *out_762241609475764552) {
  h_14(state, unused, out_762241609475764552);
}
void pose_H_14(double *state, double *unused, double *out_5109383929331041556) {
  H_14(state, unused, out_5109383929331041556);
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
