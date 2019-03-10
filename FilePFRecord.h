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
#pragma once

#include <Spore\Resource\IPFRecord.h>
#include <Spore\Resource\DatabasePackedFile.h>
#include <Spore\IO\FileStream.h>

using namespace Resource;

class FilePFRecord : public IPFRecord, IO::IStream
{
public:
	FilePFRecord(DBPFItem& itemInfo, const ResourceKey& name, DatabasePackedFile* pParentDBPF);
	FilePFRecord(FilePFRecord* pOther, const ResourceKey& name, DatabasePackedFile* pParentDBPF);

	virtual ~FilePFRecord();
	virtual int AddRef() override;
	virtual int Release() override;

	virtual void SetPath(const char16_t* pPath);

	//// IPFRecord ////

	/* 10h */	virtual ResourceKey&	GetName() override;
	/* 14h */	virtual void			SetName(const ResourceKey& name) override;

	/* 18h */	virtual IStream* GetStream() override;

	/* 1Ch */	virtual DatabasePackedFile* GetParentDBPF() const override;

	/* 20h */	virtual bool Open() override;
	/* 24h */	virtual bool Close() override;
	/* 28h */	virtual int func28h() override;

	//// IStream ////

	/* 0Ch */	virtual uint32_t	GetType() const override;
	/* 10h */	virtual int			GetAccessFlags() const override;
	/* 14h */	virtual FileError	GetState() const override;

	/* 1Ch */	virtual size_type	GetSize() const override;
	/* 20h */	virtual bool		SetSize(size_type size) override;  // does nothing
	/* 24h */	virtual int			GetPosition(PositionType positionType = kPositionTypeBegin) const override;
	/* 28h */	virtual bool		SetPosition(int distance, PositionType positionType = kPositionTypeBegin) override;
	/* 2Ch */	virtual int			GetAvailable() const override;

	/* 30h */	virtual int		Read(void* pData, size_t nSize) override;
	/* 34h */	virtual bool	Flush() override;  // does nothing
	/* 38h */	virtual int		Write(const void* pData, size_t nSize) override;  // does nothing

	static const uint32_t kType = 0x4CF999A2;  // Hash::FNV("FilePFRecord")

protected:
	FileStream mFileStream;
	int mnStreamRefCount;
	bool mbStreamOpened;
	DBPFItem mItemInfo;
};

