#include "Charactere.hpp"


Charactere::Charactere(): _pos(0, 0, 0), _root() {
}

Charactere::Charactere(Vec3 pos, std::string filename): _pos(pos), _root() {
	_root.read_bvhfile(filename);
}
Charactere::~Charactere() {}

Charactere::Charactere(const Charactere &src) {
	*this = src;
}

void Charactere::update() {

}

Charactere &Charactere::operator=(const Charactere &a) {
	if (this != &a) {
		
	}
	return *this;
}
