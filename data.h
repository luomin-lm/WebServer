#ifndef _DATA_H
#define _DATA_H
// 数据存储结构 与 Iter迭代器

#ifndef NULL
#define NULL 0
#endif

namespace ns_dsa{
	template<typename T>
	struct Datas
	{
		T m_data;
		Datas<T>* next;
	};
	template<typename T>
	class Iter
	{
	public:
		Iter(){
			m_it = NULL;
		}
		Iter(Datas<T>* it){
			m_it = it;
		}
		void operator ++ (){
			if (m_it != NULL){
				m_it = m_it->next;
			}
		}
		void operator =(const Iter<T>& obj){
			this->m_it = obj.m_it;
		}
		int operator ==(const Iter<T>& obj){
			if (this->m_it == obj.m_it){
				return 1;
			}
			return 0;
		}
		int operator !=(const Iter<T>& obj){
			if (this->m_it == obj.m_it){
				return 0;
			}
			return 1;
		}
		T operator *(){
			return m_it->m_data;
		}

	private:
		Datas<T>* m_it;
	};

	template<typename T1, typename T2>
	struct MulData{
		T1 first;
		T2 second;
	};
	template<typename T1, typename T2>
	class MulIter
	{
	public:
		MulIter(){
			m_it = NULL;
		}
		MulIter(Datas<MulData<T1, T2> >* it){
			m_it = it;
		}
		void operator ++ (){
			if (m_it != NULL){
				m_it = m_it->next;
			}
		}
		void operator =(const MulIter<T1, T2>& obj){
			this->m_it = obj.m_it;
		}
		int operator ==(const MulIter<T1, T2>& obj){
			if (this->m_it == obj.m_it){
				return 1;
			}
			return 0;
		}
		int operator !=(const MulIter<T1, T2>& obj){
			if (this->m_it == obj.m_it){
				return 0;
			}
			return 1;
		}
		MulData<T1, T2> operator *(){
			return m_it->m_data;
		}

	private:
		Datas<MulData<T1, T2> >* m_it;
	};
}

#endif

