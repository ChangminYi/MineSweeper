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
	short Number;	//ĭ �ѹ�
	char Status;	//���� or not
	short Periphery;	//�ֺ� ���� ����
	char Open;	//����ĭ or not
	char MineSel;	//���ڼ��� or not
}mine;