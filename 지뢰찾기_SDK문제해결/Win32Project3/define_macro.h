////////////////////////////////////////////////
////////////////////////////////////////////////
//각종 define문 정리한 헤더파일

//지뢰 상태와 관련한 선언
#define TILE_STATE_CLOSE 0
#define TILE_STATE_QUESTION 1
#define TILE_STATE_FLAG 2
#define TILE_STATE_OPEN 3
#define TILE_STATE_FAIL_FLAG 4
#define TILE_STATE_BOMB_MINE 5
#define TILE_STATE_BOMB_QUESTION 6

//판과 이미지 크기
#define TILE_IMAGE_WIDTH 20
#define TILE_IMAGE_HEIGHT 20
#define NUMBER_IMAGE_WIDTH 14
#define NUMBER_IMAGE_HEIGHT 20

//난이도
#define EASY_MODE 0
#define NORMAL_MODE 1
#define HARD_MODE 2

//난이도에 따른 판 사이즈
#define EASY_TILE_WIDTH 9
#define EASY_TILE_HEIGHT 9
#define NORMAL_TILE_WIDTH 16
#define NORMAL_TILE_HEIGHT 16
#define HARD_TILE_WIDTH 30
#define HARD_TILE_HEIGHT 16

//난이도에 따른 지뢰 수
#define EASY_MINE_COUNT 10
#define NORMAL_MINE_COUNT 40
#define HARD_MINE_COUNT 99

//게임 진행 상황
#define GAME_READY 0
#define GAME_PLAY 1
#define GAME_OVER 2
