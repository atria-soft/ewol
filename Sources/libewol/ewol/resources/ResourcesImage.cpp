/**
 *******************************************************************************
 * @file ewol/resources/ResourcesImage.cpp
 * @brief ewol Resources image system (sources)
 * @author Edouard DUPIN
 * @date 21/08/2012
 * @par Project
 * ewol
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

#include <ewol/resources/ResourcesManager.h>

ewol::ResourcesImage::ResourcesImage(etk::UString fileName, Vector2D<int32_t> size) :
	m_resourceID(0),
	m_counter(1),
	m_image(size)
{
	m_resourceID = ewol::resourcesManager::Add(this);
}


ewol::ResourcesImage::~ResourcesImage(void)
{
	ewol::resourcesManager::Rm(this);
}