/****************************************************************************
* Copyright (C) 2019 Eric Mor
*
* This file is part of Spore ModCreatorKit.
*
* Spore ModAPI is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include "stdafx.h"
#include "DetouredResourceManager.h"
#include "Debugging.h"

DetourDeclaration(DetouredResourceManager::GetResource);

long DetouredResourceManager::AttachDetour()
{
	SetDetourAddress(GetResource, GetMethodAddress(Resource::cResourceManager, GetResource));

	long error = 0;
	error |= AttachDetourFunction(GetResource, DetouredGetResource_redirected);

	return error;
}


// virtual bool Resource::IResourceManager::GetResource(const ResourceKey& name, ResourceObject** ppDst, int arg_8, DBPF* pDBPF, IResourceFactory* pFactory, const ResourceKey* pCacheName)
bool DetouredResourceManager::DetouredGetResource(const ResourceKey& name, ResourceObject** ppDst, int arg_8, DBPF* pDBPF, IResourceFactory* pFactory, const ResourceKey* pCacheName)
{
	DebugInformation* pDebugInformation = nullptr;

	// We only want to do the debugging thing if we are going to return a resource
	if (ppDst == nullptr) return GetResource_original(this, name, ppDst, arg_8, pDBPF, pFactory, pCacheName);


	ResourceKey realKey = name;
	if (pDBPF == nullptr)
	{
		if (pCacheName == nullptr) pDBPF = this->GetRealFileKey(name, &realKey);
		else pDBPF = this->GetDBPF(name);
	}

	if (pDBPF != nullptr)
	{
		//return CallOriginal(GetResource, name, ppDst, arg_8, pDBPF, pFactory, pCacheName);
		if (Debugging::Get().GetDebugInformation(pDBPF, &pDebugInformation)
			&& pDebugInformation->GetFilePath(realKey))
		{
			return this->ReadResource(name, ppDst, arg_8, pDBPF, pFactory, pCacheName);

		}
		else
		{
			return GetResource_original(this, name, ppDst, arg_8, pDBPF, pFactory, pCacheName);
		}

	}
	else
	{
		return GetResource_original(this, name, ppDst, arg_8, pDBPF, pFactory, pCacheName);;
	}
}

