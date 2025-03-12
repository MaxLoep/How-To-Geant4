#ifndef UTILS
#define UTILS

namespace util {

struct v3 {
	double x, y, z;
	v3(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	v3 operator+(v3& other) {
		return {
			this->x + other.x,
			this->y + other.y,
			this->z + other.z
		};
	}

	v3 operator-(v3& other) {
		return {
			this->x - other.x,
			this->y - other.y,
			this->z - other.z
		};
	}

	v3 operator*(double scalar) {
		return {
			this->x * scalar,
			this->y * scalar,
			this->z * scalar
		};
	}

};

template <typename T> struct Option{
	bool some = false;
	T inner;
	Option<T> (){
		this->some = false;
	};

	Option<T> (T elem) {
		this->some = true;
		this->inner = elem;
	};
};

template <typename A, typename B>
struct Either{
	bool has_a;
	A option_a;
	B option_b;
	Either<A, B> (A elem) {
		this->option_a = elem;
		this->has_a = true;
	}

	Either<A, B> (B elem) {
		this->option_b = elem;
		this->has_a = false;
	}

	Option<A> get_A() {
		return this->has_a? Option<A>(this->option_a): Option<A>();
	};

	Option<A> get_B() {
		return !this->has_a? Option<B>(this->option_b): Option<B>();
	};
};

}
#endif
