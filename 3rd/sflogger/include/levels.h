#ifndef __LEVELS_H__
#define __LEVELS_H__

#include "level.h"
#include "uncopyable.hpp"
#include <map>

SFLOGGER_NAMESPACE_BEGIN

#define LEVEL_DECL(LevelClass, Desp, Val) \
struct LevelClass : public DefLevel{ \
    const xCharT* repr() const { return _X(#Desp); } \
    Level* clone()const { return new LevelClass(*this); } \
    LevelClass() : DefLevel(Val) {} \
    ~LevelClass(){} \
};

LEVEL_DECL(DebugLevel, DebugLevel, 1)
SFLOGGER_API const Level* debuglevel();
LEVEL_DECL(InfoLevel, InfoLevel, 2)
SFLOGGER_API const Level* infolevel();
LEVEL_DECL(WarnLevel, WarnLevel, 3)
SFLOGGER_API const Level* warnlevel();
LEVEL_DECL(ErrLevel, ErrLevel, 4)
SFLOGGER_API const Level* errlevel();
LEVEL_DECL(FatalLevel, FatalLevel, 5)
SFLOGGER_API const Level* fatallevel();

struct SFLOGGER_API LevelObjReflection : public Uncopyable
{
    static LevelObjReflection& GetInstance();
    void registerLevelClass(Level& obj);
    Level* bornLevelObject(const std::xStrT& level_clsname);
private:
    LevelObjReflection();
    typedef std::map<std::xStrT, Level*> xStr2LevelDictType;
    xStr2LevelDictType level_reflection_dict_;
};

#define REGISTER_LEVEL_CLASS(LevelClass) LevelObjReflection::GetInstance().registerLevelClass(*new LevelClass())

NAMESPACE_END

#endif