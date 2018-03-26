#include "Bone.hpp"
#include <iostream>
#include <fstream>


std::vector<MotionInfo> infos;


std::string & ltrim(std::string & str)
{
	auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
	str.erase( str.begin() , it2);
	return str;   
}

std::string & rtrim(std::string & str)
{
	auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
	str.erase( it1.base() , str.end() );
	return str;   
}

std::string & trim(std::string & str)
{
	return ltrim(rtrim(str));
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

Bone::Bone(void) : _scale(1.f, 1.f, 1.f) , _pos(0.f, 0.f, 0.f), _rot(0.f, 0.f, 0.f){
	read_bvhfile("asset/simple.bvh");
}


Bone::Bone(Vec3 pos, std::string filename) : _scale(1.f, 1.f, 1.f), _pos(0.f, 0.f, 0.f), _rot(0.f, 0.f, 0.f){
	read_bvhfile(filename);
}


Bone::Bone(std::string name) : _scale(1.f, 1.f, 1.f), _pos(0.f, 0.f, 0.f), _rot(0.f, 0.f, 0.f){
	_name = name;
}

Bone::Bone(Bone const & src) {
	*this = src;
}

Bone::~Bone(void) {

}

Bone &Bone::operator=(Bone const & rhs) {
	if (this != &rhs){

	}
	return (*this);
}

void Bone::setPos(Vec3 pos) {
}

void check_header(std::ifstream &file) {
	std::string line;
	std::getline(file, line);
	if (line != "HIERARCHY")
		throw std::invalid_argument("File look's invalid: No HIERARCHY find");
	std::getline(file, line);
	if (line != "ROOT Hips")
		throw std::invalid_argument("File look's invalid: No ROOT Hips find");
}

void set_pos(std::string line, Bone &bone) {
	std::string posline = line.substr(line.find("OFFSET") + strlen("OFFSET") + 1);
	std::vector<std::string> x = split(trim(posline), '\t');
	if (x.size() != 3) 
		throw std::invalid_argument("3 variable for pos is requiere");
	bone.setPos(Vec3(atof(x[0].c_str()), atof(x[1].c_str()), atof(x[2].c_str())));
	/*
	   std::cout << x[0].c_str() << std::endl;
	   std::cout << x[1].c_str() << std::endl;
	   std::cout << x[2].c_str() << std::endl;
	   */
}


MotionInfo get_info(std::string update, Bone *bone) {
	MotionInfo inf;
	inf.bone = bone;
	if (update == "Xposition")	
		inf.type = TYPE::xpos;
	else if (update == "Yposition")	
		inf.type = TYPE::ypos;
	else if (update == "Zposition")	
		inf.type = TYPE::zpos;
	else if (update == "Xrotation")	
		inf.type = TYPE::xrot;
	else if (update == "Yrotation")	
		inf.type = TYPE::yrot;
	else if (update == "Zrotation")	
		inf.type = TYPE::zrot;
	else
		throw std::invalid_argument("Invalid key pos[" + update + "]");
	return inf;
}

void add_channels(std::string line, Bone *current) {
	std::string posline = line.substr(line.find("CHANNELS") + strlen("CHANNELS") + 1);
	std::vector<std::string> x = split(trim(posline), ' ');
	if (x.size() != 4 && x.size() != 7) 
		throw std::invalid_argument("3 or 6 or 9 variable for pos is requiere");
	for (int i = 1; i < x.size(); ++i) {
		infos.push_back(get_info(x[i], current));
	}
}

void recur_read(std::ifstream &file, Bone *bone) {
	std::string line;
	std::getline(file, line);
	if (trim(line) != "{")
		throw std::invalid_argument("Bad formatting");
	while (std::getline(file, line)) {
		if (trim(line).find("OFFSET") == 0)
			set_pos(line, *bone);
		else if (trim(line).find("CHANNELS") == 0)
			add_channels(line, bone);
		else if (trim(line).find("JOIN") == 0) {
			std::vector<std::string> x = split(trim(line), ' ');
			Bone *child = bone->append_child(x[1]);
			recur_read(file, child);
		}
		else if (trim(line).find("End Site") == 0) {
		std::getline(file, line);
		std::getline(file, line);
		std::cout << "End site :" << line << std::endl;
		std::getline(file, line);
		}
		else if (trim(line).find("}") == 0) {
			return ;
		}
		else {
			std::cout << line << std::endl;
		}
	}
}

Bone *Bone::append_child(std::string name) {
	_child.push_back(new Bone(name));
	return _child[_child.size() - 1];
}

void Bone::set_step(TYPE motion_type, float value) {
	if (motion_type == TYPE::xpos) {
		_pos.x = value;
	}
	else if (motion_type == TYPE::ypos) {
		_pos.y = value;
	}
	else if (motion_type == TYPE::zpos) {
		_pos.z = value;
	}
	else if (motion_type == TYPE::xrot) {
		_rot.x = value;
	}
	else if (motion_type == TYPE::yrot) {
		_rot.y = value;
	}
	else if (motion_type == TYPE::zrot) {
		_rot.z = value;
	}
}


void read_motion_info(std::ifstream &file, Bone *bone) {
	std::string line;
	std::getline(file, line);
	if (trim(line) != "MOTION")
		throw std::invalid_argument("Bad formatting");
	std::vector<std::string> x;
	while (std::getline(file, line) && atof(line.c_str()) == 0.f) {
		if (trim(line).find("Frame Time:") == 0)	
			std::cout << "FrameTime: " << &trim(line).c_str()[11] << std::endl;
		else if (trim(line).find("Frames:") == 0)	
			std::cout << "Nombre de frame dans le bvh : " << &trim(line).c_str()[8] << std::endl;
		else
			throw std::invalid_argument("Argument inconnu");
	}
	while (!file.eof()) {
		x = split(line, '\t');
		std::cout <<"infos size" << infos.size() << std::endl;
		for (int i = 0; i < x.size() - 1; i++) {
			infos[i].bone->set_step(infos[i].type, std::atof(x[i].c_str()));
		}
		bone->recup_update();
		std::getline(file, line);
	}
	std::cout << "End parsing" << std::endl;
}


void Bone::recup_update() {
	this->append_step();	
	for (int i = 0; i < _child.size(); i++) {
		_child[i]->recup_update();	
	}
}


void Bone::append_step() {
	Matrix trans;
	Matrix rot;
	Matrix scale;
	Matrix model;

	rot = get_rot(_rot);
	trans = get_rot(_pos);
	scale = get_scale(_scale);
	_rot = Vec3();
	_pos = Vec3();
	_scale = Vec3(1.0, 1.0, 1.0);
	_step.push_back(scale * trans);
}

void Bone::read_bvhfile(std::string filename) {
	std::ifstream file;
	std::string line;
	Bone *current = this;
	file.open(filename.c_str());
	try {
		check_header(file);
		recur_read(file, current);
	}
	catch ( const std::invalid_argument& e ) {
		std::cout << e.what() << std::endl;	
	}
	read_motion_info(file, this);
	/*
	for (int i = 0; i < infos.size(); ++i) {
		std::cout << (void*)infos[i].bone << ":" << infos[i].type << std::endl;	
	}
*/
}

