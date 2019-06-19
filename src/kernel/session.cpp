/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: session.cpp 
CreateTime: 2019-6-19 20:24
=========================================================================*/
#include "session.h"
#include "kernellogger.h"

RATEL_NAMESPACE_BEGIN

Session::Session(const RString& name)
	:name_(name)
{}

Session::~Session()
{}

void Session::enter()
{
	if(existActiveChild()){
		slog_err(kernellogger) << "exist active child before Session(" << name_.cstr() << ") enter!" << endl;
		active_ = false;
		return;
	}
	active_ = onEnter();
}

void Session::leave()
{
	active_ = !onLeave();
	if(active_){ //force all active children leave
		for(auto child : children_){
			child->leave();
			if(child->active())
				slog_err(kernellogger) << "child session(" << name_.cstr() << ") leave failed!" << endl;
		}
	}
}

void Session::attachChild(SessionSPtr& s)
{
	if(!isChild(s))
		children_.push_back(s);
}

void Session::detachChild(SessionSPtr& s)
{
	auto it = std::find(children_.begin(), children_.end(), s);
	if(it != children_.end())
		children_.erase(it);
}

bool Session::isChild(SessionSPtr& s) const
{
	return std::find(children_.begin(), children_.end(), s) != children_.end();
}

SessionList Session::findActiveChildren() const
{
	SessionList resultchildren;
	std::for_each(children_.begin(), 
				children_.end(),
				[&resultchildren](const SessionSPtr& child){
					if(child->active())
						resultchildren.push_back(child);
				});
	return resultchildren;
}

bool Session::existActiveChild() const
{
	return !findActiveChildren().empty();
}

RATEL_NAMESPACE_END
