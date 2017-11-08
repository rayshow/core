#pragma once

// condition need be 0 or 1
#define A3D_PP_IF( Condi, True, False)     A3D_PP_IF_FAST( Condi, True, False)
#define A3D_PP_IF_FAST(Condi, True, False) A3D_PP_IF_VALUE ## Condi( True, False)

//fork result
#define A3D_PP_IF_VALUE0(True, False) False
#define A3D_PP_IF_VALUE1(True, False) True
