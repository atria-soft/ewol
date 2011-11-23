/**
 *******************************************************************************
 * @file etk/Singleton.h
 * @brief ewol : singleton system class
 * @author Edouard DUPIN
 * @date 04/12/2010
 * @par Project
 * ewol TK
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

#ifndef __ETK_SINGLETON_H__
#define __ETK_SINGLETON_H__


namespace etk {
	template <typename T>
	class Singleton
	{
		protected:
			// Constructeur/destructeur
			Singleton() { }
			~Singleton() { /*std::cout << "destroying singleton." << std::endl;*/ }
	
		public:
			// Interface publique
			static T *Get()
			{
				if (NULL == _singleton)
				{
					_singleton = new T;
				}
	
				return (static_cast<T*> (_singleton));
			}
	
			static void Kill()
			{
				if (NULL != _singleton)
				{
					delete _singleton;
					_singleton = NULL;
				}
			}
	
		private:
		// Unique instance
			static T *_singleton;
	};
	
	template <typename T>
			T *Singleton<T>::_singleton = NULL;
}

#endif


