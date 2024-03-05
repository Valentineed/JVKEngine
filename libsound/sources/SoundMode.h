#pragma once
//FMOD_MODE
//https://fmod.com/resources/documentation-api?version=2.0&page=core-api-common.html#fmod_mode

typedef unsigned int SOUND_MODE;
#define DEFAULT                          0x00000000
#define LOOP_OFF                         0x00000001
#define LOOP_NORMAL                      0x00000002
#define LOOP_BIDI                        0x00000004
#define SOUND_2D                         0x00000008
#define SOUND_3D                         0x00000010
#define CREATESTREAM                     0x00000080
#define CREATESAMPLE                     0x00000100
#define CREATECOMPRESSEDSAMPLE           0x00000200
#define OPENUSER                         0x00000400
#define OPENMEMORY                       0x00000800
#define OPENMEMORY_POINT                 0x10000000
#define OPENRAW                          0x00001000
#define OPENONLY                         0x00002000
#define ACCURATETIME                     0x00004000
#define MPEGSEARCH                       0x00008000
#define NONBLOCKING                      0x00010000
#define UNIQUE                           0x00020000
#define SOUND_3D_HEADRELATIVE            0x00040000
#define SOUND_3D_WORLDRELATIVE           0x00080000
#define SOUND_3D_INVERSEROLLOFF          0x00100000
#define SOUND_3D_LINEARROLLOFF           0x00200000
#define SOUND_3D_LINEARSQUAREROLLOFF     0x00400000
#define SOUND_3D_INVERSETAPEREDROLLOFF   0x00800000
#define SOUND_3D_CUSTOMROLLOFF           0x04000000
#define SOUND_3D_IGNOREGEOMETRY          0x40000000
#define IGNORETAGS                       0x02000000
#define LOWMEM                           0x08000000
#define VIRTUAL_PLAYFROMSTART            0x80000000