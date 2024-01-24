/*
 * Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

    .section .rom.vector
    .extern  interrupt_register_u2d
    .extern  interrupt_register_usb_iddig
    .extern  interrupt_register_usb_vbusdig
    .extern  interrupt_register_irq
    .extern  interrupt_register_fiq
    .extern  interrupt_register_fmac
    .extern  interrupt_register_tshi

    .extern  interrupt_register_imac
    .extern  interrupt_register_h2fw
    .extern  interrupt_register_coxt
    .extern  interrupt_register_wake
    .extern  interrupt_register_hiq0
    .extern  interrupt_register_hiq1
    .extern  interrupt_register_tsiq
    .extern  interrupt_register_wdiq
    .global vector_base

vector_base:
    j _start
    .align 2
    .word  0
    .word  0
    .word  0 //eclic_msip_handler//3

    .word  0
    .word  0
    .word  0
    .word  0 //eclic_mtip_handler//7

    .word  0
    .word  0
    .word  0
    .word  0

    .word  0
    .word  0
    .word  0
    .word  0

    .word  0
    .word  0
    .word  0

    .word  interrupt_register_fiq //19
    .word  interrupt_register_irq //20
    .word  interrupt_register_fmac //21
    .word  interrupt_register_tshi //22
    .word  0
    .word  0
    .word  0
    .word  0

    .word  interrupt_register_imac //27
    .word  interrupt_register_h2fw
    .word  interrupt_register_coxt
    .word  interrupt_register_wake
    .word  interrupt_register_hiq0
    .word  interrupt_register_hiq1
    .word  interrupt_register_tsiq
    .word  interrupt_register_wdiq

    .word  interrupt_register_u2d
    .word  interrupt_register_usb_iddig
    .word  interrupt_register_usb_vbusdig
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0

    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0 //51
