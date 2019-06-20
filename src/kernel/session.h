/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: session.h 
CreateTime: 2019-6-19 20:09
=========================================================================*/
#ifndef __session_h__
#define __session_h__

#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN
class RATEL_KERNEL_API Session
{	
public:
	bool active() const { return active_; }
	void enter();	
	void leave();	
	void attachChild(SessionSPtr& s);
	void detachChild(SessionSPtr& s);
	bool isChild(SessionSPtr& s) const;
	SessionList findActiveChildren() const;
	bool existActiveChild() const;
	SessionList children() const { return children_; }
	const RString& name() const { return name_; }
	Session(const RString& name);
	virtual ~Session();

protected:
	virtual bool onEnter() = 0;
	virtual bool onLeave() = 0;
	SessionList children_;
	bool active_ = false;

private:
	RString name_;
};

RATEL_NAMESPACE_END


#endif
