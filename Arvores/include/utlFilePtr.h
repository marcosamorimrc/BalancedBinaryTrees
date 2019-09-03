/* 
  utlFilePtr.h: general file handler functions.

  Program:   util 
  Module:    $RCSfile: utlFilePtr.h,v $
  Language:  C++
  Date:      $Date: 2018/10/11 18:29:26 $
  Version:   $Revision: 1.1 $
*/

#ifndef __FILEPTR_H__
#define __FILEPTR_H__

#include "utlDefs.h"

/** Handles file related operations.
 *	@author <A HREF="mailto:ulisses@watson.ibm.com">Ulisses T. Mello</a> and
 *	<A HREF="mailto:roma@watson.ibm.com">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1 $ $Date: 2018/10/11 18:29:26 $
 *	
 *	Handles all file related operations.
 */
class utlFilePtr {

 public:

  enum { BUFFSIZE = 128 };

  /** @name constructors  */ 
  //@{
  /// 	empty constructor.
  utlFilePtr() {  
    this->init();
  }

  /// constructor from a FILE*.
  utlFilePtr(FILE* pp) {
    this->init();
    this->p = pp;
  }

  /** 	opens a file and copies its name.
   *	
   *	@param name file name.
   *	@param attr mode ("w","r").
   */
  utlFilePtr(const char* name, const char* attr) { 
    this->init();
    if ( !this->open(name,attr) ) { 
      cerr << "Could not open file: " << name << endl;
      //exit(-1); // needs exception here !!
    }
  }

  /// destructor: closes the current file.
  virtual ~utlFilePtr() {
    this->close();
    if (this->fn != 0) delete[] this->fn;
  }
  //@}

  /** 	opens a file and copies its name.
   *	
   *	@param name file name.
   *	@param attr mode ("w","r").
   *	@return 1 if the file was opened, and 0 otherwise.
   */
  int open (const char* name, const char* attr) { 
    this->p = fopen(name,attr);
    if ( this->p == 0 ) 
      return 0;
#ifndef __PRT_DRV_INFO__
    cerr << "file: " << name << " opened" << endl;
#endif
    this->copy(name);
    return 1;
  }
   
  /**	opens the current file. 
   *	
   *	@param attr mode ("w","r").
   *	@return 1 if the file was opened, and 0 otherwise.
   */
  int open( const char* attr ) {
    if (this->fn != 0) 
      return this->open(this->fn,attr);
    return 0;
  }

  /// 	closes the current file.
  void close() {
    if ( this->p == stdout ) {
      cout << "_" << endl; 
      fflush(stdout);
    } else if (this->p != 0) {
      fclose(p);
      this->p = 0;
#ifndef __PRT_DRV_INFO__
      cerr << "file: " << this->fn << " closed" << endl;
#endif
    }
  }

  /**	creates a file name by concatenating a name with an extension.
   *	
   *	@param str file name string.
   *	@param len number of characters of "str" to be used.
   *	@param extension file name extension.
   *	@param filename extended file name.
   */
  void createFileName ( const char* str, size_t len, const char* extension, char* filename ) {
    filename[0] = '\0';
    size_t lext = strlen ( extension );
    size_t lnam = strlen ( filename );
    if ( len+lext > lnam-1 ) return;
    if ( len > lnam - lext )
      len = lnam - lext;
    strncpy ( filename, str, len );
    filename[len] = '\0';
    strcat ( filename, extension );
  }

  /**	creates a file name by concatenating a name with an extension.
   *	
   *	@param str file name string.
   *	@param len number of characters of "str" to be used.
   *	@param extension file name extension.
   *	@return extended file name.
   */
  const char* createFileName ( const char* str, size_t len, const char* extension ) {
    this->tname[0] = '\0';
    size_t lext = strlen ( extension );
    size_t new_len = lext+len+1;
    if ( new_len > 256 ) return NULL;
    if ( len > 255 - lext )
      len = 255 - lext;
    strncpy ( this->tname, str, len );
    this->tname[len] = '\0';
    strcat ( this->tname, extension );
    return this->tname;
  }

  /** 	adds an extension to the current file name.
   *	
   *	@param extension given extension.
   */
  void appendExtension (const char* extension) {
    size_t lext = strlen ( extension );
    size_t len = strlen ( this->fn );
    size_t new_len = len+lext+1;
    if ( this->fsize < new_len ) 
       {
	while ( this->fsize < new_len ) this->fsize *= 2;
	char* tfn = new char[this->fsize];
	tfn[0] = '\0';
	strncpy ( tfn, this->fn, len );
	tfn[len] = '\0';
	delete[] this->fn;
	this->fn = tfn;
       }
    strcat ( this->fn, extension );
  }

  /// removes the extension, including the dot, from the current file name.
  void removeExtension () { 
    int j = this->getDot();
    this->fn[j]='\0';
  }

  /// removes the path from the current file name.
  void removePath () { 
    int j;
    size_t len = strlen (this->fn);
    for (j = (int)len-1; this->fn[j] != '/' && j >= 0; j--);

    if ( j >= 0 )
       {
	size_t i, k;
	for ( k = 0, i = j+1; i < len; i++, k++ )
	  this->fn[k] = this->fn[i]; 
	this->fn[k]='\0';
       }
  }

  /**   returns whether a given extension is present at the END
   *	of the current file name. A dot should be INCLUDED. 
   *	
   *	@param extension given extension.
   *	@return 1 if it is NOT present, and 0 otherwise.
   */
  int compareExtension(const char* extension) const {
    int j = this->getDot();
    return strcmp (&this->fn[j], extension);
  }

  /** 	replaces the current file name extension by the given extension.
   *	
   *	@param extension given extension.
   */
  void replaceExtension(const char* extension) {
    this->removeExtension();
    this->appendExtension(extension);
  }

  /**	returns the current file name.
   *	
   *	@return current file name.
   */
  const char* getFileName() const { return this->fn; }

  /** 	sets the current file name to the given file name.
   *	
   *	@param filename given file name.
   */
  void setFileName(const char* filename) {
    this->copy(filename);
  }

  /**	returns the current file pointer.
   *	
   *	@return current file pointer.
   */
  FILE* getFile() const { return this->p; }

  /**   converts a utlFilePtr in a FILE*.
   *	
   *	@return current file pointer.
   */
  operator FILE*() { return this->p; }

  /**   returns the position of a dot in the current file name.
   *	The search is carried out back to front.
   *	
   *	@return dot position or 0, if there is no dot.
   */
  int getDot() const {
    size_t j;
    size_t len = strlen (this->fn);
    for (j = len-1; this->fn[j] != '.' && j > 0; j--);
    return j;
  }

 private:

  /** 	copies a given file name to the current file name.
   *	
   *	@param name given file name.
   */
  void copy(const char* name){
    size_t len = strlen ( name )+1;
    if ( this->fsize < len ) 
      {
	delete[] this->fn;
	while ( this->fsize < len ) this->fsize *= 2;
	this->fn = new char[this->fsize];
      }
    strncpy ( this->fn, name, len );
  }

  /// initializes local variables.
  void init() {
    this->fsize = BUFFSIZE; 
    this->fn = new char[this->fsize]; 
    this->fn[0] = '\0';
    this->p = 0;
    this->tname[0]='\0';
  }

  /// current file pointer.
  FILE* p;
  /// current file name size.
  size_t fsize;
  /// current file name.
  char* fn;
  /// temporary file name.
  char tname[2*BUFFSIZE];

};

#endif // __FILEPTR_H__

