void FSMBase::Pop()
{
	owner->PopFSM();
}

Scene& UpdateBase::scene()
{
	return *(Scene*)&mempoolbase();
}
Scene& UpdateBase::scene() const
{
	return *(Scene*)&mempoolbase();
}

SceneObjBase::SceneObjBase()
	: fsmStack(mempoolbase())
{
}

template<typename T, typename ...Args>
T* SceneObjBase::CreateFSM(Args&&...args)
{
	static_assert(std::is_base_of<FSMBase, T>::value, "the T must be inherit of FSMBase.");
	auto p = scene().Create<T>(std::forward<Args>(args)...);
	return p;
}
void SceneObjBase::SetFSM(FSMBase* fsm)
{
	assert(fsm);
	assert(!deadFSM);
	deadFSM = currFSM;
	currFSM = fsm;
}
void SceneObjBase::PushFSM(FSMBase* fsm)
{
	fsmStack->Add(currFSM);
	currFSM = fsm;
}
void SceneObjBase::PopFSM()
{
	assert(!deadFSM);
	deadFSM = currFSM;
	currFSM = fsmStack->Top();
	fsmStack->Pop();
}
int SceneObjBase::Update()
{
	auto rtv = currFSM->Update();
	assert(currFSM);				// ��������ɱ
	if (deadFSM)
	{
		deadFSM->Release();
		deadFSM = nullptr;
	}
	return rtv;
}
SceneObjBase::~SceneObjBase()
{
	if (deadFSM)
	{
		deadFSM->Release();
		deadFSM = nullptr;
	}
	if (currFSM)
	{
		currFSM->Release();
		currFSM = nullptr;
	}
	while (fsmStack->dataLen)
	{
		fsmStack->Top()->Release();
		fsmStack->Pop();
	}
}
