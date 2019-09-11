
/*
**	utlString.h - class for manipulating strings.
**
**	purpose: to provide an adequate class for the manipulation
**	of strings compatible to the STL
**

  Program:   cgc util
  Module:    $RCSfile: utlString.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

**
*/

#ifndef __UTLSTRING_H__
#define __UTLSTRING_H__

#include "utlAlgo.h"

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"

#ifndef _MSC_VER
#define utlString std::string
#else
#define utlString std::basic_string<char>
#endif

#else // not using STL

#include "utl.h"
#include "utlIteratorBase.h"

/** A simpler implementation of STL class string.
 *	@author <A HREF="mailto:ulisses@watson.ibm.com">Ulisses T. Mello</a> and
 *	<A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	Resizable string.
 */
class utlString {
 public:

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlString() : d_string_p(0) { this->init(""); }

  /// copy constructor.
  utlString(const utlString& rhs, unsigned int pos =0, 
	    unsigned int npos=0) : d_string_p(0) { 
    this->init(rhs.d_string_p, pos, npos);
  }

  /// constructor from char[].
  utlString(const char* str_buf) { this->init(str_buf); }

  /// constructor from char[].
  utlString(const char* str_buf, unsigned int npos ) : d_string_p(0) { 
    this->init(str_buf, 0, npos); 
  }

  /// constructor from a char.
  utlString (int n, char c) : d_string_p(0) { this->append( n, c); }

  /// destructor.
  ~utlString() {
    if( d_string_p ) delete [] d_string_p; 
    d_string_p=0;
  }
  //@}

  /// returns the name of this object.
  static const char* getClassName() { return "utlString"; }

  /// returns this string buffer data.
  const char* data() const { return d_string_p; }

  /// returns the length of this string.
  unsigned int length() const { 
    if( d_string_p ) return strlen(d_string_p);
    else return 0;
  }

  /// returns the size of this string.
  unsigned int size() const { return this->length(); }

  /// returns the capacity of this string.
  unsigned int capacity () const { return this->length(); }

  /// returns the max size of the string buffer.
  unsigned int max_size () const { return this->length(); }

  /// returns whether this string is empty.
  bool empty () const { return this->size() == 0; }

  /** swaps the content of this string and a given string.
   *	
   *	@param rhs given string.
   */
  void swap (utlString& rhs) { 
    char* tmp = d_string_p;
    d_string_p = rhs.d_string_p;
    rhs.d_string_p = tmp;
  }

  /**   appends a given substring at the end of this string.
   *	
   *	@param s_string given substring.
   *	@param pos starting position of the substring.
   *	@param n lenght of the substring.
   *	@return this string.
   */
  utlString& append(const utlString& s_string, unsigned int pos = 0,
		    unsigned int n = 0 ) {
    if ( n == 0 ) n = s_string.size();
    return this->append(&s_string.d_string_p[pos], n);
  }
    
  /**   appends a given character array at 
   *	the end of this string.
   *	
   *	@param str_buf given character array.
   *	@return this string.
   */
  utlString& append(const char* str_buf) {
    unsigned int size = strlen(str_buf);
    return this->append(str_buf, size);
  }
    
  /**   appends a given character array at 
   *	the end of this string.
   *	
   *	@param str_buf given character array.
   *	@param npos number of characters to be appended.
   *	@return this string.
   */
  utlString& append(const char* str_buf, unsigned int npos ) {
    if( str_buf ) {
      char *tmp = d_string_p;	
      unsigned int len = this->length();
      unsigned int size = npos + 1;

      d_string_p = new char[len + size];

      strncpy(d_string_p, tmp, len);
      strncpy(d_string_p + len, str_buf, npos);
      d_string_p[len+npos] = '\0';
      if( tmp ) delete [] tmp;
    }
    return *this;
  }

  /**   appends a number of copies of a given character at 
   *	the end of this string.
   *	
   *	@param n number of copies.
   *	@param c given character.
   *	@return this string.
   */
  utlString& append(unsigned int n, char c) {
    char *pc = new char[n+1];
    memset(&pc[0], c, n);
    pc[n]='\0';
    this->append(pc, n);
    delete[] pc;
    return *this;
  }

  /// assignment / copy operator.
  utlString& assign (const utlString& rhs, unsigned int pos = 0, 
		     unsigned int n = 0 ) { 
    if (this != &rhs) {
      if( d_string_p ) delete [] d_string_p;
      d_string_p=0;
      this->init(rhs.d_string_p, pos, n);
    }
    return *this;
  }

  /// assignment method.
  utlString& assign(const char* str_buf) {
    if(d_string_p ) {
      delete [] d_string_p;
      d_string_p=0;
    }
    if( str_buf ) this->init(str_buf);
    return *this;
  }

  /// assignment method.
  utlString& assign(const char* str_buf, unsigned int npos ) {
    if(d_string_p ) {
      delete [] d_string_p;
      d_string_p=0;
    }
    if( str_buf ) this->init(str_buf, 0, npos);
    return *this;
  }

  /// assignment method.
  utlString& assign(char c) {
    if(d_string_p ) {
      delete [] d_string_p;
      d_string_p=0;
    }
    char str[2]; str[0] = c; str[1] = '\0'; 
    this->init(str);
    return *this;
  }

  /// assignment / copy operator.
  utlString& operator = (const utlString& rhs) {
    if (this != &rhs) this->assign(rhs);
    return *this;
  }

  /// assignment / copy operator.
  utlString& operator = (const char* rhs) {
    this->assign(rhs);
    return *this;
  }
  /// assignment / copy operator.
  utlString& operator = (char c) {
    this->assign(c);
    return *this;
  }

  /// comparison operator.
  int operator == ( const utlString& str ) const {
    return !this->compare ( str );
  }

  /**	comparison operator.
   *	
   *	@return 1 if this and a given string are different.
   */
  int operator != ( const utlString& str ) const {
    return !(this->operator == (str));
  }

  /// returns whether this string is lexicographically smaller than a given string.
  int operator < ( const utlString& str ) const {
    return (this->compare ( str ) < 0); 
  }

  /// returns whether this string is lexicographically larger than a given string.
  int operator > ( const utlString& str ) const {
    return (this->compare ( str ) > 0); 
  }



  /// append operator.
  utlString operator+ (const utlString& rhs) const { 
     utlString tmp (*this);
     tmp.append(rhs); 
     return tmp;
  }

  /// append operator.
  utlString operator+ (const char* rhs) const { 
     utlString tmp (*this);
     tmp.append(rhs); 
     return tmp;
  }

  /// append operator.
  utlString operator+ (char c) const { 
     utlString tmp (*this);
     tmp.append(1,c); 
     return tmp;
  }

  /// append operator.
  utlString& operator+= (const utlString& rhs) { return this->append(rhs); }

  /// append operator.
  utlString& operator+= (const char* rhs) { return this->append(rhs); }

  /// append operator.
  utlString& operator+= (char c) { return this->append(1,c); }

  /**   inserts a given substring into this string
   *	before a given position.
   *	
   *	@param rhs given substring.
   *	@param pos1 given position.
   *	@param pos2 beginning of the substring.
   *	@param npos length of the substring.
   *	@return this string.
   */
  utlString& insert ( unsigned int pos1, const utlString& rhs,
		      unsigned int pos2 = 0, unsigned int npos = 0) {
    return this->insert(pos1, &rhs.d_string_p[pos2], npos); 
  }

  /**   inserts a given character array into this string
   *	before a given position.
   *	
   *	@param s given character array.
   *	@param pos given position.
   *	@param npos number of characters to be inserted. 
   *	@return this string.
   */
  utlString& insert (unsigned int pos, const char* s, unsigned int npos=0) {
    unsigned int len = this->length();
    if (pos > len) {
      cerr << "utlString: pos > length\n";
      return *this;
    }
    char *tmp = new char[len+1];
    strcpy(tmp,d_string_p);
    this->assign(tmp,0,pos);
    if (npos==0) npos = strlen(s);
    this->append(s,npos);
    this->append(tmp, pos, len-pos);
    delete[] tmp;
    return *this;
  }

  /**   inserts a number of copies of a given character 
   *	into this string before a given position.
   *	
   *	@param pos given position.
   *	@param npos number of copies.
   *	@param c given character.
   *	@return this string.
   */
  utlString& insert (unsigned int pos, unsigned int npos, char c) {
    char *pc = new char[npos+1];
    memset(&pc[0], c, npos);
    pc[npos]='\0';
    this->insert(pos,pc);
    delete[] pc;
    return *this;
  }

  /**   erases a substring of this string.
   *	
   *	@param pos initial position of the substring.
   *	@param npos length of the substring.
   *	@return this string.
   */
  utlString& erase (unsigned int pos = 0, unsigned int npos=0) {
    if ((pos == 0) && (npos == 0)) {
      this->init("");
      return *this;
    }
    unsigned int len = this->length();
    if (npos==0) npos = len;
    char *tmp = new char[len+1];
    strcpy(tmp,d_string_p);
    if (pos==0) this->assign("");
    else this->assign(tmp,0,pos);
    this->append(tmp, pos+npos, len-pos-npos);
    delete[] tmp;
    return *this;
  }

  /**   returns a substring of this string.
   *	
   *	@param pos initial position of the substring.
   *	@param npos length of the substring.
   *	@return substring.
   */
  utlString substr (unsigned int pos = 0, unsigned int npos = 0) const {
    return utlString(*this,pos,npos);
  }

  /// read operator
  char& operator[](unsigned int i) const { return d_string_p[i]; }

  /// write operator
  char& operator[](unsigned int i) { return d_string_p[i]; }
  
  /// constant string operator.
  const char* c_str() const { return (const char*)d_string_p; }

  /**   searchs a given character into this string.
   *	
   *	@param c given character.
   *	@param pos starting position for the search.
   *	@return position of the first occurrence of the character,
   *	or length()+1, if the character was not found.
   */
  unsigned int find (char c, unsigned int pos = 0) const {
    unsigned int i;
    for(i = pos; i<this->length(); ++i) 
      if ( (*this)[i] == c) return i;
    return i++;
  }

  /**   searchs a given character into this string.
   *	
   *	@param c given character.
   *	@param pos starting position for the search.
   *	@return position of the first occurrence of the character,
   *	or length()+1, if the character was not found.
   */
  unsigned int find_first_of (char c, unsigned int pos = 0) const {
    return this->find(c,pos);
  }

  /** 	searchs a given character into this string from back to front.
   *	
   *	@param c given character.
   *	@param pos starting position for the search.
   *	@return position of the last occurrence of the character, 
   *	or length()+1, if the character was not found.
   */
  unsigned int rfind (char c, unsigned int pos = 0) const {
    unsigned int start;
    if (pos==0) start=this->length();
    else start = pos;
    for(int i=(int)start; i>=0; --i) 
      if ( (*this)[i] == c) return i;
    return this->length()+1;
  }

  /** 	searchs a given character into this string from back to front.
   *	
   *	@param c given character.
   *	@param pos starting position for the search.
   *	@return position of the last occurrence of the character, 
   *	or length()+1, if the character was not found.
   */
  unsigned int find_last_of (char c, unsigned int pos = 0) const {
    return this->rfind(c,pos);
  }

  /** 	searchs a given string into this string.
   *	
   *	@param str given string.
   *	@param pos starting position into this string for the search.
   *	@return position of the first occurrence of the string,
   *	or length()+1, if the string was not found.
   */
  unsigned int find (const utlString& str, unsigned int pos = 0) const {
    int l1 = this->length();
    int l2 = str.length();
    if ( l1 < l2 ) return l1+1;

    for ( int i = pos; i <= l1 - l2; ++i )
          if ( !this->compare ( str, i, l2 ) )
               return i;
    return l1+1;
  }

  /** 	compares this string and a given string.
   *	
   *	@param str given string.
   *	@param pos starting position into this string for comparison.
   *	@param npos number of characters to compare.
   *	@return < 0 if the substring is lexicographically less than the given string. <br>
   *	0 if the substring is lexicographically equal to the given string. <br>
   *	> 0 if the substring is lexicographically greater than the given string. <br>
   *	Note: if "npos = 0" it will be used the full length of the given string.
   */
  int compare (const utlString& str, unsigned int pos = 0, 
	       unsigned int npos=0) const {
    return this->compare(str.d_string_p, pos, npos);
  }
	
  /** 	compares this string and a given character array.
   *	
   *	@param s given character array.
   *	@param pos starting position into this string for comparison.
   * 	@param npos number of characters to compare.
   *	@return < 0 if the substring is lexicographically less than the given char array. <br>
   *	0 if the substring is lexicographically equal to the given char array. <br>
   *	> 0 if the substring is lexicographically greater than the given char array. <br>
   *	Note: if "npos = 0" it will be used the full length of the array.
   */
  int compare (const char* s, unsigned int pos=0, 
	       unsigned int npos=0 ) const {
    if ((pos==0) && (npos==0)) return this->compareQuicker(s); 

    unsigned int slen = strlen(s);
    if ( npos <= 0 || npos > slen) npos = slen;
    return strncmp(this->d_string_p+pos, s, npos);
  }

  /** 	this function prints out a utlString object.
   *	
   *	@param out output stream.
   *	@param str_buf utlString object to be printed.
   *	@return ostream& - reference to the output buffer.
   */
  friend ostream& operator<<(ostream& out, const utlString& str_buf) {
    if( str_buf.d_string_p ) out << str_buf.d_string_p;
    return out;
  }
    
  /** 	this function reads a utlString object.
   *	
   *	@param in input stream.
   *	@param str_buf utlString object to be read.
   *	@return istream& - reference to the input buffer.
   */
  friend istream& operator>>(istream& in, utlString& str_buf) {
    char buffer[1024];
    in>>buffer;
    str_buf = buffer;
    return in;
  }

 private:
  /// string array data.
  char *d_string_p;

  /// initialization of the buffer and memory allocation.
  char* init (const char *str_buf, unsigned int pos = 0, 
	      unsigned int n = 0 ) {
    if( str_buf && strlen(str_buf) ) {
      unsigned int size;
      if ( n == 0 ) size=strlen(str_buf);
      else size = n; 
      d_string_p = new char[size+1];
      strncpy(d_string_p, &str_buf[pos], size);
      d_string_p[size]='\0';
    } else {
      d_string_p = new char[1];
      d_string_p[0]='\0';
    }
    return d_string_p;
  }

  /** 	compares this string and a given character array.
   *	
   *	@param str given character array.
   *	@return < 0 if this string is lexicographically less than the given char array. <br>
   *	0 if this string is lexicographically equal to the given char array. <br>
   *	> 0 if this string is lexicographically greater than the given char array. <br>
   */
  int compareQuicker ( const char* str ) const {
    if( !this->d_string_p && str ) return -1;
    else if(this->d_string_p && !str ) return 1;
    else if( !this->d_string_p && !str ) return 0;
    else return strcmp(d_string_p, str);
  }

};

  /**	replaces the contents of "line" with characters read from the input stream. 
   *	It continues reading characters until it encounters the character delim 
   *	(in which case that character is extracted but not stored in "line"), or until end of file. 
   *	Note that getline, unlike operator>>, does not skip whitespace. 
   *	As the name suggests, it is most commonly used to read an entire line of text precisely 
   *	as the line appears in an input file.
   *
   *	@param is input stream.
   *	@param line string to be read. 
   *	@param delim end of line character.
   *	@return is.
   */
  inline istream& getline (istream& is, utlString& line, char delim = '\n') {
    char buffer[4096];
#ifdef GNUC_IOSTREAM
    if (is.ipfx1 ()) {
      _IO_size_t count = 0;
      streambuf *sb = is.rdbuf ();

      while (1) {
        int ch = sb->sbumpc ();
        if (ch == EOF) {
  	  is.setstate (count == 0
		     ? (ios::failbit|ios::eofbit)
		     : ios::eofbit);
	  break;
        }
        buffer[count++] = ch;
        if (ch == delim) {
	  buffer[count++] = '\0';
	  break;
        }             
      }
    }
    line.assign(buffer);
    // We need to be friends with istream to do this.
    // is._gcount = count;
    is.isfx ();
#else
    is.getline(buffer, 4096, delim);
    line.assign(buffer);
#endif /* GNUC_IOSTREAM */
  
    return is;
  }

#endif // USE_STL

#endif // __UTLSTRING_H__

