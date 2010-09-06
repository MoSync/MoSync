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


#include "messages.h"
#include "main.h"

Message::Message(String aMessage, int aIndex, String aTextIndex, bool aActivated, int aMessageType)
	:iMessage(aMessage),
	iIndex(aIndex),
	iTextIndex(aTextIndex),
	iActivated(aActivated),
	iMessageType(aMessageType)
	{
	}

Message::~Message()
	{
	}

void Message::SetActivationStatus(bool aActivationStatus)
	{
	iActivated = aActivationStatus;
	}

bool Message::GetActivated()
	{
	return iActivated;
	}

String Message::GetMessageOutput()
	{
	String messageOutput;
	if(iMessageType == KError)
		{
		messageOutput = "Error";
		}
	else if(iMessageType == KWarning)
		{
		messageOutput = "Warning";
		}
	else
		{
		// add fall back code
		}
	messageOutput += ": (";
	messageOutput += iTextIndex;
	messageOutput += ") ";
	messageOutput += iMessage;

	return messageOutput;
	}

int Message::GetIndex()
	{
	return iIndex;
	}

String Message::GetTextIndex()
	{
	return iTextIndex;
	}

Message * MessageArray::GetEntry(int aEntry)
	{
	MessageArrayIterator next(*this);
	Message * pRequiredMessage;
	while ((pRequiredMessage = next() ) != NULL)
		{
			if( pRequiredMessage->GetIndex() == aEntry )
			{ // this comparison returns true if the label is the same as the declared label
			return pRequiredMessage;
			}
		}
	return NULL;
	}

Message * MessageArray::GetTextEntry(String aEntry)
	{
	MessageArrayIterator next(*this);
	Message * pRequiredMessage;
	while ((pRequiredMessage = next() ) != NULL)
		{
			if( pRequiredMessage->GetTextIndex() == aEntry )
			{ // this comparison returns true if the label is the same as the declared label
			return pRequiredMessage;
			}
		}
	return NULL;
	}

void MessageArray::Add(Message * pNewItem)
	{
	Array::Add(pNewItem);
	}

MessageArrayIterator::MessageArrayIterator(const MessageArray & c)
	:ArrayIterator(c)
	{}

Message* MessageArrayIterator::operator() ()
	{
	return (Message *) ArrayIterator::operator() ();
	}

MessageArray::MessageArray()
	{
	const String LT_001_Message = "rls item used more than once."; // Warns if an rls item is used in more than one place but is not  declared using the 'multi' keyword. Emitted once for each rls item to which this applies.
	Add(new Message(LT_001_Message, LT_001, "001", true, KWarning));

	const String LT_002_Message = "if the rls item really must be exactly the same in all the places it is referenced from for the code to work, declare the rls item as 'multi', for example 'rls_string multi ss1 \"hello\"'. If localizers are free to change them independently, duplicate the rls item and reference a different copy each time."; // Gives information about the 'multi' keyword, only emitted once.
	Add(new Message(LT_002_Message, LT_002, "002", true, KWarning));

	const String LT_003_Message = "rls item redefined."; // Warns if two rls items have the same label.
	Add(new Message(LT_003_Message, LT_003, "003", true, KWarning));

	const String LT_004_Message = "unused rls item."; // .
	Add(new Message(LT_004_Message, LT_004, "004", true, KWarning));

	const String LT_005_Message = "rls item not preceded by a localisation tag."; // .
	Add(new Message(LT_005_Message, LT_005, "005", true, KWarning));

	const String LT_006_Message = "specified command not previously declared."; // Warns if a command is used in a localisation comment which has not been defined with @declaretag.
	Add(new Message(LT_006_Message, LT_006, "006", true, KWarning));

	const String LT_007_Message = "specified command has already been used in this comment."; // Warns if a command is used more than once in a localisation comment.
	Add(new Message(LT_007_Message, LT_007, "007", true, KWarning));

	const String LT_008_Message = "this tag cannot be used with this rls type."; // Warns if in a localisation comment a tag is used which has not been declared with either @tagoptional or @tagrequired for this rls type.
	Add(new Message(LT_008_Message, LT_008, "008", true, KWarning));

	const String LT_009_Message = "tag declared as void but has arguments."; // .
	Add(new Message(LT_009_Message, LT_009, "009", true, KWarning));

	const String LT_010_Message = "tag declared as single but does not have one argument."; // .
	Add(new Message(LT_010_Message, LT_010, "010", true, KWarning));

	const String LT_011_Message = "tag declared as multiple must have at least one argument."; // .
	Add(new Message(LT_011_Message, LT_011, "011", true, KWarning));

	const String LT_012_Message = "tag declared as text, must have at least one word after it."; // .
	Add(new Message(LT_012_Message, LT_012, "012", true, KWarning));

	const String LT_013_Message = "required tag not present. Tag name = "; // A tag declared with @tagrequired for this rls type is not present.
	Add(new Message(LT_013_Message, LT_013, "013", true, KWarning));

	const String LT_014_Message = "parameter(s) not legal for this tag."; // In a localisation comment a parameter is used with a tag which has not been declared with @tagvalue.
	Add(new Message(LT_014_Message, LT_014, "014", true, KWarning));

	const String LT_015_Message = "first word must start with an @ symbol."; // Warns if the first word in a localisation comment tag does not start with an @ symbol. N.B. This does not warn that an arbitrary line in a localisation comment does not start with an @ symbol as at this point have not checked whether this is a multi line comment.
	Add(new Message(LT_015_Message, LT_015, "015", true, KWarning));

	const String LT_016_Message = "invalid declaration line."; // This will be produced if an @declaretag line does not have exactly three words on it. i.e. should be of form @declaretag type tag-name . This warning might be produced if the line following this line does not start with an @ symbol.
	Add(new Message(LT_016_Message, LT_016, "016", true, KWarning));

	const String LT_017_Message = "illegal type."; // Emits a warning if in a @declaretag line the type is not one of void, single, multiple or text.
	Add(new Message(LT_017_Message, LT_017, "017", true, KWarning));

	const String LT_018_Message = "declared tag should not start with an @ symbol."; // In a @declaretag line the tag-name should not start with an @symbol, i.e. @declaretag single localise not @declaretag single @localise
	Add(new Message(LT_018_Message, LT_018, "018", true, KWarning));

	const String LT_019_Message = "redefinition of already declared tag."; // If two @declaretag lines appear for a particular tag.
	Add(new Message(LT_019_Message, LT_019, "019", true, KWarning));

	const String LT_020_Message = "tag value line must consist of three distinct tokens."; // i.e. @tagvalue localise yes
	Add(new Message(LT_020_Message, LT_020, "020", true, KWarning));

	const String LT_021_Message = "tag name must not start with an @ symbol in @tagvalue statement."; // .
	Add(new Message(LT_021_Message, LT_021, "021", true, KWarning));

	const String LT_022_Message = "parameter already added for this tag type."; // i.e. two or more instances of @tagvalue localise yes 
	Add(new Message(LT_022_Message, LT_022, "022", true, KWarning));

	const String LT_023_Message = "tag requirement line must consist of three distinct tokens."; // i.e. @tagrequired localise rls_string
	Add(new Message(LT_023_Message, LT_023, "023", true, KWarning));

	const String LT_024_Message = "second word should not begin with an @ symbol in @tagrequired line."; // .
	Add(new Message(LT_024_Message, LT_024, "024", true, KWarning));

	const String LT_025_Message = "parameter already declared as optional or required."; // Emitted on trying to redeclare a tag as required or optional, regardless of whether this is consistent with the previous declaration.
	Add(new Message(LT_025_Message, LT_025, "025", true, KWarning));

	const String LT_026_Message = "line has too many or too few parameters."; // UNUSED: Emitted on @tagoptional lines if there are less than two or more than four parameters, as must be one of the following forms: @tagoptional localise rls_string, @tagoptional localise=yes rls_string, @tagoptional localise =yes rls_string, @tagoptional localise= yes rls_string, @tagoptional localise = yes rls_string
	Add(new Message(LT_026_Message, LT_026, "026", true, KWarning));

	const String LT_027_Message = "invalid declaration of optional tag, more than one equals sign found in line."; // .
	Add(new Message(LT_027_Message, LT_027, "027", true, KWarning));

	const String LT_028_Message = "invalid declaration of optional tag."; // UNUSED: Emitted for syntax errors, more than one equals sign, missing tag name, missing default value if an = sign is present, = signs in odd places.
	Add(new Message(LT_028_Message, LT_028, "028", true, KWarning));

	const String LT_029_Message = "invalid declaration of optional tag, missing default value or rls item."; // UNUSED.
	Add(new Message(LT_029_Message, LT_029, "029", true, KWarning));

	const String LT_030_Message = "localised output file could not be produced."; // Warns if there are problems saving a copy of the rpp file after adding the default values to tags which have tags missing. 
	Add(new Message(LT_030_Message, LT_030, "030", true, KWarning));

	const String LT_031_Message = "internal error! rls type not handled."; // Emits error if try to use an rls item which is unknown, i.e. trying to use rls_chocolate
	Add(new Message(LT_031_Message, LT_031, "031", true, KError));

	const String LT_032_Message = "text length of rls string exceeds specified limit."; // Emits error if the length of the string given exceeds the length of the string specified in brackets, i.e. rls_string<5> STRING_1 "Hello World" as length of Hello World is > 5. This is an error rather than a warning as in the future the value <x> will be relevant to the storage used to store the string.
	Add(new Message(LT_032_Message, LT_032, "032", true, KError));

	const String LT_033_Message = "rls_string or rls_string8 used for character constant: use rls_long, rls_word or rls_byte."; // character constants should be declared as a number type (future requirement about storage allocaltion based on the type used), i.e. rls_byte CHAR_1 'a' not rls_string CHAR_1 'a'
	Add(new Message(LT_033_Message, LT_033, "033", true, KWarning));

	const String LT_034_Message = "could not find value to substitute in for label."; // UNUSED.
	Add(new Message(LT_034_Message, LT_034, "034", true, KWarning));

	const String LT_035_Message = "invalid declaration of optional tag, expecting rls item but found token: "; // .
	Add(new Message(LT_035_Message, LT_035, "035", true, KWarning));

	const String LT_036_Message = "invalid declaration of required tag, unknown rls type found, rls type = "; // .
	Add(new Message(LT_036_Message, LT_036, "036", true, KWarning));

	const String LT_037_Message = "localised output file could not be produced."; // Warning emitted if rcomp is not able to replace the existing rpp file with a localised version (with default tags added for optional non-present tags.
	Add(new Message(LT_037_Message, LT_037, "037", true, KWarning));

	const String LT_038_Message = "invalid declaration of optional tag, rls item name found where command name expected."; // .
	Add(new Message(LT_038_Message, LT_038, "038", true, KWarning));

	const String LT_039_Message = "invalid declaration of optional tag, equal sign found at the start of command name."; // .
	Add(new Message(LT_039_Message, LT_039, "039", true, KWarning));

	const String LT_040_Message = "invalid declaration of optional tag, rls item found where command name expected."; // .
	Add(new Message(LT_040_Message, LT_040, "040", true, KWarning));

	const String LT_041_Message = "invalid declaration of optional tag, rls item found where default value expected."; // .
	Add(new Message(LT_041_Message, LT_041, "041", true, KWarning));

	const String LT_042_Message = "invalid declaration of optional tag, incorrect syntax for declaring default value."; // .
	Add(new Message(LT_042_Message, LT_042, "042", true, KWarning));

	const String LT_043_Message = "invalid declaration of optional tag, found unknown tokens at end of line, first unknown token is: "; // .
	Add(new Message(LT_043_Message, LT_043, "043", true, KWarning));

	const String LT_044_Message = "invalid declaration of optional tag, end of line reached unexpectedly."; // .
	Add(new Message(LT_044_Message, LT_044, "044", true, KWarning));

	/* This warning has been added to hopefully fix a long standing defect where labels which 
	 * have not been declared are silently included in the emitted resource file. More 
	 * details at defect INC061459 */
	const String LT_045_Message = "the following label is used but has not been declared: ";
	Add(new Message(LT_045_Message, LT_045, "045", true, KWarning));

	const String LT_046_Message = "the STRUCT that this resource is based on contains a STRUCT data member which has not been used in this resource, the missing element is: ";
	Add(new Message(LT_046_Message, LT_046, "046", true, KWarning));

	const String LT_047_Message = "the STRUCT that this resource is based on contains a STRUCT[] data member which has not been used in this resource, the missing element is: ";
	Add(new Message(LT_047_Message, LT_047, "047", true, KWarning));

}
