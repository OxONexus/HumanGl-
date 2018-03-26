#pragma once

#include "Bone.hpp"

class Charactere {
	public :
		Charactere();
		Charactere(const Charactere &b);
		Charactere(Vec3 pos, std::string filename);
		~Charactere();
		Charactere &operator=(const Charactere &a);
		void update();
	private :
		Vec3 _pos;
		Vec3 _rot;
		Bone _root;
		std::vector<Matrix> _array_mat;
};
