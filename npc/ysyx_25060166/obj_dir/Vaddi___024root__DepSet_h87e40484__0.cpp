// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vaddi.h for the primary calling header

#include "verilated.h"

#include "Vaddi___024root.h"

VL_INLINE_OPT void Vaddi___024root___ico_sequent__TOP__0(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->addi__DOT__REG_ARR__DOT__rs1_data = vlSelf->addi__DOT__REG_ARR__DOT__reg_array
        [(0x1fU & (vlSelf->RAM_RDATA >> 0xfU))];
}

void Vaddi___024root___eval_ico(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___eval_ico\n"); );
    // Body
    if (vlSelf->__VicoTriggered.at(0U)) {
        Vaddi___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void Vaddi___024root___eval_act(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vaddi___024root___nba_sequent__TOP__0(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*0:0*/ __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v0;
    __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v0 = 0;
    CData/*4:0*/ __Vdlyvdim0__addi__DOT__REG_ARR__DOT__reg_array__v32;
    __Vdlyvdim0__addi__DOT__REG_ARR__DOT__reg_array__v32 = 0;
    IData/*31:0*/ __Vdlyvval__addi__DOT__REG_ARR__DOT__reg_array__v32;
    __Vdlyvval__addi__DOT__REG_ARR__DOT__reg_array__v32 = 0;
    CData/*0:0*/ __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v32;
    __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v32 = 0;
    // Body
    __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v0 = 0U;
    __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v32 = 0U;
    if (vlSelf->rst) {
        vlSelf->addi__DOT__REG_ARR__DOT__unnamedblk1__DOT__i = 0x20U;
        vlSelf->addi__DOT__pc = 0x80000000U;
        __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v0 = 1U;
    } else {
        vlSelf->addi__DOT__pc = ((IData)(4U) + vlSelf->addi__DOT__pc);
        __Vdlyvval__addi__DOT__REG_ARR__DOT__reg_array__v32 
            = ((0U != (0x1fU & (vlSelf->RAM_RDATA >> 7U)))
                ? ((((- (IData)((vlSelf->RAM_RDATA 
                                 >> 0x1fU))) << 0xcU) 
                    | (vlSelf->RAM_RDATA >> 0x14U)) 
                   + vlSelf->addi__DOT__REG_ARR__DOT__rs1_data)
                : 0U);
        __Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v32 = 1U;
        __Vdlyvdim0__addi__DOT__REG_ARR__DOT__reg_array__v32 
            = (0x1fU & (vlSelf->RAM_RDATA >> 7U));
    }
    if (__Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v0) {
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[1U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[2U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[3U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[4U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[5U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[6U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[7U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[8U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[9U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0xaU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0xbU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0xcU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0xdU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0xeU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0xfU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x10U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x11U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x12U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x13U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x14U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x15U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x16U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x17U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x18U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x19U] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x1aU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x1bU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x1cU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x1dU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x1eU] = 0U;
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[0x1fU] = 0U;
    }
    if (__Vdlyvset__addi__DOT__REG_ARR__DOT__reg_array__v32) {
        vlSelf->addi__DOT__REG_ARR__DOT__reg_array[__Vdlyvdim0__addi__DOT__REG_ARR__DOT__reg_array__v32] 
            = __Vdlyvval__addi__DOT__REG_ARR__DOT__reg_array__v32;
    }
    vlSelf->RAM_RADDR = vlSelf->addi__DOT__pc;
    vlSelf->addi__DOT__REG_ARR__DOT__rs1_data = vlSelf->addi__DOT__REG_ARR__DOT__reg_array
        [(0x1fU & (vlSelf->RAM_RDATA >> 0xfU))];
}

void Vaddi___024root___eval_nba(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        Vaddi___024root___nba_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vaddi___024root___eval_triggers__ico(Vaddi___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vaddi___024root___dump_triggers__ico(Vaddi___024root* vlSelf);
#endif  // VL_DEBUG
void Vaddi___024root___eval_triggers__act(Vaddi___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vaddi___024root___dump_triggers__act(Vaddi___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vaddi___024root___dump_triggers__nba(Vaddi___024root* vlSelf);
#endif  // VL_DEBUG

void Vaddi___024root___eval(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___eval\n"); );
    // Init
    CData/*0:0*/ __VicoContinue;
    VlTriggerVec<1> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    vlSelf->__VicoIterCount = 0U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        __VicoContinue = 0U;
        Vaddi___024root___eval_triggers__ico(vlSelf);
        if (vlSelf->__VicoTriggered.any()) {
            __VicoContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VicoIterCount))) {
#ifdef VL_DEBUG
                Vaddi___024root___dump_triggers__ico(vlSelf);
#endif
                VL_FATAL_MT("addi.v", 3, "", "Input combinational region did not converge.");
            }
            vlSelf->__VicoIterCount = ((IData)(1U) 
                                       + vlSelf->__VicoIterCount);
            Vaddi___024root___eval_ico(vlSelf);
        }
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            Vaddi___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vaddi___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("addi.v", 3, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                Vaddi___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vaddi___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("addi.v", 3, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vaddi___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vaddi___024root___eval_debug_assertions(Vaddi___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vaddi__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vaddi___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst & 0xfeU))) {
        Verilated::overWidthError("rst");}
}
#endif  // VL_DEBUG
