//Lithio
//December 16, 2020

#ifndef PWOBJECT_HPP
#define PWOBJECT_HPP

#include <string>

class PWEngine;

class PWObject{
public:
	//constructor - it is recommended to use a unique name but not necessary
	PWObject(std::string name, PWEngine *engine, bool is_active);
	
	//destructor
	virtual ~PWObject();
	
	//the engine calls this function every deltatime milliseconds
	virtual void update() = 0;
	
	//tell the engine to destroy this object in the next call to update_all()
	//this cannot be reversed!
	void destroy();
	
	//deactivate the object so that the engine doesn't call update()
	void activate();
	void deactivate();
	
	//used by the engine to check whether to call update()
	bool is_active();
	
	//used by the engine to check whether to destroy this object
	bool is_destroyed();
	
	//get the name of the object and id
	std::string get_name();
	int get_id();
	
protected:
	int object_id_;
	std::string name_;
	bool is_active_;
	bool to_be_destroyed_;
	PWEngine *engine_;
};

#endif
