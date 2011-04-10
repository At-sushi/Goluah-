

/*============================================================================

	�^�X�N�Ǘ�(?)

==============================================================================*/

#include "stdafx.h"
#include "global.h"
#include "task.h"

CTaskManager::CTaskManager()
{
	exNext = NULL;
}

void CTaskManager::Destroy()
{
	TaskList::iterator i,ied;

	//�ʏ�^�X�NTerminate
	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		(*i)->Terminate() ;
		delete (*i);
	}
	tasks.clear();

	//�o�b�N�O���E���h�^�X�NTerminate
	i = bg_tasks.begin();
	ied = bg_tasks.end();
	for(;i!=ied;i++){
		(*i)->Terminate() ;
		delete (*i);
	}
	bg_tasks.clear();

	//�r���^�X�NTerminate
	while(ex_stack.size()!=0){
		ex_stack.top()->Terminate();
		delete(ex_stack.top());
		ex_stack.pop();
	}
}


void CTaskManager::AddTask(CTaskBase *newTask)
{
	if(newTask->GetID() != 0){
		RemoveTaskByID(newTask->GetID());
	}

	CBackgroundTaskBase *pbgt = dynamic_cast<CBackgroundTaskBase*>(newTask);
	if(pbgt){
		//�풓�^�X�N�Ƃ���Add
		bg_tasks.push_back(pbgt);
		pbgt->Initialize();
		return;
	}

	CExclusiveTaskBase *pext = dynamic_cast<CExclusiveTaskBase*>(newTask);
	if(!pext){
		//�ʏ�^�X�N�Ƃ���Add
		tasks.push_back(newTask);
		newTask->Initialize();
		return;
	}

	//�r���^�X�N�Ƃ���Add
	//Execute����������Ȃ��̂ŁA�|�C���^�ۑ��̂�
	if(exNext){
		gbl.ods("��ALERT�� �r���^�X�N��2�ȏ�Add���ꂽ : %s / %s",
			typeid(*exNext).name() , typeid(*newTask).name());
	}
	exNext = pext;
	g_log2file.Flush();//���O�����o��
}

void CTaskManager::Execute(DWORD time)
{
	TaskList::iterator i,ied;
	std::list<TaskList::iterator> deleteList;
	std::list<TaskList::iterator>::iterator idl,idl_ed;
	CTaskBase *delTgt;

	#ifdef ARRAYBOUNDARY_DEBUG
	if(!AfxCheckMemory()){
		g_system.LogErr("AfxCheckMemory() failed");
		g_system.NotifyExcption();
		return;
	}
	#endif

	//�ʏ�^�X�NExecute
	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		#ifdef _CATCH_WHILE_EXEC
		try{
		#endif
			if( (*i)->Execute(time) == FALSE )
			{
				deleteList.push_back(i);
			}
		#ifdef _CATCH_WHILE_EXEC
		}catch(...){
			if(*i==NULL)g_system.Log("catch while execute1 : NULL",SYSLOG_ERROR);
			else g_system.LogErr("catch while execute1 : %X , %s",*i,typeid(**i).name());
			g_system.NotifyExcption();
			break;
		}
		#endif
	}
	//�ʏ�^�X�N��FALSE��Ԃ������̂�����
	if(deleteList.size()!=0){
		idl = deleteList.begin();
		idl_ed = deleteList.end();
		for(;idl!=idl_ed;idl++){
			i = *idl;
			delTgt = *i;
			delTgt->Terminate();
			delete delTgt;
			tasks.erase(i);
		}
		deleteList.clear();
	}

	//�풓�^�X�NExecute
	i = bg_tasks.begin();
	ied = bg_tasks.end();
	for(;i!=ied;i++)
	{
		#ifdef _CATCH_WHILE_EXEC
		try{
		#endif
		if( (*i)->Execute(time) == FALSE ){
			deleteList.push_back(i);
		}
		#ifdef _CATCH_WHILE_EXEC
		}catch(...){
			if(*i==NULL)g_system.Log("catch while execute2 : NULL",SYSLOG_ERROR);
			else g_system.LogErr("catch while execute2 : %X %s",*i,typeid(**i).name());
			g_system.NotifyExcption();
		}
		#endif
	}
	//�풓�^�X�N��FALSE��Ԃ������̂�����
	if(deleteList.size()!=0){
		idl = deleteList.begin();
		idl_ed = deleteList.end();
		for(;idl!=idl_ed;idl++){
			i = *idl;
			delTgt = *i;
			delTgt->Terminate();
			delete delTgt;
			bg_tasks.erase(i);
		}
		deleteList.clear();
	}
	
	CExclusiveTaskBase *exTsk;

	BOOL ex_ret;

	//�r���^�X�N�Atop�̂�Execute
	if(ex_stack.size()!=0){
		exTsk = ex_stack.top();
		#ifdef _CATCH_WHILE_EXEC
		try{
		#endif
		ex_ret = TRUE;
		ex_ret = exTsk->Execute(time);
		#ifdef _CATCH_WHILE_EXEC
		}catch(...){
			if(ex_stack.top()==NULL)g_system.Log("catch while execute3 : NULL",SYSLOG_ERROR);
			else g_system.LogErr("catch while execute3 : %X %s",ex_stack.top(),typeid(*ex_stack.top()).name());
			g_system.NotifyExcption();
		}
		#endif

		if(!ex_ret)
		{	
			g_log2file.Flush();//���O�����o��
			if(!exNext){
				//���ݔr���^�X�N�̕ύX

				//�L�[���O�N���A
				g_input.ClearKeyLog();

				#ifdef _CATCH_WHILE_EXEC
				try{
				#endif

				//�ʏ�^�X�N��S�Ĕj������
				i = tasks.begin();
				ied = tasks.end();
				for(;i!=ied;i++){
					delTgt = (*i);
					delTgt->Terminate();
					delete delTgt;
				}
				tasks.clear();

				#ifdef _CATCH_WHILE_EXEC
				}catch(...){
					if( (*i) ==NULL)g_system.Log("catch while terminate1 : NULL",SYSLOG_ERROR);
					else g_system.LogErr("catch while terminate1 : %X %s",(*i),typeid(*(*i)).name());
					g_system.NotifyExcption();
				}
				#endif

				DWORD prvID;

				#ifdef _CATCH_WHILE_EXEC
				try{
				#endif

				//���ݔr���^�X�N�̔j��
				prvID = exTsk->GetID();
				exTsk->Terminate();
				delete exTsk;
				exTsk = NULL;
				ex_stack.pop();

				#ifdef _CATCH_WHILE_EXEC
				}catch(...){
					if(exTsk==NULL)g_system.Log("catch while terminate2 : NULL",SYSLOG_ERROR);
					else g_system.LogErr("catch while terminate : %X %s",exTsk,typeid(*exTsk).name());
					g_system.NotifyExcption();
				}
				#endif


				#ifdef _CATCH_WHILE_EXEC
				try{
				#endif

				//���̔r���^�X�N��Activate����
				if(ex_stack.size()==0)return;
				exTsk = ex_stack.top();
				exTsk->Activate(prvID);

				#ifdef _CATCH_WHILE_EXEC
				}catch(...){
					if(exTsk==NULL)g_system.Log("catch while activate : NULL",SYSLOG_ERROR);
					else g_system.LogErr("catch while activate : %X %s",exTsk,typeid(*exTsk).name());
					g_system.NotifyExcption();
				}
				#endif

				
				return;
			}
		}
	}

	if(exNext){
		//�L�[���O�N���A
		g_input.ClearKeyLog();

		//�ʏ�^�X�N��S�Ĕj������
		CleanupAllSubTasks();

		//���ݔr���^�X�N��Inactivate
		if(ex_stack.size()!=0){
			exTsk = ex_stack.top();
			if(! exTsk->Inactivate(exNext->GetID()) ){
				exTsk->Terminate();
				delete exTsk;
				ex_stack.pop();
			}
		}

		//Add���ꂽ�^�X�N��Initialize���ē˂�����
		ex_stack.push(exNext);
		exNext->Initialize();

		exNext = NULL;
	}
}


void CTaskManager::Draw()
{
	std::vector<CTaskBase*> tmplist;
	TaskList::iterator i,ied;

	//�ʏ�^�X�NDraw
	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		if((*i)->GetDrawPriority() >= 0){
			tmplist.push_back(*i);
		}
	}

	//�o�b�N�O���E���h�^�X�NDraw
	i = bg_tasks.begin();
	ied = bg_tasks.end();
	for(;i!=ied;i++){
		if((*i)->GetDrawPriority() >= 0){
			tmplist.push_back(*i);
		}
	}

	//�r���^�X�NDraw
	if(ex_stack.size()!=0){
		if(ex_stack.top()->GetDrawPriority() >= 0){
			tmplist.push_back( ex_stack.top() );
		}
	}

	std::sort( tmplist.begin() , tmplist.end() , CTaskBase::CompByDrawPriority );//�`��v���C�I���e�B���Ƀ\�[�g

	//�`��
	std::vector<CTaskBase*>::iterator iv = tmplist.begin();
	std::vector<CTaskBase*>::iterator iedv = tmplist.end();
	for(;iv!=iedv;iv++)
	{
		#ifdef _CATCH_WHILE_RENDER
		try{
		#endif
		(*iv)->Draw() ;
		#ifdef _CATCH_WHILE_RENDER
		}catch(...){
			g_system.LogErr("catch while draw : %X %s",*iv,typeid(**iv).name());
			g_system.NotifyExcption();
		}
		#endif
	}

}

void CTaskManager::WndMessage(HWND hWnd,UINT msg, WPARAM wparam, LPARAM lparam)
{
	TaskList::iterator i,ied;

	//�ʏ�^�X�N
	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		(*i)->WndMessage(hWnd,msg,wparam,lparam);
	}

	//�o�b�N�O���E���h�^�X�N
	i = bg_tasks.begin();
	ied = bg_tasks.end();
	for(;i!=ied;i++){
		(*i)->WndMessage(hWnd,msg,wparam,lparam);
	}

	//�r���^�X�Na
	if(ex_stack.size()==0)return;
	ex_stack.top()->WndMessage(hWnd,msg,wparam,lparam);
}

void CTaskManager::RemoveTaskByID(DWORD id)
{
	TaskList::iterator i,ied;

	//�ʏ�^�X�N���`�F�b�N
	if (!tasks.empty())
	{
		i = tasks.begin();
		ied = tasks.end();
		for(;i!=ied;i++){
			if(id == (*i)->GetID() ){
				(*i)->Terminate();
				delete (*i);
				tasks.erase(i);
				return;
			}
		}
	}

	//�o�b�N�O���E���h�^�X�NTerminate
	if (!bg_tasks.empty())
	{
		i = bg_tasks.begin();
		ied = bg_tasks.end();
		for(;i!=ied;i++){
			if(id == (*i)->GetID() ){
				(*i)->Terminate();
				delete (*i);
				bg_tasks.erase(i);
				return;
			}
		}
	}
}


//�ŏ�ʂɂ���G�N�X�N���[�V�u�^�X�N���Q�g
CExclusiveTaskBase* CTaskManager::GetTopExclusiveTask()
{
	if(ex_stack.size()==0)return NULL;
	return ex_stack.top();
}

//�w��ID�̔r���^�X�N�܂�Terminate/pop����
void CTaskManager::ReturnExclusiveTaskByID(DWORD id)
{
	BOOL act = FALSE;
	DWORD previd = 0;

	while(ex_stack.size() != 0){
		CExclusiveTaskBase *task = ex_stack.top();
		if(task->GetID()==id){
			if(act){
				task->Activate(previd);
			}
			return;
		}
		else{
			previd = task->GetID();
			act = TRUE;
			CleanupAllSubTasks();
			task->Terminate();
			delete task;
			ex_stack.pop();
		}
	}
}

//�ʏ�^�X�N��S�Ĕj������
void CTaskManager::CleanupAllSubTasks()
{
	CTaskBase *delTgt;
	TaskList::iterator i,ied;

	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		delTgt = (*i);
		delTgt->Terminate();
		delete delTgt;
	}
	tasks.clear();
}


//�f�o�b�O�E�^�X�N�ꗗ�\��
void CTaskManager::DebugOutputTaskList()
{
	gbl.ods("\n\n��CTaskManager::DebugOutputTaskList() - start");

	TaskList::iterator i,ied;

	gbl.ods("���ʏ�^�X�N�ꗗ��");
	//�ʏ�^�X�N
	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		gbl.ods(typeid(**i).name());
	}

	gbl.ods("���풓�^�X�N�ꗗ��");
	//�o�b�N�O���E���h�^�X�N
	i = bg_tasks.begin();
	ied = bg_tasks.end();
	for(;i!=ied;i++){
		gbl.ods(typeid(**i).name());
	}

	//�r���^�X�N	
	gbl.ods("\n");
	gbl.ods("�����݂̃^�X�N�F");
	if(ex_stack.empty())
		gbl.ods("�Ȃ�");
	else
		gbl.ods(typeid(*ex_stack.top()).name());


	gbl.ods("\n\n��CTaskManager::DebugOutputTaskList() - end\n\n");
}


//�w��ID�̏풓�^�X�N�擾
CBackgroundTaskBase* CTaskManager::FindBGTask(DWORD id)
{
	TaskList::iterator i,ied;

	i = bg_tasks.begin();
	ied = bg_tasks.end();
	for(;i!=ied;i++){
		if((*i)->GetID()==id){//�n�P�[�\
			return dynamic_cast<CBackgroundTaskBase*>( *i );
		}
	}
	return NULL;
}

//�w��ID�̒ʏ�^�X�N�擾
CTaskBase* CTaskManager::FindTask(DWORD id)
{
	TaskList::iterator i,ied;

	i = tasks.begin();
	ied = tasks.end();
	for(;i!=ied;i++){
		if((*i)->GetID()==id){//�n�P�[�\
			return ( *i );
		}
	}
	return NULL;
}

//�S���Ȃ��Ȃ����������A��΂�������
BOOL CTaskManager::ExEmpty()
{
	return (ex_stack.size()==0) ? TRUE : FALSE;
}