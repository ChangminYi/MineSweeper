//status
#define MINE 1
#define NOTMINE 0

//open
#define OPEN 1
#define CLOSE 0

//mineSel
#define SEL 1
#define NOTSEL 0

typedef struct MINE_STATUS
{
	short Number;	//Ä­ ³Ñ¹ö
	char Status;	//Áö·Ú or not
	short Periphery;	//ÁÖº¯ Áö·Ú °³¼ö
	char Open;	//¿­¸°Ä­ or not
	char MineSel;	//Áö·Ú¼±ÅÃ or not
}mine;