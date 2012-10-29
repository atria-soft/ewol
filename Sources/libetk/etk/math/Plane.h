/**
 *******************************************************************************
 * @file etk/math/Plane.h
 * @brief Ewol Tool Kit : generique plane type (header)
 * @author Edouard DUPIN
 * @date 29/10/2012
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

#ifndef __ETK_TYPES_PLANE_H__
#define __ETK_TYPES_PLANE_H__

#include <etk/DebugInternal.h>
#include <etk/math/Vector3D.h>
#include <etk/Vector.h>

namespace etk {
	template <typename T> class Plane
	{
		public :
			//member variables
			etk::Vector3D<T> m_normal;	//X.N+intercept=0
			T                m_intercept;
		public:
			/*****************************************************
			 *    Constructor
			 *****************************************************/
			Plane(void) :
				m_normal(0, 0, 0),
				m_intercept(0)
			{
				
			}
			Plane(etk::Vector3D<T> _normal, T _intercept=0) :
				m_normal(_normal),
				m_intercept(_intercept)
			{
				
			}
			Plane(const Plane& obj) :
				m_normal(obj.m_normal),
				m_intercept(obj.m_intercept)
			{
				
			}
			/*****************************************************
			 *    Destructor
			 *****************************************************/
			~Plane(void)
			{
				
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			void SetNormal(const etk::Vector3D<T>& obj)
			{
				m_normal=obj;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			void SetIntercept(float _intercept)
			{
				m_intercept=_intercept;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			void SetFromPoints(const etk::Vector3D<T> & p0,
			                   const etk::Vector3D<T> & p1,
			                   const etk::Vector3D<T> & p2)
			{
				m_normal=(p1-p0).CrossProduct(p2-p0);
				m_normal.Normalize();
				CalculateIntercept(p0);
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			void CalculateIntercept(const etk::Vector3D<T>& pointOnPlane)
			{
				m_intercept=-m_normal.DotProduct(pointOnPlane);
			}
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			void Normalize(void)
			{
				float normalLength=m_normal.GetLength();
				m_normal/=normalLength;
				m_intercept/=normalLength;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			etk::Vector3D<T> GetNormal(void)
			{
				return m_normal;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			float GetIntercept()
			{
				return m_intercept;
			}
			
			//find point of intersection of 3 planes
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			bool Intersect3(const Plane<T>& p2,
			                const Plane<T> & p3,
			                etk::Vector3D<T> & result)
			{
				float denominator=m_normal.DotProduct((p2.m_normal).CrossProduct(p3.m_normal));
				//scalar triple product of normals
				if(denominator==0.0f) {
					//no intersection
					return false;
				}
				etk::Vector3D<T> temp1, temp2, temp3;
				temp1=(p2.m_normal.CrossProduct(p3.m_normal))*m_intercept;
				temp2=(p3.m_normal.CrossProduct(m_normal))*p2.m_intercept;
				temp3=(m_normal.CrossProduct(p2.m_normal))*p3.m_intercept;
				
				result=(temp1+temp2+temp3)/(-denominator);
				
				return true;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			float GetDistance(const etk::Vector3D<T> & point) const
			{
				return   point.x*m_normal.x
				       + point.y*m_normal.y
				       + point.z*m_normal.z
				       + m_intercept;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			Plane<T> LinearInterpolate(const Plane<T> & p2, float factor)
			{
				Plane<T> result;
				result.m_normal=m_normal*(1.0f-factor) + p2.m_normal*factor;
				result.m_normal.Normalize();
				result.m_intercept=m_intercept*(1.0f-factor) + p2.m_intercept*factor;
				return result;
			};
			
			//operators
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			bool operator==(const Plane<T> & obj) const
			{
				if(    m_normal==obj.m_normal
				    && m_intercept==obj.m_intercept) {
					return true;
				}
				return false;
			};
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			bool operator!=(const Plane<T> & obj) const
			{
				return!((*this)==obj);
			}
			
			//unary operators
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			Plane<T> operator-(void) const
			{
				return Plane<T>(-m_normal, -m_intercept);
			}
			
			/**
			 * @brief
			 * @param[in,out]
			 * @return
			 */
			Plane<T> operator+(void) const
			{
				return *this;
			}
	};
};

#endif

