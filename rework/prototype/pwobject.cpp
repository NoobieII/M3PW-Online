//Lithio
//December 18, 2020

#include <cstdio>

#include "pwengine.hpp"
#include "pwobject.hpp"

PWObject::PWObject(std::string name, PWEngine *engine, bool is_active):
object_id_(engine->get_object_id()),
name_(name),
is_active_(is_active),
to_be_destroyed_(false),
engine_(engine)
{
	engine_->add_object(this);
}

PWObject::~PWObject(){
}

//tell the engine to destroy this object in the next call to update_all()
//this cannot be reversed!
void PWObject::destroy(){
	to_be_destroyed_ = true;
}

//deactivate the object so that the engine doesn't call update()
void PWObject::activate(){
	is_active_ = true;
}

void PWObject::deactivate(){
	is_active_ = false;
}

bool PWObject::is_active(){
	return is_active_;
}

//used by the engine to check whether to destroy this object
bool PWObject::is_destroyed(){
	return to_be_destroyed_;
}

//get the name of the object and id
std::string PWObject::get_name(){
	return name_;
}

int PWObject::get_id(){
	return object_id_;
}
