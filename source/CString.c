//
//  CString.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CString.h"
#include <stdlib.h>
#include <string.h>

CString CString_Create(const char* str)
{
	CString string;
	unsigned long length = strlen(str);
	string.size = length + 1;
	string.data = (char*) malloc(string.size);
	
	memcpy(string.data, str, length);
	string.data[length] = '\0';
	
	return string;
}

void CString_Free(CString* string)
{
	if(string->data) {
		free(string->data);
		string->data = NULL;
		string->size = 0;
	}
}

int CString_GetSize(const CString* string)
{
	if(string == 0) {
		return 0;
	}
	
	return (int)string->size - 1;
}

int CString_Compare(const CString* string1, const CString* string2)
{
	if(string1->size != string2->size) {
		return 0;
	}
	
	int cmp = strncmp(string1->data, string2->data, string1->size);
	
	if(cmp == 0) {
		return 1;
	}
	
	return 0;
}

int CString_CompareCharPtr(const CString* string1, const char* string2)
{
	unsigned long str2_length = strlen(string2) + 1;
	
	if(string1->size != str2_length) {
		return 0;
	}
	
	int cmp = strncmp(string1->data, string2, string1->size);
	
	if(cmp == 0) {
		return 1;
	}
	
	return 0;
}

void CString_AppendChar(CString* string, char c)
{
	char* tmp_str = malloc(string->size + 1);
	strlcpy(tmp_str, string->data, string->size);
	
	// Add character.
	tmp_str[string->size - 1] = c;
	tmp_str[string->size] = '\0';
	
	free(string->data);
	string->data = tmp_str;
	string->size++;
}

void CString_InsertChar(CString* string, unsigned int pos, char c)
{
	char* tmp_str = malloc(string->size + 1);
	
	for(int i = 0; i < pos; i++) {
		tmp_str[i] = string->data[i];
	}
	
	tmp_str[pos] = c;
	
	for(int i = pos; i < string->size; i++) {
		tmp_str[i + 1] = string->data[i];
	}
	
	free(string->data);
	string->data = tmp_str;
	string->size++;
}

void CString_InsertString(CString* string1, unsigned long pos, const CString* string2)
{
	char* tmp_str = malloc(string1->size + string2->size - 1);
	
	for(unsigned long i = 0; i < pos; i++) {
		tmp_str[i] = string1->data[i];
	}
	
	for(unsigned long i = 0; i < string2->size - 1; i++) {
		tmp_str[pos + i] = string2->data[i];
	}
	
	for(unsigned long i = pos; i < string1->size; i++) {
		tmp_str[i + string2->size - 1] = string1->data[i];
	}
	
	free(string1->data);
	string1->data = tmp_str;
	string1->size += string2->size - 1;
}

void CString_InsertCharPtr(CString* string1, unsigned long pos, const char* string2)
{
	unsigned long str2_length = strlen(string2);
	char* tmp_str = malloc(string1->size + str2_length);
	
	for(unsigned long i = 0; i < pos; i++) {
		tmp_str[i] = string1->data[i];
	}
	
	for(unsigned long i = 0; i < str2_length; i++) {
		tmp_str[pos + i] = string2[i];
	}
	
	for(unsigned long i = pos; i < string1->size; i++) {
		tmp_str[i + str2_length] = string1->data[i];
	}
	
	free(string1->data);
	string1->data = tmp_str;
	string1->size += str2_length;
}

CString CString_GetSubString(CString* string, unsigned long pos, unsigned long len)
{
	if(pos + len > string->size - 1) {
		len = string->size - 1 - pos;
	}
	
	char* tmp_str = malloc(len + 1);
	
	for(unsigned long i = 0; i < len; i++) {
		tmp_str[i] = string->data[i + pos];
	}
	
	tmp_str[len] = '\0';
	
	CString sub_str;
	sub_str.size = len + 1;
	sub_str.data = tmp_str;
	
	return sub_str;
}

CString CString_Copy(const CString* string)
{
	CString str_cpy;
	str_cpy.size = string->size;
	str_cpy.data = malloc(string->size);
	memcpy(str_cpy.data, string->data, string->size);
	
	return str_cpy;
}

void CString_AppendCharPtr(CString* string, const char* string2)
{
	unsigned long str2_length = strlen(string2);
	char* tmp_str = malloc(string->size + str2_length);
	memcpy(tmp_str, string->data, string->size - 1);
	
	// Add string.
	for(int i = 0; i < str2_length + 1; i++) {
		tmp_str[i + string->size - 1] = string2[i];
	}
	
	free(string->data);
	string->data = tmp_str;
	string->size += str2_length;
}

void CString_AppendString(CString* string, const CString* string2)
{
	char* tmp_str = malloc(string->size + string2->size - 1);
	memcpy(tmp_str, string->data, string->size - 1);
	
	// Add string.
	for(int i = 0; i < string2->size; i++) {
		tmp_str[i + string->size - 1] = string2->data[i];
	}
	
	free(string->data);
	string->data = tmp_str;
	string->size += string2->size - 1;
}

void CString_PrependCharPtr(CString* string, const char* string2)
{
	unsigned long str2_length = strlen(string2);
	char* tmp_str = malloc(string->size + str2_length);
	memcpy(tmp_str, string2, str2_length);
	
	// Add string.
	for(int i = 0; i < string->size; i++) {
		tmp_str[i + str2_length] = string->data[i];
	}
	
	free(string->data);
	string->data = tmp_str;
	string->size += str2_length;
}

void CString_PrependString(CString* string, const CString* string2)
{
	char* tmp_str = malloc(string->size + string2->size - 1);
	memcpy(tmp_str, string2->data, string2->size - 1);
	
	// Add string.
	for(int i = 0; i < string->size; i++) {
		tmp_str[i + string2->size - 1] = string->data[i];
	}
	
	free(string->data);
	string->data = tmp_str;
	string->size += string2->size - 1;
}

void CString_RemoveLastChar(CString* string)
{
	char* tmp_str = malloc(string->size - 1);
	memcpy(tmp_str, string->data, string->size - 1);
	tmp_str[string->size - 2] = '\0';
	
	free(string->data);
	string->data = tmp_str;
	string->size--;
}
