/* ヘッダファイル */
#include <stdio.h>
#include<string.h>
#define _USE_MATH_DEFINES//M_PIが使えないとき
#include <stdlib.h>
#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
/**************************************************************************
* s:スタート　z:弾発射 スペース：ジャンプ　十字キー：移動回転
**************************************************************************/

#define PI 3.14159265
// 敵数
#define	E   4
#define Er  0.4
//早い敵の半径
#define SEr  0.4
//弾数
#define S 20
#define Sr 0.25
#define RAD (M_PI / 180.0)


// 自機の位置
typedef struct {
	float ShiftX;
	float ShiftY;
	float ShiftZ;
	float RotateX;
	float RotateY;
	float RotateZ;
	int hp;
	int point;
} Geometry;
Geometry Cube;

/* ウインドウのサイズ */
int winWidth = 1000;
int winHeight = 600;
// フラグなどの宣言と初期化
int  PressButton;
int  i = 0, m = 0, n = 0, j = 0;
int  jump = 0;
char left_r = 0;
char right_r = 0;
char back = 0;
char advance = 0;
char die_flag = 99;
double	mini = 0.5, max = 50;
double	Vy;

//弾
char shot_flag = 0;
char shotflag[S] = { 0.0 };
double shotx[S] = { 0.0 };
double shoty[S] = { 0.0 };
double shotz[S] = { 0.0 };
double shot_g_theta[S] = { 0.0 };

//敵
int tim = 0;
char enemy_flag = 0;
char enemyflag[E];
double enemyX[E], enemyY[E], enemyZ[E];
//早い敵
double SenemyX[E], SenemyY[E], SenemyZ[E];
char Senemyflag[E];
char Senemy_flag = 0;
//回復アイテム
double recovX, recovY, recovZ;
double recovR = 0.4;
char recov_flag = 0;

//カメラ
int	jcount = 0;
float CameraAzimuth = 90.0;   /* カメラの位置（方位角） */
float CameraElevation = 90.0;    /* カメラの位置（仰角） */
float CameraDistance = 11;    /* カメラの位置（原点からの距離） */
float CameraX;
float CameraY;
float CameraZ;
float g_theta = -1.5;

//照準
double tergetX;
double tergetY;
double tergetZ;
//色
static GLfloat red[] = { 2.0, 0.3, 0.4, 1.0 };
static GLfloat green[] = { 0.2, 1.5, 0.2, 1.0 };
static GLfloat blue[] = { 0.2, 0.2, 3.0, 1.0 };
static GLfloat yellow[] = { 2.0, 2.4, 0.2, 1.0 };
static GLfloat ground[][4] = {
	{ 0.6, 0.6, 0.6, 1.0 },
	{ 0.3, 0.3, 0.3, 1.0 }
};

//光源
static GLfloat lightpos[] = { 3.0, 40.0, 5.0, 1.0 }; /* 光源の位置 */
static GLfloat lightpos2[] = { 1.0, 1.0, 20.0, 1.0 }; /* 光源の位置 */
static GLfloat lightDiffuse[3] = { 1.0, 1.0, 1.0 }; //拡散光
//画面上の文字
int list;

// 関数宣言
void display(void);
void geometrySet(Geometry geom);
void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);//特殊なキーボードのボタンを押したとき
void specialkeyboardup(int key, int x, int y);
void myInit(char *windowTitle);
void *font = GLUT_BITMAP_TIMES_ROMAN_24;
void DRAW_STRING(int x, int y, char *string, void *font);
void DISPLAY_TEXT(int x, int y, char *string);
void DRAW_MONITER_1(int x, int y, int w, int h);
void DRAW_MONITER_2(int x, int y, int w, int h);

/***********************************************************
|  関数：main()
|  説明：メイン関数
|  引数：int argc       実行時引数の数
|  引数：char** argv    実行時引数の内容（文字列配列）
|  戻値：int            0:正常終了
***********************************************************/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);

	//カメラの初期化
	CameraX = CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
	CameraY = CameraDistance * sin(CameraElevation * RAD);
	CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);

	// 自機位置の初期化
	Cube.ShiftX = 0;
	Cube.ShiftY = 0.5;
	Cube.ShiftZ = 5;
	Cube.RotateX = 0;
	Cube.RotateY = 0;
	Cube.RotateZ = 0;
	Cube.hp = 5;
	Cube.point = 0;

	tergetX = 0;
	tergetY = 0.04;
	tergetZ = Cube.ShiftZ + sin(g_theta);

	//敵の初期設定
	for (i = 0; i < E; i++){
		enemyX[i] = -14 + rand() % 28;
		enemyY[i] = 0.0;
		enemyZ[i] = -14 + rand() % 28;
		enemyflag[i] = 1;
		//速い敵
		SenemyX[i] = -14 + rand() % 28;
		SenemyY[i] = 0.0;
		SenemyZ[i] = -14 + rand() % 28;
		Senemyflag[i] = 1;
	}

	//回復アイテムの初期化
	recovX = 0.0;
	recovY = 0.0;
	recovZ = 0.0;
	recov_flag = 0;

	glutMainLoop();

	return(0);
}

// 描画関数(決まってる)
void drawString3D(const char *str, float charSize, float lineWidth)
{
	glPushMatrix();
	glPushAttrib(GL_LINE_BIT);
	glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
	glLineWidth(lineWidth);
	while (*str){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
		++str;
	}
	glPopAttrib();
	glPopMatrix();
}

/**********************************************************
|  関数：display()
|  説明：「１枚の」グラフィック描画イベント処理
|  引数：なし
|  戻値：なし
***********************************************************/
void display(void)
{
	/* 初期化 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   /* 画面を消去 */
	glMatrixMode(GL_MODELVIEW);             /* 幾何（描画位置など設定する）モード */
	glLoadIdentity();                       /* 幾何を初期化する */

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);  // 光源の位置
	DRAW_MONITER_1(0, 0, winWidth - 255, winHeight);
	DRAW_MONITER_2(750, 290, 250, 250);
	glutSwapBuffers();
}

//メイン画面
void DRAW_MONITER_1(int x, int y, int w, int h)
{
	glViewport(x, y, w, h); //ビューポート変換
	gluLookAt(Cube.ShiftX, Cube.ShiftY, Cube.ShiftZ,
		Cube.ShiftX + cos(g_theta), Cube.ShiftY, Cube.ShiftZ + sin(g_theta),
		0.0, 1.0, 0.0);

	//文字の描画
	char t_char[20];
	char t_char2[20];
	char t_char3[20];

	//HP表示（画面左上）
	sprintf(t_char, "HP = %d", Cube.hp);
	DISPLAY_TEXT(1, 95, t_char);

	//Point
	sprintf(t_char2, "Point = %d", Cube.point);
	DISPLAY_TEXT(1, 90, t_char2);

	// 初期状態(文字の描画)
	if (die_flag == 99){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char3, "Press  s  to start ");
		DISPLAY_TEXT(37, 65, t_char3);
	}

	//ゲームオーバーの文字
	if (die_flag == 0){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char2, "GAME OVER");
		DISPLAY_TEXT(37, 55, t_char2);
	}


	// 地面 
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	for (j = -11; j < 11; ++j) {
		for (i = -16; i < 16; ++i) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
			glVertex3d((GLdouble)i, -0.5, (GLdouble)j);
			glVertex3d((GLdouble)i, -0.5, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)j);
		}
	}
	glEnd();

	//弾の描画
	for (i = 0; i < S; i++){
		if (shotflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
			glTranslatef(shotx[i], shoty[i], shotz[i]);
			glutSolidSphere(Sr, 10, 2.0);
			glPopMatrix();
		}
	}


	//敵の描画
	for (i = 0; i < E; i++){
		if (enemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
			glTranslatef(enemyX[i], enemyY[i], enemyZ[i]);
			glutSolidSphere(Er, 10, 2.0);
			glPopMatrix();
		}
		//速い敵の描画
		if (Senemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glTranslatef(SenemyX[i], SenemyY[i], SenemyZ[i]);
			glutSolidSphere(SEr, 10, 2.0);
			glPopMatrix();

		}
	}
	//回復アイテムの描画
	if (recov_flag == 1){
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		glTranslatef(recovX, recovY, recovZ);
		glutSolidSphere(recovR, 10, 2.0);
		glPopMatrix();
	}


}

//俯瞰画面
void DRAW_MONITER_2(int x, int y, int w, int h)
{

	glViewport(x, y, w, h); //ビューポート変換
	glMatrixMode(GL_MODELVIEW);             /* 幾何（描画位置など設定する）モード */
	glLoadIdentity();                       /* 幾何を初期化する */
	gluLookAt(CameraX, CameraY, CameraZ,
		0, 0, 0,
		0.0, 1.0, 0.0);

	//自機
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslatef(Cube.ShiftX, Cube.ShiftY, Cube.ShiftZ);
	glutSolidSphere(0.4, 10, 2.0);
	glPopMatrix();

	/* 地面 */
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	for (j = -11; j < 11; ++j) {
		for (i = -16; i < 16; ++i) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
			glVertex3d((GLdouble)i, -0.5, (GLdouble)j);
			glVertex3d((GLdouble)i, -0.5, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)j);
		}
	}
	glEnd();

	//弾の描画
	for (i = 0; i < S; i++){
		if (shotflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
			glTranslatef(shotx[i], shoty[i], shotz[i]);
			glutSolidSphere(Sr, 10, 2.0);
			glPopMatrix();
		}
	}

	//敵の描画
	for (i = 0; i < E; i++){
		if (enemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
			glTranslatef(enemyX[i], enemyY[i], enemyZ[i]);
			glutSolidSphere(Er, 10, 2.0);
			glPopMatrix();
		}
		//速い敵の描画
		if (Senemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glTranslatef(SenemyX[i], SenemyY[i], SenemyZ[i]);
			glutSolidSphere(SEr, 10, 2.0);
			glPopMatrix();
		}
	}

	//回復アイテムの描画
	if (recov_flag == 1){
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		glTranslatef(recovX, recovY, recovZ);
		glutSolidSphere(recovR, 10, 2.0);
		glPopMatrix();
	}


}


//////////////////////////////////////////////////////////////////////////
// 文字描画
//////////////////////////////////////////////////////////////////////////
void DISPLAY_TEXT(int x, int y, char *string){
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushAttrib(GL_ENABLE_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	glCallList(list);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	DRAW_STRING(x, y, string, GLUT_BITMAP_TIMES_ROMAN_24);
	glEndList();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void DRAW_STRING(int x, int y, char *string, void *font){
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++){
		glutBitmapCharacter(font, string[i]);
	}
}

/***********************************************************
|  関数：timer(int timerID)
|  説明：タイマー（設定時間経過）イベント処理
|  引数：int timerID    イベントが発生したタイマーの識別ID
|  戻値：なし
***********************************************************/
void timer(int value)
{
	glutTimerFunc(3, timer, 0);
	switch (die_flag)	{

		// 通常時
	case	1:

		tim++;

		// 視点の移動(右回転)
		if (right_r == 1){
			g_theta += 0.004;
		}
		// 視点の移動(左回転)
		if (left_r == 1){
			g_theta -= 0.004;
		}
		// 自機移動(進む)
		if (advance == 1){
			Cube.ShiftX += cos(g_theta) * 0.008;
			Cube.ShiftZ += sin(g_theta) * 0.008;
		}
		//自機移動(バック)
		if (back == 1){
			Cube.ShiftX -= cos(g_theta) * 0.008;
			Cube.ShiftZ -= sin(g_theta) * 0.008;
		}


		////画面外に出ないようにする
		if (Cube.ShiftX >= 15){ Cube.ShiftX = 15; }
		if (Cube.ShiftX <= -15){ Cube.ShiftX = -15; }
		if (Cube.ShiftZ >= 10){ Cube.ShiftZ = 10; }
		if (Cube.ShiftZ <= -10){ Cube.ShiftZ = -10; }

		// ジャンプと落下の制御
		Cube.ShiftY += Vy;
		Vy -= 0.001;

		if (Cube.ShiftY <= mini){
			Cube.ShiftY = mini;
			Vy = 0;
			jump = 0;
			jcount = 0;
		}
		else if (max <= Cube.ShiftY){
			Cube.ShiftY = max;
		}


		for (i = 0; i < E; i++){
			//弾と敵のあたり判定
			for (m = 0; m < S; m++){
				if (shotflag[m] == 1 && enemyflag[i] == 1){
					if (sqrt(pow(shotx[m] - enemyX[i], 2.0) + pow(shotz[m] - enemyZ[i], 2.0)) < (Sr + Er)){
						shotx[m] = 0.0;
						shoty[m] = 0.0;
						shotz[m] = 0.0;
						shotflag[m] = 0;
						enemyX[i] = 0.0;
						enemyY[i] = 0.0;
						enemyZ[i] = 0.0;
						enemyflag[i] = 0;
						Cube.point++;
					}
				}
				if (shotflag[m] == 1 && Senemyflag[i] == 1){
					if (sqrt(pow(shotx[m] - SenemyX[i], 2.0) + pow(shotz[m] - SenemyZ[i], 2.0)) < (Sr + Er)){
						shotx[m] = 0.0;
						shoty[m] = 0.0;
						shotz[m] = 0.0;
						shotflag[m] = 0;
						SenemyX[i] = 0.0;
						SenemyY[i] = 0.0;
						SenemyZ[i] = 0.0;
						Senemyflag[i] = 0;
						Cube.point++;
					}
				}
			}

			//敵と自機のあたり判定
			if (enemyflag[i] == 1){
				if (sqrt(pow(Cube.ShiftX - enemyX[i], 2.0) + pow(Cube.ShiftZ - enemyZ[i], 2.0)) < (2 * Er)){
					Cube.hp--;
					enemyX[i] = -14 + rand() % 28;
					enemyY[i] = 0.0;
					enemyZ[i] = -14 + rand() % 28;
					enemyflag[i] = 0;
				}
			}
			//速い敵
			if (Senemyflag[i] == 1){
				if (sqrt(pow(Cube.ShiftX - SenemyX[i], 2.0) + pow(Cube.ShiftZ - SenemyZ[i], 2.0)) < (2 * Er)){
					Cube.hp--;
					SenemyX[i] = -14 + rand() % 28;
					SenemyY[i] = 0.0;
					SenemyZ[i] = -14 + rand() % 28;
					Senemyflag[i] = 0;
				}
			}
		}

		////自機と回復アイテムのあたり判定
		if (recov_flag == 1){
			if (sqrt(pow(Cube.ShiftX - recovX, 2.0) + pow(Cube.ShiftY - recovY, 2.0) + pow(Cube.ShiftZ - recovZ, 2.0)) < (2 * recovR)){
				Cube.hp++;
				recovX = 0.0;
				recovY = 0.0;
				recovZ = 0.0;
				recov_flag = 0;
			}
			//画面外に出たら消去
			if ((recovX > 15) || (recovX < -15) || (recovZ > 10) || (recovZ < -10)){
				recov_flag = 0;
			}
		}

		//弾の移動と追加
		for (i = 0; i < S; i++){
			if (shot_flag == 1){
				if (shotflag[i] == 0){
					shotx[i] = Cube.ShiftX;
					shoty[i] = 0;
					shotz[i] = Cube.ShiftZ;
					shotflag[i] = 1;
					shot_flag = 0;
					shot_g_theta[i] = g_theta;
				}
			}
			if (shotflag[i] == 1){
				shotz[i] += sin(shot_g_theta[i]) * 0.08;
				shotx[i] += cos(shot_g_theta[i]) * 0.08;
			}
			if ((shotx[i] > 15.5) || (shotx[i] < -15.5) || (shotz[i] > 11) || (shotz[i] < -11)){//画面外に出たら消す 
				shotx[i] = 0.0;
				shoty[i] = 0.0;
				shotz[i] = 0.0;
				shotflag[i] = 0;
			}
		}

		//敵の移動
		for (i = 0; i < E; i++){
			if (enemyflag[i] == 1){
				//自機に向かってくる
				if (enemyX[i] - Cube.ShiftX>0){ enemyX[i] -= 0.001; }
				if (enemyX[i] - Cube.ShiftX<0){ enemyX[i] += 0.001; }
				if (enemyZ[i] - Cube.ShiftZ>0){ enemyZ[i] -= 0.001; }
				if (enemyZ[i] - Cube.ShiftZ<0){ enemyZ[i] += 0.001; }
				//画面外に出たら消去
				if ((enemyX[i] > 15.5) || (enemyX[i] < -15.5) || (enemyZ[i] > 11) || (enemyZ[i] < -11)){
					enemyflag[i] = 0;
				}
			}
			//速い敵
			if (enemyflag[i] == 1){
				//自機に向かってくる
				if (SenemyX[i] - Cube.ShiftX>0){ SenemyX[i] -= 0.002; }
				if (SenemyX[i] - Cube.ShiftX<0){ SenemyX[i] += 0.002; }
				if (SenemyZ[i] - Cube.ShiftZ>0){ SenemyZ[i] -= 0.002; }
				if (SenemyZ[i] - Cube.ShiftZ<0){ SenemyZ[i] += 0.002; }
				//画面外に出たら消去
				if ((SenemyX[i] > 15.5) || (SenemyX[i] < -15.5) || (SenemyZ[i] > 11) || (SenemyZ[i] < -11)){
					Senemyflag[i] = 0;
				}
			}
		}

		//敵の追加
		if (tim % 50 == 0){
			i = 0;
			while (i < E){
				if (enemyflag[i] == 0){
					enemyX[i] = -15 + rand() % 30;
					enemyY[i] = 0.0;
					enemyZ[i] = -12 + rand() % 24;
					enemyflag[i] = 1;
					break;

				}
				if ((tim % 150 == 0) && (Senemyflag[i] == 0)){
					SenemyX[i] = -15 + rand() % 30;
					SenemyY[i] = 0.0;
					SenemyZ[i] = -12 + rand() % 24;
					Senemyflag[i] = 1;
					break;

				}
				i++;
			}
		}

		////回復アイテムの追加
		if ((tim % 500 == 0)){
			if (recov_flag == 0){
				recovX = -12 + rand() % 24;
				recovY = 1.5;
				recovZ = -12 + rand() % 24;
				recov_flag = 1;

			}
			tim = 0;
		}

		//死亡判定
		if (Cube.hp == 0){
			die_flag = 0;
		}
		break;

		// 死亡時
	case	0:
		break;
	}


	glutPostRedisplay();

}



/***********************************************************
|  関数：keyboard()
|  説明：キーボードが押された時のイベント処理
|  引数：unsigned char key  押されたキーの文字コード
|  引数：int x              キーが押されたときのマウスポインタのX座標
|  引数：int y              キーが押されたときのマウスポインタのY座標
|  戻値：なし
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {

		// スペースでジャンプ
	case ' ':
		jcount++;
		if (jcount < 3){
			Vy = 0.08;
		}
		jump = 1;
		break;

		// "s"で開始
	case 's':
		die_flag = 1;
		break;
		//zで弾発射
	case 'z':
		shot_flag = 1;
		break;
	}

	glutPostRedisplay();

	x = y = 0;
}

void specialkeyboard(int key, int x, int y)
{
	switch (key) {
		//右ボタンで自機右回転
	case GLUT_KEY_LEFT:
		left_r = 1;
		break;
		//左ボタンで自機左回転
	case GLUT_KEY_RIGHT:
		right_r = 1;
		break;
		//上ボタンで前
	case GLUT_KEY_UP:
		advance = 1;
		break;
		//下ボタンでバック
	case GLUT_KEY_DOWN:
		back = 1;
		break;
	}


	glutPostRedisplay();
}

void specialkeyboardup(int key, int x, int y){
	switch (key) {
		//左ボタンを離すとフラグが0になる
	case GLUT_KEY_LEFT:
		left_r = 0;
		break;

	case GLUT_KEY_RIGHT:
		right_r = 0;
		break;

	case GLUT_KEY_UP:
		advance = 0;
		break;

	case GLUT_KEY_DOWN:
		back = 0;
		break;
	}

	glutPostRedisplay();
}


/***********************************************************
|  関数：myInit()
|  説明：ウインドウ表示と描画設定の初期化
|  引数：char *windowTitle      ウインドウのタイトルバーに表示する文字列
|  戻値：なし
***********************************************************/
void myInit(char *windowTitle)
{

	/* ウインドウの縦横の比を計算 */
	float aspect = (float)winWidth / (float)winHeight;

	/* OpenGLウインドウ作成までの初期化 */
	glutInitWindowPosition(0, 0);  /* ウインドウ表示位置 */
	glutInitWindowSize(winWidth, winHeight); /* ウインドウサイズ */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);/* 描画モード */
	glutCreateWindow(windowTitle); /* ウインドウの表示 */
	glClearColor(0.7, 0.7, 0.7, 1.0);  /* 画面消去色の設定 */

	/* イベント発生時に呼び出す関数の登録 */
	glutKeyboardFunc(keyboard); /* キーボードを押した時 */
	glutSpecialFunc(specialkeyboard);//特殊なキーボードを押したとき
	glutSpecialUpFunc(specialkeyboardup);//キーボードを離したとき
	glutIgnoreKeyRepeat(GL_TRUE);//キーを長押ししたときに繰り返しを無視
	glutDisplayFunc(display); /* 画面表示 */

	/* CG描画設定 */
	glMatrixMode(GL_PROJECTION);/* 透視投影(遠近投影法)設定モードに切り替え */
	glLoadIdentity(); /* 透視投影行列を初期化 */
	gluPerspective(90.0, aspect, 1.0, 20.0);/* 透視投影行列の設定 */

	glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(3, timer, 0); /* タイマーを3ミリ秒後に設定 */

	/* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */
	glEnable(GL_DEPTH_TEST);        /* 隠面消去を有効にする */
	glEnable(GL_LIGHT0);            /* 0番の照明を使う*/
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);         /* 照明計算の法線を正規化 */
	glEnable(GL_LIGHTING);          /* 照明を有効にする  */
}
