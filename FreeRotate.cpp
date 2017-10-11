/*----------------
    包含的头文件
  ----------------*/

#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "loadmodel.h"

/*----------------
       宏定义
  ----------------*/
#define PI 3.141592653	// 园周率


/*----------------
      全局变量
  ----------------*/
int wndWidth = 500;					// 窗口宽度
int wndHeight = 500;				// 窗体高度
unsigned char btnState;				// 鼠标状态
float theta;						// 旋转角度
float axis[3];						// 旋转轴
float lastPos[3], curPos[3];		// 鼠标上次和当前坐标（映射到单位半球面）
float lastMatrix[16] =				// 前一次矩阵，初始化为单位矩阵E
	{1.0, 0.0, 0.0, 0.0,
	 0.0, 1.0, 0.0, 0.0,
	 0.0, 0.0, 1.0, 0.0,
	 0.0, 0.0, 0.0, 1.0};
float eye[3] = {0.0, 0.0, 4.0};		// 观察者坐标
float at[3] = {0.0, 0.0, 0.0};		// 观察点坐标
float up[3] = {0.0, 1.0, 0.0};		// 观察者的向上向量


/*----------------
      函数声明
  ----------------*/
void init();										// 初始化
int  hemishere(int x, int y, int r,float v[3]);		// 平面鼠标坐标映射为单位半球面坐标
void display();										// 窗口重绘回调函数
void reshape(int w, int h);							// 窗口大小回调函数
void mouse(int btn, int state, int x, int y);		// 鼠标点击回调函数
void motion(int x, int y);							// 鼠标移动回调函数


/*----------------
      函数定义
  ----------------*/

  // 主函数
void main(int argc, char **argv)
{
	readmodel();
	// 初始化GLUT库
	glutInit(&argc, argv);
	// 设置显示模式：RGB彩色，双缓存，有深度
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// 初始化窗口大小
	glutInitWindowSize(wndWidth, wndHeight);
	// 初始化窗口位置
	glutInitWindowPosition(500, 400);
	// 创建窗口
	glutCreateWindow("FreeRotate");

	// 初始化
	init();
	// 注册窗口重绘回调函数
	glutDisplayFunc(display);
	// 注册窗口大小回调函数
	glutReshapeFunc(reshape);
	// 注册鼠标点击回调函数
	glutMouseFunc(mouse);
	// 注册鼠标移动回调函数
	glutMotionFunc(motion);

	// 进入消息循环
	glutMainLoop();
}

// 初始化
void init()
{


	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_position[] = { 0, 2.0, 0, 1.0 };
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

	//启动颜色追踪
	//glEnable(GL_COLOR_MATERIAL);
	//glLightModeli(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glShadeModel(GL_SMOOTH);

	
	glClearColor(1.0, 1.0, 1.0, 1.0);// 设置背景颜色	 
	glColor3f(0.0, 0.0, 0.0);//设置前景颜色 
	glEnable(GL_DEPTH_TEST);//启用深度测试
//	glEnable(GL_CULL_FACE);
}

// 平面鼠标坐标映射为单位半球面坐标
int hemishere(int x, int y, int d,float v[3])
{
	float z;
	// 计算x, y坐标
	v[0] = (float)x * 2.0 - (float)d;
	v[1] = (float)d - (float)y * 2.0;
	// 计算z坐标
	z = d * d - v[0] * v[0] - v[1] * v[1];
	if (z < 0)
	{
		return 0;
	}
	v[2] = sqrt(z);
	// 单位化
	v[0] /= (float)d;
	v[1] /= (float)d;
	v[2] /= (float)d;
	return !0;
}

void display()
{
	//int scale = 5;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3d(0, 1, 0);
	glTranslated(0, 0, -0.5);
	//	glRotated(45, 1, 0, 0);

	glMatrixMode(GL_MODELVIEW);
	// 载入单位矩阵，即：M = E
	glLoadIdentity();
	// 计算新的旋转矩阵，即：M = E · R = R
	glRotatef(theta, axis[0], axis[1], axis[2]);
	// 左乘上前一次的矩阵，即：M = R · L
	glMultMatrixf(lastMatrix);
	// 保存此次处理结果，即：L = M
	glGetFloatv(GL_MODELVIEW_MATRIX, lastMatrix);

	drawmodel();

	glutSwapBuffers();
}



// 窗口大小回调函数
void reshape(int w, int h)
{
	// 设置视口为整个窗口屏幕
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 设置透视投影
	gluPerspective(60.0, (float)w / (float)h, 0.1, 20.0);
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
	wndWidth = w;
	wndHeight = h;

	//GLfloat nRange = 600.0f;
	//if (w <= h)
	//	glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	//else
	//	glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);
}

// 鼠标点击回调函数
void mouse(int btn, int state, int x, int y)
{
	// 记录鼠标状态，供motion函数使用
	btnState = (unsigned char)btn;
	btnState <<= 4;
	btnState |= (unsigned char)state;
	
	switch (btn)
	{
	case GLUT_LEFT_BUTTON:
		// 记录鼠标刚按下时的鼠标单位半球面坐标
		hemishere(x, y, wndHeight, lastPos);
		break;
	}
}

// 鼠标移动回调函数
void motion(int x, int y)
{
	float d, dx, dy, dz;
	if (((btnState >> 4) & 0x0F) == GLUT_LEFT_BUTTON)
	{
		// 计算当前的鼠标单位半球面坐标
		if(!hemishere(x, y, wndHeight, curPos))
		{
			return;
		}
		// 计算移动量的三个方向分量
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];
		// 如果有移动
		if (dx || dy || dz)
		{	
			// 计算移动距离，用来近似移动的球面距离
			d = sqrt(dx * dx + dy * dy + dz * dz);
			// 通过移动距离计算移动的角度
			theta = d * 180.0;
			// 计算移动平面的法向量，即：lastPos × curPos
			axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];
			// 记录当前的鼠标单位半球面坐标
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
			// 重绘窗口
			glutPostRedisplay();
		}
	}	
}
