#ifndef __AK
#define __AK

#define	AkBITS_LEVELS	0x54
#define AkBITS_INIS		0x55
#define	AkBITS_STEPS	0x58

#define	AkDATA_HW_LO	0x88
#define	AkDATA_HW_HI	0x89
	
#define	AkFLAG_PAUSEPW	0x0100
#define	AkFLAG_PAUSETM	0x0200
#define	AkFLAG_END		0x0400
#define	AkFLAG_ALT		0x2000

#define	AkMODE_L_0		0x02
#define	AkMODE_READYMAX	0x07
#define	AkMODE_APPMIN	0x10
#define	AkMODE_APPMAX	0x60
#define	AkMODE_L_1		0x71	// 113
#define	AkMODE_L_2		0x72	// 114
#define	AkMODE_L_3		0x73	// 115
#define	AkMODE_L_4		0x74	// 116
#define	AkMODE_L_5		0x75	// 117
#define	AkMODE_L_6		0x76	// 118
#define	AkMODE_L_7		0x77	// 119
#define	AkMODE_L_8		0x78	// 120
#define	AkMODE_L_9		0x79	// 121
#define	AkMODE_RETURN	0x81	// 129
#define	AkMODE_PAIRING	0x84	// 132
#define	AkMODE_RUN		0xc0	// 192
#define AkMODE_PREP		0xc1	// 193
#define AkMODE_PAUSE	0xc5	// 197
#define AkMODE_WAITNEXT	0xc9	// 201
#define AkMODE_BOOTING	0xfc	// 252
#define AkMODE_OFF		0xfe	// 254

#define	AkMSG_ERROR		1
#define	AkMSG_MODECHG	2
#define	AkMSG_POWERCHG	3

#ifndef __BOOL
#define __BOOL
typedef signed char		BOOL;
typedef signed char		I08;
typedef signed short	I16;
typedef signed long		I32;
typedef void*			PTR;
typedef unsigned char	U08;
typedef unsigned short	U16;
typedef unsigned long	U32;
#endif

typedef BOOL (*AkACK)(void*, U08);
typedef void (*AkMSG)(U08);

typedef struct {
	U16	powers[9];
	U16	tmpers[9];
	U16	timers[9];
} AkLVL;

typedef struct {			// max size: 128 byte
	U32		checksum;		// 0, fixed
	U16		software;		// 4, fixed
	U16		hardware;		// 6, fixed
	U08		reserved1;		// 8
	U08		devinf[55];		// 9, fixed
	AkLVL	levels;			// 64
	U08		flags;			// 118
	U08		level_0;		// 119
	U16		reserved2[4];	// 120
} AkINI;					// 128

void*	Ak_GetBits		(U08, U16*);
U08		Ak_GetErrcode	(void);
U16		Ak_GetFlags		(void);
AkINI*	Ak_GetIni		(void);
U08		Ak_GetMode		(void);
U08		Ak_GetParam08	(U08);
U16		Ak_GetParam16	(U08);
U16		Ak_GetPower		(void);
U16		Ak_GetTimer		(void);
void	Ak_OnBeat		(U08);
void	Ak_OnClock		(U32);
void	Ak_OnIdle		(void);
void	Ak_Prepare		(void);
void	Ak_ResetErrcode	(U08);
void	Ak_ResetFlags	(U16);
U16		Ak_SetBits		(const void*, U16, AkACK);
U16		Ak_SetBitsEx	(const void*, U16, AkACK, U08, U08);
void	Ak_SetErrcode	(U08);
void	Ak_SetFlags		(U16);
void	Ak_SetMode		(U08);
void	Ak_SetParam08	(U08, U08);
void	Ak_SetParam16	(U08, U16);
void	Ak_SetSensor	(U16);
void	Ak_SetTimer		(U16);
void	Ak_Start		(AkMSG);
void	Ak_Stop			(void);

#endif
