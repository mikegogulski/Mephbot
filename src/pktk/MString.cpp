/*  MString - Dynamic string data type library
    Copyright (C) 2000 Jesse L. Lovelace (jllovela@eos.ncsu.edu)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <fstream.h>
#include <stdio.h>
#include <stdarg.h>
#include "MString.h"

class MNode {
public:
	MNode(char ch, MNode* link = NULL) {MLink_Forward = link; MInfo = ch; } 
	MNode *MLink_Forward;
	char MInfo;
};

//Begin Static Functions ---------------------------------

static void reverse_loop( MNode * & previous, MNode * & p )
{

	if ( p ) {
		MNode * next = p->MLink_Forward;
		p->MLink_Forward = previous;
		previous = p;
		p = next;
		reverse_loop( previous, p );
	}
}

static void reverse_driver( MNode * & p )
{
	MNode * previous = 0;
	reverse_loop( previous, p );
	p = previous;
}  

static MNode* copy(MNode* p, MNode*& tailNode)
{
	MNode* head = 0;
	MNode* tail = 0;


	while(p) {
		if (!head)
			head = tail = new MNode(p->MInfo);
		else {
			tail->MLink_Forward = new MNode(p->MInfo);
			tail = tail->MLink_Forward;
		}
		p = p->MLink_Forward;
	}
	tailNode = tail;
	return head;
}

/*
// Added by Bruce Riggins
// copy a char array to a node structure
static int copy(MNode* fromNode, char * toPC)
{
	MNode* head = GetPointerAt;
	MNode* tail = 0;


	while(p) {
		if (!head)
			head = tail = new MNode(p->MInfo);
		else {
			tail->MLink_Forward = new MNode(p->MInfo);
			tail = tail->MLink_Forward;
		}
		p = p->MLink_Forward;
	}
	tailNode = tail;
	return head;
}
// End added by Bruce Riggins
*/

// Moved to internal private method
// Bruce Riggins
//static void deallocate(MNode* p)
void MString::deallocate(MNode* p)
{
	MNode* tmp;
	while (p) {
		tmp = p;
		p = p->MLink_Forward;
		delete tmp;
	}
	if (pcStr) {
		delete pcStr;
		pcStr = NULL;
//		iBufferInUse = 0;
	}
}

static int string_length(char* string)
{
	int count = 0;
	if (string == NULL)
		return count;
	while( string[count] != '\0') {
		count++;
	}
	return count;
}

static void MakeLetterLower(char& ch) {

	if ((ch >= 65) && (ch <= 90))
		ch = char(ch + 32);
}

static void MakeLetterUpper(char& ch) {

	if ((ch >= 97) && (ch <= 122))
		ch = char(ch - 32);
}

static MNode* TravelList(MNode * startPointer, int nCount) {

	MNode * tmp = startPointer;
	for (int i = 0; i < nCount; i++) {
		if (!tmp)
			return 0;
		tmp = tmp->MLink_Forward;
	}
	return tmp;
}
		
static MNode* findTail(MNode * startPointer) {
	MNode * tmp=startPointer;
	while (tmp) {
		if (!tmp->MLink_Forward)
			return tmp;
		tmp = tmp->MLink_Forward;
	}
	return startPointer;
}

MNode* MString::GetPointerAt(int nIndex) {

	MNode* tmp = headMNode;
	
	for (int i = 0; (i < nIndex); i++) {

		if (NULL == tmp)
			return NULL;

		tmp = tmp->MLink_Forward;
	}
	return tmp;
}

static MNode * MakeRightChain(const MNode *node, int nStart, MNode*& finalNode) 
{
	static int count = nStart;

	MNode * tmp = NULL;
	cout << "hi\n";
	if (!node) {
		cout << "after ! " << count << endl;
		//finalNode = NULL;
		count --;
		return NULL;
	}
	else {
		while (node) {cout << "yo\n";
			tmp = MakeRightChain(node->MLink_Forward, nStart, finalNode);
			cout << "before && " << tmp->MInfo << endl;
			cout << "next " << tmp << endl;
			if ((count >= 0) && (tmp)) {
				finalNode = new MNode(tmp->MInfo, finalNode);
				count--;
				return tmp;
			}
		}
	}
	cout << "bye\n";
	return NULL;
}


//End Static Functions ------------------------------------

//Begin Constructors --------------------------------------

MString::~MString() {
	deallocate(headMNode);
	headMNode = NULL;
	tailMNode = NULL;
	if (pcStr)
	{
		delete pcStr;
		pcStr = NULL;
	}
}

MString::MString() {  //100%
	headMNode = NULL;
	tailMNode = NULL;
	precision = 3;
	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}

MString::MString(const MString &stringSrc) { //100%
	headMNode = copy(stringSrc.headMNode, tailMNode);
	precision = 3;
	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}

MString::MString(const char ch, int nRepeat) { //100%
	if (nRepeat < 1) 
		return;

	headMNode = new MNode(ch);
	nRepeat--;
	MNode *tmp = headMNode;

	for (int i=0; i < nRepeat; i++) {
		tmp->MLink_Forward = new MNode(ch);
		tmp = tmp->MLink_Forward;
	}
	tailMNode = tmp;
	precision = 3;
	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}


MString::MString(const char* string) { //100%

	headMNode = NULL;
	tailMNode = NULL;
    precision = 3;

    int i = 0;
	if (string==NULL) 
    {
		return;
	}
	
	headMNode = new MNode(string[i]);
	MNode *tmp = headMNode;


	for (i = 1; string[i] != '\0'; i++) {
		tmp->MLink_Forward = new MNode(string[i]);

		tmp=tmp->MLink_Forward;
	}
	tailMNode = tmp;

	precision = 3;
	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}

MString::MString(const char* string, int maxlen) { //100%

	headMNode = NULL;
	tailMNode = NULL;
    precision = 3;

    int i = 0;
	if (string==NULL) 
    {
		return;
	}
	
	headMNode = new MNode(string[i]);
	MNode *tmp = headMNode;


	for (i = 1; (i < maxlen) && (string[i] != '\0'); i++) {
		tmp->MLink_Forward = new MNode(string[i]);

		tmp=tmp->MLink_Forward;
	}
	tailMNode = tmp;


	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}

MString::MString(int num) {

    headMNode = NULL;
	tailMNode = NULL;
	precision = 3;


	//deallocate(headMNode);

	int k = 1,
		single = 0;
	MString tmp;
	
	while ((num % (k)) < num) {
		single = (num % (10 * k))/k;
		tmp += char(single + '0'); //Thanks to botch for reminding me about '0'
		k = k * 10;

	}
	tmp.MakeReverse();
	headMNode = copy(tmp.headMNode, tailMNode);
	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}

MString::MString(double num) {

	headMNode = NULL;
	tailMNode = NULL;
	precision = 3;

    //deallocate(headMNode);

	MString tmp;
	tmp.SetPrecision(GetPrecision());
	tmp += num;
	headMNode = copy(tmp.headMNode, tailMNode);
	pcStr = NULL;
//	iBufferInUse = 0;
	iBufLen = -1;
}



//End Constructors ----------------------------------------

//Begin Test Functions ------------------------------------

void MString::testG() { //100%

	MNode *tmp = headMNode;

	cout << "Head = " << headMNode << endl;
	while (tmp)
	{ 
		cout << tmp->MInfo << " " << tmp->MLink_Forward << endl;
		tmp = tmp->MLink_Forward;
	}
	cout << "Tail = " << tailMNode << endl;

}

MString MString::GetGVersion() { //100%

	MString tmp;
	
	for (int i = 0; MString_VERSION[i] != '\0'; i++)
		tmp += MString_VERSION[i];
	return tmp;
}


//End Test Functions --------------------------------------

//Begin Precision Functions -------------------------------

void MString::SetPrecision(int num){ //100%
	if (num > MAX_PRECISION)  //
		return;
	precision = num;
}

int MString::GetPrecision() const { //100%

	return precision;
}

MString MString::ReturnLicense() {  //100%
	MString license;
	license = "MString - Dynamic string data type library\nCopyright (C) 2000 Jesse L. Lovelace\nThis program is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n";

	return license;
}

//End Precision Functions ---------------------------------

//Begin String as Array Functions -------------------------

int MString::GetLength() const { //100%

	int length = 0;

	if (headMNode) {
	
		MNode* tmp = headMNode;
		while (tmp) {  //while pointer is not null, count characters.
			length++;
			tmp=tmp->MLink_Forward;  //next node.
		}
	}
	return length;
}

bool MString::IsEmpty() const { //100%

	if (headMNode)
		return false;
	else 
		return true;
}

void MString::Empty() { //100%

	deallocate(headMNode);
	headMNode = tailMNode = NULL;
}

char MString::GetAt(int nIndex) const {  //100%

	if ((nIndex < 0) || (nIndex > GetLength()) || (IsEmpty()))
		return 0;   //return's null if the nIndex is to big or small.

	MNode* tmp = headMNode;

	for (int i = 0; i < nIndex; i++)
		tmp=tmp->MLink_Forward;

	return tmp->MInfo;
}

char MString::operator [](int nIndex) const {  //100%
	
	return GetAt(nIndex);
}

char& MString::operator [](int nIndex) {
	
	char * nullChar = NULL;

	if ((nIndex < 0) || (nIndex > GetLength()) || (IsEmpty()))
		return *nullChar;   //return's null if the nIndex is to big or small.

	MNode* tmp = headMNode;

	for (int i = 0; i < nIndex; i++)
		tmp=tmp->MLink_Forward;

	return tmp->MInfo;
}
	

void MString::SetAt(int nIndex, char ch) {  //100%
	
	if ((nIndex < 0) || (nIndex >= GetLength()) || (IsEmpty()))
		return;

	MNode* tmp = headMNode;

	for (int i = 0; i < nIndex; i++)
		tmp=tmp->MLink_Forward;

	tmp->MInfo = ch;
}

//End String as Array Functions ---------------------------


//Begin overloaded stream functions -----------------------


ostream& operator<< (ostream& out, const MString& string) {  //100%

	if (string.IsEmpty())
		return out;

	MNode* tmp = string.headMNode; //because friend, access via dot
	while (tmp) {   //while tmp not null go on
		out << tmp->MInfo;
		tmp = tmp->MLink_Forward;
	}
	return out;
}

istream& operator>> (istream& in, MString& string) { //100%

	char ch;
	while (in) {
	
		in.get(ch);
		string += ch;
	}

	return in;
}

istream& getline(istream& in, MString& string) { //100%
	//Idea from APString

	char ch = 0;

	while(in && ('\n' != ch)) {

		in.get(ch);
		string += ch;
	}
	
	if ('\n' == ch) 
		string += ch;

	return in;
}


//End overloaded stream functions -------------------------

//Begin Assignment/Concatination operators ----------------


const MString& MString::operator=(const MString& stringSrc) { //100%

	if (this != &stringSrc) {
		deallocate(headMNode);
		headMNode = copy(stringSrc.headMNode,tailMNode);
	}
	return *this;
}

const MString& MString::operator=(char ch) {  //100%

	deallocate(headMNode);
	headMNode = tailMNode = new MNode(ch);
	return *this;
}

const MString& MString::operator=(const char* string) {  //100%

	deallocate(headMNode);

	int i = 0;
	if (string==NULL) {
		headMNode=NULL;
		tailMNode=NULL;
		precision=3;
		return *this;
		}
	
	headMNode = new MNode(string[i]);
	MNode *tmp = headMNode;


	for (i = 1; string[i] != '\0'; i++) {
		tmp->MLink_Forward = new MNode(string[i]);

		tmp=tmp->MLink_Forward;
	}
	tailMNode = tmp;

	return *this;

}


const MString& MString::operator =(int num) {   //100%
	//Original MString

	//Thanks to Rask for the math!
	deallocate(headMNode);

	int k = 1,
		single = 0;
	MString tmp;
	
	while ((num % (k)) < num) {
		single = (num % (10 * k))/k;
		tmp += char(single + '0'); //Thanks to botch for reminding me about '0'
		k = k * 10;

	}
	tmp.MakeReverse();
	headMNode = copy(tmp.headMNode, tailMNode);
	return *this;

}

const MString& MString::operator =(double num) { //100% 
	//Original MString

	deallocate(headMNode);

	MString tmp;
	tmp.SetPrecision(GetPrecision());
	tmp += num;

	headMNode = copy(tmp.headMNode, tailMNode);
	return *this;
}

const MString& MString::operator =(float num) { //100% 
	//Original MString

	deallocate(headMNode);

	MString tmp;
	tmp.SetPrecision(GetPrecision());
	tmp += num;

	headMNode = copy(tmp.headMNode, tailMNode);
	return *this;

}

//Begin added operators by Bruce Riggins ---------------
//MString::operator char * () const
MString::operator const char * () const
{
	MString * m = (MString *) this;
	return m->GetBuffer(GetLength());
}
//End added operators by Bruce Riggins -----------------

MString operator +(const MString& string1, const MString& string2) { //100%

	MString tmpStr;
	tmpStr = string1;
	tmpStr += string2;

	return tmpStr;
}

MString operator +(const MString& string, char ch) { //100%

	MString tmpStr;
	tmpStr = string;
	tmpStr += ch;

	return tmpStr;

}

MString operator +(char ch, const MString& string) { //100%

	MString tmpStr;
	tmpStr = ch;
	tmpStr += string;

	return tmpStr;
}


MString operator +(const MString& string, char* ch) { //100%

	MString tmpStr;
	tmpStr = string;
	tmpStr += ch;

	return tmpStr;
}

MString operator +(char* ch, const MString& string) { //100%

	MString tmpStr;
	tmpStr = ch;
	tmpStr += string;

	return tmpStr;
}

MString operator +(int num, const MString& string) {

	MString tmpStr;
	tmpStr = num;
	tmpStr += string;

	return tmpStr;
}

MString operator +(const MString& string, int num) {


	MString tmpStr;
	tmpStr = string;
	tmpStr += num;

	return tmpStr;
}



const MString& MString::operator +=(const MString& string) { //100% 

	if (!(string.headMNode)) { //if string is empty do nothing.
		return *this;
	}

	MNode* toAdd = string.headMNode;

	while (toAdd) {
		if (!headMNode) {
			headMNode = tailMNode = new MNode(toAdd->MInfo);
			toAdd = toAdd->MLink_Forward;
		}
		else {
			tailMNode->MLink_Forward = new MNode(toAdd->MInfo);
			tailMNode = tailMNode->MLink_Forward;
			toAdd = toAdd->MLink_Forward;
		}
	}
	return *this;
}

const MString& MString::operator +=(char ch) { //100%

	if (ch == '\0')
		return *this;

	if (!headMNode) {
		headMNode = tailMNode = new MNode(ch);
		return *this;
	}
	else {
		tailMNode->MLink_Forward = new MNode(ch);
		tailMNode = tailMNode->MLink_Forward;
	}

	return *this;
}

const MString& MString::operator +=(char string[]) { //100%
	

	for (int i = 0; string[i] != '\0'; i++) {
	
		if (!headMNode) {
			headMNode = tailMNode = new MNode(string[i]);
		}
		else {
			tailMNode->MLink_Forward = new MNode(string[i]);
			tailMNode = tailMNode->MLink_Forward;
		}
	}
		
	return *this;
}

const MString& MString::operator +=(int num) { 
	
	//Original MString function
	//Concatinates a integer to a string (woohoo!)
	
	MString tmpStr;
	int k = 1,
		single = 0;

	
	while ((num % (k)) < num) {
		single = (num % (10 * k))/k;
		tmpStr += char(single + '0'); 
		//Thanks to botch for reminding me about '0'
		k = k * 10;

	}
	tmpStr.MakeReverse();  
	//numbers are in reverse order, need to straighten

	if (!headMNode)  { 
		// if string is empty use static copy function
		headMNode = copy(tmpStr.headMNode, tailMNode);
	}
	else {
		MNode *otherTmp = tmpStr.headMNode;
	
		for (int i = 0; (otherTmp); i++) {  
			
	
			tailMNode->MLink_Forward = new MNode(otherTmp->MInfo); 
			tailMNode = tailMNode->MLink_Forward;
			otherTmp = otherTmp->MLink_Forward;

		}
	}
		
	return *this;
}
const MString& MString::operator +=(double num) { 
	//Original MString

	MString tmpStr, tmpStr2;

	int k = 1,
		single = 0;

	long int num1 = int(num),
		power = 1;


	//THIS IS NOT A GOOD IMPLIMENTATION
	//Ideas would be helpful

	// Thanks to Danny Horswell for fixing the float bug
	while ((num1 % (k)) < num1) {
		single = (num1 % (10 * k))/k;
		tmpStr += char(single + '0'); 
		//Thanks to botch for reminding me about '0'
		k = k * 10;

	}

	tmpStr.MakeReverse();  
	//numbers are in reverse order, need to straighten


	for (int i = 0; i < precision; i++) {
		power = power * 10;
	}

	num1 = int((num - num1) * power);
	single = 0; 
	k = 1;

	while (k != power) {
		single = (num1 % (10 * k))/k;
		tmpStr2 += char(single + '0'); 
		//Thanks to botch for reminding me about '0'
		k = k * 10;

	}

	tmpStr2.MakeReverse();

	if (tmpStr2.headMNode) 
		tmpStr = tmpStr + '.' + tmpStr2;
	

	MNode* toAdd = tmpStr.headMNode;

	while (toAdd) {
		if (!headMNode) {
			headMNode = tailMNode = new MNode(toAdd->MInfo);
			toAdd = toAdd->MLink_Forward;
		}
		else {
			tailMNode->MLink_Forward = new MNode(toAdd->MInfo);
			tailMNode = tailMNode->MLink_Forward;
			toAdd = toAdd->MLink_Forward;
		}
	}
	return *this;
}

const MString& MString::operator +=(float num) { 
	//Original MString

	MString tmpStr;
	tmpStr+=double(num);
	
	MNode* toAdd = tmpStr.headMNode;

	while (toAdd) {
		if (!headMNode) {
			headMNode = tailMNode = new MNode(toAdd->MInfo);
			toAdd = toAdd->MLink_Forward;
		}
		else {
			tailMNode->MLink_Forward = new MNode(toAdd->MInfo);
			tailMNode = tailMNode->MLink_Forward;
			toAdd = toAdd->MLink_Forward;
		}
	}
	return *this;
}



//End Assignment/Concatination operators ------------------

//Begin Comparison operators ------------------------------

bool operator==(const MString& s1, const MString& s2) { //tested 90% 
	//Idea from CString

	if (s1.Compare(s2) != 0)
		return false;
	return true;
}


bool operator==(const MString& s1, const char* s2) {//tested 90%

	if (s1.Compare(s2) != 0)
		return false;
	return true;
}

bool operator==(const char* s1, const MString& s2) {//tested 90%
	if (s2.Compare(s1) != 0)
		return false;
	return true;
}

bool operator!=(const MString& s1, const MString& s2) { //tested 90% 
	//Idea from CString

	if (s1 == s2)
		return false;
	else 
		return true;
}

bool operator!=(const MString& s1, char* s2) {//tested 90% 
	
	if (s1 == s2)
		return false;
	else 
		return true;
}

bool operator!=(char* s1, const MString& s2) {//tested 90% 

	if (s1 == s2)
		return false;
	else 
		return true;
}

bool operator <(const MString& s1, const MString& s2) { //tested 90% 
	//Idea from CString

	if (s1.Compare(s2) < 0)
		return true;
	else 
		return false;
}

bool operator <(const MString& s1, char* s2) {//tested 90% 
	
	if (s1.Compare(s2) < 0)
		return true;
	else 
		return false;
}

bool operator <(char* s1, const MString& s2) {//tested 90% 

	MString tmpStr(s1);

	if (tmpStr.Compare(s2) < 0)
		return true;


	return false;
}

bool operator >(const MString& s1, const MString& s2) { //tested 90% 
	//Idea from CString

	if (s1.Compare(s2) > 0)
		return true;
	else 
		return false;
}

bool operator >(const MString& s1, char* s2) {//tested 90% 

	if (s1.Compare(s2) > 0)
		return true;
	else 
		return false;
}

bool operator >(char* s1, const MString& s2) {//tested 90% 

	if (s2.Compare(s1) < 0)
		return true;
	else
		return false;
}

bool operator <=(const MString& s1, const MString& s2) { //tested 90% 
	//Idea from CString

	if (s1.Compare(s2) <= 0)
		return true;
	else 
		return false;
}

bool operator <=(const MString& s1, char* s2){//tested 90% 
	
	if (s1.Compare(s2) <= 0)
		return true;
	else 
		return false;
}

bool operator <=(char* s1, const MString& s2) {//tested 90% 

	if (s2.Compare(s1) >= 0)
		return true;
	else
		return false;
}

bool operator >=(const MString& s1, const MString& s2) { //tested 90% 
	//Idea from CString

	if (s1.Compare(s2) >= 0)
		return true;
	else 
		return false;
}

bool operator >=(const MString& s1, char* s2) {//tested 90% 

	if (s1.Compare(s2) >= 0)
		return true;
	else 
		return false;
}

bool operator >=(char* s1, const MString& s2){//tested 90% 

	if (s2.Compare(s1) <= 0)
		return true;
	else
		return false;
}


int MString::Compare(const char* string) const { //not fully tested
	//Idea from CString
	
	//This function is pretty well optimized
	//because i didn't use [] but traversed list

	MNode *tmp = headMNode;
	int error = 1;
	for (int i = 0; string[i] != '\0'; i++) {
		error = 0;
		if (!tmp) {
			return ((i + 1) * -1);
		}
		if (tmp->MInfo > string[i]) 
			return (i + 1);		
		//because what if we are at index 0?

		if (tmp->MInfo < string[i])
			return ((i + 1) * -1);
		tmp=tmp->MLink_Forward;
	}
	return error;

}

int MString::Compare(const MString& string) const {

	
	MNode *tmpString = string.headMNode;
	MNode *tmp = headMNode;
	int error = 1;
	for (int i = 0; (tmpString); i++) {
		error = 0;
		if (!tmp) {
			return ((i+1) * -1);
		}
		if (tmp->MInfo > tmpString->MInfo) 
			return (i+1);
		if (tmp->MInfo < tmpString->MInfo)
			return ((i+1) * -1);
		tmp=tmp->MLink_Forward;
		tmpString = tmpString->MLink_Forward;
	}

	// the next 4 lines need to be re-written
	if (GetLength() > string.GetLength())
		return GetLength();
	if (GetLength() < string.GetLength())
		return (string.GetLength() * -1);
	return error;
}



int MString::CompareNoCase(const char * string) const { //not fully tested
	//Idea from CString
/*
	MString tmpStr1(string);
	MString tmpStr2; //make local copy of string, bad code, uses to much room.
	tmpStr2.headMNode = copy(headMNode, tmpStr2.tailMNode);

	tmpStr1.MakeLower();
	tmpStr2.MakeLower();


	MNode *tmpString = tmpStr1.headMNode;
	MNode *tmp = tmpStr2.headMNode;

	for (int i = 0; ((tmp) && (tmpString)); i++) {

		if (tmp->MInfo > tmpString->MInfo) 
			return i;
		if (tmp->MInfo < tmpString->MInfo)
			return (i * -1);
		tmp=tmp->MLink_Forward;
		tmpString = tmpString->MLink_Forward;
	}
	return 0;
	*/

	MNode * tmp = headMNode;
	char c1 = 0,
		c2 = 0;
	int error = 1;
	for (int i = 0; (string[i] != '\0'); i++) {
		error = 0;
		if (!tmp) {
			return ((i+1) * -1);
		}
		c1 = tmp->MInfo;
		c2 = string[i];
		MakeLetterLower(c1);
		MakeLetterLower(c2);
		if (c1 > c2)
			return (i+1);
		if (c1 < c2)
			return ((i+1) * -1);
		tmp = tmp->MLink_Forward;
	}
	return error;
}
int MString::Collate(char* string) const{ 
	//Idea from CString

	//WARNING, COLLATE CURRENTLY IS THE SAME AS COMPARE
	//BECAUSE I DON'T KNOW HOW TO IMPLIMENT IT.

	return Compare(string);
}

int MString::CollateNoCase(char* string) const{ 
	//Idea from CString

	//WARNING, COLLATE CURRENTLY IS THE SAME AS COMPARE
	//BECAUSE I DON'T KNOW HOW TO IMPLIMENT IT.

	return CompareNoCase(string);
}


//End Comparison operators --------------------------------

//Begin Extraction operators ------------------------------

MString MString::Mid(int nFirst) const {  //100%
	//Idea from CString

	MString tmp;

//	int length = GetLength();

	if (!headMNode)  //checks to see if the head pointer is null
		return tmp;

	MNode* travel = TravelList(headMNode, nFirst);

	while (travel) {
		
		tmp += travel->MInfo;  
		travel = travel->MLink_Forward;
	}

	return tmp;
}

MString MString::Mid(int nFirst, int nCount) const {  //100%
	//Idea from CString

	MString tmp;

	if (!headMNode)
		return tmp;

	MNode* travel = TravelList(headMNode, nFirst);
	while ((travel) && (nCount > 0) ) {
		nCount--;
		tmp += travel->MInfo;  
		travel = travel->MLink_Forward;
	}
	return tmp;
}

MString MString::Left(int nCount) const {  //100%
	//Idea from CString

	MString tmp;

	if (!headMNode)
		return tmp;

	MNode* travel = headMNode;
	while ((travel) && (nCount > 0)) {
		nCount--;
		tmp += travel->MInfo;  
		travel = travel->MLink_Forward;
	}
	return tmp;
}

MString MString::Right(int nCount) const {  //100%


	MString tmp;

	if (!headMNode)
		return tmp;	

	int bound = 0,
		length = GetLength();

	if (nCount > length)
		bound = length;
	else 
		bound = nCount;

	MNode* travel = TravelList(headMNode, (length - bound));

	for (int i = (length - bound); i < length; i++) {

		tmp += travel->MInfo;  
		travel = travel->MLink_Forward;
	}
	return tmp;

//	MString tmp;
//	MakeRightChain(headMNode, nCount, tmp.headMNode);

}

MString MString::SpanIncluding(char* string) const { 

	MString tmpStr;

	if ((!headMNode) || (string_length(string) < 1))
		return tmpStr;

	MNode *tmp = headMNode;

	while (tmp) {
		
		int strCount = 0;
		while (string[strCount] != '\0') {

			if (tmp->MInfo == string[strCount])
				tmpStr += tmp->MInfo;
			strCount++;
		}
		tmp = tmp->MLink_Forward;

		if (tmpStr.IsEmpty())  //because MFC exits if first character doesn't contain
			return tmpStr;     //one of the desired characters.
	}
	return tmpStr;
}

MString MString::SpanExcluding(char* string) const { 

	MString tmpStr;
	
	if ((!headMNode) || (string_length(string) < 1))
		return tmpStr;

	bool isIn = false;
	MNode *tmp = headMNode;

	while (tmp) {
		
		int strCount = 0;
		while (string[strCount] != '\0') {

			if (tmp->MInfo == string[strCount])
				isIn = true;
			strCount++;
		}
		if (isIn == false)
			tmpStr += tmp->MInfo;
		isIn = false;
		tmp = tmp->MLink_Forward;
	}
	return tmpStr;
}

char* MString::ToChar(int nStart) {

	int length = GetLength(),
		count = 0;

	char* myChar = NULL;

	if ((nStart < 0) || (nStart >= length))
		return myChar;
	

	myChar = new char[(length - nStart) + 1]; //a new char string, +1 is for null
	MNode* tmp = TravelList(headMNode, nStart);

	for (int i = nStart; i < length; i++) {

		myChar[count++] = tmp->MInfo;
		tmp = tmp->MLink_Forward;
	}

	myChar[count] = '\0'; //add ending null;
	return myChar;

}

int MString::ToInt(int nStart) {

	if ((nStart < 0) || (nStart > GetLength()))
		return 0;

	MNode*tmp = TravelList(headMNode, nStart);

	MString tmpStr;
	int power = 1,
		tmpInt = 0;

	while((tmp) && (tmp->MInfo >= '0') && (tmp->MInfo <='9')) {
		tmpStr+= tmp->MInfo;
		tmp = tmp->MLink_Forward;
	}

	tmpStr.MakeReverse();

	tmp = tmpStr.headMNode;

	while(tmp) {
		tmpInt = tmpInt + ((tmp->MInfo - '0') * power);
		power = power * 10;
		tmp = tmp->MLink_Forward;
	}
	return tmpInt;
}

//End Extraction operators --------------------------------

//Begin Other Conversions ---------------------------------

void MString::MakeUpper() { 
	
	MNode * tmp = headMNode;
	while(tmp) {
		MakeLetterUpper(tmp->MInfo);
		tmp = tmp->MLink_Forward;
	}
}

void MString::MakeLower() {

	MNode * tmp = headMNode;
	while(tmp) {
		MakeLetterLower(tmp->MInfo);
		tmp = tmp->MLink_Forward;
	}
}
	
void MString::MakeReverse() {

	if (!headMNode) 
		return;

	reverse_driver( headMNode );
	tailMNode = findTail(headMNode);
}

int MString::Replace(char chOld, char chNew) { 

	if (!headMNode) 
		return 0;

	int timesReplaced = 0;
	MNode* tmp = headMNode;

	while (tmp) {
		
		if (tmp->MInfo == chOld) {
			tmp->MInfo = chNew;
			timesReplaced++;
		}
		tmp = tmp->MLink_Forward; 
		//Thanks to Michael Scheuner for pointing out the missing line (above)

	}
	return timesReplaced;
}


int MString::Replace(char* stringOld, char* stringNew) { 
	//Idea from CString

	int size1 = string_length(stringOld),
		size2 = string_length(stringNew),
		length = GetLength(),
		count = 0,
		place = 0;

	if ( (length == 0) || (size1 < 1) || (size2 < 1) ) {
		return 0;
	}

	while (Find(stringOld, place) >= 0 ) {
		place = Find(stringOld, place);
		Delete(place, size1);
		Insert(place, stringNew);
		place+=size2;
		count++;
	}
	
	return count;
}
	
int MString::Remove(char ch){ 
	//Idea from CString  
	//TO DO: add string remove.

	int count = 0;
	while (Find(ch) >= 0) {
		Delete(Find(ch));
		count++;
	}

	return count;
}

int MString::Insert(int nIndex, char ch) { 
	//Idea from CString

	int length = GetLength();

	if (nIndex < 0)
		return -1;

	if (nIndex >= length) //i don't know if I should pad the string or not
		nIndex = length;

	if (0 == nIndex) {
		headMNode = tailMNode = new MNode(ch, headMNode);
		return 1;
	}


	if (nIndex == length) {
		tailMNode->MLink_Forward = new MNode(ch);
		tailMNode = tailMNode->MLink_Forward;
		return (length+1);
	}
	else
	{
		GetPointerAt(nIndex - 1)->MLink_Forward =
			new MNode(ch, GetPointerAt(nIndex - 1)->MLink_Forward);
	}
	return (length + 1);

}


int MString::Insert(int nIndex, char* string) { 
	//Idea from CString

	int length = GetLength();

	if (nIndex < 0)
		return -1;

	if (nIndex >= length)
		nIndex = length;

	for (int i = 0; string[i] != '\0'; i++) {
		Insert(nIndex++, string[i]);
	}

	return GetLength();
}
		
int MString::Delete(int nIndex, int nCount) { 
	//Idea from CString

	int length = GetLength();

	if ((!headMNode) || (nIndex < 0) || (nIndex > length) || (nCount < 1))
		return length;

	if (nCount > (length - nIndex))
		nCount = (length - nIndex);

	if (0 == nIndex) {
		MNode* first = headMNode;
		MNode* last = GetPointerAt((nIndex + nCount)-1);
	
		if (last->MLink_Forward) {
			if (last->MLink_Forward->MLink_Forward) {
				headMNode = last->MLink_Forward;
				last->MLink_Forward = NULL;
				deallocate(first);
			}
		}
		else {
			headMNode = 0;
			deallocate(first);
		}

	}	
	else {
		MNode* toHead = GetPointerAt(nIndex - 1);
		MNode* first = GetPointerAt(nIndex);

		MNode* last = GetPointerAt((nCount + nIndex) - 1);
	
		if (last->MLink_Forward) {
			if (last->MLink_Forward->MLink_Forward) {
				toHead->MLink_Forward = last->MLink_Forward;
				last->MLink_Forward = NULL;
				deallocate(first);
			}
			else {
				toHead->MLink_Forward=last->MLink_Forward;
				last->MLink_Forward = NULL;
				deallocate(first);
			}
		}
		else {
			toHead->MLink_Forward = NULL;
			deallocate(first);
		}
	}
	tailMNode = findTail(headMNode);

	return GetLength();

}

//Additions by Bruce Riggins 11/14/00
// Use the given data to build a string.
#ifdef WIN32
#define vsnprintf _vsnprintf
#endif
//
void MString::Format(char * sFormat, ...)
{
	char * s = new char[2000];
	va_list arglist;
	va_start(arglist, sFormat);
	vsnprintf(s, 2000, sFormat, arglist);
	va_end(arglist);
	*this = s;
	delete s;
}
// End additions by Bruce Riggins 11/14/00

void MString::Trim() { 
	//Original MString

	if (!headMNode)
		return;
	
	TrimLeft();
	TrimRight();

}

void MString::Trim(char ch) { 
	//Original MString

	if (!headMNode)
		return;
	
	TrimLeft(ch);
	TrimRight(ch);

}
void MString::Trim(char* string){

	if (!headMNode)
		return;

	TrimLeft(string);
	TrimRight(string);

}	

void MString::TrimLeft() { 
	//Idea from CString
	if (!headMNode)
		return;

	TrimLeft(' ');
	TrimLeft('\t');
	TrimLeft('\n');

}

void MString::TrimLeft(char ch) {
	if (!headMNode)
		return;

	// not using a "int length" because it will change constantly.
	while(Find(ch) == 0) {
		Delete(Find(ch));

	}
}

void MString::TrimLeft(char* string) {
	
	if (!headMNode)
		return;	

	int strLen = string_length(string);

	while(Find(string) == 0) {
		Delete(0, strLen);
	}
}

void MString::TrimRight() { 
	//Idea from CString
	if (!headMNode)
		return;

	TrimRight(' ');
	TrimRight('\t');
	TrimRight('\n');
}

void MString::TrimRight(char ch) {
	
	if (!headMNode)
		return;	
	
	// not using a "int length" because it will change constantly.
	while ( ReverseFind(ch) == (GetLength() - 1)) {
		
	Delete(ReverseFind(ch));

	}
}


void MString::TrimRight(char* string) {

	if (!headMNode)
		return;	

	int strLen = string_length(string);

	if (strLen > GetLength())
		return;

	while ( ReverseFind(string) == ( GetLength() - strLen ) ) {
		Delete(ReverseFind(string), strLen); 
	}

}

//End Other Conversions -----------------------------------

//Begin Searching -----------------------------------------

int MString::Find(char ch, int nStart) const { 
	//Idea from CString

	if (!headMNode) 
		return -1;

	MNode *tmp = TravelList(headMNode, nStart);

	for (int i = nStart; (tmp) ; i++) {
		
		if (tmp->MInfo == ch) {
			return i;
		}
		tmp = tmp->MLink_Forward;
	}
	return -1;
}


int MString::Find(char* string, int nStart) const { 
	//Idea from CString

	int strLength = string_length(string);

	if ((!headMNode) || (strLength == 0))  
		//if bigger than MString or not a string.
		return -1;

	bool found = false;
	MNode *tmp = TravelList(headMNode, nStart);

	while (tmp) {

		if (tmp->MInfo == string[0]) { 

			int strPos = 0;
			MNode* subTmp = tmp;
			
			while ( (tmp) && (string[strPos] != '\0') ) {
				if  (subTmp->MInfo == string[strPos++])
					found = true;
				else {
					found = false;
					break;
				}
				subTmp = subTmp ->MLink_Forward;
			}
			if (found == true)
				return nStart;
		}
		tmp = tmp->MLink_Forward;
		nStart++;

	}
	return -1;
}

int MString::ReverseFind(char ch) const { //Idea from CString


	if (!headMNode) 
		return -1;

	int length = GetLength();

	for (int i = (length - 1); i >= 0; i--) {
		
		if (GetAt(i) == ch) {
			return i;
		}

	}

	return -1;
}

int MString::ReverseFind(char* string) const {

	if (!headMNode) 
		return -1;

	int length = GetLength(),
		strLen = string_length(string);

	bool found = false;

	if (strLen < 1)
		return -1;

	for (int i = (length - strLen); i >= 0; i--) {
		if (GetAt(i) == string[0]) { 

			int strPos = 0;
			
			for (int k = i; k < (i + strLen); k++) {
				if  (GetAt(k) == string[strPos++])
					found = true;
				else {
					found = false;
					break;
				}
			}
			if (found == true)
				return i;
		}
	}
	return -1;
}


int MString::FindOneOf(char* string) const { 
	//Idea from CString
	if (!headMNode) 
		return -1;

	int strLen = string_length(string);

	for (int i = 0; i < strLen; i++) {
		if (Find(string[i]) >= 0)
			return i;
	}

	return -1;
}


//End Searching -------------------------------------------

//Begin Buffer Access -------------------------------------
	// Additions by Bruce Riggins 11/7/00
	// Access to string implementation buffer as "C" character array
char * MString::GetBuffer(int nMinBufLength) {
	//Idea from CString
//	char * pc = NULL;
	
	if (pcStr) {
		// if buffer is in existence already, simply delete it and start over
		delete pcStr;	
	}
	
	// create a buffer and copy list to it
  	int iLen = GetLength();
  	iLen = iLen < nMinBufLength ? nMinBufLength : iLen;
  	pcStr = new char[iLen + 1];	// room for trailing zero
  	if (pcStr) {
  		char * tmppc = pcStr;
  		MNode * pn = headMNode;
  		iBufLen = iLen;
  		while (pn) {
  			*tmppc++ = pn->MInfo;
  			pn = pn->MLink_Forward;
  		}			
  	}		
	
	return pcStr;
}

void MString::ReleaseBuffer(int nNewLength) {
	//Idea from CString
	if (pcStr){
		if (nNewLength == -1) {
		   *this = pcStr;
		}
		else {
			*this = pcStr;
		}
		
		if (pcStr) {
			delete pcStr;
			pcStr = NULL;
//			iBufferInUse = 0;
		}
	}
}
	
//	LPTSTR GetBufferSetLength(int nNewLength);	//Idea from CString
//	void FreeExtra();	//Idea from CString

// Use LockBuffer/UnlockBuffer to turn refcounting off
char * MString::LockBuffer() {
	//Idea from CString
	
	// Simple, non-functional implementation
	return pcStr;
}
	
void MString::UnlockBuffer() {
	//Idea from CString
	
	// Simple, non-functional implementation
	return;
}
	
	// End of BR additions

//End Buffer Access ---------------------------------------

