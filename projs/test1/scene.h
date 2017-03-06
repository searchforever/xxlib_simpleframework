#pragma once
#include "xx_list.h"

/*

MP �������뺬����������������

typedef xx::MemPool<............. xx::List<FSMBase*>, xx::List<SceneObjBase*, true> ................> MP;

*/

// �� Update �麯������Ļ���
struct UpdateBase : xx::MPObject
{
	// ���ط� 0 ��ʾ��ɱ. ����ͨ����������������, ����ͨ����ʾ����
	virtual int Update() = 0;
};

struct SceneBase;
struct SceneObjBase;

// ״̬������. ֻ���� SceneObjBase �ĺ�����������ɱ��. ������ֱ�� Release
struct FSMBase : UpdateBase
{
	SceneObjBase* owner;
};

// �������ӵĻ���( �������� ). ֻ���� SceneBase �ĺ�����������ɱ��. ������ֱ�� Release
struct SceneObjBase : UpdateBase
{
	// ָ�򳡾�����
	SceneBase* sceneBase;

	// λ�ڳ����������е��±�( for ����ʽ��ɾ )
	uint32_t sceneObjsIndex;

	// todo: �ṩһ�������������ϸ� FSM ��ִ�н��( ��������, �����˳�����, ����¼�����֮�� )
	xx::List<FSMBase*>* fsmStack;
	FSMBase* currFSM = nullptr;
	FSMBase* deadFSM = nullptr;		// �ӳ�ɱ��

	SceneObjBase();
	~SceneObjBase();

	template<typename T, typename ...Args>
	T* CreateFSM(Args&&...args);
	void SetFSM(FSMBase* fsm);
	void PushFSM(FSMBase* fsm);
	void PopFSM();
	virtual int Update() override;
};

// ��������( �������ڴ��, ��������, LUA State )
struct SceneBase : UpdateBase
{
	// ���� SceneObjBase* ��Ψһ��������. ֻ���� SceneBase �� Create, Release ������
	xx::List<SceneObjBase*, true>* objs;

	SceneBase();
	~SceneBase();

	template<typename T, typename...Args>
	xx::MPtr<T> Create(Args&&...args);

	template<typename T, typename...Args>
	void CreateTo(xx::MPtr<T>& tar, Args&&...args);

	// ��Ŀ�����������, �� objs �Ƴ�. ʹ�ý����Ƴ���, �����Ƿ������������Ϊ
	bool Release(SceneObjBase* p);
};