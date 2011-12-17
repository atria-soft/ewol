/**
 *******************************************************************************
 * @file etk/VectorType.h
 * @brief Ewol Tool Kit : Basic VectorType for direct data insertion (template)
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

#ifndef __ETK_VECTOR_TYPE_H__
#define __ETK_VECTOR_TYPE_H__

#include <etk/Types.h>
#include <etk/DebugInternal.h>
#include <etk/Memory.h>

#undef __class__
#define __class__	"etk::VectorType"

/**
 * @brief VectorType classes ...
 *
 * @tparam[in] SIZE Size of the current element.
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

namespace etk
{

template<typename MY_TYPE=int32_t> class VectorType
{
	public:
		class Iterator
		{
			// Private data :
			private:
				int32_t                     m_current;          //!< curent Id on the vector
				VectorType<MY_TYPE> *       m_VectorType;       //!< Pointer on the curent element of the vectorBin
			public:
				/**
				 * @brief Basic itarator constructor with no link with an etkVector
				 */
				Iterator():
					m_current(-1),
					m_VectorType(NULL)
				{
					// nothing to do ...
				}
				/**
				 * @brief Recopy constructor on a specific etkVector.
				 * @param[in] otherIterator		The Iterator that might be copy
				 */
				Iterator(const Iterator & otherIterator):
					m_current(otherIterator.m_current),
					m_VectorType(otherIterator.m_VectorType)
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
					m_VectorType = otherIterator.m_VectorType;
					return *this;
				}
				/**
				 * @brief Basic destructor
				 */
				~Iterator()
				{
					m_current = -1;
					m_VectorType = NULL;
				}
				/**
				 * @brief basic boolean cast
				 * @return true if the element is present in the etkVector size
				 */
				operator bool ()
				{
					if(		0 <= m_current
						&&	m_current < m_VectorType->Size() )
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
					if(		NULL != m_VectorType
						&&	m_current < m_VectorType->Size() )
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
					TK_CHECK_INOUT(m_current >= 0 && m_current < m_VectorType->Size());
					return &m_VectorType->Get(m_current);
				}
				/**
				 * @brief Get reference on the current Element
				 * @return the reference on the current Element 
				 */
				MY_TYPE & operator* () const
				{
					TK_CHECK_INOUT(m_current >= 0 && m_current < m_VectorType->Size());
					return m_VectorType->Get(m_current);
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
				Iterator(VectorType<MY_TYPE> * Evb, int32_t pos):
					m_current(pos),
					m_VectorType(Evb)
				{
					// nothing to do ...
				}
				friend class VectorType;
		};

	private:
		MY_TYPE *   m_data;         //!< pointer on the curetn table of Data
		int32_t     m_size;         //!< nb Element in the buffer
		int32_t     m_allocated;    //!< Current allocated size
		int32_t     m_increment;    //!< methode of increment
	public:
		/**
		 * @brief Create an empty vector
		 * @param[in] count		Minimum request size of the Buffer
		 */
		VectorType(int32_t count = 0):
			m_data(NULL),
			m_size(0),
			m_allocated(0),
			m_increment(1)
		{
			ChangeAllocation(count);
		}

		/**
		 * @brief Re-copy constructor (copy all needed data)
		 * @param[in] Evb	Vector that might be copy
		 */
		VectorType(const etk::VectorType<MY_TYPE> & Evb)
		{
			m_allocated = Evb.m_allocated;
			m_size      = Evb.m_size;
			m_increment = Evb.m_increment;
			m_data      = NULL;
			//TK_DEBUG("USE Specific vector allocator ... Evb.m_size=" << Evb.m_size << " Evb.m_increment=" << Evb.m_increment);
			// allocate all same data
			ETK_MALLOC(m_data, m_allocated, MY_TYPE);
			TK_ASSERT(NULL!=m_data, "Error in data allocation");
			// Copy all data ...
			memcpy(m_data, Evb.m_data, m_allocated * sizeof(MY_TYPE) );
		}

		/**
		 * @brief Destructor of the current Class
		 */
		~VectorType()
		{
			if (NULL!=m_data) {
				ETK_FREE(m_data);
				m_data = NULL;
				m_allocated = 0;
				m_size = 0;
				m_increment = 0;
			}
		}

		/**
		 * @brief Re-copy operator
		 * @param[in] Evb	Vector that might be copy
		 * @return reference on the curent re-copy vector
		 */
		VectorType& operator=(const etk::VectorType<MY_TYPE> & Evb)
		{
			//TK_DEBUG("USE RECOPY vector ... Evb.m_size=" << Evb.m_size << " Evb.m_increment=" << Evb.m_increment);
			if( this != &Evb ) // avoid copy to itself
			{
				if (NULL!=m_data) {
					ETK_FREE(m_data);
					m_data = NULL;
				}
				// Set the new value
				m_allocated = Evb.m_allocated;
				m_size      = Evb.m_size;
				m_increment = Evb.m_increment;
				// allocate all same data
				ETK_MALLOC(m_data, m_allocated, MY_TYPE);
				TK_ASSERT(NULL!=m_data, "Error in data allocation");
				// Copy all data ...
				memcpy(m_data, Evb.m_data, m_allocated * sizeof(MY_TYPE) );
			}
			// Return the curent pointer
			return *this;
		}

		/**
		 * @brief Add at the Last position of the Vector
		 * @param[in] item	Element to add at the end of vector
		 */
		VectorType& operator+= (const etk::VectorType<MY_TYPE> & Evb)	// += operator
		{
			int32_t nbElememt = Evb.Size();
			int32_t idx = m_size;
			Resize(m_size+nbElememt);
			memcpy(&m_data[idx], &Evb.m_data[0], nbElememt*sizeof(MY_TYPE) );
			// Return the curent pointer
			return *this;
		}

		/**
		 * @brief Set increment mode of this vector (default it match corectly with the number of element inside)
		 * @param[in] newIncrementNumber methode requested
		 */
		void SetIncrement(int32_t newIncrementNumber)
		{
			m_increment = newIncrementNumber;
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
			m_data[idx] = item;
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
			memcpy(&m_data[idx], item, nbElement*sizeof(MY_TYPE) );
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
		void Insert(int32_t pos, const MY_TYPE& item)
		{
			if (pos>m_size) {
				TK_ERROR(" can not insert Element at this position : " << pos << " > " << m_size<< " add it at the end ... ");
				PushBack(item);
				return;
			}
			int32_t tmpSize = m_size;
			// Request resize of the current buffer
			Resize(m_size+1);
			// move curent data
			memmove((m_data + pos + 1), (m_data + pos), (tmpSize - pos)*sizeof(MY_TYPE) );
			// affectation of the current element
			m_data[pos] = item;
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
			int32_t tmpSize = m_size;
			// Request resize of the current buffer
			Resize(m_size+nbElement);
			// move curent data (after the position)
			memmove((m_data + pos + nbElement), (m_data + pos), (tmpSize - pos)*sizeof(MY_TYPE) );
			// affectation of all input element
			memcpy(&m_data[pos], item, nbElement*sizeof(MY_TYPE) );
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
			if (pos>m_size) {
				TK_ERROR(" can not Erase Element at this position : " << pos << " > " << m_size);
				return;
			}
			int32_t tmpSize = m_size;
			// move curent data
			memmove((m_data + pos), (m_data + pos + 1), (tmpSize - (pos+1))*sizeof(MY_TYPE) );
			// Request resize of the current buffer
			Resize(m_size-1);
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
			memmove((m_data + pos), (m_data + pos + nbElement), (tmpSize - (pos+nbElement))*sizeof(MY_TYPE) );
			// Request resize of the current buffer
			Resize(m_size-nbElement);
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
			int32_t tmpSize = m_size;
			// move curent data
			memmove((m_data + pos), (m_data + pos + nbElement), (tmpSize - (pos+nbElement))*sizeof(MY_TYPE) );
			// Request resize of the current buffer
			Resize(m_size-nbElement);
		}

		/**
		 * @brief extract data between two point : 
		 * @param[in] posStart start position to extract data
		 * @param[in] posEnd End position to extract data
		 * @return the extracted vector
		 */
		VectorType Extract(int32_t posStart = 0, int32_t posEnd=0x7FFFFFFF)
		{
			VectorType<MY_TYPE> out;
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
		 * @brief Set the minimum allocation in memory for the curent vector ==> reallocate the 
		 *     buffer to fit exactly the mumber of element needed
		 */
		void Fit(void)
		{
			if (m_size > m_allocated) {
				// Reallocate the curent data to the correct size ...
				ETK_REALLOC(m_data, m_size, MY_TYPE);
			}
			// Check result with assert : 
			TK_ASSERT(NULL!=m_data, "Error in data Fitting");
			m_allocated = m_size;
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
			int32_t requestSize = m_allocated;
			// set the size with the corect chose type : 
			if (newSize == m_allocated) {
				return;
			} else if (newSize < requestSize) {
				// down the size of the vector:
				if (0==m_increment) {
					// never down size...
				} else {
					int32_t devide = m_increment;
					if (devide == 0) {
						devide = 1;
					}
					int32_t numberOfStep = m_allocated / devide;
					if (newSize< ((numberOfStep-2)*devide + devide/2) ) {
						//Allow Reallocation of a new size shoerter
						requestSize = ((newSize / devide)+1) * devide;
					}
				}
			} else {
				while(newSize > requestSize) {
					if (0 == requestSize) {
						requestSize = 1;
					} else if (0==m_increment) {
						requestSize = requestSize * 2;
					} else {
						requestSize = (requestSize + m_increment);
					}
				}
			}
			// No reallocation needed :
			if (requestSize == m_allocated) {
				return;
			}
			// check if something is allocated : 
			if (NULL == m_data) {
				// no data allocated ==> request an allocation (might be the first)
				ETK_MALLOC(m_data, requestSize, MY_TYPE);
			} else {
				// move datas
				ETK_REALLOC(m_data, requestSize, MY_TYPE);
			}
			// Check result with assert : 
			TK_ASSERT(NULL!=m_data, "Error in data allocation");
			// set the new allocation size
			m_allocated = requestSize;
		}
};
}

#undef __class__
#define __class__	NULL

#endif

