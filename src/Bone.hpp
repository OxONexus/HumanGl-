#pragma once

#include "Human.hpp"
class Bone;


enum TYPE {xpos = 0, ypos = 1, zpos = 2, xrot = 3, yrot = 4, zrot = 5, xscl = 6, yscl = 7, zscl = 8};

typedef struct MotionInfo {
	enum TYPE type;
	Bone *bone;
} m_info;

class Bone {
	public:
		Bone(void);
		Bone(Vec3 pos, std::string filename);
		Bone(std::string name);
		Bone(Bone const & src);
		virtual ~Bone(void);
		Bone & operator=(Bone const & rhs);
		void read_bvhfile(std::string filename);
		void set_step(TYPE motion_type, float value);
		void append_step();
		void recup_update();
		void setPos(Vec3 pos);
		Bone* append_child(std::string name);
	private:
		Vec3 _rot;
		Vec3 _pos;
		Vec3 _scale;
		std::vector<Matrix> _step;
		std::string _name;
		std::vector<Bone*> _child = {};
};



