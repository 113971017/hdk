;
; Copyright (C) 2016-2021 Melexis N.V.
;
; Software Platform
;

#include "i2c_sw_master.h"

;***************************************************************************
; Global symbols
;***************************************************************************
.global _I2C_sw_Start
.global _I2C_sw_Restart
.global _I2C_sw_Stop
.global _I2C_sw_TransmitBytes
.global _I2C_sw_TransmitByte
.global _I2C_sw_TransmitWord
.global _I2C_sw_ReceiveBytes

.text

;***************************************************************************
; Macros and Functions
;***************************************************************************

; A single nop takes 5 CPU clocks => 5/8 = 0.625 us at 8 MHz CPU clock
; Define zero if the parameter is not specified
    .macro DELAY_NOPS nops_number=0
    .rept \nops_number
    nop
    .endr
    .endm

    .macro DELAY_SLOWING_I2C
#ifndef I2C_FASTEST_MODE
    call delay_slowing_I2C
#endif
    .endm

;---------------------------------------------------------------------------

    .macro SCL_0            ; It is presumed Yl contains GPIO_DIG_OUT_0
    mov SCL_CFG, Yl         ; Pull-down SCL
    DELAY_SLOWING_I2C
    .endm

;---------------------------------------------------------------------------

    .macro SCL_1            ; It is presumed Yh contains GPIO_RELEASE
    mov SCL_CFG, Yh         ; Pull-up SCL to release it
    DELAY_SLOWING_I2C
    .endm

;---------------------------------------------------------------------------

    .macro SDA_0            ; It is presumed Yl contains GPIO_DIG_OUT_0
    mov SDA_CFG, Yl         ; Pull-down SDA
    DELAY_SLOWING_I2C
    .endm

;---------------------------------------------------------------------------

    .macro SDA_1            ; It is presumed Yh contains GPIO_RELEASE
    mov SDA_CFG, Yh         ; Pull-up SDA to release it
    DELAY_SLOWING_I2C
    .endm

;***************************************************************************
; Basic delay to decrease I2C data rate
;***************************************************************************

delay_slowing_I2C:
    DELAY_NOPS I2C_NOPS_DELAY
    ret

;***************************************************************************
; Configure I2C bus, check that it is free and send I2C start symbol
;***************************************************************************
; After execution:
; SCL line: pulled-down if no error
; SDA line: pulled-down if no error
; Y = Overwritten with GPIO configuration constant
; X = Overwritten
; A = return code as follows
;   0 = no error
;   1 = bus not free (timed out)
;***************************************************************************

_I2C_sw_Start:
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a basic pointer to GPIOs registers
    SCL_1                   ; Release I2C bus
    SDA_1

    mov X, #TIMEOUT
wait_bus_free:              ; Check until it becomes free
    mov Al, GPIO_INPUT_ADDR
    and Al, #(SCL_BITMASK | SDA_BITMASK)
    cmp Al, #(SCL_BITMASK | SDA_BITMASK)
    je bus_free
    djnz X, wait_bus_free   ; Timeout

    mov A, #1               ; Error code
    ret

bus_free:
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a basic pointer to GPIOs registers
    SDA_0                   ; Start condition
    SCL_0
    mov A, #0               ; Error-free return code
    ret

;***************************************************************************
; Send I2C restart symbol (after start)
;***************************************************************************
; After execution:
; SCL line: pulled-down
; SDA line: pulled-down
; Y = Overwritten with GPIO configuration constant
; X = Overwritten with #GPIO_INPUT_ADDR
; A = is not affected
;***************************************************************************

_I2C_sw_Restart:
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a basic pointer to GPIOs registers
    SDA_1
    SCL_1
    SDA_0                   ; Start condition
    SCL_0
    ret

;***************************************************************************
; Send stop sequence and free I2C bus
;***************************************************************************
; Prior to execution:
; SCL line: pulled-down
; SDA line: don't care
; After execution:
; SCL line: pulled-up
; SDA line: pulled-up
; Y = Overwritten with GPIO configuration constant
; X = Overwritten with #GPIO_INPUT_ADDR
; A = is not affected
;***************************************************************************

_I2C_sw_Stop:
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a basic pointer to GPIOs registers
    SDA_0
    SCL_1
    SDA_1
    ret

;***************************************************************************
; I2C TX bytes routine
; Send several consecutive bytes in burst on the I2C bus
;***************************************************************************
; Prior to execution:
; Assumes the START symbol has been already sent
; SCL line: pulled-down
; SDA line: don't care
; A = pointer to the byte table
; [S-4] = Number of bytes to transmit
; Ah = Don't care
; Yl = Don't care
; Yh = Don't care
; After execution:
; SCL line: pulled-down
; SDA line: hi-Z
; Yl = Overwritten with GPIO configuration for "0" for SCL or SDA
; Yh = Overwritten with GPIO configuration for "1" for SCL
; X = pointer to the next byte after sending the required number of bytes.
; A = return code as follows
;   0 = no error
;   1 or more = number of byte not transmitted (incl. the one with error)
;***************************************************************************

_I2C_sw_TransmitBytes:
    mov X, A                ; Register X = 1st parameter (pointer to the byte table)
    mov A, [S-4]            ; Register A = 2nd parameter (number of bytes to transmit)
    mov Ah, Al              ; Keep number of bytes in Ah
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)

_send_byte:                 ; Send one byte
    mov Al, [X++]           ; Load Al with de-referenced X pointer = next byte of the pattern
    push X                  ; Push register X to the stack (= pointer to next byte)
    call _i2c_tx_byte       ; Call transmit byte routine
#ifndef I2C_IGNORE_NACK
    jnz _abort_bytes        ; Abort communication if ACK not received
#endif
    pop X                   ; Get back pointer to next byte from stack
    sub Ah, #1              ; Decrement byte counter
    jnz _send_byte          ; If not 0, send next byte

_end_bytes:                 ; End
    mov Al, Ah              ; Return number of remaining bytes
    usex A                  ; A = 0x00:Al
    ret                     ; Return
    
_abort_bytes:               ; Abort bytes transmission
    pop X                   ; Get back pointer to next byte from stack
    jmp _end_bytes

;***************************************************************************
; I2C TX byte routine
; Send one single byte on the I2C bus
;***************************************************************************
;Prior to execution:
; SCL line: don't care
; SDA line: don't care
; Al = Byte value to transmit
;After execution:
; SCL line: pulled-down
; SDA line: hi-Z
; Al = return code as follows
;   0 = no error (ACK received)
;   2 = SDA_BITMASK = error (NACK)
; Ah is not modified
; X = overwritten to #GPIO_INPUT_ADDR value
; Yl = Overwritten to GPIO configuration for "0" for SCL or SDA
; Yh = Overwritten to GPIO configuration for "1" for SCL or SDA
;***************************************************************************

_I2C_sw_TransmitByte:
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)
_i2c_tx_byte:               ; Called from I2C_sw_TransmitBytes
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a pointer to GPIOs read address
    mov Cx, #8              ; Initialize bit counter in register X
_send_bit:                  ; Send one bit
    lsl Al, #1              ; Load MSB of first pattern word in carry flag C
    SCL_0                   ; Pull-down SCL line, C not affected
    jnc _space_bit          ; If C=0, to go space_bit
    SDA_1                   ; Else pull-up SDA line
    jmp _bit_end            ; Jump to end of bit
_space_bit:                 ; Starts a space bit transmission
    SDA_0                   ; Pull-down SDA line
    nop                     ; Padding
_bit_end:                   ; SCL pulse and end of bits
    SCL_1                   ; Pull-up SCL line
    djnz Cx, _send_bit      ; Decrease-and-jump if not zero, C not affected
                            ; Prepare I2C bus to check ACK from slave
    SCL_0                   ; Pull-down SCL line
    SDA_1                   ; Pull-up SDA line to free it for the slave and ACK read
                            ; Check ACK from slave
    SCL_1                   ; Pull-up SCL line
_wait_scl_ack:              ; Clock stretching while waiting on ACK
    mov Al, GPIO_INPUT      ; Read GPIO0/SCL and GPIO1/SDA value
#ifdef I2C_CLK_STRETCH_TX_ACK
    mov C, Al.SCL_BIT       ; Load SCL bit in C
    jnc _wait_scl_ack       ; Loop until SCL=1
#endif
    SCL_0                   ; Pull-down SCL line

    and Al, #SDA_BITMASK    ; Check SDA bit
    ret                     ; Return

;***************************************************************************
; I2C TX word routine
; Send one word (2 bytes) on the I2C bus
;***************************************************************************
; Prior to execution:
; SCL line: pulled-down
; SDA line: don't care
; A = Word value to transmit
; After execution:
; SCL line: pulled-down
; SDA line: hi-Z
; A = return code as follows
;   0 = no error (ACK received)
;   1 = error (NACK)
; X = overwritten to #GPIO_INPUT_ADDR value
; Yl = Overwritten to GPIO configuration for "0" for SCL or SDA
; Yh = Overwritten to GPIO configuration for "1" for SCL
;***************************************************************************

_I2C_sw_TransmitWord:
    push A
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a pointer to GPIOs read address
    mov Cx, #8              ; Initialize bit counter in register X
    mov Al, [S-2]
    call _send_byte_w
    jnz _abort_nack_w
    mov Al, [S-1]
    call _send_byte_w
    usex A
    ret #4
_abort_nack_w:
    mov A, #1
    ret #4

;***************************************************************************
; I2C TX byte routine for optimized TX word
;***************************************************************************

_send_byte_w:
    lsl Al, #1              ; Load MSB of first pattern word in carry flag C
_send_bit_w:                ; Send one bit
    SCL_0                   ; Pull-down SCL line, C not affected
    jnc _space_bit_w        ; If C=0, to go space_bit
    SDA_1                   ; Else pull-up SDA line
    jmp _bit_end_w          ; Jump to end of bit
_space_bit_w:               ; Starts a space bit transmission
    SDA_0                   ; Pull-down SDA line
    nop                     ; Padding
_bit_end_w:                 ; SCL pulse and end of bits
    lsl Al, #1              ; Load MSB of first pattern word in carry flag C
    SCL_1                   ; Pull-up SCL line
    djnz Cx, _send_bit_w    ; Decrease-and-jump if not zero, C not affected
                            ; Check ACK by slave
    SCL_0                   ; Pull-down SCL line
    mov Cx, #8              ; Re-initialize bit counter in register X
    SDA_1                   ; Pull-up SDA line to free it for the slave and ACK read

#ifdef I2C_CLK_STRETCH_TX_ACK
    SCL_1                   ; Pull-up SCL line
_wait_scl_ack_w:
    mov Al, GPIO_INPUT      ; Read GPIO0/SCL and GPIO1/SDA value
    mov C, Al.SCL_BIT       ; Load SCL bit in C
    jnc _wait_scl_ack_w     ; Loop until SCL=1
#else
    nop                     ; Padding
    SCL_1                   ; Pull-up SCL line
    mov Al, GPIO_INPUT      ; Read GPIO0/SCL and GPIO1/SDA value
#endif

    and Al, #SDA_BITMASK    ; Check SDA bit
    mov SCL_CFG, Yl         ; Pull-down SCL line, Z flag not affected
    ret                     ; Returns

;***************************************************************************
; I2C RX bytes routine
; Receives several consecutive bytes in burst on the I2C bus
;***************************************************************************
; Prior to execution:
; Assumes that HFO_CLK_SEL = 2 (8MHz)
; Assumes that the bus is free (has to be checked) and that the START symbol
; has already been sent.
; SCL line: pulled-down
; SDA line: don't care
; Al = Number of bytes to receive
; Ah = Don't care
; X  = pointer to the byte table where received values will be stored
; Y  = Don't care
; After execution:
; SCL line: pulled-down
; SDA line: hi-Z
; Al = 0
; Ah = Overwritten with GPIO configuration for "1" for SDA
; X  = incremented pointer [X++]
; Yl = Overwritten with GPIO configuration for "0" for SCL or SDA
; Yh = Overwritten with GPIO configuration for "1" for SCL
;***************************************************************************

_I2C_sw_ReceiveBytes:
    mov X, A                ;Register X = 1st parameter
    mov A, [S-4]            ;Register A = 2nd parameter
    mov Y, #((GPIO_RELEASE << 8) + GPIO_DIG_OUT_0)
    pushw #GPIO_DIG_OUT_0   ;
    sub Al, #1
    jz _receive_byte_last
_receive_byte:
    push A                  ; Push register A to the stack
    push X                  ; Push register X to the stack (= pointer to next byte)
    call i2c_rx_byte        ; Call RX byte routine
    pop X                   ; Get back pointer to next byte from stack
    mov [X++], Al           ; Store received byte
    pop A                   ; Get back byte counter variable from stack
    sub Al, #1              ; Decrement byte counter
    jnz _receive_byte       ; If not 0, receive next byte
_receive_byte_last:
    push A                  ; Push register A to the stack
    push X                  ; Push register X to the stack (= pointer to next byte)
    mov X, [S-10]           
    jz _rx_byte_last_end
    mov X, #GPIO_RELEASE
    mov [S-6], X
_rx_byte_last_end:
    call i2c_rx_byte        ; Call RX byte routine
    pop X                   ; Get back pointer to next byte from stack
    mov [X++], Al           ; Store received byte
    pop A                   ; Get back byte counter variable from stack
    ret #4                  ; NEW

;***************************************************************************
; I2C RX byte routine
; Receive one single byte on the I2C bus
;***************************************************************************
; Prior to execution:
; SCL line: pulled-down
; SDA line: don't care
; X = Don't care
; Al = Don't care
; Ah = GPIO configuration for "1" for SDA
; Yl = GPIO configuration for "0" for SCL or SDA
; Yh = GPIO configuration for "1" for SCL
; After execution:
; SCL line: pulled-down
; SDA line: hi-Z
; X = overwritten to #GPIO_INPUT_ADDR
; Al = received byte
; Y and Ah same as prior to execution
;***************************************************************************

i2c_rx_byte:
    mov X, #GPIO_INPUT_ADDR ; Initialize X as a pointer
    mov Cx, #8              ; Initialize bit counter in register X

#ifdef I2C_CLK_STRETCH_RX_BYTE
    SCL_1                   ; Pull-up SCL line
_wait_scl_rx_byte:          ; Wait SCL high (clock stretching)
    mov Ah, GPIO_INPUT      ; Read GPIO0/SCL and GPIO1/SDA value
    mov C, Ah.SCL_BIT       ; Load SCL bit in C
    jc _rx_1st_bit          ; If SCL=1, move on
    jmp _wait_scl_rx_byte   ; Loop until SCL=1
#endif

_rx_bit:                    ; Receive one bit
    SCL_1                   ; Pull-up SCL line
    lsl Al, #1              ; Shift left Al
    mov Ah, GPIO_INPUT      ; Read GPIO0/SCL and GPIO1/SDA value
_rx_1st_bit:
    mov C, Ah.SDA_BIT       ; Load SDA value into carry  
    SCL_0                   ; Pull-down SCL line
    DELAY_SLOWING_I2C
    mov Al.0, C             ; C in Al LSB
    djnz Cx, _rx_bit        ; Decrease-and-jump if not zero, C not affected

    push A
                            ; Send ACK
    mov A, [S-10]           ; Load ACK configuration from stack
    mov SDA_CFG, Al         ; Set SDA line to ACK or NACK byte reception
    DELAY_SLOWING_I2C
    SCL_1                   ; Pull-up SCL line
    DELAY_SLOWING_I2C
    DELAY_SLOWING_I2C
    SCL_0                   ; Pull-down SCL line
    SDA_1                   ; Pull-up SDA line to free it for the slave

    pop A                   ; Pop A out of the stack
    ret                     ; Returns

;***************************************************************************
; EOF
