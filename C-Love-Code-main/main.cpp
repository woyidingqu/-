#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>


//���ĵ�ṹ��
struct Point {
	double x, y;     //����
	COLORREF color;  //��ɫ
};

//��ɫ����
COLORREF colors[7] = { RGB(255,32,83),RGB(252,222,250) ,RGB(255,0,0) ,RGB(255,0,0) ,RGB(255,2,2) ,RGB(255,0,8) ,RGB(255,5,5) };
//COLORREF colors[7] = { RGB(55,132,83),RGB(252,222,250) ,RGB(25,120,130) ,RGB(25,230,40) ,RGB(25,24,112) ,RGB(255,230,128) ,RGB(25,5,215) };


const int xScreen = 1200;				  //��Ļ���
const int yScreen = 800;				  //��Ļ�߶�
const double PI = 3.1426535159;         //Բ����
const double e = 2.71828;				  //��Ȼ��e
const double averag_distance = 0.162;    //������0.01����ʱ��ԭʼ��������ÿ�����ƽ������
const int quantity = 506;				  //һ��������������������
const int circles = 210;				  //��ɰ�������İ��ĸ�����ÿ�����Ļ���Բ�ͬϵ����
const int frames = 20;					  //��������һ�ε�֡��
Point  origin_points[quantity];			  //����һ������ԭʼ�������ݵ�����
Point  points[circles * quantity];      //����һ���������а������ݵ�����
IMAGE images[frames];					  //����ͼƬ����

//����ת������
double screen_x(double x)
{
	x += xScreen / 2;
	return x;
}
//����ת������
double screen_y(double y)
{
	y = -y + yScreen / 2;
	return y;
}

//����x1-x2��������ĺ���
int creat_random(int x1, int x2)
{
	if (x2 > x1)
		return  rand() % (x2 - x1 + 1) + x1;
	else
		return 0;
}

//������������һ�ε�ȫ�����ݣ������Ƴ�20��ͼƬ����
// 1 �ò������̼����һ�����ĵ��������겢������ origin_points ��
// 2 �ظ��� origin_points ������������ϲ�ͬ��ϵ�����һ�������İ����������ݣ��������� points ��
// 3 ͨ��һЩ��ѧ�߼����� points �����е����ź�����겢���ƣ������ǵ�ԭ�������ݣ�ѭ��20�Σ�
// 4 ����Բ�������Щ�����ĵ㣬��������Щ������ݣ�ѭ��20�Σ�
void creat_data()
{

	int index = 0;

	//�������ڵ�������Ϣ�Ա����ڼ������
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	for (double radian = 0.1; radian <= 2 * PI; radian += 0.005)
	{
		//���ĵĲ�������
		x2 = 16 * pow(sin(radian), 3);
		y2 = 13 * cos(radian) - 5 * cos(2 * radian) - 2 * cos(3 * radian) - cos(4 * radian);

		//��������֮��ľ��� �����ţ�(x1-x2)ƽ�� + ��y1-y1��ƽ����
		double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

		//ֻ�е�����֮��ľ������ƽ������ű�������㣬�������������
		if (distance > averag_distance)
		{
			//x1��y1������ǰ����
			//x2��y2������һ�ε��������һ���������
			x1 = x2, y1 = y2;
			origin_points[index].x = x2;
			origin_points[index++].y = y2;
		}
	}

	index = 0;
	for (double size = 0.1; size <= 20; size += 0.1)
	{
		//��sigmoid�������㵱ǰϵ���ĳɹ�����
		//�ø�����˵��һ�£�������100����ɹ�����Ϊ 90%����ô�Ϳ��ܻ���90���㾭��ɸѡ��������
		//					������100����ɹ�����Ϊ 20%����ô�Ϳ��ܻ���20���㾭��ɸѡ��������
		double success_p = 1 / (1 + pow(e, 8 - size / 2));

		//��������ԭʼ����
		for (int i = 0; i < quantity; ++i)
		{
			//�ø��ʽ���ɸѡ
			if (success_p > creat_random(0, 100) / 100.0)
			{
				//����ɫ����������һ����ɫ
				points[index].color = colors[creat_random(0, 6)];

				//��ԭʼ���ݳ���ϵ��������points��
				points[index].x = size * origin_points[i].x + creat_random(-4, 4);
				points[index++].y = size * origin_points[i].y + creat_random(-4, 4);
			}
		}
	}

	//index��ǰֵ����points�б����˽ṹ�������
	int points_size = index;

	for (int frame = 0; frame < frames; ++frame)
	{
		//��ʼ��ÿ��ͼƬ��xScreen����yScreen
		images[frame] = IMAGE(xScreen, yScreen);

		//�ѵ�frame��ͼ����Ϊ��ǰ����ͼƬ
		SetWorkingImage(&images[frame]);

		//���㰮������������
		for (index = 0; index < points_size; ++index)
		{

			double x = points[index].x, y = points[index].y;                              //�ѵ�ǰֵ��ֵ��x��y
			double distance = sqrt(pow(x, 2) + pow(y, 2));										//���㵱ǰ����ԭ��ľ���
			double diatance_increase = -0.0009 * distance * distance + 0.35714 * distance + 5;	//�ѵ�ǰ������뷽�̻�øõ����������

			//���������������x�᷽����������� x_increase = diatance_increase * cos����ǰ�Ƕȣ�
			//cos����ǰ�Ƕȣ�= x / distance
			double x_increase = diatance_increase * x / distance / frames;
			//���������������x�᷽����������� x_increase = diatance_increase * sin����ǰ�Ƕȣ�
			//sin����ǰ�Ƕȣ�= y / distance
			double y_increase = diatance_increase * y / distance / frames;

			//��Ϊ���ϼ���õ�����һ�������̵��������룬���������̳���20֡�����Ҫ��20


			//���µ����ݸ���ԭ��������
			points[index].x += x_increase;
			points[index].y += y_increase;

			//��ȡ��ǰ�����ɫ����Ϊ�滭��ɫ
			setfillcolor(points[index].color);
			//ע�⣬��Ϊ�������������ǻ�����ѧ�����
			//��˻��Ƶ���Ļ�Ǿ�Ҫת��Ϊ��Ļ����
			solidcircle(screen_x(points[index].x), screen_y(points[index].y), 1);
		}

		//������Χ�����ĵ�
		for (double size = 17; size < 23; size += 0.3)
		{
			for (index = 0; index < quantity; ++index)
			{
				//��ϵ�����ڵ���20��ͨ������Ϊ�ٷ�֮��ʮ����ϵ��С��20��ͨ������Ϊ�ٷ�֮��
				//20��Ϊ�ؼ�ֵ����Ϊ������������ϵ������20
				if ((creat_random(0, 100) / 100.0 > 0.6 && size >= 20) || (size < 20 && creat_random(0, 100) / 100.0 > 0.95))
				{
					double x, y;
					if (size >= 20)
					{
						//��frame��ƽ��������ֵ��Ϊ�����޲��Ӽ�15���������
						//��frame��ƽ���ĺô���frameԽ����Χ�����ĵ��˶���ΧԽ��
						x = origin_points[index].x * size + creat_random(-frame * frame / 5 - 15, frame * frame / 5 + 15);
						y = origin_points[index].y * size + creat_random(-frame * frame / 5 - 15, frame * frame / 5 + 15);
					}
					else
					{
						//����ϵ��С��20�Ĵ����밮�ĵ�һ��
						x = origin_points[index].x * size + creat_random(-5, 5);
						y = origin_points[index].y * size + creat_random(-5, 5);
					}


					//�����ȡ��ɫ������Ϊ��ǰ��ͼ��ɫ
					setfillcolor(colors[creat_random(0, 6)]);
					//����ѧ����ת��Ϊ��Ļ�����ٽ��л���
					solidcircle(screen_x(x), screen_y(y), 1);
					//��Ҫע����ǣ��Ҳ�û�б�����Щ�㣬��Ϊ��Щ�㲻��Ҫǰһ֡����������
					//ֻ��Ҫ��ǰϵ���Ϳɻ��Ƴ��������û ��Ҫ����
				}
			}
		}
	}
}

int main()
{
	initgraph(xScreen, yScreen);  //������Ļ
	BeginBatchDraw();			  //��ʼ������ͼ
	srand(time(0));				  //��ʼ���������
	creat_data();				  //���ú�������20��ͼƬ
	SetWorkingImage();			  //���ú����ѹ���ͼ��ָ�Ϊ���ڣ�û����Ӳ���Ĭ��Ϊ����
	//��Ϊ�������ô��ڲ���ͼƬ�����Ҫ�ѻ�ͼЧ������Ϊ����

	bool extend = true, shrink = false;
	for (int frame = 0; !_kbhit();)     //�˳�����Ϊ��⵽������Ϣ
	{
		putimage(0, 0, &images[frame]); //���ŵ�frame��ͼƬ
		FlushBatchDraw();				//ˢ��������ͼ
		Sleep(20);						//��ʱ20����
		cleardevice();					//�����Ļ������������һ֡ͼƬ


		//ע�� creat data ������ֻ�ǰ������ŵ�20��ͼƬ����û�в�������������ͼƬ
		//���ǰ����ŵ�ͼƬ���Ų��žͲ�����������Ч��
		//����������� if else �����Ǿ���ͼƬ���������Ż��ǵ��Ų���

		if (extend)  //����ʱ�� ++frame����������
			frame == 19 ? (shrink = true, extend = false) : ++frame;
		else         //����ʱ�� --frame�����Ų���
			frame == 0 ? (shrink = false, extend = true) : --frame;
	}

	EndBatchDraw(); //�ر�������ͼ
	closegraph();	//�رջ�ͼ����
	return 0;		//��������
}