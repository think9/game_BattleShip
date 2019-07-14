# BattleShip

윈도우 폼을 이용한 배틀쉽 게임

두 명의 플레이어가 서로 번갈아가며 미사일을 발사하여 상대방의 체력을 먼저 소진시키는 사람이 승리

각 플레이어의 배(Ship)는 구조체로 정의되며 다음과 같은 정보를 가지고 있다.

<h3>배 구조체 정보</h3>

* POINT (x, y) - 배의 중심 좌표
* RECT (x_start, x_end, y_start, y_end) - 배의 영역 좌표
* int degree - 미사일 발사 각도 정보
* bool turn - 자신의 차례
* int health - 배의 체력 정보


<h3>미사일 발사 과정</h3>

<h4>1. 플레이어가 배의 좌표 및 발사각도를 설정한 후 스페이스 버튼 입력</h4>

```C++
case VK_SPACE:
    //nowDraw = TRUE를 통하여 이벤트 발생
    if (nowDraw == FALSE)
      nowDraw = TRUE;
```

<h4>2. WM_PAINT문 안의 함수 실행</h4>

```C++
if (nowDraw)
    {
      if (!t1.turn)	//t1의 미사일
        t2.health = throwBall(hdc, t1.player.right, t1.player.top, t1.degree, time, map, nowDraw, t2.player, t2.health, gameState);
      if (!t2.turn)	//t2의 미사일
        t1.health = throwBall(hdc, t2.player.left, t2.player.top, t2.degree, time, map, nowDraw, t1.player, t1.health, gameState);
      //미사일이 날아가는 동안 최대 이동횟수로 설정하여 배를 움직일 수 없도록 함
      move = 11;
    }
```
<h4>3. throwBall 함수 실행</h4>

해당 함수의 parameter들의 역할은 다음과 같다.
* x, y - 현재 배의 위치 좌표
* degree - 포물선 운동을 위한 미사일의 발사 각도
* map - 현재 윈도우 창
* nowDraw - 미사일 이벤트 종료 여부
* plyaer - 상대의 위치 좌표
* health - 체력 정보
* state - 턴 정보
```C++
int throwBall(HDC hdc, int x, int y, int degree, int time, RECT map, bool &nowDraw, RECT player, int &health, bool &state)
{
	state = FALSE;
	static int ballx = x;
	static int bally = y;
	double dx, dy;
	
	//최초 공의 위치 설정
	if (time < 2)
	{
		ballx = x;
		bally = y;
	}
	
	//시간 가속도 설정 및 위치비교를 통한 진행방향 설정
	if (x < player.left)
		dx = 4 * cos(radian(degree))*time;
	else
		dx = 4 * cos(radian(degree))*time*-1;
	
	//시간 가속도를 통한 진행방향 설정
	dy = (80 * (sin(radian(degree))*time) - ((gravity*time*time) / 2)) / 50;
	
	//MoveToEx(hdc, ballx, bally, NULL);
	//LineTo(hdc, ballx + dx, bally - dy);
	DrawBitmap(hdc, ballx, bally, ballsize, ballsize, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Bomb)));
	ballx = ballx + dx;
	bally = bally - dy;

	//맵 밖으로 나갔을 시 종료
	if (ballx > map.right)
	{
		nowDraw = FALSE;
		state = TRUE;
	}
	else if (bally > map.bottom)
	{
		nowDraw = FALSE;
		state = TRUE;
	}
	else if (ballx < map.left)
	{
		nowDraw = FALSE;
		state = TRUE;
	}
	else if (bally < map.top)
	{
		nowDraw = FALSE;
		state = TRUE;
	}

	//다른 객체와의 충돌
	if (player.left <= ballx + ballsize / 2 && ballx + ballsize / 2 <= player.right
		&& player.top <= bally + ballsize / 2 && bally + ballsize / 2 <= player.bottom)
	{
		health--;
		nowDraw = FALSE;
		state = TRUE;
	}
	return health;
}
```

<h2>REFERENCE</h2>

* https://ko.wikipedia.org/wiki/%ED%8F%AC%EB%AC%BC%EC%84%A0_%EC%9A%B4%EB%8F%99 (위키피디아 - 포물선 운동)

* https://m.blog.naver.com/PostView.nhn?blogId=giriffing&logNo=220369990252&proxyReferer=https%3A%2F%2Fwww.google.com%2F

![Alt text](/Projectile_motion.jpg)
