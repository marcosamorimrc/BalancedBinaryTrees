/*
   AbsStack.h

   Base class for stacks.

  Program:   weiss
  Module:    $RCSfile: AbsStack.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 17 November 2000.
*/
#ifndef __AbsStack
#define __AbsStack

/** Stack abstract class interface.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 * Etype: must have zero-parameter constructor; <br>
 *     implementation will require either. <br>
 *     operator= or copy constructor, perhaps both. <br>
 * CONSTRUCTION: with (a) no initializer; <br>
 *     copy construction of Stack objects is DISALLOWED <br>
 *
 * ******************PUBLIC OPERATIONS********************* <br>
 *     All of the following are pure virtual functions <br>
 * void Push( const Etype& X )   --> Insert X <br>
 * void Pop( )            	 --> Remove most recently inserted item <br>
 * Etype Top( )           	 --> Return most recently inserted item <br>
 * int IsEmpty( )         	 --> Return 1 if empty; else return 0 <br>
 * int IsFull( )          	 --> Return 1 if full; else return 0 <br>
 * void MakeEmpty( )      	 --> Remove all items <br>
 * ******************ERRORS******************************** <br>
 * Top or Pop on empty stack. <br>
 */

template <class Etype>
class AbsStack
{
  public:

  /** @name constructors  */ 
  //@{

/// Default constructor
    AbsStack( ) { }                 
/// Destructor
    virtual ~AbsStack( ) { }        

  //@}

/// Insert.
    virtual void Push( const Etype & X ) = 0;   
/// Remove.
    virtual void Pop( ) = 0;                    
/// Find.
    virtual const Etype & Top( ) const = 0;     
/// returns whether this stack is empty.
    virtual int IsEmpty( ) const = 0;
/// returns whether this stack is full.
    virtual int IsFull( ) const = 0;
/// makes this stack empty.
    virtual void MakeEmpty( ) = 0;

  private:
/// disable copy constructor.    
    AbsStack( const AbsStack & ) { }
};
#endif
