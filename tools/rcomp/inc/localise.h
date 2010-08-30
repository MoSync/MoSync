/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#ifndef __LOCALISE_H__
#define __LOCALISE_H__

#ifdef __VC32__ 
#pragma warning( push, 1 )	// MS STL libraries do not compile cleanly, temporarily set warning level to 1
#pragma warning( disable : 4710 )	// function not inlined.
#pragma warning( disable : 4530 )	// function not inlined.
#endif
#include <vector>
#include <map>
#include <new>

#ifdef _MSC_VER
#pragma warning( disable : 4710 )	// function not inlined.
#endif

#include "astring.h"

typedef std::vector<String> CommentTagStore;

enum OutputStates {	EStartOfLine, EGeneral, EFindLineNo, EInLineNo, 
					EFindFileName, EInFileName, EFindDigit, EAfterDigit };

enum States { EStartOfComment, EAfterFirstWordOfTag, ENewLineAtStart, EInTag, ENewLineInTag };

enum Requirements { ERequired, EOptional, EOptionalWithDefault, EForbidden };

enum ProcessOptionalStates { ELookingForCommand, ELookingForEqualSignOrRlsItem, ELookingForDefault,
								ELookingForRlsItem, EShouldBeFinished };

enum NumberOfRlsTypes { ENumberOfRlsTypes = 7 }; 
// ENumberOfRlsTypes is used as the dimension of the array iRlsTypes declared in GlobalLocalisationData.
// These values are filled in in the constructor for that class.

class RlsItemRequirements
	{
public:
	RlsItemRequirements(String aRlsName, int aRequirement=EForbidden);
	RlsItemRequirements(const RlsItemRequirements & aRlsItemRequirements);
	~RlsItemRequirements();
private:
	String iRlsName;
	int iRequirement;
	};

class CommandRequirementsHolder
	{
public:
	CommandRequirementsHolder(String aCommandName);
	CommandRequirementsHolder(const CommandRequirementsHolder & aCommandRequirmentsHolder);
	~CommandRequirementsHolder();
private:
	String iCommandName;
	std::vector<RlsItemRequirements> iRlsRequirements;
	};


class TagParameterData
	{
public:
	TagParameterData(String aTagParameter);
	TagParameterData(const TagParameterData & aTagParameterData);
	~TagParameterData();
	void SetTagParameter(String aTagParameter);
	String GetTagParameter();
private:
	String iTagParameter;
	};

class CommentRequirement // requirements for an rls item
	{
public:
	CommentRequirement(int aRequirementStatus, String aDefaultParameter="");
	CommentRequirement(const CommentRequirement & aCommentRequirement);
	~CommentRequirement();
	int GetRequirementStatus();
	String GetDefault();
private:
	int iRequirementStatus;
	String iDefaultParameter;
	};

class CommentRequirementPair // for a particular tag holds the requirements for an rls item
	{
public:
	CommentRequirementPair(String aRlsItem, int aStatus, String aDefault="");
	CommentRequirementPair(const CommentRequirementPair & aCommentRequirementPair);
	~CommentRequirementPair();
	String CheckIfOptionalAndHasDefault(String aRlsTypeName, String aCommandName);
	int GetRequirementStatus();
	String GetRlsItem();
	void SetRlsItem(String aRlsItem);
private:
	String iRlsItem;
	CommentRequirement iCommentRequirementData;
	};

class CommentTag // the declared data which goes with a particular command tag
	{
public:
	CommentTag(String aParameterType="", bool aTagDeclared=false);
	CommentTag(const CommentTag & aCommentTag);
	~CommentTag();
	bool IsTagDeclared();
	bool IsTagLegalForRlsType(String aRlsTypeName);
	bool IsParameterAlreadyDeclared(String aTagParameter);
	int GetRequirementStatus(String aRlsItem);
	String CheckIfOptionalAndHasDefault(String aRlsTypeName, String aCommandName);
	String GetParameterType();
	String GetPermissibleParameter(int aIndex);
	int GetNumberOfPermissibleParameters();
	void SetDeclared();
	void SetParameterType(String aParameterType);
	void AddTagParameterData(String aPermissibleParameter);
	void AddRequirements(String aRlsItem, int aRequired, String aDefault);
private:
	bool iTagDeclared;
	String iParameterType;
	std::vector<TagParameterData> iPermissibleParameters;
	std::vector<CommentRequirementPair> iRequirements;
	};

class CommentTagPair // a mapping between command tag names and the data which goes with them
	{
public:
	CommentTagPair(String aCommandName, String aParameterType="", bool aTagDeclared=false);
	CommentTagPair(const CommentTagPair & aCommentTagPair);
	~CommentTagPair();
	bool TagDeclared();
	bool IsTagLegalForRlsType(String aRlsTypeName);
	bool IsParameterAlreadyDeclared(String aTagParameter);
	int GetRequirementStatus(String aRlsItem);
	String CheckIfOptionalAndHasDefault(String aRlsTypeName);
	String GetCommandName();
	String GetParameterType();
	String GetPermissibleParameter(int aIndex);
	int GetNumberOfPermissibleParameters();
	void SetCommandName(String aCommandName);
	void SetTagDeclared();
	void AddPermissibleParameter(String aPermissibleParameter);
	void AddRlsItem(String aRlsItem, int aRequired, String aDefault);
	
private:
	String iCommandName;
	CommentTag iTagVariables;
	};


	
class LocalisationLine // an individual line from a localisation comment
	{
public:
	LocalisationLine(String iFileName="", int iLineNumber=-1);
	LocalisationLine(const LocalisationLine & aClass);
	~LocalisationLine();
	void Reset(String iFileName, int iLineNumber);
	String GetFileName();
	int GetFirstLine();
	int GetLastLine();
	void SetFirstLine(int aFirstLine);
	void SetFileName(String aFileName);
	void SetLastLine(int aLastLine);
	unsigned int GetNumberOfTokens();
	String GetElement(unsigned int aIndex);
	void AddElement(String aElement);
	void SetAnalysed(bool aParity);
	void Empty();
	bool IsLineAnalysed();
private:
	bool iAnalysed;
	int iFirstLine;
	int iLastLine;
	String iFileName;
	std::vector<String> iData;
	};


class LocalisationComment // holds the data from a localisation comment
	{
public:
	LocalisationComment(LocalisationLine & aComment);
	~LocalisationComment();
	String GetFileName();
	int GetFirstLineOfComment();
	int GetLastLineOfComment();
	int GetNumberOfOptionalLinesToAdd();
	LocalisationLine GetLine(unsigned int aIndex);
	unsigned int GetNumberOfTokensInComment(); // size of originalData
	unsigned int GetNumberOfTokensInLine(unsigned int aIndex); // size of iData[aIndex]
	unsigned int GetNumberOfLinesInComment(); // size of iData
	String GetOriginalElement(unsigned int aIndex);
	String GetElementFromLine(unsigned int aLine, unsigned int aElement);
	void AddDataLine(LocalisationLine aDataLine);
	void SetAnalysed(unsigned int aIndex, bool aParity);
	void SetLastLine(int aLineNumber);
	bool IsLineAnalysed(unsigned int aIndex);
	void SetWholeTagAnalysed(bool aParity);
	int GetFirstLine(unsigned int aIndex);
	void AddOptionalData(String aOptionalData);
	String GetOptionalLineToAdd(unsigned int aLine);
private:
	LocalisationLine iOriginalData;
	std::vector<LocalisationLine> iData;
	std::vector<String> iOptionalLinesToAdd;
	};

class WarningToOutput // an individual localisation warning
	{
public:
	WarningToOutput(const String aFileName, int aLineNumber, String aComment);
	WarningToOutput(const WarningToOutput & aWarningToOutput);
	~WarningToOutput();
	WarningToOutput& operator=(const WarningToOutput& aWarningToOutput);
	const String GetFileName();
	int GetLineNumber();
	String GetComment();
private:
	String iFileName;
	int iLineNumber;
	String iComment;
	};

class GlobalLocalisationData
	{
public:
	GlobalLocalisationData();
	~GlobalLocalisationData();

	bool LocalisationCommentsExist();
	void AnalyseLocalisationData();
	void OutputLocalisedFile(String SourceFileName);
	void PrintLocalisationWarnings();

	void AddWarningToStore(const String aFileName, int aLineNumber, String aComment);
	void SetStart(String aFileName, int aLineNumber);
	void StoreComment(String aString);
	void StoreFinalComment();

private:
	void ParseCommentTags(unsigned int i);
	void CheckIfCommentTagsFullyAnalysed();
	void CheckRlsEntries();
	void Process(unsigned int aStoreIndex, unsigned int aCurrentIndex);
	void ProcessDeclaration(unsigned int aStoreIndex, unsigned int aCurrentIndex);
	void ProcessTagParameter(unsigned int aStoreIndex, unsigned int aCurrentIndex);
	void ProcessOptional(unsigned int aStoreIndex, unsigned int aCurrentIndex);
	void ProcessRequired(unsigned int aStoreIndex, unsigned int aCurrentIndex);
	void StoreTagParameter(String aCommandName, String aParameter);
	void StoreRlsItem(String aCommandName, String aRlsItem, int aRequired, String aDefault="");
	void CheckWhetherAllCommentsPresent(String rlsTypeName, int commentOfInterest, std::vector<String> commentsSeen);
	void CheckParametersArePermitted(int aCommentOfInterest, int aLineInComment, int aDefinitionNumber);
	void CheckCommands(String aRlsTypeName, int aCommentOfInterest);
	void AddCommentToStore();
	bool IsAnRlsItem(String aString) const;
	
	int NeedToAddDefaultData(String aFileName, int aFileLine);
	int FindIndex(const String aFileName, int aLineNumber);
	bool CheckFirstIsCommand(LocalisationLine aCommandLine);
	int GetTagDeclaredIndex(String aCommandName);

	bool iCommentStarted;
	std::vector<LocalisationComment> iCommentTagStore; // data split up into individual command lines
	LocalisationLine iTempCommentStore; // holds the data brought in from yacc parsing
	std::vector<CommentTagPair> iCommentDefinitions;
	std::vector<WarningToOutput> iWarningStore;
	String iTypes[4];
	String iRlsTypes[ENumberOfRlsTypes];
	};

extern GlobalLocalisationData *pGL;	
#ifdef __VC32__
#pragma warning( pop )
#endif

#endif

