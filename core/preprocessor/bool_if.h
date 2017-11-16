#pragma once

// condition need be 0 or 1
#define CCDK_PP_IF( Condi, True, False)     CCDK_PP_IF_FAST( Condi, True, False)
#define CCDK_PP_IF_FAST(Condi, True, False) CCDK_PP_IF_VALUE ## Condi( True, False)

//fork result
#define CCDK_PP_IF_VALUE0(True, False) False
#define CCDK_PP_IF_VALUE1(True, False) True
