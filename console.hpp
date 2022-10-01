#ifndef CONSOLE_HPP
#define CONSOLE_HPP

/*
* Console类可用于控制台绘图，可绘制彩色字符（仅可用于Windows平台）
*
* Console类内部封装了一些简单的绘图和输出函数，同时也提供了一些光标操作和窗口大小控制的函数，如setCursor、resize等
*
*		以draw开头的函数会在调用时将字符存储在内部缓冲空间（用于去除绘图时的闪烁），当rapaint函数被调用时统一绘制。
*		（空白字符如 \t \r \n ' ' 全部当作空格处理）
*
*		log函数会直接输出字符，不会将字符存储在内部空间。
*
*		函数具体功能参见函数定义上方的注释
*
*																					2022.6.16
*/


#ifdef _MSC_VER // msvc

#if _MSVC_LANG < 201103L
#error C++版本过低，至少需要C++11。
#endif // !_MSVC_LANG < 201103L

#else // gcc/clang
#if __cplusplus < 201103L
#error C++版本过低，至少需要C++11。
#endif // !__cplusplus <201103L

#endif // !_MSC_VER


#include <iostream>
#include <vector>
#include <windows.h>

#undef min
#undef max

/*文字 & 文字背景颜色*/
using ColorType = int;
enum Color : ColorType
{
	/*color*/
	normal = 0x0008,    //正常颜色
	blue = 0x0001,      //蓝色
	green = 0x0002,     //绿色
	cyan = 0x0003,      //青色
	red = 0x0004,		//红色
	pink = 0x0005,		//黄色
	yellow = 0x0006,	//粉色
	white = 0x0007,		//白色

	/*style*/
	strong = 0x0008,    //颜色加强
	underline = 0x8000,	//下划线


	/*bkcolor*/
	bkblue = 0x0010,      //蓝色背景
	bkgreen = 0x0020,     //绿色背景
	bkcyan = 0x0030,      //青色背景
	bkred = 0x0040,		  //红色背景
	bkpink = 0x0050,   	  //黄色背景
	bkyellow = 0x0060,	  //粉色背景
	bkwhite = 0x0070,	  //白色背景

	/*bkstyle*/
	bkstrong = 0x0080,    //背景颜色加强
};

/*方向*/
using DirectionType = int;
enum Direction :int
{
	Up = 0x0001,		//上
	Down = 0x0002,		//下
	Left = 0x0004,		//左
	Right = 0x0008,		//右
};


/*位置*/
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

/*字符 & 字符颜色*/
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

	/*字符颜色是否包含color*/
	bool hasColor(ColorType color) const
	{
		return (_color & color) == color;
	}

	/*判断字符是否设置了背景颜色*/
	bool hasBackGround() const
	{
		return hasColor(bkred) || hasColor(bkblue) || hasColor(bkgreen) || hasColor(bkstrong);
	}

	/*判断是否是空白字符*/
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


/*返回此值的绘图函数，强制重绘时不会保留*/
using TEMPORARY = void;

/*返回此值的绘图函数，强制重绘时会保留*/
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

	/*清空控制台
	clearBuf：是否清空保留在缓冲的绘图*/
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

	/*在控制台上重新绘图
	repaintALL：是否重绘全部，默认为是。为否则仅仅重绘变化的地方*/
	void repaint(bool repaintALL = true)
	{
		if(repaintALL) {
			drawAll();
		} else {
			draw();
		}
	}

	/*设置控制台大小*/
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

	/*设置控制台大小*/
	void resize(Size NewSize)
	{
		resize(NewSize.width(), NewSize.height());
	}

	/*获取控制台大小*/
	Size size() const
	{
		return ConsoleSize;
	}

	/*判断点是不是在窗口内*/
	bool inConsole(Point p) const
	{
		return p.x() >= 0 && p.y() >= 0 && p.x() < ConsoleSize.width() && p.y() < ConsoleSize.height();
	}

	/*判断点是不是在窗口内*/
	bool inConsole(SizeType x, SizeType y) const
	{
		return inConsole({ x,y });
	}

	/*在控制台上输出语句
	msg：要输出的内容
	color：颜色*/
	template<class T>
	TEMPORARY log(const T& msg, ColorType color = white | strong)
	{
		setTextColor(color);
		std::cout << msg;
	}

	/*在控制台上输出语句
	start end：首尾迭代器
	sep：中间间隔字符(想要无间隔则传入'\0')
	color：颜色*/
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

	/*绘制直线
	StartPos：起始点
	ch：绘制的字符
	lenth：长度
	dir：直线绘制方向(如右下则传入Right | Down)*/
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

	/*绘制直线
	(x, y)：起始点
	ch：绘制的字符
	lenth：长度
	dir：直线绘制方向(如右下则传入Right | Down)*/
	PERMANENT drawLine(SizeType x, SizeType y, Char ch, SizeType lenth, Direction dir)
	{
		drawLine(Point(x, y), ch, lenth, dir);
	}

	/*绘制点
	p：点
	ch：绘制的字符*/
	PERMANENT drawPoint(Point p, Char ch)
	{
		drawLine(p, ch, 1, Up);
	}

	/*绘制点
	(x, y)：点
	ch：绘制的字符*/
	PERMANENT drawPoint(SizeType x, SizeType y, Char ch)
	{
		drawPoint(Point(x, y), ch);
	}

	/*绘制矩形
	StartPos EndPos：对角顶点
	ch：绘制的字符
	fill：实心 & 空心*/
	PERMANENT drawRect(Point StartPos, Point EndPos, Char ch, bool fill = false)
	{
		SizeType width = std::abs(EndPos.x() - StartPos.x()) + 1;
		SizeType height = std::abs(EndPos.y() - StartPos.y()) + 1;

		SizeType x1 = std::min(StartPos.x(), EndPos.x());
		SizeType y1 = std::min(StartPos.y(), EndPos.y());

		SizeType y2 = std::max(StartPos.y(), EndPos.y());
		SizeType x2 = std::max(StartPos.x(), EndPos.x());
		/*(x1, y1) 左上角
		* (x2, y1) 右上角
		* (x1, y2) 左下角
		* (x2, y2) 右下角*/
		if(!fill) {
			drawLine(x1, y1, ch, width, Right);
			drawLine(x1, y2, ch, width, Right);

			drawLine(x1, y1, ch, height, Down);
			drawLine(x2, y1, ch, height, Down);
		} else
			for(SizeType i = y1; i <= y2; i++)
				drawLine(x1, i, ch, width, Right);
	}

	/*绘制矩形
	(x1, x2) (y1, y2)：对角顶点
	ch：绘制的字符
	fill：实心 & 空心*/
	PERMANENT drawRect(SizeType x1, SizeType y1, SizeType x2, SizeType y2, Char ch, bool fill = false)
	{
		drawRect(Point(x1, y1), Point(x2, y2), ch, fill);
	}

	/*绘制文字
	StartPos：起点
	msg：文字
	color：颜色*/
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

	/*绘制文字
	(x, y)：起点
	msg：文字
	color：颜色*/
	PERMANENT drawText(SizeType x, SizeType y, const std::string& msg, ColorType color = white | strong)
	{
		drawText(Point(x, y), msg, color);
	}

	/*设置控制台文本颜色
	参数：color | style | bkcolor | bkstyle
	如黄色带下划线则传入yellow | underline*/
	inline void setTextColor(ColorType colors)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors);
	}

	/*设置光标位置*/
	void setCursor(SizeType x, SizeType y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
	}

	/*设置光标位置*/
	void setCursor(Point p)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { p.x(),p.y() });
	}

	/*获取光标位置*/
	Point CursorPos() const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		return Point(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}

	/*设置光标是否隐藏*/
	void setCursor(bool visible)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO CursorInfo;
		GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
		CursorInfo.bVisible = visible; //隐藏控制台光标
		SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
	}
private:
	/*要输出的字符*/
	std::vector<std::vector<Char>> chars;

	/*用于缓冲的字符*/
	std::vector<std::vector<Char>> oldChars;

	/*窗口大小*/
	Size ConsoleSize;

	/*下次缓冲绘图时是否需要重绘*/
	bool needRepaint = false;

	/*缓冲绘图*/
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

	/*先清屏再重绘全部*/
	void drawAll()
	{
		std::vector<Char> msg;
		for(SizeType i = 0; i < ConsoleSize.height(); i++) {
			for(SizeType j = 0; j < ConsoleSize.width(); j++) {
				msg.push_back(chars[j][i]);
			}
			msg.push_back(Char('\n'));
		}
		msg.pop_back();//去掉最后一个\n

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

		/*用于减轻闪烁*/
		Sleep(1);

		needRepaint = false;
	}

	/*调整字符缓冲区大小*/
	void resize(std::vector<std::vector<Char>>& rect, Size size, Char ch)
	{
		rect.resize(ConsoleSize.width(), std::vector<Char>(ConsoleSize.height(), ch));
	}

	/*判断是否是输出时可忽略的字符*/
	bool canIgnoreWhenPaint(Char ch) const
	{
		if(ch.hasBackGround())
			return false;
		return ch.isBlank();
	}

	/*判断chars[i-1][j]和chars[i][j]是否是GBK编码中文字符*/
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