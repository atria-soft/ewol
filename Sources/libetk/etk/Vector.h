/**
 *******************************************************************************
 * @file etk/Vector.h
 * @brief Ewol Tool Kit : Basic etk::Vector (template)
 * @author Edouard DUPIN
 * @date 07/04/2011
 * @par Project
 * Ewol TK
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __ETK_VECTOR_H__
#define __ETK_VECTOR_H__

#include <etk/Types.h>
#include <etk/DebugInternal.h>
#include <etk/Memory.h>

#undef __class__
#define __class__	"etk::Vector"

/**
 * @brief etkVector classes ...
 *
 * @tparam[in] T The type of objects to store.
 * @tparam[in] INC Incrementation mode (0 : Exponential to 200 and increment by stemp of 200)
 *
 * @todo : Need to add : popBack / Assign / Insert / Erase / Swap / Clear
 *
 *          m_data
 *         ----------         |-----------------------|                             
 *         |    0   |-------->|  Class Data           |                             
 *         |--------|         |-----------------------|                             
 *         |    1   |----|                                                          
 *         |--------|    |                                                          
 *         |    2   |====|==============|   |-----------------------|               
 *         |--------|    |              --->|  Class Data           |               
 * m_count |    3   |-|  |                  |-----------------------|               
 *         |--------| |  |                                                          
 *         |    x   | |  |        |-----------------------|                         
 *         |--------| |  -------->|  Class Data           |                         
 *         |    x   | |           |-----------------------|                         
 *         |--------| |                                                             
 *         |    x   | |                                                             
 *         |--------| |                     |-----------------------|               
 *         |    x   | --------------------->|  Class Data           |               
 *         |--------|                       |-----------------------|               
 *         |    x   |                                                               
 *         |--------|                                                               
 *         |    x   |                                                               
 *         |--------|                                                               
 * m_size  |    x   |                                                               
 *         ----------                                                               
 *
 */
namespace etk
{

template<class T, int32_t INC=0> class Vector
{
	public:
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	Vector(int count = 0):
		m_data(NULL),
		m_count(0),
		m_size(0)
	{
		Resize(count);
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	Vector(const etk::Vector<T> & myVector):
		m_size(myVector.m_size),
		m_count(myVector.m_count),
		m_data(NULL)
	{
		int32_t i;
		ETK_MALLOC_CAST(m_data, m_size, T, T*);//reinterpret_cast<T*>);
		for(i=0; i<m_count; i++) {
			new (&m_data[i]) T(myVector[i]);
		}
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	~Vector()
	{
		Destroy();
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	Vector& operator=(const etk::Vector<T> & etkVector)
	{
		int32_t i;
		this->~etkVector(); 
		m_size = etkVector.m_size;
		m_count = etkVector.m_count;
		ETK_MALLOC_CAST(m_data, m_size, T, T*);//reinterpret_cast<T*>);
		for(i=0; i<m_count; i++) {
			new (&m_data[i]) T(etkVector[i]);
		}
		return *this;
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	int32_t Size()
	{
		return m_count;
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	T& Get(int32_t pos)
	{
		return m_data[pos];
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	T& operator[] (int32_t pos)
	{
		return Get(pos);
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	const T& operator[] (int32_t pos) const
	{
		return m_data[pos];
	}
	
	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	int IndexOf(const T * item) const
	{
		int32_t res = item - m_data;
		if(		0 > res
			||	res >= Size())
		{
			return -1;
		} else {
			return res;
		}
	}

	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	void PushBack(const T& item)
	{
		int32_t idx = Size();
		Resize(idx+1);
		Get(idx) = item;
	}

	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	void Resize(int32_t count)
	{
		int32_t i;
		// Reallocate memory
		if (count > m_size) {
			ChangeAllocation(count);
		}

		// Remove deprecated element
		for(i=count; i<m_count; i++) {
			m_data[i].~T();
		}

		// Create nex item
		for(i=m_count;i<count;i++) {
			new (&m_data[i]) T();
		}

		m_count = count;
	}

	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	void ChangeAllocation(int count)
	{
		if (count > m_size) {
			// generate new size
			while(count > m_size) {
				if (INC) {
					m_size = (m_size + INC);
				} else if (m_size==0) {
					m_size = 1;
				} else {
					m_size = m_size * 2;
				}
			}
			// Allocate the curent element
			T * data = NULL;
			ETK_MALLOC_CAST(data, m_size, T, T*);//reinterpret_cast<T*>);
			for(int i=0; i<m_count; i++) {
				new (&data[i]) T(m_data[i]);
			}
			Destroy();
			m_data = data;
		}
	}

private:
	T		* m_data;		//!< pointer on the current Data
	int32_t	  m_count;		//!< number of element
	int32_t	  m_size;		//!< current allocated size

	/**
	 * @brief 
	 *
	 * @param[in,out] ---
	 *
	 * @return ---
	 *
	 */
	void Destroy()
	{
		for(int i=0; i<m_count; i++) {
			m_data[i].~T();
		}
		if (m_data) {
			ETK_FREE(m_data);
		}
	}
};

}

#undef __class__
#define __class__	NULL

#endif

