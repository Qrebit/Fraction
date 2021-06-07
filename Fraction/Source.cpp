#include<iostream>
using namespace std;

#define tab "\t"

class Fraction;
Fraction operator+(Fraction left, Fraction right);	
Fraction operator-(Fraction left, Fraction right);
Fraction operator*(Fraction left, Fraction right);
Fraction operator/(Fraction left, Fraction right);

class Fraction
{
	int integer;	
	int numerator;	
	int denominator;
	bool minus;
public:

	//Get Set Methods
	bool get_minus()const
	{
		return minus;
	}
	int get_integer()const
	{
		return integer;
	}
	int get_numerator()const
	{
		return numerator;
	}
	int get_denominator()const
	{
		return denominator;
	}
	void set_minus(bool minus)
	{
		this->minus = minus;
	}
	void set_integer(int integer)
	{
		this->integer = integer;
	}
	void set_numerator(int numerator)
	{
		this->numerator = numerator;
	}
	void set_denominator(int denominator)
	{
		if (denominator == 0)denominator = 1;
		this->denominator = denominator;
	}

	Fraction& set_minus_to_number()
	{
		if (minus)
		{
			integer = -integer;
			numerator = -numerator;
			minus = false;
		}
		return *this;
	}
	Fraction& get_minus_from_number()
	{
		if (integer < 0)
		{
			integer = -integer;
			minus = true;
		}
		if (numerator < 0)
		{
			numerator = -numerator;
			minus = true;
		}
		return *this;
	}

	//Constructors
	Fraction()
	{
		this->minus = false;
		this->integer = 0;
		this->numerator = 0;
		this->denominator = 1;
	}
	explicit Fraction(int integer)	
	{
		this->minus = false;
		this->integer = integer;
		this->numerator = 0;
		this->denominator = 1;
		if (integer < 0)
		{
			minus = true;
			this->integer = -integer;
		}
	}
	Fraction(double decimal)
	{
		decimal += 1e-10;
		minus = false;
		if (decimal < 0)
		{
			minus = true;
			decimal = -decimal;
		}
		integer = decimal;
		decimal -= integer;
		denominator = 1e+9;
		numerator = decimal * denominator;
		reduce();

	}
	Fraction(int numerator, int denominator)
	{
		this->minus = false;
		this->integer = 0;
		this->numerator = numerator;
		this->set_denominator(denominator);
		if (numerator < 0)
		{
			minus = true;
			this->numerator = -numerator;
		}

	}
	Fraction(int integer, int numerator, int denominator)
	{
		this->minus = false;
		this->integer = integer;
		this->numerator = numerator;
		this->set_denominator(denominator);
		if (integer < 0)
		{
			minus = true;
			this->integer = -integer;
		}
		if (numerator < 0)
		{
			minus = true;
			this->numerator = -numerator;
		}
	}
	~Fraction(){}


	//Operators
	Fraction& operator=(const Fraction& other)
	{
		this->minus = other.minus;
		this->integer = other.integer;
		this->numerator = other.numerator;
		this->denominator = other.denominator;
		return *this;
	}


	Fraction& operator+=(const Fraction& other)
	{
		return *this = *this + other;
	}
	Fraction& operator-=(const Fraction& other)
	{
		return *this = *this - other;
	}
	Fraction& operator*=(const Fraction& other)
	{
		return *this = *this * other;
	}
	Fraction& operator/=(const Fraction& other)
	{
		return *this = *this / other;
	}


	Fraction& operator++()
	{
		this->integer++;
		return *this;
	}
	Fraction operator++(int)
	{
		Fraction old = *this;
		this->integer++;
		return old;
	}


	explicit operator int()const
	{
		return minus ? -integer : integer;
	}
	operator double()const
	{
		double number = integer + (double)numerator / denominator;
		if (minus)number = -number;
		return number;
	}



	//Methods
	Fraction& to_proper()
	{
		get_minus_from_number();
		integer += numerator / denominator;
		numerator %= denominator;
		return *this;
	}
	Fraction& to_improper()
	{
		get_minus_from_number();
		numerator += integer * denominator;
		integer = 0;
		return *this;
	}
	Fraction& reduce()
	{
		if (numerator == 0)return *this;
		int more, less, rest;
		if (numerator > denominator)more = numerator, less = denominator;
		else less = numerator, more = denominator;
		do
		{
			rest = more % less;
			more = less;
			less = rest;
		} while (rest);
		int GCD = more;
		numerator /= GCD;
		denominator /= GCD;
		return *this;
	}
};

//Out Of Class operators
Fraction operator+(Fraction left, Fraction right)
{
	left.to_proper();
	right.to_proper();
	left.set_minus_to_number();
	right.set_minus_to_number();

	Fraction result;
	result.set_integer(left.get_integer() + right.get_integer());
	result.set_numerator(left.get_numerator() * right.get_denominator() + right.get_numerator() * left.get_denominator());
	result.set_denominator(left.get_denominator() * right.get_denominator());
	result.to_proper().get_minus_from_number();
	return result.reduce();
}
Fraction operator-(Fraction left, Fraction right)
{
	left.to_improper().set_minus_to_number();
	right.to_improper().set_minus_to_number();
	return Fraction
	(
		left.get_numerator() * right.get_denominator() - right.get_numerator() * left.get_denominator(),
		left.get_denominator() * right.get_denominator()
	).to_proper().reduce();
}
Fraction operator*(Fraction left, Fraction right)
{
	left.to_improper().set_minus_to_number();
	right.to_improper().set_minus_to_number();
	return Fraction
	(
		left.get_numerator() * right.get_numerator(),
		left.get_denominator() * right.get_denominator()
	).to_proper().get_minus_from_number().reduce();
}
Fraction operator/(Fraction left, Fraction right)
{
	left.to_improper();
	right.to_improper();
	Fraction result
	(
		left.get_numerator() * right.get_denominator(),
		right.get_numerator() * left.get_denominator()
	);
	result.to_proper();
	if (left.get_minus() || right.get_minus())result.set_minus(true);
	if (left.get_minus() && right.get_minus())result.set_minus(false);
	return result.reduce();
}

bool operator==(Fraction left, Fraction right)
{
	left.to_improper().reduce();
	right.to_improper().reduce();
	return left.get_numerator() == right.get_numerator() && left.get_denominator() == right.get_denominator();
}
bool operator!=(const Fraction& left, const Fraction& right)
{
	return !(left == right);
}
bool operator<(const Fraction& left, const Fraction& right)
{
	return (double)left < (double)right;
}
bool operator>(const Fraction& left, const Fraction& right)
{
	return (double)left > (double)right;
}
bool operator>=(const Fraction& left, const Fraction& right)
{
	return left > right || left == right;
}
bool operator<=(const Fraction& left, const Fraction& right)
{
	return !(left > right);
}

ostream& operator<<(ostream& os, const Fraction& obj)
{
	if (obj.get_minus())os << "(-";
	if (obj.get_integer())os << obj.get_integer();
	if (obj.get_integer() && obj.get_numerator())os << "(";
	if (obj.get_numerator())os << obj.get_numerator() << "/" << obj.get_denominator();
	if (obj.get_integer() && obj.get_numerator())os << ")";
	if (obj.get_integer() == 0 && obj.get_numerator() == 0)os << 0;
	if (obj.get_minus())os << ")";
	return os;
}

void main()
{
	setlocale(LC_ALL, "Russian");
}