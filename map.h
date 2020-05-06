#ifndef _CLM_MAP_H
#define _CLM_MAP_H
// Map映射表

#include "data.h"

namespace ns_dsa{
	template<typename T1, typename T2>
	class CLMMap
	{
	public:
		CLMMap(){
			m_datas = new Datas<MulData<T1, T2> >;
			m_datas->next = NULL;
			m_datas->m_data;
		}
		CLMMap(const CLMMap& obj){
			throw "copy error";
		}
		~CLMMap(){
			while (!IsEmity()){
				PopFront();
			}
			delete m_datas;
		}

	public:
		// 首部插入
		void PushFront(const T1& t1, const T2& t2){
			Datas<MulData<T1, T2> >* d = new Datas<MulData<T1, T2> >;
			d->m_data.first = t1;
			d->m_data.second = t2;
			d->next = m_datas->next;

			m_datas->next = d;
		}
		// 尾部插入
		void PushBack(const T1& t1, const T2& t2){
			Datas<MulData<T1, T2> >* p = m_datas;
			Datas<MulData<T1, T2> >* d = new Datas<MulData<T1, T2> >;
			d->m_data.first = t1;
			d->m_data.second = t2;
			d->next = NULL;

			while (NULL != (p->next)){
				p = p->next;
			}

			p->next = d;
		}
		// 首部删除,返回删除的值
		MulData<T1, T2> PopFront(){
			Datas<MulData<T1, T2> >* p = m_datas->next;
			MulData<T1, T2> t;

			if (p != NULL){
				m_datas->next = p->next;
				t = p->m_data;
				delete p;
			}

			return t;
		}
		// 尾部删除,返回删除的值
		MulData<T1, T2> PopBack(){
			Datas<MulData<T1, T2> >* p = m_datas;
			Datas<MulData<T1, T2> >* end;
			MulData<T1, T2> t;

			if (NULL == p->next){
				return t;
			}

			while (NULL != (p->next->next)){
				p = p->next;
			}
			end = p->next;
			t = end->m_data;
			p->next = NULL;
			delete end;

			return t;
		}
		// 是否为空链表
		int IsEmity(){
			if (NULL == m_datas->next){
				return 1;
			}
			return 0;
		}

		// 查找
		T1 FindKey(const T2& value){
			Datas<MulData<T1, T2> >* p = m_datas->next;

			while (NULL != p){

				if ((p->m_data).second == value){
					return (p->m_data).first;
				}
				p = p->next;
			}
			return 0;
		}
		T1 FindValue(const T2& key){
			Datas<MulData<T1, T2> >* p = m_datas->next;

			while (NULL != p){

				if ((p->m_data).first == key){
					return (p->m_data).second;
				}
				p = p->next;
			}
			return 0;
		}
		// 删除
		int PopValue(const T1& key){
			Datas<MulData<T1, T2> > *p1 = m_datas;
			Datas<MulData<T1, T2> > *p2 = m_datas->next;
			Datas<MulData<T1, T2> > *pd;

			while (p2 != NULL){
				if (key == p2->m_data.first){
					pd = p2;
					p1->next = pd->next;
					delete pd;
					return 1;
				}

				p1 = p2;
				p2 = p2->next;
			}

			return 0;
		}

		MulIter<T1, T2> begin(){
			MulIter<T1, T2> it(m_datas->next);
			return it;
		}
		MulIter<T1, T2> end(){
			MulIter<T1, T2> it(NULL);
			return it;
		}

	private:
		Datas<MulData<T1, T2> > *m_datas;
	};
}

#endif

