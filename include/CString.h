//
//  CString.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CString_h
#define CString_h

#include <stdio.h>

typedef struct _CString {
	unsigned long size;
	char* data;
} CString;


CString CString_Create(const char* str);

void CString_Free(CString* string);

int CString_GetSize(const CString* string);

int CString_Compare(const CString* string1, const CString* string2);

int CString_CompareCharPtr(const CString* string1, const char* string2);

void CString_AppendChar(CString* string, char c);

void CString_InsertChar(CString* string, unsigned int pos, char c);

void CString_InsertString(CString* string1, unsigned long pos, const CString* string2);

void CString_InsertCharPtr(CString* string1, unsigned long pos, const char* string2);

CString CString_GetSubString(CString* string, unsigned long pos, unsigned long len);

CString CString_Copy(const CString* string);

void CString_AppendCharPtr(CString* string, const char* string2);

void CString_AppendString(CString* string, const CString* string2);

void CString_PrependCharPtr(CString* string, const char* string2);

void CString_PrependString(CString* string, const CString* string2);

void CString_RemoveLastChar(CString* string);

#endif /* CString_h */
