/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.kernel
Module: xsignal.hxx 
CreateTime: 2019-7-7 21:11
=======================================================================*/
#ifndef __xsignal_hxx__
#define __xsignal_hxx__

#include "kernelpublic.h"
#include <boost/signals2.hpp>

RATEL_NAMESPACE_BEGIN

typedef boost::signals2::connection SignalCon;

template <typename EvtCls>
struct XSignal
{
    typedef EvtCls MyEvtCls;
    typedef XSignal<EvtCls> Self;    
    typedef boost::signals2::signal<void(const EvtCls&)> BSig2Type;
    typedef typename BSig2Type::slot_type SigSlot;
    static Self& Inst()
    {
        static Self s;
        return s;
    }
    enum PosType
    {
        kBack,
        kFront,
    };
    SignalCon connect(const SigSlot& subscriber, PosType postype = kBack)
    {
        return sigsource_.connect(subscriber, (boost::signals2::connect_position)postype);
    }
    SignalCon connect(const SigSlot& subscriber, int group, PosType postype = kBack)
    {
        return sigsource_.connect(group, subscriber, (boost::signals2::connect_position)postype);
    }         
    void trigger(const EvtCls& event)
    {
        sigsource_(event);
    }
    void disconnectAll()
    {
        sigsource_.disconnect_all_slots();
    }
    bool empty()const
    {
        return sigsource_.empty();
    }
private:
    BSig2Type sigsource_;
};

RATEL_NAMESPACE_END

#endif

