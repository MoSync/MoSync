typedef enum {
        // Lump Start
        H_BJPIC=3,
        H_CASTLEPIC,                         // 4
        H_KEYBOARDPIC,                       // 5
        H_JOYPIC,                            // 6
        H_HEALPIC,                           // 7 

        H_GUNPIC=9,                          // 9
        H_KEYPIC,                            // 10
        H_BLAZEPIC,                          // 11
        H_WEAPON1234PIC,                     // 12
        H_WOLFLOGOPIC,                       // 13
        H_VISAPIC,                           // 14
        H_MCPIC,                             // 15
        H_IDLOGOPIC,                         // 16
        H_TOPWINDOWPIC,                      // 17
        H_RIGHTWINDOWPIC,                    // 18
        H_LEFTWINDOWPIC,                     // 19
        H_BOTTOMINFOPIC,                     // 20

        // Lump Start
        C_OPTIONSPIC,                        // 21
        C_CURSOR1PIC,                        // 22
        C_CURSOR2PIC,                        // 23
        C_JOY1PIC = 22,                      // 22 not used but needed
        C_JOY2PIC = 23,                      // 23 not used but needed
        C_NOTSELECTEDPIC,                    // 24
        C_SELECTEDPIC,                       // 25
        C_FXTITLEPIC,                        // 26
        C_DIGITITLEPIC,                      // 27
        C_MUSICTITLEPIC,                     // 28
        C_MOUSELBACKPIC,                     // 29
        C_BABYMODEPIC,                       // 30
        C_EASYPIC,                           // 31
        C_NORMALPIC,                         // 32
        C_HARDPIC,                           // 33
        C_LOADSAVEDISKPIC,                   // 34
        C_DISKLOADING1PIC,                   // 35
        C_DISKLOADING2PIC,                   // 36
        C_CONTROLPIC,                        // 37
        C_CUSTOMIZEPIC,                      // 38
        C_LOADGAMEPIC,                       // 39
        C_SAVEGAMEPIC,                       // 40
        C_EPISODE1PIC,                       // 41
        C_EPISODE2PIC,                       // 42
        C_EPISODE3PIC,                       // 43
        C_EPISODE4PIC,                       // 44
        C_EPISODE5PIC,                       // 45
        C_EPISODE6PIC,                       // 46
        C_CODEPIC,                           // 47
        C_TIMECODEPIC,                       // 48
        C_LEVELPIC,                          // 49
        C_NAMEPIC,                           // 50
        C_SCOREPIC,                          // 51

        // Lump Start
        L_GUYPIC,                            // 52
        L_COLONPIC,                          // 53
        L_NUM0PIC,                           // 54
        L_NUM1PIC,                           // 55
        L_NUM2PIC,                           // 56
        L_NUM3PIC,                           // 57 
        L_NUM4PIC,                           // 58
        L_NUM5PIC,                           // 59
        L_NUM6PIC,                           // 60
        L_NUM7PIC,                           // 61
        L_NUM8PIC,                           // 62
        L_NUM9PIC,                           // 63
        L_PERCENTPIC,                        // 64
        L_APIC,                              // 65
        L_BPIC,                              // 66
        L_CPIC,                              // 67
        L_DPIC,                              // 68
        L_EPIC,                              // 69
        L_FPIC,                              // 70
        L_GPIC,                              // 71
        L_HPIC,                              // 72
        L_IPIC,                              // 73
        L_JPIC,                              // 74
        L_KPIC,                              // 75
        L_LPIC,                              // 76
        L_MPIC,                              // 77
        L_NPIC,                              // 78
        L_OPIC,                              // 79
        L_PPIC,                              // 80
        L_QPIC,                              // 81
        L_RPIC,                              // 82
        L_SPIC,                              // 83
        L_TPIC,                              // 84
        L_UPIC,                              // 85
        L_VPIC,                              // 86
        L_WPIC,                              // 87
        L_XPIC,                              // 88
        L_YPIC,                              // 89
        L_ZPIC,                              // 90
        L_EXPOINTPIC,                        // 91
        L_APOSTROPHEPIC,                     // 92
        L_GUY2PIC,                           // 93
        L_BJWINSPIC,                         // 94
        STATUSBARPIC,                        // 95
        TITLEPIC,                            // 96
        PG13PIC,                             // 97
        CREDITSPIC,                          // 98
        HIGHSCORESPIC,                       // 99

        // Lump Start
        KNIFEPIC,                            // 100
        GUNPIC,                              // 101
        MACHINEGUNPIC,                       // 102
        GATLINGGUNPIC,                       // 103
        NOKEYPIC,                            // 104
        GOLDKEYPIC,                          // 105
        SILVERKEYPIC,                        // 106
        N_BLANKPIC,                          // 107
        N_0PIC,                              // 108
        N_1PIC,                              // 109
        N_2PIC,                              // 110
        N_3PIC,                              // 111
        N_4PIC,                              // 112
        N_5PIC,                              // 113
        N_6PIC,                              // 114
        N_7PIC,                              // 115
        N_8PIC,                              // 116
        N_9PIC,                              // 117
        FACE1APIC,                           // 118
        FACE1BPIC,                           // 119
        FACE1CPIC,                           // 120
        FACE2APIC,                           // 121
        FACE2BPIC,                           // 122
        FACE2CPIC,                           // 123
        FACE3APIC,                           // 124
        FACE3BPIC,                           // 125
        FACE3CPIC,                           // 126
        FACE4APIC,                           // 127
        FACE4BPIC,                           // 128
        FACE4CPIC,                           // 129
        FACE5APIC,                           // 130
        FACE5BPIC,                           // 131
        FACE5CPIC,                           // 132
        FACE6APIC,                           // 133
        FACE6BPIC,                           // 134
        FACE6CPIC,                           // 135
        FACE7APIC,                           // 136
        FACE7BPIC,                           // 137
        FACE7CPIC,                           // 138
        FACE8APIC,                           // 139
        GOTGATLINGPIC,                       // 140
        MUTANTBJPIC,                         // 141
        PAUSEDPIC,                           // 142
        GETPSYCHEDPIC,                       // 143



        ORDERSCREEN=145,
        ERRORSCREEN,                         // 137
        T_HELPART,                           // 138
        T_DEMO0,                             // 139
        T_DEMO1,                             // 140
        T_DEMO2,                             // 141
        T_DEMO3,                             // 142
        T_ENDART1,                           // 143
        T_ENDART2,                           // 144
        T_ENDART3,                           // 145
        T_ENDART4,                           // 146
        T_ENDART5,                           // 147
        T_ENDART6,                           // 148
        ENUMEND
         } graphicnums;

//
// Data LUMPs
//
#define README_LUMP_START       3
#define README_LUMP_END         20

#define CONTROLS_LUMP_START     21
#define CONTROLS_LUMP_END       51

#define LEVELEND_LUMP_START     52
#define LEVELEND_LUMP_END       99

#define LATCHPICS_LUMP_START    100
#define LATCHPICS_LUMP_END      143


//
// Amount of each data item
//
#define NUMCHUNKS    149
#define NUMFONT      2
#define NUMPICS      141
#define NUMTILE8     35
#define NUMEXTERNS   13
//
// File offsets for data items
//
#define STRUCTPIC    0

#define STARTFONT    1
#define STARTPICS    3
#define STARTTILE8   144
#define STARTEXTERNS 145
