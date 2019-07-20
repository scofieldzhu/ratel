/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.kernel
Module: uimediator.h
CreateTime: 2019-6-18 22:04
=========================================================================*/
#ifndef __uimediator_hxx__
#define __uimediator_hxx__

#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN

template <class UI>
class UIMediator
{
public:
	typedef UIMediator<UI> MediatorSelf;
	virtual void subscribeEvents() = 0;
	virtual void unsubscribe() = 0;
	void setUI(UI* ui) { ui_ = ui; }
	UI* ui() { return ui_; }
	UIMediator(UI* ui):ui_(ui){}
	virtual ~UIMediator(){}

protected:
	UI* ui_;
};

RATEL_NAMESPACE_END

#endif
