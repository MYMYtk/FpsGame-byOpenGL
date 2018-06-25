/* �w�b�_�t�@�C�� */
#include <stdio.h>
#include<string.h>
#define _USE_MATH_DEFINES//M_PI���g���Ȃ��Ƃ�
#include <stdlib.h>
#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
/**************************************************************************
* s:�X�^�[�g�@z:�e���� �X�y�[�X�F�W�����v�@�\���L�[�F�ړ���]
**************************************************************************/

#define PI 3.14159265
// �G��
#define	E   4
#define Er  0.4
//�����G�̔��a
#define SEr  0.4
//�e��
#define S 20
#define Sr 0.25
#define RAD (M_PI / 180.0)


// ���@�̈ʒu
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

/* �E�C���h�E�̃T�C�Y */
int winWidth = 1000;
int winHeight = 600;
// �t���O�Ȃǂ̐錾�Ə�����
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

//�e
char shot_flag = 0;
char shotflag[S] = { 0.0 };
double shotx[S] = { 0.0 };
double shoty[S] = { 0.0 };
double shotz[S] = { 0.0 };
double shot_g_theta[S] = { 0.0 };

//�G
int tim = 0;
char enemy_flag = 0;
char enemyflag[E];
double enemyX[E], enemyY[E], enemyZ[E];
//�����G
double SenemyX[E], SenemyY[E], SenemyZ[E];
char Senemyflag[E];
char Senemy_flag = 0;
//�񕜃A�C�e��
double recovX, recovY, recovZ;
double recovR = 0.4;
char recov_flag = 0;

//�J����
int	jcount = 0;
float CameraAzimuth = 90.0;   /* �J�����̈ʒu�i���ʊp�j */
float CameraElevation = 90.0;    /* �J�����̈ʒu�i�p�j */
float CameraDistance = 11;    /* �J�����̈ʒu�i���_����̋����j */
float CameraX;
float CameraY;
float CameraZ;
float g_theta = -1.5;

//�Ə�
double tergetX;
double tergetY;
double tergetZ;
//�F
static GLfloat red[] = { 2.0, 0.3, 0.4, 1.0 };
static GLfloat green[] = { 0.2, 1.5, 0.2, 1.0 };
static GLfloat blue[] = { 0.2, 0.2, 3.0, 1.0 };
static GLfloat yellow[] = { 2.0, 2.4, 0.2, 1.0 };
static GLfloat ground[][4] = {
	{ 0.6, 0.6, 0.6, 1.0 },
	{ 0.3, 0.3, 0.3, 1.0 }
};

//����
static GLfloat lightpos[] = { 3.0, 40.0, 5.0, 1.0 }; /* �����̈ʒu */
static GLfloat lightpos2[] = { 1.0, 1.0, 20.0, 1.0 }; /* �����̈ʒu */
static GLfloat lightDiffuse[3] = { 1.0, 1.0, 1.0 }; //�g�U��
//��ʏ�̕���
int list;

// �֐��錾
void display(void);
void geometrySet(Geometry geom);
void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);//����ȃL�[�{�[�h�̃{�^�����������Ƃ�
void specialkeyboardup(int key, int x, int y);
void myInit(char *windowTitle);
void *font = GLUT_BITMAP_TIMES_ROMAN_24;
void DRAW_STRING(int x, int y, char *string, void *font);
void DISPLAY_TEXT(int x, int y, char *string);
void DRAW_MONITER_1(int x, int y, int w, int h);
void DRAW_MONITER_2(int x, int y, int w, int h);

/***********************************************************
|  �֐��Fmain()
|  �����F���C���֐�
|  �����Fint argc       ���s�������̐�
|  �����Fchar** argv    ���s�������̓��e�i������z��j
|  �ߒl�Fint            0:����I��
***********************************************************/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);

	//�J�����̏�����
	CameraX = CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
	CameraY = CameraDistance * sin(CameraElevation * RAD);
	CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);

	// ���@�ʒu�̏�����
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

	//�G�̏����ݒ�
	for (i = 0; i < E; i++){
		enemyX[i] = -14 + rand() % 28;
		enemyY[i] = 0.0;
		enemyZ[i] = -14 + rand() % 28;
		enemyflag[i] = 1;
		//�����G
		SenemyX[i] = -14 + rand() % 28;
		SenemyY[i] = 0.0;
		SenemyZ[i] = -14 + rand() % 28;
		Senemyflag[i] = 1;
	}

	//�񕜃A�C�e���̏�����
	recovX = 0.0;
	recovY = 0.0;
	recovZ = 0.0;
	recov_flag = 0;

	glutMainLoop();

	return(0);
}

// �`��֐�(���܂��Ă�)
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
|  �֐��Fdisplay()
|  �����F�u�P���́v�O���t�B�b�N�`��C�x���g����
|  �����F�Ȃ�
|  �ߒl�F�Ȃ�
***********************************************************/
void display(void)
{
	/* ������ */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   /* ��ʂ����� */
	glMatrixMode(GL_MODELVIEW);             /* �􉽁i�`��ʒu�Ȃǐݒ肷��j���[�h */
	glLoadIdentity();                       /* �􉽂����������� */

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);  // �����̈ʒu
	DRAW_MONITER_1(0, 0, winWidth - 255, winHeight);
	DRAW_MONITER_2(750, 290, 250, 250);
	glutSwapBuffers();
}

//���C�����
void DRAW_MONITER_1(int x, int y, int w, int h)
{
	glViewport(x, y, w, h); //�r���[�|�[�g�ϊ�
	gluLookAt(Cube.ShiftX, Cube.ShiftY, Cube.ShiftZ,
		Cube.ShiftX + cos(g_theta), Cube.ShiftY, Cube.ShiftZ + sin(g_theta),
		0.0, 1.0, 0.0);

	//�����̕`��
	char t_char[20];
	char t_char2[20];
	char t_char3[20];

	//HP�\���i��ʍ���j
	sprintf(t_char, "HP = %d", Cube.hp);
	DISPLAY_TEXT(1, 95, t_char);

	//Point
	sprintf(t_char2, "Point = %d", Cube.point);
	DISPLAY_TEXT(1, 90, t_char2);

	// �������(�����̕`��)
	if (die_flag == 99){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char3, "Press  s  to start ");
		DISPLAY_TEXT(37, 65, t_char3);
	}

	//�Q�[���I�[�o�[�̕���
	if (die_flag == 0){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char2, "GAME OVER");
		DISPLAY_TEXT(37, 55, t_char2);
	}


	// �n�� 
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

	//�e�̕`��
	for (i = 0; i < S; i++){
		if (shotflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
			glTranslatef(shotx[i], shoty[i], shotz[i]);
			glutSolidSphere(Sr, 10, 2.0);
			glPopMatrix();
		}
	}


	//�G�̕`��
	for (i = 0; i < E; i++){
		if (enemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
			glTranslatef(enemyX[i], enemyY[i], enemyZ[i]);
			glutSolidSphere(Er, 10, 2.0);
			glPopMatrix();
		}
		//�����G�̕`��
		if (Senemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glTranslatef(SenemyX[i], SenemyY[i], SenemyZ[i]);
			glutSolidSphere(SEr, 10, 2.0);
			glPopMatrix();

		}
	}
	//�񕜃A�C�e���̕`��
	if (recov_flag == 1){
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		glTranslatef(recovX, recovY, recovZ);
		glutSolidSphere(recovR, 10, 2.0);
		glPopMatrix();
	}


}

//���Չ��
void DRAW_MONITER_2(int x, int y, int w, int h)
{

	glViewport(x, y, w, h); //�r���[�|�[�g�ϊ�
	glMatrixMode(GL_MODELVIEW);             /* �􉽁i�`��ʒu�Ȃǐݒ肷��j���[�h */
	glLoadIdentity();                       /* �􉽂����������� */
	gluLookAt(CameraX, CameraY, CameraZ,
		0, 0, 0,
		0.0, 1.0, 0.0);

	//���@
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslatef(Cube.ShiftX, Cube.ShiftY, Cube.ShiftZ);
	glutSolidSphere(0.4, 10, 2.0);
	glPopMatrix();

	/* �n�� */
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

	//�e�̕`��
	for (i = 0; i < S; i++){
		if (shotflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
			glTranslatef(shotx[i], shoty[i], shotz[i]);
			glutSolidSphere(Sr, 10, 2.0);
			glPopMatrix();
		}
	}

	//�G�̕`��
	for (i = 0; i < E; i++){
		if (enemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
			glTranslatef(enemyX[i], enemyY[i], enemyZ[i]);
			glutSolidSphere(Er, 10, 2.0);
			glPopMatrix();
		}
		//�����G�̕`��
		if (Senemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glTranslatef(SenemyX[i], SenemyY[i], SenemyZ[i]);
			glutSolidSphere(SEr, 10, 2.0);
			glPopMatrix();
		}
	}

	//�񕜃A�C�e���̕`��
	if (recov_flag == 1){
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		glTranslatef(recovX, recovY, recovZ);
		glutSolidSphere(recovR, 10, 2.0);
		glPopMatrix();
	}


}


//////////////////////////////////////////////////////////////////////////
// �����`��
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
|  �֐��Ftimer(int timerID)
|  �����F�^�C�}�[�i�ݒ莞�Ԍo�߁j�C�x���g����
|  �����Fint timerID    �C�x���g�����������^�C�}�[�̎���ID
|  �ߒl�F�Ȃ�
***********************************************************/
void timer(int value)
{
	glutTimerFunc(3, timer, 0);
	switch (die_flag)	{

		// �ʏ펞
	case	1:

		tim++;

		// ���_�̈ړ�(�E��])
		if (right_r == 1){
			g_theta += 0.004;
		}
		// ���_�̈ړ�(����])
		if (left_r == 1){
			g_theta -= 0.004;
		}
		// ���@�ړ�(�i��)
		if (advance == 1){
			Cube.ShiftX += cos(g_theta) * 0.008;
			Cube.ShiftZ += sin(g_theta) * 0.008;
		}
		//���@�ړ�(�o�b�N)
		if (back == 1){
			Cube.ShiftX -= cos(g_theta) * 0.008;
			Cube.ShiftZ -= sin(g_theta) * 0.008;
		}


		////��ʊO�ɏo�Ȃ��悤�ɂ���
		if (Cube.ShiftX >= 15){ Cube.ShiftX = 15; }
		if (Cube.ShiftX <= -15){ Cube.ShiftX = -15; }
		if (Cube.ShiftZ >= 10){ Cube.ShiftZ = 10; }
		if (Cube.ShiftZ <= -10){ Cube.ShiftZ = -10; }

		// �W�����v�Ɨ����̐���
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
			//�e�ƓG�̂����蔻��
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

			//�G�Ǝ��@�̂����蔻��
			if (enemyflag[i] == 1){
				if (sqrt(pow(Cube.ShiftX - enemyX[i], 2.0) + pow(Cube.ShiftZ - enemyZ[i], 2.0)) < (2 * Er)){
					Cube.hp--;
					enemyX[i] = -14 + rand() % 28;
					enemyY[i] = 0.0;
					enemyZ[i] = -14 + rand() % 28;
					enemyflag[i] = 0;
				}
			}
			//�����G
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

		////���@�Ɖ񕜃A�C�e���̂����蔻��
		if (recov_flag == 1){
			if (sqrt(pow(Cube.ShiftX - recovX, 2.0) + pow(Cube.ShiftY - recovY, 2.0) + pow(Cube.ShiftZ - recovZ, 2.0)) < (2 * recovR)){
				Cube.hp++;
				recovX = 0.0;
				recovY = 0.0;
				recovZ = 0.0;
				recov_flag = 0;
			}
			//��ʊO�ɏo�������
			if ((recovX > 15) || (recovX < -15) || (recovZ > 10) || (recovZ < -10)){
				recov_flag = 0;
			}
		}

		//�e�̈ړ��ƒǉ�
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
			if ((shotx[i] > 15.5) || (shotx[i] < -15.5) || (shotz[i] > 11) || (shotz[i] < -11)){//��ʊO�ɏo������� 
				shotx[i] = 0.0;
				shoty[i] = 0.0;
				shotz[i] = 0.0;
				shotflag[i] = 0;
			}
		}

		//�G�̈ړ�
		for (i = 0; i < E; i++){
			if (enemyflag[i] == 1){
				//���@�Ɍ������Ă���
				if (enemyX[i] - Cube.ShiftX>0){ enemyX[i] -= 0.001; }
				if (enemyX[i] - Cube.ShiftX<0){ enemyX[i] += 0.001; }
				if (enemyZ[i] - Cube.ShiftZ>0){ enemyZ[i] -= 0.001; }
				if (enemyZ[i] - Cube.ShiftZ<0){ enemyZ[i] += 0.001; }
				//��ʊO�ɏo�������
				if ((enemyX[i] > 15.5) || (enemyX[i] < -15.5) || (enemyZ[i] > 11) || (enemyZ[i] < -11)){
					enemyflag[i] = 0;
				}
			}
			//�����G
			if (enemyflag[i] == 1){
				//���@�Ɍ������Ă���
				if (SenemyX[i] - Cube.ShiftX>0){ SenemyX[i] -= 0.002; }
				if (SenemyX[i] - Cube.ShiftX<0){ SenemyX[i] += 0.002; }
				if (SenemyZ[i] - Cube.ShiftZ>0){ SenemyZ[i] -= 0.002; }
				if (SenemyZ[i] - Cube.ShiftZ<0){ SenemyZ[i] += 0.002; }
				//��ʊO�ɏo�������
				if ((SenemyX[i] > 15.5) || (SenemyX[i] < -15.5) || (SenemyZ[i] > 11) || (SenemyZ[i] < -11)){
					Senemyflag[i] = 0;
				}
			}
		}

		//�G�̒ǉ�
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

		////�񕜃A�C�e���̒ǉ�
		if ((tim % 500 == 0)){
			if (recov_flag == 0){
				recovX = -12 + rand() % 24;
				recovY = 1.5;
				recovZ = -12 + rand() % 24;
				recov_flag = 1;

			}
			tim = 0;
		}

		//���S����
		if (Cube.hp == 0){
			die_flag = 0;
		}
		break;

		// ���S��
	case	0:
		break;
	}


	glutPostRedisplay();

}



/***********************************************************
|  �֐��Fkeyboard()
|  �����F�L�[�{�[�h�������ꂽ���̃C�x���g����
|  �����Funsigned char key  �����ꂽ�L�[�̕����R�[�h
|  �����Fint x              �L�[�������ꂽ�Ƃ��̃}�E�X�|�C���^��X���W
|  �����Fint y              �L�[�������ꂽ�Ƃ��̃}�E�X�|�C���^��Y���W
|  �ߒl�F�Ȃ�
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {

		// �X�y�[�X�ŃW�����v
	case ' ':
		jcount++;
		if (jcount < 3){
			Vy = 0.08;
		}
		jump = 1;
		break;

		// "s"�ŊJ�n
	case 's':
		die_flag = 1;
		break;
		//z�Œe����
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
		//�E�{�^���Ŏ��@�E��]
	case GLUT_KEY_LEFT:
		left_r = 1;
		break;
		//���{�^���Ŏ��@����]
	case GLUT_KEY_RIGHT:
		right_r = 1;
		break;
		//��{�^���őO
	case GLUT_KEY_UP:
		advance = 1;
		break;
		//���{�^���Ńo�b�N
	case GLUT_KEY_DOWN:
		back = 1;
		break;
	}


	glutPostRedisplay();
}

void specialkeyboardup(int key, int x, int y){
	switch (key) {
		//���{�^���𗣂��ƃt���O��0�ɂȂ�
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
|  �֐��FmyInit()
|  �����F�E�C���h�E�\���ƕ`��ݒ�̏�����
|  �����Fchar *windowTitle      �E�C���h�E�̃^�C�g���o�[�ɕ\�����镶����
|  �ߒl�F�Ȃ�
***********************************************************/
void myInit(char *windowTitle)
{

	/* �E�C���h�E�̏c���̔���v�Z */
	float aspect = (float)winWidth / (float)winHeight;

	/* OpenGL�E�C���h�E�쐬�܂ł̏����� */
	glutInitWindowPosition(0, 0);  /* �E�C���h�E�\���ʒu */
	glutInitWindowSize(winWidth, winHeight); /* �E�C���h�E�T�C�Y */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);/* �`�惂�[�h */
	glutCreateWindow(windowTitle); /* �E�C���h�E�̕\�� */
	glClearColor(0.7, 0.7, 0.7, 1.0);  /* ��ʏ����F�̐ݒ� */

	/* �C�x���g�������ɌĂяo���֐��̓o�^ */
	glutKeyboardFunc(keyboard); /* �L�[�{�[�h���������� */
	glutSpecialFunc(specialkeyboard);//����ȃL�[�{�[�h���������Ƃ�
	glutSpecialUpFunc(specialkeyboardup);//�L�[�{�[�h�𗣂����Ƃ�
	glutIgnoreKeyRepeat(GL_TRUE);//�L�[�𒷉��������Ƃ��ɌJ��Ԃ��𖳎�
	glutDisplayFunc(display); /* ��ʕ\�� */

	/* CG�`��ݒ� */
	glMatrixMode(GL_PROJECTION);/* �������e(���ߓ��e�@)�ݒ胂�[�h�ɐ؂�ւ� */
	glLoadIdentity(); /* �������e�s��������� */
	gluPerspective(90.0, aspect, 1.0, 20.0);/* �������e�s��̐ݒ� */

	glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(3, timer, 0); /* �^�C�}�[��3�~���b��ɐݒ� */

	/* ����p45�x, �c���� aspect�C�`��O�ʂ܂ł̉��s 1.0�C�`��w�ʂ܂ł̉��s 20.0 */
	glEnable(GL_DEPTH_TEST);        /* �B�ʏ�����L���ɂ��� */
	glEnable(GL_LIGHT0);            /* 0�Ԃ̏Ɩ����g��*/
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);         /* �Ɩ��v�Z�̖@���𐳋K�� */
	glEnable(GL_LIGHTING);          /* �Ɩ���L���ɂ���  */
}
