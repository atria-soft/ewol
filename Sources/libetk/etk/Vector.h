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


namespace etk
{
	/**
	 * @brief Vector classes ...
	 *
	 * @tparam[in] MY_TYPE class type of the current element.
	 *
	 *              m_data
	 *               <------------ m_dataSize ------------>
	 *              ----------------------------------------
	 *              |    0                                 |
	 *              |--------------------------------------|
	 *              |    1                                 |
	 *              |--------------------------------------|
	 *              |    2                                 |
	 *              |--------------------------------------|
	 * m_size       |    3                                 |
	 *              |--------------------------------------|
	 *              |    x                                 |
	 *              |--------------------------------------|
	 *              |    x                                 |
	 *              |--------------------------------------|
	 *              |    x                                 |
	 *              |--------------------------------------|
	 *              |    x                                 |
	 *              |--------------------------------------|
	 *              |    x                                 |
	 *              |--------------------------------------|
	 *              |    x                                 |
	 *              |--------------------------------------|
	 * m_allocated  |    x                                 |
	 *              ----------------------------------------
	 *
	 */
	template<class MY_TYPE=int32_t> class Vector
	{
		public:
			class Iterator
			{
				// Private data :
				private:
					int32_t            m_current;      //!< curent Id on the vector
					Vector<MY_TYPE> *  m_vector;       //!< Pointer on the curent element of the vectorBin
				public:
					/**
					 * @brief Basic itarator constructor with no link with an etkVector
					 */
					Iterator():
						m_current(-1),
						m_vector(NULL)
					{
						// nothing to do ...
					}
					/**
					 * @brief Recopy constructor on a specific etkVector.
					 * @param[in] otherIterator		The Iterator that might be copy
					 */
					Iterator(const Iterator & otherIterator):
						m_current(otherIterator.m_current),
						m_vector(otherIterator.m_vector)
					{
						// nothing to do ...
					}
					/**
					 * @brief Asignation operator.
					 * @param[in] otherIterator		The Iterator that might be copy
					 * @return reference on the curent Iterator
					 */
					Iterator& operator=(const Iterator & otherIterator)
					{
						m_current = otherIterator.m_current;
						m_vector = otherIterator.m_vector;
						return *this;
					}
					/**
					 * @brief Basic destructor
					 */
					~Iterator()
					{
						m_current = -1;
						m_vector = NULL;
					}
					/**
					 * @brief basic boolean cast
					 * @return true if the element is present in the etkVector size
					 */
					operator bool ()
					{
						if(		0 <= m_current
							&&	m_current < m_vector->Size() )
						{
							return true;
						} else {
							return false;
						}
					}
					/**
					 * @brief Incremental operator
					 * @return Reference on the current iterator incremented
					 */
					Iterator& operator++ ()
					{
						if(		NULL != m_vector
							&&	m_current < m_vector->Size() )
						{
							m_current++;
						}
						return *this;
					}
					/**
					 * @brief Decremental operator
					 * @return Reference on the current iterator decremented
					 */
					Iterator& operator-- ()
					{
						if (m_current >= 0) {
							m_current--;
						}
						return *this;
					}
					/**
					 * @brief Incremental operator
					 * @return Reference on a new iterator and increment the other one
					 */
					Iterator operator++ (int32_t)
					{
						Iterator it(*this);
						++(*this);
						return it;
					}
					/**
					 * @brief Decremental operator
					 * @return Reference on a new iterator and decrement the other one
					 */
					Iterator operator-- (int32_t)
					{
						Iterator it(*this);
						--(*this);
						return it;
					}
					/**
					 * @brief Get reference on the current Element
					 * @return the reference on the current Element 
					 */
					MY_TYPE & operator-> () const
					{
						TK_CHECK_INOUT(m_current >= 0 && m_current < m_vector->Size());
						return &m_vector->Get(m_current);
					}
					/**
					 * @brief Get reference on the current Element
					 * @return the reference on the current Element 
					 */
					MY_TYPE & operator* () const
					{
						TK_CHECK_INOUT(m_current >= 0 && m_current < m_vector->Size());
						return m_vector->Get(m_current);
					}
				private:
					/**
					 * @brief 
					 *
					 * @param[in,out] ---
					 *
					 * @return ---
					 *
					 */
					Iterator(Vector<MY_TYPE> * Evb, int32_t pos):
						m_current(pos),
						m_vector(Evb)
					{
						// nothing to do ...
					}
					friend class Vector;
			};
	
		private:
			MY_TYPE *     m_data;         //!< pointer on the curetn table of Data
			int32_t       m_size;         //!< nb Element in the buffer
			int32_t       m_allocated;    //!< Current allocated size
		public:
			/**
			 * @brief Create an empty vector
			 * @param[in] count		Minimum request size of the Buffer
			 */
			Vector(int32_t count = 0):
				m_data(NULL),
				m_size(0),
				m_allocated(0)
			{
				ChangeAllocation(count);
			}
	
			/**
			 * @brief Re-copy constructor (copy all needed data)
			 * @param[in] Evb	Vector that might be copy
			 */
			Vector(const etk::Vector<MY_TYPE> & Evb)
			{
				m_allocated = Evb.m_allocated;
				m_size      = Evb.m_size;
				m_data      = NULL;
				//TK_DEBUG("USE Specific vector allocator ... Evb.m_size=" << Evb.m_size << " Evb.m_increment=" << Evb.m_increment);
				// allocate all same data
				m_data = new MY_TYPE[m_allocated];
				if (NULL==m_data) {
					TK_CRITICAL("Vector : Error in data allocation ... might nor work corectly anymore");
					return;
				}
				// Copy all data ...
				for(int32_t iii=0; iii<m_allocated; iii++) {
					// copy operator ...
					m_data[iii] = Evb.m_data[iii];
				}
			}
	
			/**
			 * @brief Destructor of the current Class
			 */
			~Vector()
			{
				if (NULL!=m_data) {
					delete [] m_data;
					m_data = NULL;
				}
				m_allocated = 0;
				m_size = 0;
			}
	
			/**
			 * @brief Re-copy operator
			 * @param[in] Evb	Vector that might be copy
			 * @return reference on the curent re-copy vector
			 */
			Vector& operator=(const etk::Vector<MY_TYPE> & Evb)
			{
				//TK_DEBUG("USE RECOPY vector ... Evb.m_size=" << Evb.m_size << " Evb.m_increment=" << Evb.m_increment);
				if( this != &Evb ) // avoid copy to itself
				{
					if (NULL!=m_data) {
						delete[] m_data;
						m_data = NULL;
					}
					// Set the new value
					m_allocated = Evb.m_allocated;
					m_size      = Evb.m_size;
					// allocate all same data
					m_data = new MY_TYPE[m_allocated];
					if (NULL==m_data) {
						TK_CRITICAL("Vector : Error in data allocation ... might nor work corectly anymore");
						return *this;
					}
					for(int32_t iii=0; iii<m_allocated; iii++) {
						// copy operator ...
						m_data[iii] = Evb.m_data[iii];
					}
				}
				// Return the curent pointer
				return *this;
			}
			
			/**
			 * @brief Add at the Last position of the Vector
			 * @param[in] item	Element to add at the end of vector
			 */
			Vector& operator+= (const etk::Vector<MY_TYPE> & Evb)	// += operator
			{
				int32_t nbElememt = Evb.Size();
				int32_t idx = m_size;
				Resize(m_size+nbElememt);
				if (m_size<=idx) {
					TK_CRITICAL("allocation error");
					return *this;
				}
				for(int32_t iii=0; iii<nbElememt; iii++) {
					// copy operator ...
					m_data[idx+iii] = Evb.m_data[iii];
				}
				// Return the curent pointer
				return *this;
			}
			
	
			/**
			 * @brief Get the number of element in the vector
			 * @return The number requested
			 */
			int32_t Size() const
			{
				return m_size;
			}
	
			/**
			 * @brief Get the number of element in the vector
			 * @return The number requested
			 */
			void ReSize(int32_t newSize, MY_TYPE& basicElement)
			{
				int32_t idx = m_size;
				Resize(newSize);
				if (m_size != newSize) {
					TK_CRITICAL("error to resize vector");
					return;
				}
				if (newSize > idx) {
					// initialize data ...
					for(int32_t iii=idx; iii<newSize; iii++) {
						m_data[iii] = basicElement;
					}
				}
			}
	
			/**
			 * @brief Get the Allocated size in the vector
			 * @return The size of allocation
			 */
			int32_t AllocatedSize() const
			{
				return m_allocated;
			}
	
			/**
			 * @brief Get a current element in the vector
			 * @param[in] pos Desired position read
			 * @return Reference on the Element
			 */
			MY_TYPE& Get(int32_t pos)
			{
				return m_data[pos];
			}
	
			/**
			 * @brief Get an copy Element an a special position
			 * @param[in] pos	Position in the vector that might be get [0..Size()]
			 * @return An reference on the copy of selected element
			 */
			MY_TYPE& operator[] (int32_t pos)
			{
				return Get(pos);
			}
	
			/**
			 * @brief Get an Element an a special position
			 * @param[in] pos	Position in the vector that might be get [0..Size()]
			 * @return An reference on the selected element
			 */
			const MY_TYPE& operator[] (int32_t pos) const
			{
				return m_data[pos];
			}
	
			/**
			 * @brief Add at the Last position of the Vector
			 * @param[in] item	Element to add at the end of vector
			 */
			void PushBack(const MY_TYPE& item)
			{
				int32_t idx = m_size;
				Resize(m_size+1);
				if (idx < m_size) {
					m_data[idx] = item;
				} else {
					TK_ERROR("Resize does not work corectly ... not added item");
				}
			}
	
			/**
			 * @brief Add at the Last position of the Vector
			 * @param[in] item	Element to add at the end of vector
			 */
			void PushBack(const MY_TYPE * item, int32_t nbElement)
			{
				if (NULL == item) {
					return;
				}
				int32_t idx = m_size;
				Resize(m_size+nbElement);
				if (idx >= m_size) {
					TK_ERROR("Resize does not work corectly ... not added item");
					return;
				}
				for (int32_t iii=0; iii<nbElement; iii++) {
					m_data[idx+iii] = item[iii];
				}
			}
	
			/**
			 * @brief Remove the last element of the vector
			 */
			void PopBack(void)
			{
				if(m_size>0) {
					Resize(m_size-1);
				}
			}
	
			/**
			 * @brief Remove all alement in the current vector
			 */
			void Clear(void)
			{
				if(m_size>0) {
					Resize(0);
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
			void Insert(int32_t pos, const MY_TYPE * item, int32_t nbElement)
			{
				if (pos>m_size) {
					TK_WARNING(" can not insert Element at this position : " << pos << " > " << m_size << " add it at the end ... ");
					PushBack(item, nbElement);
					return;
				}
				int32_t idx = m_size;
				// Request resize of the current buffer
				Resize(m_size+nbElement);
				if (idx>=m_size) {
					TK_ERROR("Resize does not work corectly ... not added item");
					return;
				}
				// move curent data (after the position)
				int32_t sizeToMove = (idx - pos);
				if ( 0 < sizeToMove) {
					for (int32_t iii=1; iii<=sizeToMove; iii++) {
						m_data[m_size-iii] = m_data[idx-iii];
					}
				}
				// affectation of all input element
				for (int32_t iii=0; iii<nbElement; iii++) {
					m_data[pos+iii] = item[iii];
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
			void Insert(int32_t pos, const MY_TYPE& item)
			{
				Insert(pos, &item, 1);
			}
			
			/**
			 * @brief Remove N element
			 *
			 * @param[in] pos Position to remove the data
			 * @param[in] nbElement number of element to remove
			 *
			 * @return ---
			 *
			 */
			void EraseLen(int32_t pos, int32_t nbElement)
			{
				if (pos>m_size) {
					TK_ERROR(" can not Erase Len Element at this position : " << pos << " > " << m_size);
					return;
				}
				if (pos+nbElement>m_size) {
					nbElement = m_size - pos;
				}
				int32_t idx = m_size;
				// move curent data
				int32_t sizeToMove = (idx - (pos+nbElement));
				if ( 0 < sizeToMove) {
					for (int32_t iii=0; iii<sizeToMove; iii++) {
						m_data[pos+iii] = m_data[pos+nbElement+iii];
					}
				}
				// Request resize of the current buffer
				Resize(m_size-nbElement);
			}
			
			/**
			 * @brief Remove one element
			 *
			 * @param[in] pos Position to remove the data
			 *
			 * @return ---
			 *
			 */
			void Erase(int32_t pos)
			{
				EraseLen(pos, 1);
			}
	
			/**
			 * @brief Remove N elements
			 *
			 * @param[in] pos Position to remove the data
			 * @param[in] posEnd Last position number
			 *
			 * @return ---
			 *
			 */
			void Erase(int32_t pos, int32_t posEnd)
			{
				if (pos>m_size) {
					TK_ERROR(" can not Erase Element at this position : " << pos << " > " << m_size);
					return;
				}
				if (posEnd>m_size) {
					posEnd = m_size;
				}
				int32_t nbElement = m_size - pos;
				int32_t tmpSize = m_size;
				// move curent data
				int32_t sizeToMove = (tmpSize - (pos+nbElement));
				if ( 0 < sizeToMove) {
					for (int32_t iii=0; iii<sizeToMove; iii++) {
						m_data[pos+iii] = m_data[pos+nbElement+iii];
					}
				}
				// Request resize of the current buffer
				Resize(m_size-nbElement);
			}
	
	
			/**
			 * @brief extract data between two point : 
			 * @param[in] posStart start position to extract data
			 * @param[in] posEnd End position to extract data
			 * @return the extracted vector
			 */
			Vector<MY_TYPE> Extract(int32_t posStart = 0, int32_t posEnd=0x7FFFFFFF)
			{
				Vector<MY_TYPE> out;
				if (posStart < 0) {
					posStart = 0;
				} else if (posStart >= Size() ) {
					return out;
				}
				if (posEnd < 0) {
					return out;
				} else if (posEnd >= Size() ) {
					posEnd = Size();
				}
				out.PushBack(&m_data[posStart], posEnd-posStart);
				return out;
			}
	
			/**
			 * @brief Get an iterator an an specific position
			 * @param[in] pos Requested position of the iterator in the vector
			 * @return The Iterator
			 */
			Iterator Position(int32_t pos)
			{
				return Iterator(this, pos);
			}
	
			/**
			 * @brief Get an Iterator on the start position of the Vector
			 * @return The Iterator
			 */
			Iterator Begin(void)
			{
				return Position(0);
			}
	
			/**
			 * @brief Get an Iterator on the end position of the Vector
			 * @return The Iterator
			 */
			Iterator End(void)
			{
				return Position( Size()-1 );
			}
	
		private:
			/**
			 * @brief Change the current size of the vector
			 * @param[in] newSize New requested size of element in the vector
			 */
			void Resize(int32_t newSize)
			{
				// Reallocate memory
				if (newSize > m_allocated) {
					ChangeAllocation(newSize);
				}
				m_size = newSize;
			}
			
			/**
			 * @brief Change the current allocation to the corect one (depend on the current size)
			 * @param[in] newSize Minimum number of element needed
			 */
			void ChangeAllocation(int32_t newSize)
			{
				// set the minimal size to 1
				if(newSize <= 0) {
					newSize = 1;
				}
				if (m_allocated<0) {
					m_allocated = 0;
				}
				int32_t requestSize = m_allocated;
				// set the size with the corect chose type : 
				if (newSize == requestSize) {
					return;
				} else if (newSize < requestSize) {
					// we did not remove data ???
				} else {
					while(newSize > requestSize) {
						if (0 == requestSize) {
							requestSize = 1;
						} else {
							requestSize = requestSize * 2;
						}
					}
				}
				// No reallocation needed :
				if (requestSize <= m_allocated) {
					return;
				}
				//TK_INFO("Change vector allocation : " << m_allocated << "==>" << requestSize);
				// check if something is allocated : 
				if (NULL == m_data) {
					// no data allocated ==> request an allocation (might be the first)
					m_data = new MY_TYPE[requestSize];
					if (NULL==m_data) {
						TK_CRITICAL("Vector : Error in data allocation request allocation:" << requestSize << "*" << (int32_t)(sizeof(MY_TYPE)) << "bytes" );
						m_allocated = 0;
						return;
					}
					// no data to copy
				} else {
					// allocate a new pool of data:
					MY_TYPE* m_dataTmp = new MY_TYPE[requestSize];
					if (NULL==m_dataTmp) {
						TK_CRITICAL("Vector : Error in data allocation request allocation:" << requestSize << "*" << (int32_t)(sizeof(MY_TYPE)) << "bytes" );
						m_allocated = 0;
						return;
					}
					// copy data in the new pool
					int32_t nbElements = etk_min(requestSize, m_allocated);
					for(int32_t iii=0; iii<nbElements; iii++) {
						m_dataTmp[iii] = m_data[iii];
					}
					// switch pointer:
					MY_TYPE* m_dataTmp2 = m_data;
					m_data = m_dataTmp;
					// remove old pool
					if (m_dataTmp2!= NULL) {
						delete [] m_dataTmp2;
					}
				}
				// set the new allocation size
				m_allocated = requestSize;
			}
	};

	/**
	 * @brief List classes ...
	 *
	 * @tparam[in] T The type of objects to store.
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
	/*
	template<class MY_CLASS> class List
	{
		
	};
	*/
}

#undef __class__
#define __class__	NULL

#endif

