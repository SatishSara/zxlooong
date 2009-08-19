/*
 *  This is a part of the Microsoft Source Code Samples.
 *  Copyright 1996 - 1998 Microsoft Corporation.
 *  All rights reserved.
 *
 *	This sample code shows the usage of some portions
 *	of the NTMS API.
 *	
 *	Return codes are, for the most part, not checked in
 *	this code. See the Programmer's reference for error
 *	return information.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * Copyright 1997 - 1998 Microsoft Corporation.  All Rights Reserved. *
 */
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <ntmsapi.h>


static NTMS_GUID globalPoolID, globalMediaID;


// in a real app the following functions use a database or some such
// thing to persist these GUIDs across app invocations.

	void
persistPoolID(NTMS_GUID guid)
{
	globalPoolID = guid;
}

	NTMS_GUID
recallPoolID()
{
	return globalPoolID;
}

	void
persistMediaID(NTMS_GUID guid)
{
	globalMediaID = guid;
}

	NTMS_GUID
recallMediaID()
{
	return globalMediaID;
}

