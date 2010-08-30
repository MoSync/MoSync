/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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




#include "main.h"
#include "localise.h"

#ifdef __VC32__ 
#pragma warning( push, 1 )	// MS STL libraries do not compile cleanly, temporarily set warning level to 1
#pragma warning( disable : 4710 )	// function not inlined.
#pragma warning( disable : 4530 )	// function not inlined.
#endif

#include <stdio.h>
#include <iostream>
#include "errorhan.h"

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
using std::ifstream;
using std::cerr;
using std::endl;
#endif //__MSVCDOTNET__



/******************************************************************************************
**
** RlsItemRequirements
**
******************************************************************************************/

RlsItemRequirements::RlsItemRequirements(String aRlsName, int aRequirement)
	:iRlsName(aRlsName),
	iRequirement(aRequirement)
	{}

RlsItemRequirements::RlsItemRequirements(const RlsItemRequirements & aRlsItemRequirements)
	:iRlsName(aRlsItemRequirements.iRlsName),
	iRequirement(aRlsItemRequirements.iRequirement)
	{}
	
RlsItemRequirements::~RlsItemRequirements() 
	{}

/******************************************************************************************
**
** CommandRequirementsHolder
**
******************************************************************************************/

CommandRequirementsHolder::CommandRequirementsHolder(String aCommandName)
	:iCommandName(aCommandName)
	{
	iRlsRequirements.clear();
	}

CommandRequirementsHolder::CommandRequirementsHolder(const CommandRequirementsHolder & aCommandRequirmentsHolder)
	:iCommandName(aCommandRequirmentsHolder.iCommandName)
	{
	for(unsigned int j = 0; j < aCommandRequirmentsHolder.iRlsRequirements.size() ; j++)
		{
		iRlsRequirements.push_back(aCommandRequirmentsHolder.iRlsRequirements[j]);
		}
	}
	
CommandRequirementsHolder::~CommandRequirementsHolder()
	{}

/******************************************************************************************
**
** TagParameterData
**
******************************************************************************************/

TagParameterData::TagParameterData(String aTagParameter)
	:iTagParameter(aTagParameter)
	{}

TagParameterData::TagParameterData(const TagParameterData & aTagParameterData)
	:iTagParameter(aTagParameterData.iTagParameter)
	{}

TagParameterData::~TagParameterData()
	{}

void TagParameterData::SetTagParameter(String aTagParameter)
	{
	iTagParameter = aTagParameter;
	}

String TagParameterData::GetTagParameter()
	{
	return iTagParameter;
	}

/******************************************************************************************
**
** CommentRequirement
**
******************************************************************************************/

CommentRequirement::CommentRequirement(int aRequirementStatus, String aDefaultParameter)
	:iRequirementStatus(aRequirementStatus),
	iDefaultParameter(aDefaultParameter)
	{
	}

CommentRequirement::CommentRequirement(const CommentRequirement & aCommentRequirement)
	:iRequirementStatus(aCommentRequirement.iRequirementStatus),
	iDefaultParameter(aCommentRequirement.iDefaultParameter)
	{
	}

CommentRequirement::~CommentRequirement()
	{}

String CommentRequirement::GetDefault()
	{
	return iDefaultParameter;
	}

int CommentRequirement::GetRequirementStatus()
	{
	return iRequirementStatus;
	}

/******************************************************************************************
**
** CommentRequirementPair
**
******************************************************************************************/

CommentRequirementPair::CommentRequirementPair(String aRlsItem, int aStatus, String aDefault)
	:iRlsItem(aRlsItem),
	iCommentRequirementData(aStatus, aDefault)
	{
	}

CommentRequirementPair::CommentRequirementPair(const CommentRequirementPair & aCommentRequirementPair)
	:iRlsItem(aCommentRequirementPair.iRlsItem),
	iCommentRequirementData(aCommentRequirementPair.iCommentRequirementData)
	{}

CommentRequirementPair::~CommentRequirementPair()
	{}

int CommentRequirementPair::GetRequirementStatus()
	{
	return iCommentRequirementData.GetRequirementStatus();
	}

String CommentRequirementPair::CheckIfOptionalAndHasDefault(String aRlsTypeName, String aCommandName)
	{ // returns a String containing a line which needs to be written out to the rpp file, or returns ""
	String optionalLineToAdd = "";
	if(iRlsItem == aRlsTypeName)
		{
		int requirementStatus = iCommentRequirementData.GetRequirementStatus();
		if(requirementStatus == EOptionalWithDefault)
			{
			String defaultValue = iCommentRequirementData.GetDefault();
			if(defaultValue.Length()) // will only return true if a default exists
				{
				optionalLineToAdd = "@";
				optionalLineToAdd += aCommandName;
				String tempString(" ");
				optionalLineToAdd += tempString;
				optionalLineToAdd += defaultValue;
				tempString = "\n";
				optionalLineToAdd += tempString;
				}
			}
		}
	return optionalLineToAdd;
	}

String CommentRequirementPair::GetRlsItem()
	{
	return iRlsItem;
	}

void CommentRequirementPair::SetRlsItem(String aRlsItem)
	{
	iRlsItem = aRlsItem;
	}

/******************************************************************************************
**
** CommentTag
**
******************************************************************************************/

CommentTag::CommentTag(String aTagType, bool aTagDeclared)
	:iTagDeclared( aTagDeclared ),
	iParameterType( aTagType )
	{
	iPermissibleParameters.clear();
	iRequirements.clear();
	}

CommentTag::CommentTag(const CommentTag & aCommentTag)
	:iTagDeclared( aCommentTag.iTagDeclared ),
	iParameterType( aCommentTag.iParameterType )
	{
	unsigned int j;
	for(j=0; j< aCommentTag.iPermissibleParameters.size(); j++)
		{
		iPermissibleParameters.push_back(aCommentTag.iPermissibleParameters[j]);
		}
	for(j=0; j< aCommentTag.iRequirements.size(); j++)
		{
		iRequirements.push_back(aCommentTag.iRequirements[j]);
		}
	}		
			
CommentTag::~CommentTag()
	{}

String CommentTag::CheckIfOptionalAndHasDefault(String aRlsTypeName, String aCommandName)
	{
	if(iTagDeclared)
		{
		for(unsigned int j=0; j<iRequirements.size(); j++)
			{
			String defaultParameter = iRequirements[j].CheckIfOptionalAndHasDefault(aRlsTypeName, aCommandName);
			if(defaultParameter.Length())
				return defaultParameter; // returns default parameter if it exists
			}
		}
	return ""; // returns "" if no default parameter exists
	}
		
int CommentTag::GetRequirementStatus(String aRlsItem)
	{
	for(unsigned int j=0; j<iRequirements.size(); j++)
		{
		if(iRequirements[j].GetRlsItem() == aRlsItem)
			{
			int requirementStatus = iRequirements[j].GetRequirementStatus();
			return requirementStatus;
			}
		}
	return EForbidden;
	}

bool CommentTag::IsTagLegalForRlsType(String aRlsTypeName)
	{
	for(unsigned int j=0; j<iRequirements.size(); j++)
		{
		if(iRequirements[j].GetRlsItem() == aRlsTypeName)
			{
			return iRequirements[j].GetRequirementStatus() != EForbidden ? true : false; 
			}
		}
	return false;
	}

bool CommentTag::IsParameterAlreadyDeclared(String aTagParameter)
	{
	for(int j=0; j<GetNumberOfPermissibleParameters(); j++)
		{
		if(iPermissibleParameters[j].GetTagParameter() == aTagParameter)
			{
			return true; // returns true if the given parameter is already declared
			}
		}
	return false;
	}

String CommentTag::GetPermissibleParameter(int aIndex) // returns pointer to aIndex-th parameter
	{
	return iPermissibleParameters[aIndex].GetTagParameter();
	}

int CommentTag::GetNumberOfPermissibleParameters()
	{
	return iPermissibleParameters.size();
	}

bool CommentTag::IsTagDeclared()
	{
	return iTagDeclared;
	}

void CommentTag::SetDeclared()
	{
	iTagDeclared = true;
	}


void CommentTag::SetParameterType(String aParameterType)
	{
	iParameterType = aParameterType;
	}

String CommentTag::GetParameterType()
	{
	return iParameterType;
	}

void CommentTag::AddTagParameterData(String aTagParameterData)
	{
	TagParameterData dataToAdd( aTagParameterData );
	iPermissibleParameters.push_back( dataToAdd );
	}

void CommentTag::AddRequirements(String aRlsItem, int aRequired, String aDefault)
	{
	CommentRequirementPair commentRequirementPair( aRlsItem, aRequired, aDefault );
	iRequirements.push_back( commentRequirementPair );
	}

/******************************************************************************************
**
** CommentTag
**
******************************************************************************************/

CommentTagPair::CommentTagPair(String aCommandName, String aParameterType, bool aTagDeclared)
	:iCommandName(aCommandName),
	iTagVariables(aParameterType, aTagDeclared)
	{}
	
CommentTagPair::CommentTagPair(const CommentTagPair & aCommentTagPair)
	:iCommandName(aCommentTagPair.iCommandName),
	iTagVariables(aCommentTagPair.iTagVariables)
	{}
	
CommentTagPair::~CommentTagPair()
	{}

String CommentTagPair::CheckIfOptionalAndHasDefault(String aRlsTypeName)
	{
	return iTagVariables.CheckIfOptionalAndHasDefault(aRlsTypeName, iCommandName);
	}

int CommentTagPair::GetRequirementStatus(String aRlsItem)
	{
	return iTagVariables.GetRequirementStatus( aRlsItem );
	}

bool CommentTagPair::IsTagLegalForRlsType(String aRlsTypeName)
	{
	return iTagVariables.IsTagLegalForRlsType(aRlsTypeName);
	}

bool CommentTagPair::IsParameterAlreadyDeclared(String aTagParameter)
	{
	return iTagVariables.IsParameterAlreadyDeclared(aTagParameter);
	}

String CommentTagPair::GetPermissibleParameter(int aIndex)
	{
	return iTagVariables.GetPermissibleParameter(aIndex);
	}

int CommentTagPair::GetNumberOfPermissibleParameters()
	{
	return iTagVariables.GetNumberOfPermissibleParameters();
	}

void CommentTagPair::SetCommandName(String aCommandName)
	{
	iCommandName = aCommandName;
	}

String CommentTagPair::GetCommandName()
	{
	return iCommandName;
	}

void CommentTagPair::SetTagDeclared()
	{
	iTagVariables.SetDeclared();
	}

bool CommentTagPair::TagDeclared()
	{
	return iTagVariables.IsTagDeclared();
	}

String CommentTagPair::GetParameterType()
	{
	return iTagVariables.GetParameterType();
	}

void CommentTagPair::AddPermissibleParameter(String aPermissibleParameter)
	{
	iTagVariables.AddTagParameterData( aPermissibleParameter );
	}

void CommentTagPair::AddRlsItem(String aRlsItem, int aRequired, String aDefault)
	{
	iTagVariables.AddRequirements(aRlsItem, aRequired, aDefault);
	}

/******************************************************************************************
**
** LocalisationLine
**
******************************************************************************************/

LocalisationLine::LocalisationLine(String aFileName, int aFirstLine)
	:iAnalysed(false),
	iFirstLine(aFirstLine),
	iLastLine(aFirstLine),
	iFileName(aFileName)
	{
	iData.clear();
	}

LocalisationLine::LocalisationLine(const LocalisationLine & aLocalisationLine)
	:iAnalysed(aLocalisationLine.iAnalysed),
	iFirstLine(aLocalisationLine.iFirstLine),
	iLastLine(aLocalisationLine.iLastLine),
	iFileName(aLocalisationLine.iFileName)	
	{
	for(unsigned int j=0; j<aLocalisationLine.iData.size(); j++)
		{
		iData.push_back(aLocalisationLine.iData[j]);
		}
	}

LocalisationLine::~LocalisationLine()
	{}

void LocalisationLine::Reset(String aFileName, int aFirstLine)
	{
	iFirstLine = aFirstLine;
	iLastLine = aFirstLine;
	iFileName = aFileName;
	iAnalysed = false;
	iData.clear();
	}

String LocalisationLine::GetFileName()
	{
	return iFileName;
	}

int LocalisationLine::GetFirstLine()
	{
	return iFirstLine;
	}

int LocalisationLine::GetLastLine()
	{
	return iLastLine;
	}

void LocalisationLine::SetFirstLine(int aFirstLine)
	{
	iFirstLine = aFirstLine;
	}

void LocalisationLine::SetFileName(String aFileName)
	{
	iFileName = aFileName;
	}

unsigned int LocalisationLine::GetNumberOfTokens()
	{
	return iData.size();
	}

String LocalisationLine::GetElement(unsigned int aIndex)
	{
	return iData[aIndex];
	}

void LocalisationLine::AddElement(String aElement)
	{
	iData.push_back(aElement);
	}

void LocalisationLine::SetLastLine(int aLastLine)
	{
	iLastLine = aLastLine;
	}

void LocalisationLine::SetAnalysed(bool aParity)
	{
	iAnalysed = aParity;
	}

void LocalisationLine::Empty()
	{
	iData.clear();
	}

bool LocalisationLine::IsLineAnalysed()
	{
	return iAnalysed;
	}

/******************************************************************************************
**
** LocalisationComment
**
******************************************************************************************/


LocalisationComment::LocalisationComment(LocalisationLine & aComment)
	:iOriginalData(aComment)
	{
	iData.clear();
	iOptionalLinesToAdd.clear();
	}

LocalisationComment::~LocalisationComment()
	{
	}

String LocalisationComment::GetFileName()
	{
	return iOriginalData.GetFileName();
	}

int LocalisationComment::GetFirstLineOfComment()
	{
	return iOriginalData.GetFirstLine();
	}

int LocalisationComment::GetLastLineOfComment()
	{
	return iOriginalData.GetLastLine();
	}

int LocalisationComment::GetNumberOfOptionalLinesToAdd()
	{
	return iOptionalLinesToAdd.size();
	}

unsigned int LocalisationComment::GetNumberOfTokensInComment()
	{
	return iOriginalData.GetNumberOfTokens();
	}

String LocalisationComment::GetOriginalElement(unsigned int aIndex)
	{
	return iOriginalData.GetElement(aIndex);
	}

void LocalisationComment::AddDataLine(LocalisationLine aDataLine)
	{
	iData.push_back( aDataLine );
	}

unsigned int LocalisationComment::GetNumberOfTokensInLine(unsigned int aIndex)
	{
	return iData[aIndex].GetNumberOfTokens();
	}

LocalisationLine LocalisationComment::GetLine(unsigned int aIndex)
	{
	return iData[aIndex];
	}

void LocalisationComment::SetAnalysed(unsigned int aIndex, bool aParity)
	{
	iData[aIndex].SetAnalysed(aParity);
	}

void LocalisationComment::SetLastLine(int aLineNumber)
	{
	iOriginalData.SetLastLine(aLineNumber);
	}

bool LocalisationComment::IsLineAnalysed(unsigned int aIndex)
	{
	return iData[aIndex].IsLineAnalysed();
	}

void LocalisationComment::SetWholeTagAnalysed(bool aParity)
	{
	iOriginalData.SetAnalysed(aParity);
	}

String LocalisationComment::GetElementFromLine(unsigned int aLine, unsigned int aElement)
	{
	return iData[aLine].GetElement(aElement);
	}

int LocalisationComment::GetFirstLine(unsigned int aIndex)
	{
	return iData[aIndex].GetFirstLine();
	}

void LocalisationComment::AddOptionalData(String aOptionalData)
	{
	iOptionalLinesToAdd.push_back(aOptionalData);
	}

String LocalisationComment::GetOptionalLineToAdd(unsigned int aLine)
	{
	return iOptionalLinesToAdd[aLine];
	}

unsigned int LocalisationComment::GetNumberOfLinesInComment()
	{
	return iData.size();
	}

/******************************************************************************************
**
** WarningToOutput
**
******************************************************************************************/

WarningToOutput::WarningToOutput(const String aFileName, int aLineNumber, String aComment)
	:iFileName(aFileName),
	iLineNumber(aLineNumber),
	iComment(aComment)
	{}

WarningToOutput::WarningToOutput(const WarningToOutput & aWarningToOutput)
	:iFileName(aWarningToOutput.iFileName),
	iLineNumber(aWarningToOutput.iLineNumber),
	iComment(aWarningToOutput.iComment)
	{}
	
WarningToOutput::~WarningToOutput()
	{}

const String WarningToOutput::GetFileName()
	{
	return iFileName;
	}

int WarningToOutput::GetLineNumber()
	{
	return iLineNumber;
	}

String WarningToOutput::GetComment()
	{
	return iComment;
	}

WarningToOutput& WarningToOutput::operator=(const WarningToOutput& aWarningToOutput)
	{
	if(&aWarningToOutput==this)
		return *this;
	iFileName = aWarningToOutput.iFileName;
	iLineNumber = aWarningToOutput.iLineNumber;
	iComment = aWarningToOutput.iComment;
	return *this;
	}

/******************************************************************************************
**
** GlobalLocalisationData
**
******************************************************************************************/

GlobalLocalisationData::GlobalLocalisationData() 
	:iCommentStarted(true)
	{
	iTypes[0] = "single";
	iTypes[1] = "multiple";
	iTypes[2] = "void";
	iTypes[3] = "text";
	iRlsTypes[0] = "rls_string";
	iRlsTypes[1] = "rls_string8";
	iRlsTypes[2] = "rls_byte";
	iRlsTypes[3] = "rls_word";
	iRlsTypes[4] = "rls_long";
	iRlsTypes[5] = "rls_double";
	iRlsTypes[6] = "RESOURCE";
	iCommentTagStore.clear();
	iCommentDefinitions.clear();
	iWarningStore.clear();
	}

GlobalLocalisationData::~GlobalLocalisationData() 
	{
	}
	
int GlobalLocalisationData::NeedToAddDefaultData(String aFileName, int aFileLine)
	{
	for(unsigned int j=0; j<iCommentTagStore.size(); j++)
		{
		String fileName = iCommentTagStore[j].GetFileName();
		if(fileName == aFileName)
			{
			int lastLine = iCommentTagStore[j].GetLastLineOfComment();
			if(lastLine == aFileLine)
				{
				if(iCommentTagStore[j].GetNumberOfOptionalLinesToAdd())
					return j; // returns the index of the localisation comment which needs default data adding
				else
					return -1; // if default data doesn't need adding for this rls item then returns -1
				}
			}
		}
	return -1;
	}

void GlobalLocalisationData::AddCommentToStore()
	{
	LocalisationComment aLocalisationComment( iTempCommentStore );
	iCommentTagStore.push_back(aLocalisationComment);
	}

void GlobalLocalisationData::SetStart(String aFileName, int aLineNumber)
	{
	if(iCommentStarted == false) //means that any time other than first time, contents will be added to Store
		{
		AddCommentToStore();
		iTempCommentStore.Reset(aFileName, aLineNumber); // empties the temporary comment store
		}
	else
		{
		iTempCommentStore.SetFirstLine( aLineNumber );
		iTempCommentStore.SetFileName( aFileName );
		}
	iCommentStarted = false;
	}

void GlobalLocalisationData::AnalyseLocalisationData()
	{
	for(unsigned int i = 0; i < iCommentTagStore.size(); i++)
		{
		ParseCommentTags(i);
		}
	CheckIfCommentTagsFullyAnalysed();
	CheckRlsEntries();
	}

void GlobalLocalisationData::ParseCommentTags(unsigned int i)
	{
	LocalisationLine tagCommandLine;
	int newLinesSeen = 0;
	int currentIndex;
	int state = EStartOfComment;
	tagCommandLine.SetFileName(iCommentTagStore[i].GetFileName());
	int startLineNumber = iCommentTagStore[i].GetFirstLineOfComment();

	// parse all the comment tags to find definitions and values
	unsigned int originalNumberOfTokensInComment = iCommentTagStore[i].GetNumberOfTokensInComment();
	for(unsigned int j = 0; j < originalNumberOfTokensInComment; j++)
		{
		bool wordIsNewLine = iCommentTagStore[i].GetOriginalElement(j) == "\n" ? true : false;

		switch(state)
			{
			case(EStartOfComment):
				switch(wordIsNewLine)
					{
					case(true): 
						state = ENewLineAtStart;
						newLinesSeen++;
						break;
					default: 
						state = EAfterFirstWordOfTag;
						tagCommandLine.SetFirstLine(startLineNumber + newLinesSeen);
						tagCommandLine.SetLastLine(tagCommandLine.GetFirstLine());
 						tagCommandLine.AddElement(iCommentTagStore[i].GetOriginalElement(j));
					}
				break;
			case(EAfterFirstWordOfTag):
				switch(wordIsNewLine)
					{
					case(true):
						state = ENewLineInTag;
						newLinesSeen++;
						break;
					default:
						state = EInTag;
						tagCommandLine.AddElement( iCommentTagStore[i].GetOriginalElement(j) );
					}
				break;
			case(ENewLineAtStart):
				switch(wordIsNewLine)
					{
					case(true):
						newLinesSeen++;
						break;
					default:
						state = EAfterFirstWordOfTag;
						tagCommandLine.SetFirstLine(startLineNumber + newLinesSeen);
						tagCommandLine.SetLastLine( tagCommandLine.GetFirstLine() );
						tagCommandLine.AddElement( iCommentTagStore[i].GetOriginalElement(j) );
					}
				break;
			case(EInTag):
				switch(wordIsNewLine)
					{
					case(true):
						state = ENewLineInTag;
						newLinesSeen++;
						break;
					default:
						state = EInTag;
						tagCommandLine.AddElement( iCommentTagStore[i].GetOriginalElement(j) );
					}
				break;
			case(ENewLineInTag):
				{
				if(wordIsNewLine)
					{
					newLinesSeen++;
					}
				else if( iCommentTagStore[i].GetOriginalElement(j).FindSubString("@")==0)
					{
					iCommentTagStore[i].AddDataLine( tagCommandLine );
					currentIndex = iCommentTagStore[i].GetNumberOfLinesInComment()-1;
					Process(i, currentIndex);
					tagCommandLine.Empty();
					tagCommandLine.AddElement(iCommentTagStore[i].GetOriginalElement(j));
					tagCommandLine.SetFirstLine( startLineNumber + newLinesSeen );
					tagCommandLine.SetLastLine( tagCommandLine.GetFirstLine() );
					state = EAfterFirstWordOfTag;
					}
				else
					{
					state = EInTag;
					tagCommandLine.AddElement(iCommentTagStore[i].GetOriginalElement(j));
					tagCommandLine.SetLastLine( startLineNumber + newLinesSeen );
					}
				}
			}
		}

	if( tagCommandLine.GetNumberOfTokens() )
		{
		iCommentTagStore[i].AddDataLine( tagCommandLine );
		currentIndex = iCommentTagStore[i].GetNumberOfLinesInComment()-1;
		Process( i, currentIndex ); // process the current tag and parameters
		}
	iCommentTagStore[i].SetLastLine( startLineNumber + newLinesSeen );
	}


void GlobalLocalisationData::CheckIfCommentTagsFullyAnalysed()
	{
	for(unsigned int i = 0; i < iCommentTagStore.size(); i++)	//for each of the original comments sets whether or not all parts analysed
		{
		bool allAnalysed = true; //set to true then see if any are not analysed in which case will set to false
		unsigned int numberOfLinesInComment = iCommentTagStore[i].GetNumberOfLinesInComment();
		for(unsigned int j=0;j < numberOfLinesInComment && allAnalysed; j++)
			{
			if(iCommentTagStore[i].IsLineAnalysed(j) == false)
				allAnalysed = false;
			}
		if(allAnalysed)
			{
			iCommentTagStore[i].SetWholeTagAnalysed( true );
			}
		}
	}

void GlobalLocalisationData::CheckRlsEntries()
	{
	// check each rls item to see if there is a localisation comment before it

	TNameIndex::iterator end = pG->RlsNameIndex.end();
	String rlsTypeName;
	for (TNameIndex::iterator k = pG->RlsNameIndex.begin(); k != end; ++k)
		{
		int index = k->second;
		int rlsLineNumber = pG->RlsValues[index].iLineNumber;
		switch(pG->RlsValues[index].iType) 
			{
			case(ERlsString):
			case(ERlsStringChar):
				rlsTypeName = iRlsTypes[0];
				break;
			case(ERlsString8):
				rlsTypeName = iRlsTypes[1];
				break;
			case(ERlsByte):
			case(ERlsByteChar):
				rlsTypeName = iRlsTypes[2];
				break;
			case(ERlsWord):
			case(ERlsWordChar):
				rlsTypeName = iRlsTypes[3];
				break;
			case(ERlsLong):
			case(ERlsLongChar):
				rlsTypeName = iRlsTypes[4];
				break;
			case(ERlsDouble):
				rlsTypeName = iRlsTypes[5];
				break;
			default:
				rlsTypeName="";
			}
		const String rlsFileName = *(pG->RlsValues[index].iFileName);		
		int commentOfInterest = FindIndex(rlsFileName, rlsLineNumber);
		if(commentOfInterest == -1) // signifies there is no comment before rls declaration
			{
			Message * message = pG->Messages.GetEntry(LT_005);
			if(message->GetActivated())
				{
				pGL->AddWarningToStore(rlsFileName, rlsLineNumber, message->GetMessageOutput());
				}
			}
		else //know that there exists a tag directly before the rls item
			{
			CheckCommands(rlsTypeName, commentOfInterest);
			}
		}
	}

bool GlobalLocalisationData::LocalisationCommentsExist()
	{
	return (iCommentTagStore.size()) ? true : false;
	}

void GlobalLocalisationData::CheckCommands(String aRlsTypeName, int aCommentOfInterest)
	{
	std::vector<String> commentsSeen;
	unsigned int numberOfLines = iCommentTagStore[aCommentOfInterest].GetNumberOfLinesInComment();
	for(unsigned int j=0; j<numberOfLines; j++) // for each command in the particular comment
		{
		if(iCommentTagStore[aCommentOfInterest].IsLineAnalysed(j) == false)
			{
			iCommentTagStore[aCommentOfInterest].SetAnalysed( j, true );
			String commandName = iCommentTagStore[aCommentOfInterest].GetElementFromLine( j, 0 ); //gets tag value
			int length = commandName.Length();
			String tempCommandName = commandName.ExtractSubString(1, length-1); //strips off @ symbol

			int definitionNumber = iCommentDefinitions.size();
			for(int k=0; k<definitionNumber; k++) // finds tempCommandName in iCommentDefinitions
				if(iCommentDefinitions[k].GetCommandName() == tempCommandName)
					definitionNumber = k;

			if(definitionNumber == (int)iCommentDefinitions.size()) // if tempCommandName not found
				{
				Message * message = pG->Messages.GetEntry(LT_006);
				String fileName = iCommentTagStore[aCommentOfInterest].GetFileName();
				int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
				if(message->GetActivated())
					{
					pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
					}
				}
			else if(iCommentDefinitions[definitionNumber].TagDeclared() == false) // if tempCommandName not declared
				{
				Message * message = pG->Messages.GetEntry(LT_006);
				String fileName = iCommentTagStore[aCommentOfInterest].GetFileName();
				int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
				if(message->GetActivated())
					{
					pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
					}
				}
			else 
				{
				bool commentAlreadySeen=false;
				unsigned int numberOfCommentsSeen = commentsSeen.size();
				for(unsigned int k=0; k<numberOfCommentsSeen && commentAlreadySeen == false ;k++) 
					{ // checks in the commentsSeen vector for whether or not the tag has already been used
					if(commandName==commentsSeen[k])
						{
						Message * message = pG->Messages.GetEntry(LT_007);
						String fileName = iCommentTagStore[aCommentOfInterest].GetFileName();
						int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						commentAlreadySeen = true;
						}
					}
				if(commentAlreadySeen == false)
					{
					commentsSeen.push_back(commandName); // new tag so store it
					if(iCommentDefinitions[definitionNumber].IsTagLegalForRlsType(aRlsTypeName) == false)
						{
						Message * message = pG->Messages.GetEntry(LT_008);
						String fileName =iCommentTagStore[aCommentOfInterest].GetFileName();
						int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						}
					else
						{
						String parameterType = iCommentDefinitions[definitionNumber].GetParameterType();
						int numberOfArgs = iCommentTagStore[aCommentOfInterest].GetNumberOfTokensInLine(j) - 1;
						if(parameterType == "void")
							{
							if(numberOfArgs!=0) //void must have no arguments
								{	
								Message * message = pG->Messages.GetEntry(LT_009);
								String fileName =iCommentTagStore[aCommentOfInterest].GetFileName();
								int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
								if(message->GetActivated())
									{
									pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
									}
								}
							}
						else if(parameterType == "single")
							{
							if(numberOfArgs!=1) //single must have one argument
								{
								Message * message = pG->Messages.GetEntry(LT_010);
								String fileName =iCommentTagStore[aCommentOfInterest].GetFileName();
								int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
								if(message->GetActivated())
									{
									pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
									}							
								}
							else
								{
								CheckParametersArePermitted(aCommentOfInterest, j, definitionNumber);
								}
							}
						else if(parameterType == "multiple")
							{
							if(numberOfArgs<1) // multiple must have at least one argument
								{
								Message * message = pG->Messages.GetEntry(LT_011);
								String fileName =iCommentTagStore[aCommentOfInterest].GetFileName();
								int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
								if(message->GetActivated())
									{
									pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
									}	
								}
							else
								{
								CheckParametersArePermitted(aCommentOfInterest, j, definitionNumber);
								}
							}
						else if(parameterType == "text")
							{
							if(numberOfArgs<1) // text must have somethign after it so numberOfArgs must be >= 1
								{
								Message * message = pG->Messages.GetEntry(LT_012);
								String fileName =iCommentTagStore[aCommentOfInterest].GetFileName();
								int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(j);
								if(message->GetActivated())
									{
									pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
									}	
								}
							}
						}
					}
				}
			}
		}
	CheckWhetherAllCommentsPresent(aRlsTypeName, aCommentOfInterest, commentsSeen);
	}

void GlobalLocalisationData::CheckWhetherAllCommentsPresent(String aRlsTypeName, int aCommentOfInterest, std::vector<String> aCommentsSeen)
	{
	for(unsigned int j=0; j<iCommentDefinitions.size(); j++)
		{
		bool commentAlreadyUsed = false;
		for(unsigned int k=0; k<aCommentsSeen.size() && commentAlreadyUsed == false;k++)
			{
			String storedCommandName = iCommentDefinitions[j].GetCommandName();
			String extractedCommandName = aCommentsSeen[k].ExtractSubString(1,aCommentsSeen[k].Length()-1);
			if(storedCommandName == extractedCommandName)
				{
				//this command already used in the comment tag
				commentAlreadyUsed = true;
				}
			}
		if(commentAlreadyUsed == false) // if tag wasn't present in the localisation comment
			{
			int requirementStatus = iCommentDefinitions[j].GetRequirementStatus(aRlsTypeName);
			if(iCommentDefinitions[j].GetRequirementStatus(aRlsTypeName) == ERequired) // if required emit warning
				{
				Message * message = pG->Messages.GetEntry(LT_013);
				String fileName = iCommentTagStore[aCommentOfInterest].GetFileName();
				int lineNumber = iCommentTagStore[aCommentOfInterest].GetLastLineOfComment() + 1;
				if(message->GetActivated())
					{
					String comment = message->GetMessageOutput();
					comment += iCommentDefinitions[j].GetCommandName();
					pGL->AddWarningToStore(fileName, lineNumber, comment);
					}
				}
			else if(requirementStatus == EOptionalWithDefault) // if optional check for default and add to file if default exists
				{
				String dataToAdd = iCommentDefinitions[j].CheckIfOptionalAndHasDefault(aRlsTypeName);
				if(dataToAdd != "")
					{
					iCommentTagStore[aCommentOfInterest].AddOptionalData(dataToAdd);
					}
				}
			}
		}
	}

void GlobalLocalisationData::CheckParametersArePermitted(int aCommentOfInterest, int aLineInComment, int aDefinitionNumber)
	{
	unsigned int elementsInLine = iCommentTagStore[aCommentOfInterest].GetNumberOfTokensInLine(aLineInComment);
	for(unsigned int j=1; j< elementsInLine;j++)
		{ // start iterating at 1 as 0th element is tag name
		bool parameterPermitted = false;
		unsigned int permissibleParameters = iCommentDefinitions[aDefinitionNumber].GetNumberOfPermissibleParameters();
		for(unsigned int k=0; k<permissibleParameters && parameterPermitted==false;k++)
			{
			if(iCommentTagStore[aCommentOfInterest].GetElementFromLine(aLineInComment,j)== iCommentDefinitions[aDefinitionNumber].GetPermissibleParameter(k))
				{
				parameterPermitted = true;
				}
			}
		if(parameterPermitted == false)
			{
			Message * message = pG->Messages.GetEntry(LT_014);
			String fileName = iCommentTagStore[aCommentOfInterest].GetFileName();
			int lineNumber = iCommentTagStore[aCommentOfInterest].GetFirstLine(aLineInComment);
			if(message->GetActivated())
				{
				pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
				}
			return;
			}
		}
	}

int GlobalLocalisationData::FindIndex(const String aFileName, int aLineNumber)
	{
	for(unsigned int j=0;j<iCommentTagStore.size(); j++)
		{
		if(iCommentTagStore[j].GetFileName() == aFileName)
			{
			if(iCommentTagStore[j].GetLastLineOfComment() == aLineNumber - 1)
				{
				return j;
				}
			}
		}
	return -1; //signifies that there is no localisation comment preceding this rls line
	}


bool GlobalLocalisationData::CheckFirstIsCommand(LocalisationLine aCommandLine)
	{
	return aCommandLine.GetElement(0).FindSubString("@") == 0 ? true : false;
	}

void GlobalLocalisationData::StoreFinalComment()
	{
	if(iTempCommentStore.GetFirstLine() >= 0)
		AddCommentToStore();
	}

void GlobalLocalisationData::StoreComment(String aString) 
	{
	iTempCommentStore.AddElement(aString);
	}

void GlobalLocalisationData::Process(unsigned int aStoreIndex, unsigned int aCurrentIndex)
	{
	LocalisationLine tagCommandLine(iCommentTagStore[aStoreIndex].GetLine(aCurrentIndex));
	String firstWord = tagCommandLine.GetElement(0);
	iCommentTagStore[aStoreIndex].SetAnalysed(aCurrentIndex, true); // will reset to false if turns out not to be a declaration
	if(firstWord == "@declaretag") 
		ProcessDeclaration(aStoreIndex, aCurrentIndex);
	else if(firstWord == "@tagvalue")
		ProcessTagParameter(aStoreIndex, aCurrentIndex);
	else if(firstWord == "@tagoptional")
		{
		ProcessOptional(aStoreIndex, aCurrentIndex);
		}
	else if(firstWord == "@tagrequired")
		ProcessRequired(aStoreIndex, aCurrentIndex);
	else if(firstWord[0]=='@') // must be a tag like 'localise' or 'description'
		{
		iCommentTagStore[aStoreIndex].SetAnalysed(aCurrentIndex, false);
		}
	else // first word on a line (not wrapping over from a previous line) does not start with an @ symbol, enit warning
		{
		Message * message = pG->Messages.GetEntry(LT_015);
		String fileName = tagCommandLine.GetFileName();
		int lineNumber = tagCommandLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		}
	}

void GlobalLocalisationData::ProcessDeclaration(unsigned int aStoreIndex, unsigned int aCurrentIndex)
	{
	LocalisationLine tagDeclarationLine(iCommentTagStore[aStoreIndex].GetLine(aCurrentIndex));
	if(tagDeclarationLine.GetNumberOfTokens()!=3) // tag declaration lines must have three words in them
		{
		Message * message = pG->Messages.GetEntry(LT_016);
		String fileName = tagDeclarationLine.GetFileName();
		int lineNumber = tagDeclarationLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}
	String tagType = tagDeclarationLine.GetElement(1);
	bool acceptableParameter = false; // check that tagType is one of void, single, multiple or text
	for(int i=0; i<4 && acceptableParameter == false; i++)
		{
		if(tagType == iTypes[i])
			{
			acceptableParameter = true;
			}
		}
	if(acceptableParameter == false)
		{
		Message * message = pG->Messages.GetEntry(LT_017);
		String fileName = tagDeclarationLine.GetFileName();
		int lineNumber = tagDeclarationLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}
	if(tagDeclarationLine.GetElement(2).FindSubString("@")==0)
		{
		Message * message = pG->Messages.GetEntry(LT_018);
		String fileName = tagDeclarationLine.GetFileName();
		int lineNumber = tagDeclarationLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}
	String tagName = tagDeclarationLine.GetElement(2);
	CommentTagPair dataToAdd(tagName, tagType, true);
	int entryNumber = GetTagDeclaredIndex(tagName); // check whether tag has already been declared
	if(entryNumber == EStringNotFound) // if not then add to store
		{
		iCommentDefinitions.push_back( dataToAdd );
		}
	else if(iCommentDefinitions[entryNumber].TagDeclared()) // emit warning if declared
		{
		Message * message = pG->Messages.GetEntry(LT_019);
		String fileName = tagDeclarationLine.GetFileName();
		int lineNumber = tagDeclarationLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		}
	else
		iCommentDefinitions[entryNumber].SetTagDeclared(); // set tag as declared
	}

void GlobalLocalisationData::StoreTagParameter(String aCommandName, String aParameter)
	{
	int entryNumber = GetTagDeclaredIndex(aCommandName);
	iCommentDefinitions[entryNumber].AddPermissibleParameter(aParameter);
	}

int GlobalLocalisationData::GetTagDeclaredIndex(String aCommandName)
	{ // return index number of tag in iCommentDefinitions store
	for(unsigned int j = 0; j < iCommentDefinitions.size(); j++)
		{
		if(iCommentDefinitions[j].GetCommandName() == aCommandName)
			{
			return j;
			}
		}
	return -1;
	}

void GlobalLocalisationData::ProcessTagParameter(unsigned int aStoreIndex, unsigned int aCurrentIndex)
	{
	LocalisationLine tagParameterLine(iCommentTagStore[aStoreIndex].GetLine(aCurrentIndex));
	if(tagParameterLine.GetNumberOfTokens()!=3)
		{
		Message * message = pG->Messages.GetEntry(LT_020);
		String fileName = tagParameterLine.GetFileName();
		int lineNumber = tagParameterLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}
	String tagName = tagParameterLine.GetElement(1);
	if(tagName.FindSubString("@") == 0)
		{
		Message * message = pG->Messages.GetEntry(LT_021);
		String fileName = tagParameterLine.GetFileName();
		int lineNumber = tagParameterLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}
	int entryNumber = GetTagDeclaredIndex(tagName);
	String parameter = tagParameterLine.GetElement(2);
	if(entryNumber == -1) // if tag not declared then add to store, doesn't set tag as declared though as haven't seen a @declaretag line for this tag
		{
		CommentTagPair dataToAdd(tagName);
		iCommentDefinitions.push_back(dataToAdd);
		}
	else
		{
		// check whether this parameter has alreasy been declared for this tag
		bool alreadyDeclared = iCommentDefinitions[entryNumber].IsParameterAlreadyDeclared(parameter);
		if(alreadyDeclared)
			{
			Message * message = pG->Messages.GetEntry(LT_022);
			String fileName = tagParameterLine.GetFileName();
			int lineNumber = tagParameterLine.GetFirstLine();
			if(message->GetActivated())
				{
				pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
				}
			return;
			}
		}
	StoreTagParameter(tagName, parameter); // store the parameter if all conditions met
	}

void GlobalLocalisationData::ProcessRequired(unsigned int aStoreIndex, unsigned int aCurrentIndex)
	{
	LocalisationLine tagRequiredLine(iCommentTagStore[aStoreIndex].GetLine(aCurrentIndex));

	// @tagrequired line must consist of three distinct tokens

	if(tagRequiredLine.GetNumberOfTokens()!=3)
		{
		Message * message = pG->Messages.GetEntry(LT_023);
		String fileName = tagRequiredLine.GetFileName();
		int lineNumber = tagRequiredLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}

	// get the tag name and check it doesn't start with an @ symbol
	
	String tagName = tagRequiredLine.GetElement(1);
	if(tagName.FindSubString("@")==0)
		{
		Message * message = pG->Messages.GetEntry(LT_024);
		String fileName = tagRequiredLine.GetFileName();
		int lineNumber = tagRequiredLine.GetFirstLine();
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}

	String rlsItem = tagRequiredLine.GetElement(2);

	//check that the rls type passed is one of the legally allowed values

	bool validRlsFlag = false;
	for(int j=0; (j < ENumberOfRlsTypes) && (validRlsFlag == false); j++)
		{
		if(rlsItem == iRlsTypes[j])
			{
			validRlsFlag = true;
			}
		}
	if(validRlsFlag == false)
		{
		Message * message = pG->Messages.GetEntry(LT_036);
		String fileName = tagRequiredLine.GetFileName();
		int lineNumber = tagRequiredLine.GetFirstLine();
		if(message->GetActivated())
			{
			String comment = message->GetMessageOutput();
			comment += rlsItem;
			pGL->AddWarningToStore(fileName, lineNumber, comment);
			}
		return;
		}
	

	// store the data

	int entryNumber = GetTagDeclaredIndex(tagName);
	if(entryNumber == -1)
		{
		CommentTagPair dataToAdd(tagName);
		iCommentDefinitions.push_back(dataToAdd);
		}
	else
		{
		int requirementStatus = iCommentDefinitions[entryNumber].GetRequirementStatus(rlsItem);
		if(requirementStatus != EForbidden)
			{
			Message * message = pG->Messages.GetEntry(LT_025);
			String fileName = tagRequiredLine.GetFileName();
			int lineNumber = tagRequiredLine.GetFirstLine();
			if(message->GetActivated())
				{
				pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
				}
			return;
			}
		}
	StoreRlsItem(tagName, tagRequiredLine.GetElement(2), ERequired);
	}

void GlobalLocalisationData::StoreRlsItem(String aCommandName, String aRlsItem, int aRequired, String aDefault)
	{
	int entryNumber = GetTagDeclaredIndex(aCommandName);
	iCommentDefinitions[entryNumber].AddRlsItem(aRlsItem,aRequired,aDefault);
	}

bool GlobalLocalisationData::IsAnRlsItem(String aString) const
	{
	//check whether aString is one of the legally allowed rls items

	for(int j=0; j < ENumberOfRlsTypes ; j++)
		{
		if(aString == iRlsTypes[j])
			{
			return true;
			}
		}
	return false;
	}

void GlobalLocalisationData::ProcessOptional(unsigned int aStoreIndex, unsigned int aCurrentIndex)
	{
	LocalisationLine tagOptionalLine(iCommentTagStore[aStoreIndex].GetLine(aCurrentIndex));
	int tagLineSize = tagOptionalLine.GetNumberOfTokens();
	
	String fileName = tagOptionalLine.GetFileName();
	int lineNumber = tagOptionalLine.GetFirstLine();
	String comment;
	
	String commandName;
	String defaultValue;
	String rlsItem;
	bool validOptionalDeclaration = false; // will be set to true if the line conforms to the accepted syntax

	unsigned int state = ELookingForCommand;
	int firstOccurance;
	String currentToken;

	for(int j=1; j<tagLineSize; j++) // start at 1 as 0th element is @tagoptional
		{
		currentToken = tagOptionalLine.GetElement(j);
		switch(state)
			{
			case(ELookingForCommand):
				firstOccurance = currentToken.FindSubString("=");
				if(firstOccurance == EStringNotFound)
					{
					if(IsAnRlsItem(currentToken))
						{
						Message * message = pG->Messages.GetEntry(LT_038);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						return;
						}
					else
						{
						commandName = currentToken;
						state = ELookingForEqualSignOrRlsItem;
						}
					}
				else if(firstOccurance == 0) // = sign is at the start
					{
					Message * message = pG->Messages.GetEntry(LT_039);
					if(message->GetActivated())
						{
						pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
						}
					return;
					}
				else if(currentToken.FindSubString("=",firstOccurance+1)!=EStringNotFound) // more than one = sign
					{
					Message * message = pG->Messages.GetEntry(LT_027);
					if(message->GetActivated())
						{
						pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
						}
					return;
					}
				else if((unsigned int)firstOccurance == currentToken.Length() - 1) // = sign at end
					{
					commandName = currentToken.ExtractSubString( 0, currentToken.Length()-2 );
					state = ELookingForDefault;
					}
				else // know that is of from <string1>=<string2>
					{
					commandName = currentToken.ExtractSubString( 0, firstOccurance - 1 );
					defaultValue = currentToken.ExtractSubString( firstOccurance + 1, currentToken.Length()-1 );
					if(IsAnRlsItem(commandName))
						{
						Message * message = pG->Messages.GetEntry(LT_040);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						return;
						}
					else if(IsAnRlsItem(defaultValue))
						{
						Message * message = pG->Messages.GetEntry(LT_041);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						return;
						}
					else // no problems so go and look next for an rls item
						{
						state = ELookingForRlsItem;
						}
					}
				break;
			case(ELookingForEqualSignOrRlsItem):
				firstOccurance = currentToken.FindSubString("=");
				if(firstOccurance == EStringNotFound) // element must be rls item or error
					{
					if(IsAnRlsItem(currentToken))
						{
						rlsItem = currentToken;
						state = EShouldBeFinished;
						validOptionalDeclaration = true;
						}
					else // not an rls item
						{
						Message * message = pG->Messages.GetEntry(LT_035);
						if(message->GetActivated())
							{
							String comment = message->GetMessageOutput();
							comment += currentToken;
							pGL->AddWarningToStore(fileName, lineNumber, comment);
							}
						return;
						}
					}
				else if(currentToken.FindSubString("=",firstOccurance+1)!=EStringNotFound) // more than one = sign
					{
					Message * message = pG->Messages.GetEntry(LT_027);
					if(message->GetActivated())
						{
						pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
						}
					return;
					}
				else if(firstOccurance != 0) // = sign not at start
					{
					Message * message = pG->Messages.GetEntry(LT_042);
					if(message->GetActivated())
						{
						pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
						}
					return;
					}
				else if(currentToken.Length() == 1) // = sign by itself
					{
					state = ELookingForDefault;
					}
				else
					{
					defaultValue = currentToken.ExtractSubString( 1, currentToken.Length()-1 );
					if(IsAnRlsItem(defaultValue)) // if found rls item where default value should be
						{
						Message * message = pG->Messages.GetEntry(LT_041);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						return;
						}
					else
						{
						state = ELookingForRlsItem;
						}
					}
				break;
			case(ELookingForDefault):
				firstOccurance = currentToken.FindSubString("=");
				if(firstOccurance != EStringNotFound) // should not find any more equal signs on the line
					{
					Message * message = pG->Messages.GetEntry(LT_027);
					if(message->GetActivated())
						{
						pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
						}
					return;
					}
				else
					{
					defaultValue = currentToken;
					if(IsAnRlsItem(defaultValue)) //check that default value is not an rls item
						{
						Message * message = pG->Messages.GetEntry(LT_041);
						if(message->GetActivated())
							{
							pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
							}
						return;
						}
					else
						{
						state = ELookingForRlsItem;
						}
					}
				break;
			case(ELookingForRlsItem):
				rlsItem = currentToken;
				if(IsAnRlsItem(rlsItem) == false)
					{
					Message * message = pG->Messages.GetEntry(LT_035);
					if(message->GetActivated())
						{
						String comment = message->GetMessageOutput();
						comment += rlsItem;
						pGL->AddWarningToStore(fileName, lineNumber, comment);
						}
					return;
					}
				else
					{
					state = EShouldBeFinished;
					validOptionalDeclaration = true;
					}
				break;
			case(EShouldBeFinished):
				Message * message = pG->Messages.GetEntry(LT_043);
				if(message->GetActivated())
					{
					String comment = message->GetMessageOutput();
					comment += currentToken;
					pGL->AddWarningToStore(fileName, lineNumber, comment);
					}
				return;
			}
		}

	if(validOptionalDeclaration == false) // end of line reached prematurely
		{
		Message * message = pG->Messages.GetEntry(LT_044);
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		return;
		}


	int entryNumber = GetTagDeclaredIndex(commandName);
	if(entryNumber == -1)
		{
		CommentTagPair dataToAdd(commandName);
		iCommentDefinitions.push_back(dataToAdd);
		}
	else
		{
		int requirementStatus = iCommentDefinitions[entryNumber].GetRequirementStatus(rlsItem);
		if(requirementStatus != EForbidden)
			{
			Message * message = pG->Messages.GetEntry(LT_025);
			if(message->GetActivated())
				{
				pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
				}
			return;
			}
		}
	
	if(defaultValue.Length())
		StoreRlsItem(commandName, rlsItem, EOptionalWithDefault, defaultValue);
	else
		StoreRlsItem(commandName, rlsItem, EOptional);
	}
			

void GlobalLocalisationData::AddWarningToStore(const String aFileName, int aLineNumber, String aComment)
	{
	WarningToOutput warning(aFileName, aLineNumber, aComment);
	iWarningStore.push_back(warning);
	}

void GlobalLocalisationData::PrintLocalisationWarnings()
	{
	std::vector<WarningToOutput> warningStore;
	unsigned int j;
	unsigned int numberOfWarnings = iWarningStore.size();
	std::vector<int> flags;
	for(j=0 ; j<numberOfWarnings; j++)
		{
		warningStore.push_back(iWarningStore[j]);
		flags.push_back(1);
		}

	iWarningStore.clear();
	StringLess stringCompare;
	int currentBestIndex;
	int totalLeft = numberOfWarnings;
	while(totalLeft) // loop orders stored warnings alphabetically by file name and then by line number
		{
		String fileName;
		int lineNumber = -1;
		String comment;
		String currentBestFileName;
		int currentBestLineNumber = -1;
		String currentBestComment;
		bool beforeFirst = true;
		for(j=0,totalLeft=0,currentBestIndex=-1; j< numberOfWarnings; j++)
			{
			if(flags[j])
				{
				if(beforeFirst)
					{
					currentBestFileName = warningStore[j].GetFileName();
					currentBestLineNumber = warningStore[j].GetLineNumber();
					currentBestComment = warningStore[j].GetComment();
					currentBestIndex = j;
					beforeFirst = false;
					}
				else 
					{
					fileName = warningStore[j].GetFileName();
					lineNumber = warningStore[j].GetLineNumber();
					comment = warningStore[j].GetComment();
					if(!stringCompare(fileName, currentBestFileName) && !stringCompare(currentBestFileName, fileName))
						{
						if(lineNumber < currentBestLineNumber)
							{
							currentBestLineNumber = lineNumber;
							currentBestComment = comment;
							currentBestIndex = j;
							}
						}
					else if(stringCompare(fileName, currentBestFileName))
						{
						currentBestFileName = fileName;
						currentBestLineNumber = lineNumber;
						currentBestComment = comment;
						currentBestIndex = j;
						}
					}
				totalLeft++;
				}
			}
		if(currentBestIndex>=0)
			{
			flags[currentBestIndex]=0;
			WarningToOutput warning(warningStore[currentBestIndex]);
			iWarningStore.push_back(warning);
			totalLeft--;
			}
		}
	for(j=0; j< numberOfWarnings; j++) // outputs warning in alphabetical file order, by line number within a file
		{
		String fileName = iWarningStore[j].GetFileName();
		ErrorHandler::Register(& fileName, iWarningStore[j].GetLineNumber());
		ErrorHandler::OutputErrorLine(iWarningStore[j].GetComment());
		}
	}

void GlobalLocalisationData::OutputLocalisedFile(String SourceFileName)
	{ // reparse the source file and output a file with the optional tags with defaults added in

	char fileChar;
	char * fileCharAsArray = new char[2];
	fileCharAsArray[0] = 'a';
	fileCharAsArray[1]='\0';
	const char * fileName = SourceFileName.GetAssertedNonEmptyBuffer();
	ifstream iFile(fileName);

	char * LocalisationOutputFileName;
	LocalisationOutputFileName = tmpnam(NULL);
	ofstream outputFile(LocalisationOutputFileName);

	String fileLine("");
	int fileLineLength = fileLine.Length();
	int previousState;
	int state = EStartOfLine;
	int currentFileLine = 1;
	String currentFileName = SourceFileName;
	int newLineNumber = -1;
	String newFileName("");
	String newLineNumberAsString("");
	bool validLine = false;
	while(iFile.get(fileChar)) // read file character by character looking for file declaration lines
		{
		fileCharAsArray[0]=fileChar;
		if(state == EStartOfLine) // add character to store of this line
			fileLine = fileCharAsArray;
		else
			fileLine += fileCharAsArray;
		previousState = state;
		switch(state)
			{
			case(EStartOfLine):
				switch(fileChar)
					{
					case('#'):
						state = EFindLineNo;
						break;
					case('\n'):
						break;
					default:
						state = EGeneral;
					}
				break;
			case(EGeneral):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					}
				break;
			case(EFindLineNo):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					case('0'):
					case('1'):
					case('2'):
					case('3'):
					case('4'):
					case('5'):
					case('6'):
					case('7'):
					case('8'):
					case('9'):
						state = EInLineNo;
						break;
					case(' '):
					case('\t'):
						break;
					default:
						state = EGeneral;
					}
				break;
			case(EInLineNo):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					case('0'):
					case('1'):
					case('2'):
					case('3'):
					case('4'):
					case('5'):
					case('6'):
					case('7'):
					case('8'):
					case('9'):
						break;
					case(' '):
					case('\t'):
						state = EFindFileName;
						break;
					default:
						state = EGeneral;
					}
				break;
			case(EFindFileName):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					case(' '):
					case('\t'):
						break;
					case('\"'):
						state = EInFileName;
						break;
					default:
						state = EGeneral;
					}
				break;
			case(EInFileName):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					case(' '):
					case('\t'):
						state = EGeneral;
						break;
					case('\"'):
						state = EFindDigit;
						break;
					}
				break;
			case(EFindDigit):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					case(' '):
					case('\t'):
						break;
					case('1'):
					case('2'):
						state = EAfterDigit;
						break;
					default:
						state = EGeneral;
					}
				break;
			case(EAfterDigit):
				switch(fileChar)
					{
					case('\n'):
						state = EStartOfLine;
						break;
					case(' '):
					case('\t'):
						break;
					default:
						state = EGeneral;
					}
				break;
			}
		int lineOfInterest;
		switch(state)
			{
			case(EStartOfLine):
				if(validLine) //validline will be true if this line is a file line declaration
					{
					currentFileName = newFileName;
					currentFileLine = newLineNumber - 1;
					}
				validLine = false;
				newFileName.Reset();
				newLineNumber = -1;
				newLineNumberAsString.Reset();
				fileLineLength = fileLine.Length();
				lineOfInterest = NeedToAddDefaultData(currentFileName, currentFileLine); //checks if previous line was the end of a localisation tag and whether that tag has optional data to add
				if(lineOfInterest > -1)		// data to add									   
					{
					int end = fileLine.FindSubString("*/");
					String tempString;
					if(end)
						{
						tempString = fileLine.ExtractSubString(0,end-1);
						outputFile.write(tempString.GetBuffer(),tempString.Length());
						outputFile.write("\n",1);
						}
					unsigned int numberOfLinesToAdd = iCommentTagStore[lineOfInterest].GetNumberOfOptionalLinesToAdd();
					for(unsigned int j=0; j<numberOfLinesToAdd; j++)
						{
						tempString = iCommentTagStore[lineOfInterest].GetOptionalLineToAdd(j);
						const char * pTempString = tempString.GetBuffer();
						outputFile.write(pTempString,tempString.Length()-1);
						outputFile.write("\n",1);
						}
					tempString = fileLine.ExtractSubString(end, fileLine.Length()-1);
					outputFile.write(tempString.GetBuffer(),tempString.Length());
					}
				else
					outputFile.write(fileLine.GetBuffer(), fileLineLength);
				fileLine.Reset();
				currentFileLine++;
				break;
			case(EGeneral):
				if(previousState!=EGeneral)
					{
					newLineNumber = -1;
					newLineNumberAsString.Reset();
					newFileName.Reset();
					validLine = false;
					}
				break;
			case(EInLineNo):
				if(newLineNumberAsString.Length())
					newLineNumberAsString+= fileCharAsArray;
				else
					newLineNumberAsString = fileCharAsArray;
				break;
			case(EFindFileName):
				if(previousState == EInLineNo)
					{
					newLineNumber = newLineNumberAsString.Atoi();
					}
				break;
			case(EInFileName):
				if(previousState == EInFileName)
					{
					if(newFileName.Length())
						newFileName += fileCharAsArray;
					else
						newFileName  = fileCharAsArray;
					}
				break;
			case(EFindDigit):
				if(previousState == EInFileName)
					validLine = true;
				break;
			}
		}
		iFile.close();
		outputFile.close();
		if(remove(fileName) != 0)
			{
			Message * message = pG->Messages.GetEntry(LT_037);
			if(message->GetActivated())
				{
				cerr << message->GetMessageOutput() << endl;
				}			
			}
		else
			{
			rename(LocalisationOutputFileName, fileName);
			}
		delete []fileCharAsArray;
	}


#ifdef __VC32__
#pragma warning( pop )
#endif
