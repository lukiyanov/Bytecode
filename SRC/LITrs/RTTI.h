#pragma once
#include <string>

// ----------------------------------------------------------------
// Изъято из недр MFC и очищено от всего ненужного.
// ----------------------------------------------------------------
#define RUNTIME_CLASS(class_name)\
	(&class_name::class##class_name)

#define DECLARE_DYNAMIC(class_name)					\
public:												\
	static const CRuntimeClass class##class_name;	\
	virtual const CRuntimeClass* GetRuntimeClass();	\
	static CRuntimeObject* CreateObject();

#define IMPLEMENT_DYNAMIC(class_name)									\
	CRuntimeClass const class_name::class##class_name =					\
		{#class_name, sizeof(class_name), class_name::CreateObject};	\
	const CRuntimeClass* class_name::GetRuntimeClass()					\
		{ return &class##class_name; }									\
	CRuntimeObject* class_name::CreateObject()							\
		{ return new class_name; }

// ----------------------------------------------------------------
class CRuntimeObject;
struct CRuntimeClass;

// ----------------------------------------------------------------
struct CRuntimeClass
{
public:
	std::string		m_strClassName;
	unsigned long	m_ulClassSize;
	CRuntimeObject* (*m_pfnCreateObject)();

public:
	CRuntimeObject* CreateObject() const
	{
		return (*m_pfnCreateObject)();
	}
};

// ----------------------------------------------------------------
class CRuntimeObject
{
public:
	virtual ~CRuntimeObject();
	DECLARE_DYNAMIC(CRuntimeObject)
};

// ----------------------------------------------------------------
typedef const CRuntimeClass* RTTI;

// ----------------------------------------------------------------
