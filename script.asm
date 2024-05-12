ORG 0x00
        WB 0x15 ; MBR = 0x15                                                    ; 0x00, 0x01
        WM 0x400 ; dataMemory[0x400] : 0x15                                     ; 0x02, 0x03
        WB 0x05 ; MBR = 0x05                                                    ; 0x04, 0x05
        WACC ; ACC = 0x05                                                       ; 0x06, 0x07
        WB 0x08 ; MBR = 0x08                                                    ; 0x08, 0x09
        ADD ; ACC = (0x05) + (0x08) = 0x0D ZF=0, CF=0, OF=0, SF=0               ; 0x0A, 0x0B
        RM 0x400 ; MBR = 0x15                                                   ; 0x0C, 0x0D
        MUL ; ACC = (0x0D) x (0x15) = 0x11 ZF=0, CF=1, OF=1, SF=0 ; 0x0E, 0x0F  ; 0x0E, 0x0F
        RACC ; MBR = 0x11                                                       ; 0x10, 0x11
        WM 0x401 ; dataMemory[0x401] : 0x11                                     ; 0x12, 0x13
        WIB 0x0B ; IOBR = 0x0B                                                  ; 0x14, 0x15
        WIO 0x000 ; ioBuffer[0x000] : 0x0B                                      ; 0x16, 0x17
        WB 0x10 ; MBR = 0x10                                                    ; 0x18, 0x19
        SUB ; ACC = (0x11) - (0x10) = 0x01 ZF=0, CF=0, OF=0, SF=0               ; 0x1A, 0x1B
        RACC ; MBR = 0x01                                                       ; 0x1C, 0x1D
        WIO 0x001 ; ioBuffer[0x001] : 0x0B                                      ; 0x1E, 0x1F
        SHL ; ACC = (0x01) << 1 = 0x02 ZF=0, CF=0, OF=0, SF=0                   ; 0x20, 0x21
        SHL ; ACC = (0x02) << 1 = 0x04 ZF=0, CF=0, OF=0, SF=0                   ; 0x22, 0x23
        SHR ; ACC = (0x04) >> 1 = 0x02 ZF=0, CF=0, OF=0, SF=0                   ; 0x24, 0x25
        RM 0x401 ; MBR = 0x11                                                   ; 0x26, 0x27
        OR ; ACC = (0x02) OR (0x11) = 0x13 ZF=0, CF=0, OF=0, SF=0               ; 0x28, 0x29
        NOT ; ACC = NOT (0x13) = 0xEC ZF=0, CF=0, OF=0, SF=0                    ; 0x2A, 0x2B
        RIO 0x01F ; IOBR = 0xD0                                                 ; 0x2C, 0x2D
        SWAP ; MBR = 0xD0, IOBR = 0x11                                          ; 0x2E, 0x2F
        XOR ; ACC = (0xEC) XOR (0xD0) = 0x3C ZF=0, CF=0, OF=0, SF=0             ; 0x30, 0x31
        WB 0xFF ; MBR = 0xFF                                                    ; 0x32, 0x33
        AND ; ACC = (0x3C) AND (0xFF) = 0x3C ZF=0, CF=0, OF=0, SF=0             ; 0x34, 0x35
        RM 0x401 ; MBR = 0x11                                                   ; 0x36, 0x37
        BRE LABEL0 ; ACC = (0x3C) - (0x11) = 0x2B ZF=0, CF=0, OF=0, SF=0         ; 0x38, 0x39
        WB 0x2E ; MBR = 0x2E                                                    ; 0x3A, 0x3B
        LABEL0 BRGT LABEL1 ; ACC = (0x2B) - (0x2E) = 0xFD ZF=0, CF=0, OF=0, SF=1; 0x3C, 0x3D
        AND ; ACC = (0xFD) AND (0x2E) = 0x2C ZF=0, CF=0, OF=0, SF=0             ; 0x3E, 0x3F
        BRLT LABEL2      ; ACC = (0x2C) - (0x2E) = 0xFE ZF=0, CF=0, OF=0, SF=1   ; 0x40, 0x41
        LABEL1 WB 0x00         ; unreachable                                           ; 0x42, 0x43
        WACC            ; unreachable                                           ; 0x44, 0x45
        LABEL2 WB 0x03         ; MBR = 0x03                                            ; 0x46, 0x47
        WACC            ; ACC = 0x03                                            ; 0x48, 0x49
        ; is part is a controlled loop
        LOOP WB 0x00         ; MBR = 0x00                                       ; 0x4A, 0x4B
        BRE EXITLOOP    ; ACC = (0x03) - (0x00) = 0x03 ZF=0, CF=0, OF=0, SF=0   ; 0x4C, 0x4D
                        ; ACC = (0x02) - (0x00) = 0x02 ZF=0, CF=0, OF=0, SF=0
                        ; ACC = (0x01) - (0x00) = 0x01 ZF=0, CF=0, OF=0, SF=0
                        ; ACC = (0x00) - (0x00) = 0x00 ZF=1, CF=0, OF=0, SF=0
                        ; PC <- 0x52 (when ACC == MBR and ZF=1)
        WB 0x01         ; MBR = 0x01                                            ; 0x4E, 0x4F
        SUB             ; ACC = (0x03) - (0x01) = 0x02 ZF=0, CF=0, OF=0, SF=0
                        ; ACC = (0x02) - (0x01) = 0x01 ZF=0, CF=0, OF=0, SF=0
                        ; ACC = (0x01) - (0x01) = 0x00 ZF=1, CF=0, OF=0, SF=0
        BR LOOP         ; PC <- 0x048 (loop)                                     ; 0x50, 0x51
        EXITLOOP EOP    ; end of program                                         ; 0x52, 0x53