#ifndef CONSOLE_HPP
#define CONSOLE_HPP

/*
* Console������ڿ���̨��ͼ���ɻ��Ʋ�ɫ�ַ�����������Windowsƽ̨��
*
* Console���ڲ���װ��һЩ�򵥵Ļ�ͼ�����������ͬʱҲ�ṩ��һЩ�������ʹ��ڴ�С���Ƶĺ�������setCursor��resize��
*
*		��draw��ͷ�ĺ������ڵ���ʱ���ַ��洢���ڲ�����ռ䣨����ȥ����ͼʱ����˸������rapaint����������ʱͳһ���ơ�
*		���հ��ַ��� \t \r \n ' ' ȫ�������ո���
*
*		log������ֱ������ַ������Ὣ�ַ��洢���ڲ��ռ䡣
*
*		�������幦�ܲμ����������Ϸ���ע��
*
*																					2022.6.16
*/


#ifdef _MSC_VER // msvc

#if _MSVC_LANG < 201103L
#error C++�汾���ͣ�������ҪC++11��
#endif // !_MSVC_LANG < 201103L

#else // gcc/clang
#if __cplusplus < 201103L
#error C++�汾���ͣ�������ҪC++11��
#endif // !__cplusplus <201103L

#endif // !_MSC_VER


#include <iostream>
#include <vector>
#include <windows.h>

#undef min
#undef max

/*���� & ���ֱ�����ɫ*/
using ColorType = int;
enum Color : ColorType
{
	/*color*/
	normal = 0x0008,    //������ɫ
	blue = 0x0001,      //��ɫ
	green = 0x0002,     //��ɫ
	cyan = 0x0003,      //��ɫ
	red = 0x0004,		//��ɫ
	pink = 0x0005,		//��ɫ
	yellow = 0x0006,	//��ɫ
	white = 0x0007,		//��ɫ

	/*style*/
	strong = 0x0008,    //��ɫ��ǿ
	underline = 0x8000,	//�»���


	/*bkcolor*/
	bkblue = 0x0010,      //��ɫ����
	bkgreen = 0x0020,     //��ɫ����
	bkcyan = 0x0030,      //��ɫ����
	bkred = 0x0040,		  //��ɫ����
	bkpink = 0x0050,   	  //��ɫ����
	bkyellow = 0x0060,	  //��ɫ����
	bkwhite = 0x0070,	  //��ɫ����

	/*bkstyle*/
	bkstrong = 0x0080,    //������ɫ��ǿ
};

/*����*/
using DirectionType = int;
enum Direction :int
{
	Up = 0x0001,		//��
	Down = 0x0002,		//��
	Left = 0x0004,		//��
	Right = 0x0008,		//��
};


/*λ��*/
using SizeType = short;
class Point
{
public:
	explicit Point() : _x(0), _y(0) {}
	Point(int _x, int _y) : _x(_x), _y(_y) {}

	bool operator==(Point rhs) const
	{
		return _x == rhs._x && _y == rhs._y;
	}

	bool operator!=(Point rhs) const
	{
		return !(*this == rhs);
	}

	Point operator-()
	{
		return Point(-_x, -_y);
	}

	Point operator+(Point rhs) const
	{
		return Point(_x + rhs._x, _y + rhs._y);
	}

	Point& operator+=(Point rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	Point operator-(Point rhs) const
	{
		return Point(_x - rhs._x, _y - rhs._y);
	}

	Point& operator-=(Point rhs)
	{
		*this = *this - rhs;
		return *this;
	}

	void setX(SizeType x)
	{
		_x = x;
	}

	void setY(SizeType y)
	{
		_y = y;
	}

	SizeType x() const
	{
		return _x;
	}

	SizeType y() const
	{
		return _y;
	}

private:
	SizeType _x;
	SizeType _y;
};

/*�ַ� & �ַ���ɫ*/
using ChType = unsigned char;
class Char
{
public:
	explicit Char() :_ch('\0'), _color(white | strong) {}
	explicit Char(char ch, ColorType col = white | strong) :_ch(ch), _color(col) {}

	ChType ch() const
	{
		return _ch;
	}

	ColorType color() const
	{
		return _color;
	}

	void setCh(ChType ch)
	{
		_ch = ch;
	}

	void setColor(ColorType color)
	{
		_color = color;
	}

	bool operator==(Char rhs)
	{
		return _ch == rhs._ch && _color == rhs._color;
	}

	bool operator!=(Char rhs)
	{
		return _ch != rhs._ch || _color != rhs._color;
	}

	/*�ַ���ɫ�Ƿ����color*/
	bool hasColor(ColorType color) const
	{
		return (_color & color) == color;
	}

	/*�ж��ַ��Ƿ������˱�����ɫ*/
	bool hasBackGround() const
	{
		return hasColor(bkred) || hasColor(bkblue) || hasColor(bkgreen) || hasColor(bkstrong);
	}

	/*�ж��Ƿ��ǿհ��ַ�*/
	bool isBlank() const
	{
		return (_ch == ' ' || _ch == '\n' || _ch == '\r' || _ch == '\t');
	}
private:
	ChType _ch;
	ColorType _color;
};

class Size
{
public:
	Size() :_width(0), _height(0) {}
	Size(SizeType _width, SizeType _height) :_width(_width), _height(_height) {}

	SizeType width() const
	{
		return _width;
	}

	SizeType height() const
	{
		return _height;
	}

	void setWidth(SizeType NewWidth)
	{
		_width = NewWidth;
	}

	void setHeight(SizeType NewHeight)
	{
		_height = NewHeight;
	}

	bool operator==(Size rhs) const
	{
		return _width == rhs._width && _height == rhs._height;
	}

	bool operator!=(Size rhs) const
	{
		return !(*this == rhs);
	}

private:
	SizeType _width;
	SizeType _height;
};


/*���ش�ֵ�Ļ�ͼ������ǿ���ػ�ʱ���ᱣ��*/
using TEMPORARY = void;

/*���ش�ֵ�Ļ�ͼ������ǿ���ػ�ʱ�ᱣ��*/
using PERMANENT = void;

class Console
{

public:
	explicit Console(Size ConsoleSize) : ConsoleSize(ConsoleSize)
	{
		resize(ConsoleSize);
		resize(chars, ConsoleSize, Char(' '));
		resize(oldChars, ConsoleSize, Char(' '));
	}

	Console(SizeType width, SizeType height) : Console(Size(width, height)) {}

	/*��տ���̨
	clearBuf���Ƿ���ձ����ڻ���Ļ�ͼ*/
	void clear(bool clearBuf = false)
	{
		system("cls");
		if(!clearBuf) {
			return;
		}

		chars.clear();
		oldChars.clear();
		resize(chars, ConsoleSize, Char(' '));
		resize(oldChars, ConsoleSize, Char(' '));
	}

	/*�ڿ���̨�����»�ͼ
	repaintALL���Ƿ��ػ�ȫ����Ĭ��Ϊ�ǡ�Ϊ��������ػ�仯�ĵط�*/
	void repaint(bool repaintALL = true)
	{
		if(repaintALL) {
			drawAll();
		} else {
			draw();
		}
	}

	/*���ÿ���̨��С*/
	void resize(SizeType width, SizeType height)
	{
		HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SMALL_RECT wrt = { 0, 0, SHORT(width - 1), SHORT(height - 1) };
		SetConsoleWindowInfo(hStdOutput, TRUE, &wrt);
		COORD coord = { SHORT(width),SHORT(height) };
		SetConsoleScreenBufferSize(hStdOutput, coord);

		ConsoleSize = Size(width, height);
		resize(chars, ConsoleSize, Char(' '));
		resize(oldChars, ConsoleSize, Char(' '));
	}

	/*���ÿ���̨��С*/
	void resize(Size NewSize)
	{
		resize(NewSize.width(), NewSize.height());
	}

	/*��ȡ����̨��С*/
	Size size() const
	{
		return ConsoleSize;
	}

	/*�жϵ��ǲ����ڴ�����*/
	bool inConsole(Point p) const
	{
		return p.x() >= 0 && p.y() >= 0 && p.x() < ConsoleSize.width() && p.y() < ConsoleSize.height();
	}

	/*�жϵ��ǲ����ڴ�����*/
	bool inConsole(SizeType x, SizeType y) const
	{
		return inConsole({ x,y });
	}

	/*�ڿ���̨��������
	msg��Ҫ���������
	color����ɫ*/
	template<class T>
	TEMPORARY log(const T& msg, ColorType color = white | strong)
	{
		setTextColor(color);
		std::cout << msg;
	}

	/*�ڿ���̨��������
	start end����β������
	sep���м����ַ�(��Ҫ�޼������'\0')
	color����ɫ*/
	template<class Iter>
	TEMPORARY log(Iter start, Iter end, ChType sep = ' ', ColorType color = white | strong)
	{
		setTextColor(color);
		std::cout << (*start);
		for(auto it = start + 1; it != end; ++it) {
			std::cout << (*it);
			if(sep != '\0') {
				std::cout << sep;
			}
		}
	}

	/*����ֱ��
	StartPos����ʼ��
	ch�����Ƶ��ַ�
	lenth������
	dir��ֱ�߻��Ʒ���(����������Right | Down)*/
	PERMANENT drawLine(Point StartPos, Char ch, SizeType lenth, DirectionType dir)
	{
		SizeType dx = 0, dy = 0;
		dx += (dir & Left) ? -1 : 0;
		dx += (dir & Right) ? 1 : 0;
		dy += (dir & Up) ? -1 : 0;
		dy += (dir & Down) ? 1 : 0;

		SizeType x = StartPos.x(), y = StartPos.y();

		Char _ch = ch;
		if(ch.isBlank()) {
			_ch.setCh(' ');
		}

		for(SizeType i = 0; i < lenth; i++) {
			if(inConsole(x, y)) {
				chars[x][y] = _ch;
			}
			x += dx;
			y += dy;
		}
		needRepaint = true;
	}

	/*����ֱ��
	(x, y)����ʼ��
	ch�����Ƶ��ַ�
	lenth������
	dir��ֱ�߻��Ʒ���(����������Right | Down)*/
	PERMANENT drawLine(SizeType x, SizeType y, Char ch, SizeType lenth, Direction dir)
	{
		drawLine(Point(x, y), ch, lenth, dir);
	}

	/*���Ƶ�
	p����
	ch�����Ƶ��ַ�*/
	PERMANENT drawPoint(Point p, Char ch)
	{
		drawLine(p, ch, 1, Up);
	}

	/*���Ƶ�
	(x, y)����
	ch�����Ƶ��ַ�*/
	PERMANENT drawPoint(SizeType x, SizeType y, Char ch)
	{
		drawPoint(Point(x, y), ch);
	}

	/*���ƾ���
	StartPos EndPos���ԽǶ���
	ch�����Ƶ��ַ�
	fill��ʵ�� & ����*/
	PERMANENT drawRect(Point StartPos, Point EndPos, Char ch, bool fill = false)
	{
		SizeType width = std::abs(EndPos.x() - StartPos.x()) + 1;
		SizeType height = std::abs(EndPos.y() - StartPos.y()) + 1;

		SizeType x1 = std::min(StartPos.x(), EndPos.x());
		SizeType y1 = std::min(StartPos.y(), EndPos.y());

		SizeType y2 = std::max(StartPos.y(), EndPos.y());
		SizeType x2 = std::max(StartPos.x(), EndPos.x());
		/*(x1, y1) ���Ͻ�
		* (x2, y1) ���Ͻ�
		* (x1, y2) ���½�
		* (x2, y2) ���½�*/
		if(!fill) {
			drawLine(x1, y1, ch, width, Right);
			drawLine(x1, y2, ch, width, Right);

			drawLine(x1, y1, ch, height, Down);
			drawLine(x2, y1, ch, height, Down);
		} else
			for(SizeType i = y1; i <= y2; i++)
				drawLine(x1, i, ch, width, Right);
	}

	/*���ƾ���
	(x1, x2) (y1, y2)���ԽǶ���
	ch�����Ƶ��ַ�
	fill��ʵ�� & ����*/
	PERMANENT drawRect(SizeType x1, SizeType y1, SizeType x2, SizeType y2, Char ch, bool fill = false)
	{
		drawRect(Point(x1, y1), Point(x2, y2), ch, fill);
	}

	/*��������
	StartPos�����
	msg������
	color����ɫ*/
	PERMANENT drawText(Point StartPos, const std::string& msg, ColorType color = white | strong)
	{
		SizeType x = StartPos.x();
		for(SizeType i = 0; i < msg.size(); i++) {
			if(inConsole(x + i, StartPos.y())) {
				Char ch(msg[i], color);
				if(ch.isBlank()) {
					ch.setCh(' ');
				}
				chars[SizeType(x + i)][StartPos.y()] = ch;
			}
		}
		needRepaint = true;
	}

	/*��������
	(x, y)�����
	msg������
	color����ɫ*/
	PERMANENT drawText(SizeType x, SizeType y, const std::string& msg, ColorType color = white | strong)
	{
		drawText(Point(x, y), msg, color);
	}

	/*���ÿ���̨�ı���ɫ
	������color | style | bkcolor | bkstyle
	���ɫ���»�������yellow | underline*/
	inline void setTextColor(ColorType colors)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors);
	}

	/*���ù��λ��*/
	void setCursor(SizeType x, SizeType y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
	}

	/*���ù��λ��*/
	void setCursor(Point p)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { p.x(),p.y() });
	}

	/*��ȡ���λ��*/
	Point CursorPos() const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		return Point(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}

	/*���ù���Ƿ�����*/
	void setCursor(bool visible)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO CursorInfo;
		GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
		CursorInfo.bVisible = visible; //���ؿ���̨���
		SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
	}
private:
	/*Ҫ������ַ�*/
	std::vector<std::vector<Char>> chars;

	/*���ڻ�����ַ�*/
	std::vector<std::vector<Char>> oldChars;

	/*���ڴ�С*/
	Size ConsoleSize;

	/*�´λ����ͼʱ�Ƿ���Ҫ�ػ�*/
	bool needRepaint = false;

	/*�����ͼ*/
	void draw()
	{
		if(!needRepaint) {
			return;
		}
		for(SizeType i = 0; i < ConsoleSize.height(); i++) {
			for(SizeType j = 0; j < ConsoleSize.width(); j++) {
				if(oldChars[j][i] != chars[j][i]) {
					if(!isChinese(j, i)) {
						setCursor(j, i);
					} else {
						setCursor(j - 1, i);
					}
					log(chars[j][i].ch(), chars[j][i].color());
					oldChars[j][i] = chars[j][i];
				}
			}
		}
		needRepaint = false;
	}

	/*���������ػ�ȫ��*/
	void drawAll()
	{
		std::vector<Char> msg;
		for(SizeType i = 0; i < ConsoleSize.height(); i++) {
			for(SizeType j = 0; j < ConsoleSize.width(); j++) {
				msg.push_back(chars[j][i]);
			}
			msg.push_back(Char('\n'));
		}
		msg.pop_back();//ȥ�����һ��\n

		std::string buf;
		ColorType color = msg[0].color();
		clear();
		setCursor(0, 0);
		for(size_t i = 0; i < msg.size(); i++) {
			if(msg[i].color() == color ||
			   (msg[i].isBlank() && !msg[i].hasBackGround() && !Char(' ', color).hasBackGround())) {
				buf.push_back(msg[i].ch());
			} else {
				setTextColor(color);
				log(buf, color);
				buf.clear();
				color = msg[i].color();
				buf.push_back(msg[i].ch());
			}
		}
		log(buf, color);

		oldChars = chars;

		/*���ڼ�����˸*/
		Sleep(1);

		needRepaint = false;
	}

	/*�����ַ���������С*/
	void resize(std::vector<std::vector<Char>>& rect, Size size, Char ch)
	{
		rect.resize(ConsoleSize.width(), std::vector<Char>(ConsoleSize.height(), ch));
	}

	/*�ж��Ƿ������ʱ�ɺ��Ե��ַ�*/
	bool canIgnoreWhenPaint(Char ch) const
	{
		if(ch.hasBackGround())
			return false;
		return ch.isBlank();
	}

	/*�ж�chars[i-1][j]��chars[i][j]�Ƿ���GBK���������ַ�*/
	bool isChinese(SizeType i, SizeType j) const
	{
		if(!inConsole(i - 1, j) || !inConsole(i, j)) {
			return false;
		}

		if(!(chars[SizeType(i - 1)][j].ch() >= 0x81 && chars[SizeType(i - 1)][j].ch() <= 0xFE)) {
			return false;
		}
		if(!(chars[i][j].ch() >= 0x40 && chars[i][j].ch() <= 0xFE)) {
			return false;
		}

		return true;
	}
};
static Console console(120, 30);
#endif // !CONSOLE_HPP