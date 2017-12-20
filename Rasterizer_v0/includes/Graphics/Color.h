#pragma once
namespace Graphics
{
	class Color
	{
	private:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		
	public:
		Color();
		Color(unsigned char p_r, unsigned char p_g, unsigned char p_b, unsigned char p_a = 255);
		Color(const Color& c);
		~Color();
	};
}
